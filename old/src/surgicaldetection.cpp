#include "surgicaldetection.h"
#include "qt_toolbox.h"

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/utils/trace.hpp>
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace cv;
using namespace cv::dnn;
using namespace std;

/* �e�N���X�̂����A�ł��m���������N���X�𓾂� */
static void getMaxClass(const Mat &probBlob, int *classId, double *classProb)
{
	Mat probMat = probBlob.reshape(1, 1);
	Point classNumber;
	minMaxLoc(probMat, NULL, classProb, NULL, &classNumber);
	*classId = classNumber.x;
}

/* �w�K���ɗp����ID���烉�x������������ */
static std::vector<String> readClassNames(const String filename)
{
	std::vector<String> classNames;
	std::ifstream fp(filename);
	if (!fp.is_open())
	{
		std::cerr << "File with classes labels not found: " << filename << std::endl;
		exit(-1);
	}
	std::string name;
	while (!fp.eof())
	{
		std::getline(fp, name);
		if (name.length())
			classNames.push_back(name.substr(name.find(' ') + 1));
	}
	fp.close();
	return classNames;
}

/* �R���X�g���N�^ */
SurgicalDetection::SurgicalDetection()
{
}
/* YOLO��p�����p��o */
void SurgicalDetection::Detection_YOLO(const Mat &ImageFile, Mat &surgicaldetection_area)
{
	Mat frame = ImageFile;//���ʉ摜��n���D
	surgicaldetection_area = Mat_<float>(1, 5);
	surgicaldetection_area.at<float>(0, 0) = 0;
	surgicaldetection_area.at<float>(0, 1) = 0;
	surgicaldetection_area.at<float>(0, 2) = 0;
	surgicaldetection_area.at<float>(0, 3) = 0;
	surgicaldetection_area.at<float>(0, 4) = -1;
	if (frame.empty())
	{
		cout << "No_Image" << endl;
		return;
	}
	//�p��ROI�w��(YOLO)
	String modelConfiguration = "E:/DEVEL/nambu_surgicaltooldetection/build/learning.cfg";
	String modelBinary = "E:/DEVEL/nambu_surgicaltooldetection/build/learning_10000.weights";
	Net caffe_net, dark_net;
	try
	{
		dark_net = readNetFromDarknet(modelConfiguration, modelBinary);
	}
	catch (const std::exception&)
	{
		//this->setWindowTitle("NetWork Setting ERROR");
	}
	/* ���ʂɗ^���閼�̂��` */
	CV_TRACE_FUNCTION();
	/* ���ʂɗp����f�o�C�X���`�A�������uCPU�v�uOPENCL�v�őI�ׂ� */
	caffe_net.setPreferableTarget(DNN_TARGET_OPENCL);
	dark_net.setPreferableTarget(DNN_TARGET_OPENCL);

	int classNum = 0;
	vector<string> classNamesVec;
	ifstream classNamesFile("E:/DEVEL/nambu_surgicaltooldetection/build/learning.names");
	if (classNamesFile.is_open())
	{
		string className = "";
		while (std::getline(classNamesFile, className)) {
			classNamesVec.push_back(className);
			classNum++;
		}
	}
	//color�`�����l���̒���
	if (frame.channels() == 4) cvtColor(frame, frame, COLOR_BGRA2BGR);
	//�摜�̕ό`
	Mat dark_inputBlob = blobFromImage(frame, 1 / 255.F, Size(416, 416), Scalar(), true, false);
	dark_net.setInput(dark_inputBlob, "data");
	Mat detectionMat = dark_net.forward("detection_out");   //compute output �ȑO�́AgetOutputsNames(net)[0]);
	ostringstream ss;
	//ROI�̕`����w��
	float confidenceThreshold = 0.25;//�����ς���K�v�����鉽�p�[�Z���g�ȉ���ROI���͂���
	//�Ԃ�l�Ƃ��ĕԂ�ROI���܂Ƃ߂�Mat
	
	int threshold_truecount = 0;
	//img�摜�ւ̏d��@detectionMat.rows�͂����炭�������ꂽROI�̌�
	for (int i = 0; i < detectionMat.rows; i++)
	{
		const int probability_index = 5;
		const int probability_size = detectionMat.cols - probability_index;
		float *prob_array_ptr = &detectionMat.at<float>(i, probability_index);
		size_t objectClass = max_element(prob_array_ptr, prob_array_ptr + probability_size) - prob_array_ptr;
		float confidence = detectionMat.at<float>(i, (int)objectClass + probability_index);
		if (confidence > confidenceThreshold)
		{
			if (threshold_truecount > 0) {
				surgicaldetection_area.resize(++threshold_truecount);
			}
			else {
				threshold_truecount++;
			}

			float x = detectionMat.at<float>(i, 0);
			float y = detectionMat.at<float>(i, 1);
			float width = detectionMat.at<float>(i, 2);
			float height = detectionMat.at<float>(i, 3);

			int xLeftBottom = static_cast<int>((x - width / 2) * frame.cols);
			int yLeftBottom = static_cast<int>((y - height / 2) * frame.rows);
			int xRightTop = static_cast<int>((x + width / 2) * frame.cols);
			int yRightTop = static_cast<int>((y + height / 2) * frame.rows);
			//Rect object(xLeftBottom, yLeftBottom,
				//xRightTop - xLeftBottom,
				//yRightTop - yLeftBottom);

			if (xLeftBottom >= frame.cols) xLeftBottom = frame.cols;
			if (yRightTop >= frame.rows) yRightTop = frame.rows;
			if (xRightTop >= frame.cols) xRightTop = frame.cols;
			if (yLeftBottom >= frame.rows) yLeftBottom = frame.rows;

			int xwidth = xRightTop - xLeftBottom - 1;
			int yheight = yRightTop - yLeftBottom - 1;

			//�i�[
			surgicaldetection_area.at<float>(threshold_truecount - 1, 0) = xLeftBottom;
			surgicaldetection_area.at<float>(threshold_truecount - 1, 1) = yLeftBottom;
			surgicaldetection_area.at<float>(threshold_truecount - 1, 2) = xwidth;
			surgicaldetection_area.at<float>(threshold_truecount - 1, 3) = yheight;
			surgicaldetection_area.at<float>(threshold_truecount - 1, 4) = (int)objectClass;
			cout << surgicaldetection_area << endl;
		}
	}//for (int i = 0; i < detectionMat.rows; i++)
	//return surgicaldetection_area;
}
/* Caffe��p�����p��o */
void SurgicalDetection::Detection_Caffe(const Mat &ImageFile, Mat &Result_IdProb)
{
	Mat InputImg = ImageFile;
	Result_IdProb = Mat_<double>(1, 2);
	
	if (InputImg.empty()) return;
	//�p���(Caffe)
	String modelTxt = "deploy_mine.prototxt";
	String modelBin = "bvlc_googlenet_quick_mine_iter_10000.caffemodel";
	String labelTxt = "labels.txt";
	Net caffe_net;
	try
	{
		caffe_net = readNetFromCaffe(modelTxt, modelBin);;
	}
	catch (const std::exception&)
	{
		//this->setWindowTitle("NetWork Setting ERROR");
	}
	/* ���ʂɗ^���閼�̂��` */
	CV_TRACE_FUNCTION();
	/* ���ʂɗp����f�o�C�X���`�A�������uCPU�v�uOPENCL�v�őI�ׂ� */
	caffe_net.setPreferableTarget(DNN_TARGET_OPENCL);

	//�摜��ROI���W�̎󂯓n��

	//cout << xLeftBottom << "," << yLeftBottom << ","<< xwidth<<","<< yheight << endl;

	//imshow("aaa",cut_img);
	//Mat cut_img(frame, Rect(xRightTop - xLeftBottom, yRightTop - yLeftBottom, xLeftBottom, yLeftBottom));
	Mat caffe_inputBlob = blobFromImage(InputImg, 1.0f, Size(224, 224), Scalar(104, 117, 123), false);

	caffe_net.setInput(caffe_inputBlob, "data");
	// �o�͑w(Softmax)�̏o�͂��擾, �����ɗ\�����ʂ��i�[����Ă���
	Mat prob = caffe_net.forward("prob");
	int classId;
	double classProb;
	getMaxClass(prob, &classId, &classProb);
	//Caffe�ł̉񓚌��� a:(���M�p�[�Z���e�[�W)classNames.at(classID):�񓚂����p��̖��O
	double a = classProb;
	
	Result_IdProb.at<int>(0, 1) = classId;
	Result_IdProb.at<double>(0, 2) = classProb;

	//return Result_IdProb;
}
/* YOLO��Caffe��p�����p��o */
//void SurgicalDetection::Caffe_YOLO(String FileName)
//{
//	//40��ނ��炢�̃T���v�����ʃ��f��
//	//String modelTxt = "bvlc_googlenet.prototxt";
//	//String modelBin = "bvlc_googlenet.caffemodel";
//	//String labelTxt = "synset_words.txt";
//
//	//�p���(Caffe)
//	String modelTxt = "deploy_mine.prototxt";
//	String modelBin = "bvlc_googlenet_quick_mine_iter_10000.caffemodel";
//	String labelTxt = "labels.txt";
//	//�p��ROI�w��(YOLO)
//	String modelConfiguration = "E:/DEVEL/nambu_surgicaltooldetection/build/learning.cfg";
//	String modelBinary = "E:/DEVEL/nambu_surgicaltooldetection/build/learning_10000.weights";
//	//String sourceName = "E:/DEVEL/nambu_surgicaltooldetection/build/1.avi";
//
//	//TESTimportFile
//	//String imageFile = "E:/Visual Studio 2015/Projects/QtGuiApplication3/whity.JPG";
//	Net caffe_net, dark_net;
//	try
//	{
//		//Caffe_net
//		caffe_net = readNetFromCaffe(modelTxt, modelBin);;
//		//Dark_net
//		dark_net = readNetFromDarknet(modelConfiguration, modelBinary);
//	}
//	catch (const std::exception&)
//	{
//		//this->setWindowTitle("NetWork Setting ERROR");
//	}
//	/*
//	if (caffe_net.empty() || dark_net.empty())
//	{
//	this->setWindowTitle("NetWork Setting ERROR");
//	}
//	*/
//	/* ���ʂɗ^���閼�̂��` */
//	CV_TRACE_FUNCTION();
//	/* ���ʂɗp����f�o�C�X���`�A�������uCPU�v�uOPENCL�v�őI�ׂ� */
//	caffe_net.setPreferableTarget(DNN_TARGET_OPENCL);
//	dark_net.setPreferableTarget(DNN_TARGET_OPENCL);
//
//	int classNum = 0;
//	vector<string> classNamesVec;
//	ifstream classNamesFile("E:/DEVEL/nambu_surgicaltooldetection/build/learning.names");
//	if (classNamesFile.is_open())
//	{
//		string className = "";
//		while (std::getline(classNamesFile, className)) {
//			classNamesVec.push_back(className);
//			classNum++;
//		}
//	}
//	//�����܂ŋ@�B�w�K�̃Z�b�e�B���O
//	/*����̂ق��̏���*/
//	String ImportFile = "";
//	String imageFile = "";
//
//	//����̐؂�o�����摜�i�[�C�o�͌��ʊi�[
//	VideoCapture cap;
//	VideoWriter writer;
//	Mat frame;
//
//	int frameNo = 0;//�t���[���i���o�[
//	bool *checker = new bool[classNum];
//	//CSV�̐���
//	ofstream log;
//	String fileName;
//
//	/*�t�@�C���̃C���|�[�g*/
//	ImportFile = FileName;
//	//imageFile = this->fileName;
//
//	cap = VideoCapture(ImportFile);
//	if (!cap.isOpened())
//	{
//	//	this->setWindowTitle("Couldn't open image or video: ");
//		waitKey(0);
//	}
//
//	/*�o�͓���̏����o���ݒ�*/
//	writer = VideoWriter("output.wmv", VideoWriter::fourcc('W', 'M', 'V', '1'), cap.get(CV_CAP_PROP_FPS), cv::Size(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT)), true);
//
//	if (!writer.isOpened()) {
//		cout << "dame" << endl;
//	//	this->setWindowTitle("Couldn't write a video:");
//	}
//
//	/*���O�f�[�^�̏����o���ƃt�@�C���������݉\*/
//	fileName = "log.csv";
//	log.open(fileName, ios::trunc);  // ofstream log("log.csv")�ł��B��2������trunc�͓����t�@�C�����������ꍇ�ɂ͈ȑO�̓��e��S�ď����A�Ƃ����I�v�V�����B
//
//									 //EXCEL�V�[�g�̖`���Ɍ��o����t����
//	log << "frame,";
//	for (int i = 0; i < classNum; i++) {
//		log << i << ",";
//	}
//	log << endl;
//	/*���攻��*/
//
//	for (;;)
//	{
//		cap >> frame; // get a new frame from camera/video or read image
//		if (frame.empty())
//		{
//			cout << "hoge_fin" << endl;
//			//exit(-1);
//			break;
//		}
//
//		for (int i = 0; i < classNum; i++) {
//			checker[i] = false;
//		}
//
//		//color�`�����l���̒���
//		if (frame.channels() == 4) cvtColor(frame, frame, COLOR_BGRA2BGR);
//
//		//����
//		//cv::resize(img1, img1, cv::Size(), 0.3, 0.3);
//		//imshow("hoge", img1);
//		//imshow("hoge1", img);
//
//		//�摜�̕ό`
//		Mat dark_inputBlob = blobFromImage(frame, 1 / 255.F, Size(416, 416), Scalar(), true, false);
//		//cout << frame.cols << "," << frame.rows << endl;
//		//Convert Mat to batch of images
//		//�l�b�g�̃Z�b�e�B���O
//		// ���͑w�ɉ摜�����
//		dark_net.setInput(dark_inputBlob, "data");
//
//		// �t�H���[�h�p�X(���`�d)�̌v�Z������detection���ʂ������Ă���
//		Mat detectionMat = dark_net.forward("detection_out");   //compute output �ȑO�́AgetOutputsNames(net)[0]);
//		ostringstream ss;
//		//vector<double> layersTimings;
//		//double freq = getTickFrequency() / 1000;
//		//double time = dark_net.getPerfProfile(layersTimings) / freq;
//		//ss << "FPS: " << 1000 / time << "; time: " << time << " ms";
//		//putText(frame, ss.str(), Point(20, 20), 0, 0.5, Scalar(0, 0, 255));
//
//		//ROI�̕`����w��
//		float confidenceThreshold = 0.25;//�����ς���K�v�����鉽�p�[�Z���g�ȉ���ROI���͂���
//		//1*5�s��
//		Mat surgicaldetection_area = Mat_<float>(1, 5);
//		surgicaldetection_area.resize(10);//10*3
//		int threshold_truecount = 0;
//		//img�摜�ւ̏d��@detectionMat.rows�͂����炭�������ꂽROI�̌�
//		for (int i = 0; i < detectionMat.rows; i++)
//		{
//			//���H
//			const int probability_index = 5;
//			const int probability_size = detectionMat.cols - probability_index;
//			float *prob_array_ptr = &detectionMat.at<float>(i, probability_index);
//
//			size_t objectClass = max_element(prob_array_ptr, prob_array_ptr + probability_size) - prob_array_ptr;
//			float confidence = detectionMat.at<float>(i, (int)objectClass + probability_index);
//
//			if (confidence > confidenceThreshold)
//			{
//				if (threshold_truecount > 0) {
//					surgicaldetection_area.resize(++threshold_truecount);
//				}
//				else {
//					threshold_truecount++;
//				}
//				float x = detectionMat.at<float>(i, 0);
//				float y = detectionMat.at<float>(i, 1);
//				float width = detectionMat.at<float>(i, 2);
//				float height = detectionMat.at<float>(i, 3);
//				//�摜��ɂ�������W(xLeftButtom,yLeftBottom,xRightTop - xLeftBottom,yRightTop - yLeftBottom)
//				int xLeftBottom = static_cast<int>((x - width / 2) * frame.cols);
//				int yLeftBottom = static_cast<int>((y - height / 2) * frame.rows);
//				int xRightTop = static_cast<int>((x + width / 2) * frame.cols);
//				int yRightTop = static_cast<int>((y + height / 2) * frame.rows);
//				Rect object(xLeftBottom, yLeftBottom,
//					xRightTop - xLeftBottom,
//					yRightTop - yLeftBottom);
//				//Caffe�����Ȃ炱��
//				//�摜�؂���
//				if (xLeftBottom >= frame.cols) xLeftBottom = frame.cols;
//				if (yRightTop >= frame.rows) yRightTop = frame.rows;
//				if (xRightTop >= frame.cols) xRightTop = frame.cols;
//				if (yLeftBottom >= frame.rows) yLeftBottom = frame.rows;
//
//				int xwidth = xRightTop - xLeftBottom - 1;
//				int yheight = yRightTop - yLeftBottom - 1;
//
//				//�i�[
//				surgicaldetection_area.at<float>(threshold_truecount - 1, 0) = xLeftBottom;
//				surgicaldetection_area.at<float>(threshold_truecount - 1, 1) = yLeftBottom;
//				surgicaldetection_area.at<float>(threshold_truecount - 1, 2) = xwidth;
//				surgicaldetection_area.at<float>(threshold_truecount - 1, 3) = yheight;
//				surgicaldetection_area.at<float>(threshold_truecount - 1, 4) = detectionMat.at<float>(i, 0);//�����Ńo�b�N�Ŋm�F
//
//
//				//cout << xLeftBottom << "," << yLeftBottom << ","<< xwidth<<","<< yheight << endl;
//				Mat cut_img(frame, Rect(xLeftBottom, yLeftBottom, xwidth, yheight));
//
//				//imshow("aaa",cut_img);
//				//Mat cut_img(frame, Rect(xRightTop - xLeftBottom, yRightTop - yLeftBottom, xLeftBottom, yLeftBottom));
//				Mat caffe_inputBlob = blobFromImage(cut_img, 1.0f, Size(224, 224), Scalar(104, 117, 123), false);
//
//				caffe_net.setInput(caffe_inputBlob, "data");
//				// �o�͑w(Softmax)�̏o�͂��擾, �����ɗ\�����ʂ��i�[����Ă���
//				Mat prob = caffe_net.forward("prob");
//				int classId;
//				double classProb;
//				getMaxClass(prob, &classId, &classProb);
//				//Caffe�ł̉񓚌��� a:(���M�p�[�Z���e�[�W)classNames.at(classID):�񓚂����p��̖��O
//				double a = classProb;
//				vector<String> classNames = readClassNames(labelTxt);
//				String Caffe_classname = classNames.at(classId);
//				String Caffe_classprob = to_string(classProb * 100);
//				//this->setWindowTitle("Answer!!");
//				//ui.label->setText("Class is " + classname + "/" + "Acuracy is" + classprob);
//
//				rectangle(frame, object, Scalar(0, 255, 0));
//				//�摜��ւ�ROI�w�肳�ꂽ�����̖��O�w��
//				if (objectClass < classNamesVec.size())
//				{
//					ss.str("");
//					ss << confidence;
//					String conf(ss.str());
//					String label = "YOLO" + String(classNamesVec[objectClass]) + ": " + conf;
//					String label_2 = "Caffe" + Caffe_classname + ": " + Caffe_classprob;
//
//					int baseLine = 0;
//					Size labelSize = getTextSize(label_2, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
//					rectangle(frame, Rect(Point(xLeftBottom, yLeftBottom),
//						Size(labelSize.width, labelSize.height + baseLine)),
//						Scalar(255, 255, 255), CV_FILLED);
//					putText(frame, label, Point(xLeftBottom, yLeftBottom + labelSize.height),
//						FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
//				}
//			}
//		}//for (int i = 0; i < detectionMat.rows; i++)
//		//imshow("YOLO: Detections", frame);
//
//		//EXCEL�ւ̏o�͏���
//		log << frameNo << ",";
//		for (int i = 0; i < classNum; i++) {
//			if (checker[i]) {
//				log << "1,";
//			}
//			else {
//				log << "0,";
//			}
//		}
//		log << endl;
//		//����ւ̏�������
//		writer << frame;
//		if (waitKey(1) >= 0) exit(-1);
//	}//for(;;)
//	log.close();
//	waitKey(0);
//}