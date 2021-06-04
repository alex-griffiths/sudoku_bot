#include <ros/ros.h>
#include <move_arm_joints/move_and_confirm.h>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

bool move_and_confirm(move_arm_joints::move_and_confirm::Request &req,
                   move_arm_joints::move_and_confirm::Response &resp)
{
	// Capture request
	stringstream req_string;

	req_string << "./arm_wrist_flex_servo " << req.move;

	cout << "arm_wrist_flex_servo " << req.move << endl;

	system(req_string.str().c_str());

	// Send response
	ifstream log_file;
	log_file.open("arm_wrist_flex_servo_confirm.log");

	if (log_file.is_open())
	{
		string res_str;
		getline(log_file, res_str);
		resp.confirm = stod(res_str);

		cout << res_str << endl;
	}
	return true;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "arm_wrist_flex_joint_node");
	ros::NodeHandle nh;
	ros::ServiceServer myServer = nh.advertiseService("arm_wrist_flex_joint_service", &move_and_confirm);

	ros::spin();
	return 0;
}


