#include <ros/ros.h>
#include "mover_client/grid_num_vector.h"
#include "move_arm_joints/move_and_confirm.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace::std;

ros::ServiceClient arm_shoulder_pan_joint_client;
ros::ServiceClient arm_elbow_flex_joint_client;
ros::ServiceClient arm_wrist_flex_joint_client;
ros::ServiceClient arm_shoulder_lift_joint_client;
ros::ServiceClient gripper_client;

struct param {
	int row;
	int col;
	int num;
	double arm_shoulder_pan;
	double arm_elbow_flex;
	double arm_wrist_flex;
	double arm_shoulder_lift;
	double gripper;
};

struct param find_param(mover_client::grid_num, vector<param>&);

void sol_callback(const mover_client::grid_num_vector &msg) {
	ROS_INFO_STREAM("SOLUTION VECTOR RECEIVED" << endl);


	// Open param.csv and parse it into usable data.
	ifstream param("param.csv");
	string p_line;
	int line_idx = 0;

	vector<struct param> params;

	// Parse params.csv into a vector of param structs that
	// will be easier to use for determining the appropriate 
	// servo and sensor data to send.
	ROS_INFO_STREAM("PARSING PARAMS INTO VECTOR OF STRUCTURES" << endl);
	while(getline(param, p_line)) {
		if (line_idx > 1) {
			struct param tmp_param;
			int p_idx = 0;
			string sub_param;

			stringstream p_line_ss(p_line);
			while(getline(p_line_ss, sub_param, ',')) {
				switch (p_idx) {
					case 0: tmp_param.row = stoi(sub_param); break;
					case 1: tmp_param.col = stoi(sub_param); break;
					case 2: tmp_param.num = stoi(sub_param); break;
					case 3: tmp_param.arm_shoulder_pan = stod(sub_param); break;
					case 4: tmp_param.arm_elbow_flex = stod(sub_param); break;
					case 5: tmp_param.arm_wrist_flex = stod(sub_param); break;
					case 6: tmp_param.arm_shoulder_lift = stod(sub_param); break;
					case 7: tmp_param.gripper = stod(sub_param); break;
				}

				p_idx++;
			}
		}

		line_idx++;
	}

	// Iterate over the received message
	for (int i = 0; i < msg.numbered_grids.size(); i++) {
		mover_client::grid_num grid_num = msg.numbered_grids[i];

		struct param param_data = find_param(grid_num, params);

		// Send out service data.
		move_arm_joints::move_and_confirm::Request asp_req;
		move_arm_joints::move_and_confirm::Response asp_res;
		move_arm_joints::move_and_confirm::Request aef_req;
		move_arm_joints::move_and_confirm::Response aef_res;
		move_arm_joints::move_and_confirm::Request awf_req;
		move_arm_joints::move_and_confirm::Response awf_res;
		move_arm_joints::move_and_confirm::Request asl_req;
		move_arm_joints::move_and_confirm::Response asl_res;
		move_arm_joints::move_and_confirm::Request grip_req;
		move_arm_joints::move_and_confirm::Response grip_res;

		bool service_res;

		/*
		 * Make calls to each service continually until
		 * we receive a successful confirmation.
		 * We keep making the call just in case the movement
		 * fails. We do this for each joint in sequence.
		 */

		ROS_INFO_STREAM("BEGIN MOVING ARM" << endl);

		asp_req.move = param_data.arm_shoulder_pan;
		while(asp_res.confirm != asp_req.move) {
			service_res = arm_shoulder_pan_joint_client.call(asp_req, asp_res);

			if (service_res) {
				ROS_INFO_STREAM("MC: " << grid_num.row << "," << grid_num.col << "," << grid_num.num << " MOVING" << endl);
			} else {
				ROS_INFO_STREAM("MOVEMENT FAILED" << endl);
			}
		}
	}
}

struct param find_param(mover_client::grid_num grid_num, vector<param> &params) {
	for (int i = 0; i < params.size(); i++) {
		if (grid_num.row == params[i].row && grid_num.col == params[i].col && grid_num.num == params[i].num) {
			return params[i];
		}
	}

	// No param was found.. this is an error.
	exit(0);
}

int main(int argc, char **argv) {
	ros::init(argc, argv, "mover_client_node");

	ros::NodeHandle nh;
	ros::Subscriber sub = nh.subscribe("num_grids_sol/vector", 1000, sol_callback);
	
	// Create service clients
	arm_shoulder_pan_joint_client = nh.serviceClient<move_arm_joints::move_and_confirm>("arm_shoulder_pan_joint_service");
	arm_elbow_flex_joint_client = nh.serviceClient<move_arm_joints::move_and_confirm>("arm_elbow_flex_joint_service");
	arm_wrist_flex_joint_client = nh.serviceClient<move_arm_joints::move_and_confirm>("arm_wrist_flex_joint_service");
	arm_shoulder_lift_joint_client = nh.serviceClient<move_arm_joints::move_and_confirm>("arm_shoulder_lift_joint_service");
	gripper_client = nh.serviceClient<move_arm_joints::move_and_confirm>("gripper_joint_service");

	ros::spin();
	return 0;
}
