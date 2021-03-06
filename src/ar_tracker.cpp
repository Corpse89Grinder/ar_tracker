#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <ar_pose/ARMarker.h>

void markerCallback(ar_pose::ARMarker marker);
void commandsCallback(geometry_msgs::Twist twist);

bool printed = false;

int sequence = -1;
int currentSequence = sequence;

double ratio;

int counter = 15;

ros::Publisher pub;

int main(int argc, char** argv)
{
	ros::init(argc, argv, "ar_tracker");

	ros::NodeHandle nh;

	std::string marker_t, cmd_t, vel_t;
	ros::param::param<std::string>("~/marker_topic", marker_t, "ar_pose_marker");
	ros::param::param<std::string>("~/command_topic", cmd_t, "commands");
	ros::param::param<std::string>("~/velocity_topic", vel_t, "turtle1/cmd_vel");


    ros::Subscriber markerSub = nh.subscribe(marker_t, 1, markerCallback);
    ros::Subscriber commandsSub = nh.subscribe(cmd_t, 1, commandsCallback);

    pub = nh.advertise<geometry_msgs::Twist>(vel_t, 1);

    ros::spin();

    return 0;
}

void markerCallback(ar_pose::ARMarker marker)
{
	double distance = marker.pose.pose.position.z - 0.50;

	if(distance >= 0 && distance <= 1)
	{
		ratio = 1;
	}
	else if(distance > 1 && distance <= 2)
	{
		ratio = 1 - (distance - 1);
	}
	else if(distance > 2)
	{
		ratio = 0;
	}

	if(ratio < 0)
	{
		ratio = 0;
	}
	else if(ratio > 1)
	{
		ratio = 1;
	}

	currentSequence = marker.header.seq;

	return;
}

void commandsCallback(geometry_msgs::Twist received)
{
	geometry_msgs::Twist twist;

	if(counter == 15)
	{
		twist.linear.x = 0;
		twist.angular.z = 0;
	}

	if(currentSequence == sequence)
	{
		counter++;
	}
	else
	{
		counter = 0;
		sequence = currentSequence;

		twist.linear.x = received.linear.x * ratio;
		twist.angular.z = received.angular.z * ratio;
	}

	pub.publish(twist);

	return;
}
