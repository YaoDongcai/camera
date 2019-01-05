#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <tf/tf.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "fly_read.hpp"

using namespace cv;

image_transport::Publisher image_pub;

int main(int argc, char** argv)
{
	ros::init(argc, argv, "fly_read");
	ros::NodeHandle nh;
	image_transport::ImageTransport it(nh);
	ros::Rate loopRate(30);
	image_pub = it.advertise("camera/image_raw", 1);

	ros::Time now_time = ros::Time::now();

	Mat frame;
	panghaoFlyCapture fly_cap;
	bool flag;
	flag = fly_cap.SetUp();

	ros::Rate loop_rate(30);

	while (flag)
	{
		std::cout << "now time" << now_time << std::endl;

		fly_cap.ReadNextFrame(frame);
		sensor_msgs::ImagePtr msg;
		*msg.header.stamp  = now_time;
		*msg.header.frame_id = "world";
		msg = cv_bridge::CvImage(std_msg::Header(), "bgr8", frame).toImageMsg();
		
		image_pub.Publish(msg);

		std::cout << *msg.header.stamp << std::endl;

		// imshow("lalala", frame);
		// waitKey(30);

		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;
}