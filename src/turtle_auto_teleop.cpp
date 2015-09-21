#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

int main(int argc, char** argv)
{
	ros::init(argc, argv, "turtle_auto_teleop");

	ros::NodeHandle nh;

	ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("commands", 1);

	ros::Rate loop_rate(30);

	while(ros::ok())
	{
		geometry_msgs::Twist twist;

		twist.linear.x = 2;
		twist.angular.z = 2;

		pub.publish(twist);

		loop_rate.sleep();
	}
}
