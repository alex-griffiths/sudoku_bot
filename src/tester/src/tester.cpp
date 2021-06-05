#include <ros/ros.h>
#include <mover_client/grid_num.h>
#include <mover_client/grid_num_vector.h>
#include <vector>
#include <sstream>

using namespace::std;

void sub_callback(const ros::SingleSubscriberPublisher &publisher) {
	ROS_INFO_STREAM("GRID_NUM_FACTS" << endl);
	mover_client::grid_num fact1;
	mover_client::grid_num fact2;
	mover_client::grid_num fact3;
	mover_client::grid_num fact4;
	mover_client::grid_num fact5;
	mover_client::grid_num fact6;
	mover_client::grid_num fact7;
	mover_client::grid_num fact8;
	mover_client::grid_num fact9;
	mover_client::grid_num fact10;
	mover_client::grid_num fact11;
	mover_client::grid_num fact12;
	mover_client::grid_num fact13;
	mover_client::grid_num fact14;
	mover_client::grid_num fact15;
	mover_client::grid_num fact16;
	mover_client::grid_num fact17;
	mover_client::grid_num fact18;
	mover_client::grid_num fact19;
	mover_client::grid_num fact20;
	mover_client::grid_num fact21;

	// Define the row/col/num for each fact
	fact1.row = 1;
	fact1.col = 1;
	fact1.num = 8;

	fact2.row = 1;
	fact2.col = 4;
	fact2.num = 5;

	fact3.row = 1;
	fact3.col = 5;
	fact3.num = 7;

	fact4.row = 1;
	fact4.col = 8;
	fact4.num = 4;

	fact5.row = 1;
	fact5.col = 9;
	fact5.num = 6;

	fact6.row = 2;
	fact6.col = 5;
	fact6.num = 4;

	fact7.row = 3;
	fact7.col = 4;
	fact7.num = 8;

	fact8.row = 3;
	fact8.col = 5;
	fact8.num = 3;

	fact9.row = 4;
	fact9.col = 1;
	fact9.num = 7;

	fact10.row = 4;
	fact10.col = 9;
	fact10.num = 9;

	fact11.row = 5;
	fact11.col = 1;
	fact11.num = 9;

	fact12.row = 5;
	fact12.col = 2;
	fact12.num = 3;

	fact13.row = 5;
	fact13.col = 3;
	fact13.num = 1;

	fact14.row = 6;
	fact14.col = 1;
	fact14.num = 2;

	fact15.row = 6;
	fact15.col = 3;
	fact15.num = 4;

	fact16.row = 6;
	fact16.col = 6;
	fact16.num = 8;

	fact17.row = 7;
	fact17.col = 7;
	fact17.num = 9;

	fact18.row = 7;
	fact18.col = 9;
	fact18.num = 2;

	fact19.row = 8;
	fact19.col = 1;
	fact19.num = 5;

	fact20.row = 9;
	fact20.col = 4;
	fact20.num = 4;

	fact21.row = 9;
	fact21.col = 5;
	fact21.num = 5;

	mover_client::grid_num_vector facts_vector;

	facts_vector.numbered_grids.push_back(fact1);
	facts_vector.numbered_grids.push_back(fact2);
	facts_vector.numbered_grids.push_back(fact3);
	facts_vector.numbered_grids.push_back(fact4);
	facts_vector.numbered_grids.push_back(fact5);
	facts_vector.numbered_grids.push_back(fact6);
	facts_vector.numbered_grids.push_back(fact7);
	facts_vector.numbered_grids.push_back(fact8);
	facts_vector.numbered_grids.push_back(fact9);
	facts_vector.numbered_grids.push_back(fact10);
	facts_vector.numbered_grids.push_back(fact11);
	facts_vector.numbered_grids.push_back(fact12);
	facts_vector.numbered_grids.push_back(fact13);
	facts_vector.numbered_grids.push_back(fact14);
	facts_vector.numbered_grids.push_back(fact15);
	facts_vector.numbered_grids.push_back(fact16);
	facts_vector.numbered_grids.push_back(fact17);
	facts_vector.numbered_grids.push_back(fact18);
	facts_vector.numbered_grids.push_back(fact19);
	facts_vector.numbered_grids.push_back(fact20);
	facts_vector.numbered_grids.push_back(fact21);

	// Output facts to ros info
	for (int i = 0; i < facts_vector.numbered_grids.size(); i++)
	{
		ROS_INFO_STREAM("(" << facts_vector.numbered_grids[i].row << "," << facts_vector.numbered_grids[i].col << "," << facts_vector.numbered_grids[i].num << ")" << endl);
	}

	publisher.publish(facts_vector);

	ros::shutdown();
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "tester_node");

	ros::NodeHandle nh;

	ros::Publisher pub = nh.advertise<mover_client::grid_num_vector>("num_grids/vector", 1000, (ros::SubscriberStatusCallback)sub_callback);

	ros::spin();

	return 0;
}

