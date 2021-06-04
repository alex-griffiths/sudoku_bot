#include <ros/ros.h>
#include <mover_client/grid_num.h>
#include <mover_client/grid_num_vector.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

using namespace::std;

mover_client::grid_num_vector parse_solution(string);

ros::Publisher interface_pub;

void ng_callback(const mover_client::grid_num_vector &msg) {
	string fname = "my_sudoku_facts.lp";
	
	// Create initial facts file
	ROS_INFO_STREAM("OPENING " << fname << " FOR FACT OUTPUT" << endl);
	ofstream facts(fname.c_str());
	for (int i = 0; i < msg.numbered_grids.size(); i++)
	{
		facts << "gridNum(" << msg.numbered_grids[i].row << "," << msg.numbered_grids[i].col << "," << msg.numbered_grids[i].num << ")." << endl;


		
		ROS_INFO_STREAM("(" << msg.numbered_grids[i].row << "," << msg.numbered_grids[i].col << "," << msg.numbered_grids[i].num << ")" << endl);
	}

	ROS_INFO_STREAM("CLOSING " << fname << endl << msg.numbered_grids.size() << " FACTS OUTPUT" << endl);
	facts.close();

	ROS_INFO_STREAM("SOLVING PUZZLE");

	system("./clingo my_sudoku_facts.lp my_sudoku_ASP_program.lp > result.txt");

	// Setup solution variables
	mover_client::grid_num sol;
	mover_client::grid_num_vector sol_vec;

	// Parse result file and store the grid positions to be sent
	// to the mover_client
	fstream res_file("result.txt");
	string gn_line;
	int line_idx = 0;

	while(getline(res_file, gn_line)) {
		line_idx++;

		if (line_idx == 5) {
			sol_vec = parse_solution(gn_line);

			continue;
		}
	}

	// Publish the solution vector to the topic for the mover_client
	interface_pub.publish(sol_vec);
}

mover_client::grid_num_vector parse_solution(string solution) {
	/*
	 * Split the solution on spaces as this is what separates
	 * each gridNum atom
	 */
	stringstream sol_stream(solution);
	string grid_num_string;

	mover_client::grid_num_vector sol_vec;

	while(getline(sol_stream, grid_num_string, ' ')) {

		mover_client::grid_num grid_num;
		/*
		 * We now have an individual grid num. We need to
		 * remove the "gridNum(...)" characters so we can parse
		 * the row/col/num properly.
		 */
		
		// Remove the last character so that our substr works
		grid_num_string.erase(grid_num_string.size() - 1);

		/*
		 * substr gives us a string that starts at the row value
		 * from there we can use getline to split on commas
		 */
		stringstream val_substr(grid_num_string.substr(8));	
		string tmp; // The temp string the getline value will be stored in.
		int substr_idx = 0;

		while (getline(val_substr, tmp, ',')) {
			switch (substr_idx) {
				case 0: grid_num.row = stoi(tmp); break;
				case 1: grid_num.col = stoi(tmp); break;
				case 2: grid_num.num = stoi(tmp); break;
			}

			substr_idx++;
		}
		sol_vec.numbered_grids.push_back(grid_num);
	}

	return sol_vec;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "interface_node");

	ros::NodeHandle nh;

	ros::Subscriber interface_sub = nh.subscribe("num_grids/vector", 1000, ng_callback);

	// Make sure publisher is hooked up to the right topic
	interface_pub = nh.advertise<mover_client::grid_num_vector>("num_grid_sol/vector", 1000);

	ros::spin();
}


