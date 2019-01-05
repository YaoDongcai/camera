//#define FLYCAP
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/Imu.h>
#include <std_msgs/Header.h>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "fly_read.hpp"

using namespace cv;

image_transport::Publisher image_pub;
std_msgs::Header imu_header;

void imu_callback(sensor_msgs::Imu imu)
{
	imu_header = imu.header;
}


int main(int argc, char** argv)
{
	ros::init(argc, argv, "fly_read");
	ros::NodeHandle nh;
	image_transport::ImageTransport it(nh);
	//ros::Rate loopRate(10);
	image_pub = it.advertise("/camera/image_raw", 20);
	//ros::Subscriber sub_imu = nh.subscribe("/dji_sdk/imu", 2000, imu_callback, ros::TransportHints().tcpNoDelay() );

	Mat frame;
#ifdef FLYCAP
	panghaoFlyCapture fly_cap;
	bool flag;
	flag = fly_cap.SetUp();
#endif

#ifndef FLYCAP
	VideoCapture cap(0);
	bool flag = cap.isOpened();
#endif
	ros::Rate loop_rate(10);

	while (flag && ros::ok())
	{
#ifdef FLYCAP
		fly_cap.ReadNextFrame(frame);
#endif

#ifndef FLYCAP
		cap >> frame;
#endif
		std::cout << frame << std::endl;
		//resize(frame, frame, Size(644, 482));
		sensor_msgs::ImagePtr msg;
		msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();

		msg->header.stamp = ros::Time::now();
		msg->header.frame_id ="/world";
		image_pub.publish(msg);

		//imshow("lalala", frame);
		//waitKey(10);
		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;
}