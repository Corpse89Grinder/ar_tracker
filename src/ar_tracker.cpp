#include "ar_tracker/ARMarker.h"

#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <geometry_msgs/Twist.h>

void markerCallback(ar_tracker::ARMarker marker);
void commandsCallback(geometry_msgs::Twist twist);

double ticks = 0;
bool printed = false;

int sequence = -1;
int currentSequence = sequence;

double distance;
double ratio;
double linear;
double angular;

int counter = 15;

int main(int argc, char** argv)
{
	ros::init(argc, argv, "ar_tracker");

	ros::NodeHandle nh;

    ros::Subscriber markerSub = nh.subscribe("ar_pose_marker", 1, markerCallback);
    ros::Subscriber commandsSub = nh.subscribe("commands", 1, commandsCallback);

    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1);

    ros::Rate loop_rate(50);

    while(ros::ok())
    {
    	double precTick = ticks;

		ticks = (double) cv::getTickCount();

		double dT = (ticks - precTick) / cv::getTickFrequency();

    	ros::spinOnce();

    	geometry_msgs::Twist twist;

    	if(counter == 15)
		{
    		twist.linear.x = 0;
    		twist.angular.z = 0;
		}

    	if(currentSequence == sequence)
    	{
    		counter++;

    		twist.linear.x = linear * ratio / counter;
    		twist.angular.z = angular * ratio / counter;
    	}
    	else
		{
    		counter = 0;
			sequence = currentSequence;

			twist.linear.x = linear * ratio;
			twist.angular.z = angular * ratio;
		}

    	pub.publish(twist);

    	if(dT * 1000 < 50)
    	{
    		loop_rate = ros::Rate(50 - (dT * 1000));
    		loop_rate.sleep();
    	}
    }

    return 0;
}

void markerCallback(ar_tracker::ARMarker marker)
{
	distance = marker.pose.pose.position.z;
	ratio = (2 - distance) / 2;
	currentSequence = marker.header.seq;

	return;
}

void commandsCallback(geometry_msgs::Twist twist)
{
	linear = twist.linear.x;
	angular = twist.angular.z;

	return;
}
