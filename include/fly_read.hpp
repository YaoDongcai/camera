#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv/cv.h>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>
#include <vector>

#include <flycapture/FlyCapture2.h>

class panghaoFlyCapture
{
public:
	FlyCapture2::Error error;
	FlyCapture2::Camera camera;
	FlyCapture2::CameraInfo camInfo;

	bool SetUp()
	{
		error = camera.Connect(0);
		if (error != FlyCapture2::PGRERROR_OK)
		{
			std::cout << "Failed to connect to camera" << std::endl;
			return false;
		}

		error = camera.GetCameraInfo( &camInfo );
		if (error != FlyCapture2::PGRERROR_OK)
		{
			std::cout << "Failed to get camera info from camera" << std::endl;
			return false;
		}
		std::cout << camInfo.vendorName << " "
				  << camInfo.modelName << " "
				  << camInfo.serialNumber << std::endl;
		error = camera.StartCapture();
		if (error == FlyCapture2::PGRERROR_ISOCH_BANDWIDTH_EXCEEDED )
		{
			std::cout << "Bandwidth exceeded" << std::endl;
			return false;
		}
		else if(error != FlyCapture2::PGRERROR_OK)
		{
			std::cout << "Failed to start image capture" << std::endl;
			return false;
		}
		return true;
	}

	void ReadNextFrame(cv::Mat& frame)
	{
		FlyCapture2::Image rawImage;
		FlyCapture2::Error error = camera.RetrieveBuffer( &rawImage );
		if (error != FlyCapture2::PGRERROR_OK)
		{
			std::cout << "capture error" << std::endl;
			return;
		}
		FlyCapture2::Image rgbImage;
		rawImage.Convert(FlyCapture2::PIXEL_FORMAT_BGR, &rgbImage );

		unsigned int rowBytes = (double)rgbImage.GetReceivedDataSize()/(double)rgbImage.GetRows();
		cv::Mat cvImage = cv::Mat(rgbImage.GetRows(), rgbImage.GetCols(), CV_8UC3, rgbImage.GetData(), rowBytes);
		cvImage.copyTo(frame);
	}
};