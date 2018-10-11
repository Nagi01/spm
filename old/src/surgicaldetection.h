#pragma once

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utils/trace.hpp>
#include <fstream>
#include <iostream>
#include <cstdlib>
 
class SurgicalDetection
{
public:
	SurgicalDetection();
	void Caffe_YOLO(std::string FileName);
	void Detection_YOLO(const cv::Mat &ImageFile, cv::Mat &surgicaldetection_area);
	void Detection_Caffe(const cv::Mat &ImageFile, cv::Mat &Result_IdProb);

	~SurgicalDetection() {
	};
private:
	std::string fileName;

};