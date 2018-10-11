#pragma once

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utils/trace.hpp>
#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace cv;
using namespace cv::dnn;
using namespace std;
 
class SurgicalDetection
{
public:
	SurgicalDetection();
	void inputdir(String &inputfileName);
	String outputdir();
	void Caffe_YOLO(String FileName);
	void Detection_YOLO(const Mat &ImageFile, Mat &surgicaldetection_area);
	void Detection_Caffe(const Mat &ImageFile, Mat &Result_IdProb);

	~SurgicalDetection() {
	};
private:
	String fileName;

};