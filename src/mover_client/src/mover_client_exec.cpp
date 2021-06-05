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

void find_param(mover_client::grid_num, vector<param>&, param &);

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
		if (line_idx % 1000 == 0) {
			ROS_INFO_STREAM("LINE " << line_idx << endl);
		}

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
 

				params.push_back(tmp_param);
 				p_idx++;
 			}
 		}
 
 		line_idx++;
 	}

	ROS_INFO_STREAM("PARSING PARAMS COMPLETE" << endl);
 
 	// Iterate over the received message
 	for (int i = 0; i < msg.numbered_grids.size(); i++) {
 		mover_client::grid_num grid_num = msg.numbered_grids[i];
 
 		struct param param_data;
		find_param(grid_num, params, param_data);
 
		ROS_INFO_STREAM("FOUND PARAM: " << param_data.row << "," << param_data.col << "," << param_data.num << endl);
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
 
		// Arm Shoulder Pan Joint
 		asp_req.move = param_data.arm_shoulder_pan;
 		while(asp_res.confirm != asp_req.move) {
 			service_res = arm_shoulder_pan_joint_client.call(asp_req, asp_res);
 
 			if (service_res) {
 				ROS_INFO_STREAM("MC: " << grid_num.row << "," << grid_num.col << "," << grid_num.num << " MOVING" << endl);
 			} else {
 				ROS_INFO_STREAM("MOVEMENT FAILED" << endl);
 			}
 		}
		
		// Arm Elbow Flex Joint
 		aef_req.move = param_data.arm_elbow_flex;
 		while(aef_res.confirm != aef_req.move) {
 			service_res = arm_elbow_flex_joint_client.call(aef_req, aef_res);
 
 			if (service_res) {
 				ROS_INFO_STREAM("MC: " << grid_num.row << "," << grid_num.col << "," << grid_num.num << " MOVING" << endl);
 			} else {
 				ROS_INFO_STREAM("MOVEMENT FAILED" << endl);
 			}
 		}
		
		// Arm Wrist Flex Joint
 		awf_req.move = param_data.arm_wrist_flex;
 		while(awf_res.confirm != awf_req.move) {
 			service_res = arm_wrist_flex_joint_client.call(awf_req, awf_res);
 
 			if (service_res) {
 				ROS_INFO_STREAM("MC: " << grid_num.row << "," << grid_num.col << "," << grid_num.num << " MOVING" << endl);
 			} else {
 				ROS_INFO_STREAM("MOVEMENT FAILED" << endl);
 			}
 		}
		
		// Arm Shoulder Lift Joint
 		asl_req.move = param_data.arm_shoulder_lift;
 		while(asl_res.confirm != asl_req.move) {
 			service_res = arm_shoulder_lift_joint_client.call(asl_req, asl_res);
 
 			if (service_res) {
 				ROS_INFO_STREAM("MC: " << grid_num.row << "," << grid_num.col << "," << grid_num.num << " MOVING" << endl);
 			} else {
 				ROS_INFO_STREAM("MOVEMENT FAILED" << endl);
 			}
 		}
		
		// Arm Shoulder Lift Joint
 		grip_req.move = param_data.gripper;
 		while(grip_res.confirm != grip_req.move) {
 			service_res = gripper_client.call(grip_req, grip_res);
 
 			if (service_res) {
 				ROS_INFO_STREAM("MC: " << grid_num.row << "," << grid_num.col << "," << grid_num.num << " MOVING" << endl);
 			} else {
 				ROS_INFO_STREAM("MOVEMENT FAILED" << endl);
 			}
 		}
 	}
}

void find_param(mover_client::grid_num grid_num, vector<param> &params, param &param) {
	ROS_INFO_STREAM("SEARCHING FOR MATCHING PARAM" << endl);
	ROS_INFO_STREAM("GRID NUM: " << grid_num.row << "," << grid_num.col << "," << grid_num.num << endl);
	for (int i = 0; i < params.size(); i++) {
		if (grid_num.row == params[i].row && grid_num.col == params[i].col && grid_num.num == params[i].num) {
			ROS_INFO_STREAM("FOUND MATCHING PARAM" << endl);
			param = params[i];
			return;
		}
	}
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
