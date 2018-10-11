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

/* 各クラスのうち、最も確率が高いクラスを得る */
static void getMaxClass(const Mat &probBlob, int *classId, double *classProb)
{
	Mat probMat = probBlob.reshape(1, 1);
	Point classNumber;
	minMaxLoc(probMat, NULL, classProb, NULL, &classNumber);
	*classId = classNumber.x;
}

/* 学習時に用いたIDからラベルを検索する */
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

/* コンストラクタ */
SurgicalDetection::SurgicalDetection()
{
}
/* YOLOを用いた術具検出 */
void SurgicalDetection::Detection_YOLO(const Mat &ImageFile, Mat &surgicaldetection_area)
{
	Mat frame = ImageFile;//識別画像を渡す．
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
	//術具ROI指定(YOLO)
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
	/* 判別に与える名称を定義 */
	CV_TRACE_FUNCTION();
	/* 判別に用いるデバイスを定義、末尾を「CPU」「OPENCL」で選べる */
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
	//colorチャンネルの調節
	if (frame.channels() == 4) cvtColor(frame, frame, COLOR_BGRA2BGR);
	//画像の変形
	Mat dark_inputBlob = blobFromImage(frame, 1 / 255.F, Size(416, 416), Scalar(), true, false);
	dark_net.setInput(dark_inputBlob, "data");
	Mat detectionMat = dark_net.forward("detection_out");   //compute output 以前は、getOutputsNames(net)[0]);
	ostringstream ss;
	//ROIの描画個数指定
	float confidenceThreshold = 0.25;//ここ変える必要がある何パーセント以下のROIをはじく
	//返り値として返すROIをまとめたMat
	
	int threshold_truecount = 0;
	//img画像への重畳　detectionMat.rowsはおそらく生成されたROIの個数
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

			//格納
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
/* Caffeを用いた術具検出 */
void SurgicalDetection::Detection_Caffe(const Mat &ImageFile, Mat &Result_IdProb)
{
	Mat InputImg = ImageFile;
	Result_IdProb = Mat_<double>(1, 2);
	
	if (InputImg.empty()) return;
	//術具判別(Caffe)
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
	/* 判別に与える名称を定義 */
	CV_TRACE_FUNCTION();
	/* 判別に用いるデバイスを定義、末尾を「CPU」「OPENCL」で選べる */
	caffe_net.setPreferableTarget(DNN_TARGET_OPENCL);

	//画像のROI座標の受け渡し

	//cout << xLeftBottom << "," << yLeftBottom << ","<< xwidth<<","<< yheight << endl;

	//imshow("aaa",cut_img);
	//Mat cut_img(frame, Rect(xRightTop - xLeftBottom, yRightTop - yLeftBottom, xLeftBottom, yLeftBottom));
	Mat caffe_inputBlob = blobFromImage(InputImg, 1.0f, Size(224, 224), Scalar(104, 117, 123), false);

	caffe_net.setInput(caffe_inputBlob, "data");
	// 出力層(Softmax)の出力を取得, ここに予測結果が格納されている
	Mat prob = caffe_net.forward("prob");
	int classId;
	double classProb;
	getMaxClass(prob, &classId, &classProb);
	//Caffeでの回答結果 a:(自信パーセンテージ)classNames.at(classID):回答した術具の名前
	double a = classProb;
	
	Result_IdProb.at<int>(0, 1) = classId;
	Result_IdProb.at<double>(0, 2) = classProb;

	//return Result_IdProb;
}
/* YOLOとCaffeを用いた術具検出 */
//void SurgicalDetection::Caffe_YOLO(String FileName)
//{
//	//40種類くらいのサンプル判別モデル
//	//String modelTxt = "bvlc_googlenet.prototxt";
//	//String modelBin = "bvlc_googlenet.caffemodel";
//	//String labelTxt = "synset_words.txt";
//
//	//術具判別(Caffe)
//	String modelTxt = "deploy_mine.prototxt";
//	String modelBin = "bvlc_googlenet_quick_mine_iter_10000.caffemodel";
//	String labelTxt = "labels.txt";
//	//術具ROI指定(YOLO)
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
//	/* 判別に与える名称を定義 */
//	CV_TRACE_FUNCTION();
//	/* 判別に用いるデバイスを定義、末尾を「CPU」「OPENCL」で選べる */
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
//	//ここまで機械学習のセッティング
//	/*動画のほうの処理*/
//	String ImportFile = "";
//	String imageFile = "";
//
//	//動画の切り出した画像格納，出力結果格納
//	VideoCapture cap;
//	VideoWriter writer;
//	Mat frame;
//
//	int frameNo = 0;//フレームナンバー
//	bool *checker = new bool[classNum];
//	//CSVの生成
//	ofstream log;
//	String fileName;
//
//	/*ファイルのインポート*/
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
//	/*出力動画の書き出し設定*/
//	writer = VideoWriter("output.wmv", VideoWriter::fourcc('W', 'M', 'V', '1'), cap.get(CV_CAP_PROP_FPS), cv::Size(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT)), true);
//
//	if (!writer.isOpened()) {
//		cout << "dame" << endl;
//	//	this->setWindowTitle("Couldn't write a video:");
//	}
//
//	/*ログデータの書き出しとファイル書き込み可能*/
//	fileName = "log.csv";
//	log.open(fileName, ios::trunc);  // ofstream log("log.csv")でも可。第2引数のtruncは同名ファイルがあった場合には以前の内容を全て消す、というオプション。
//
//									 //EXCELシートの冒頭に見出しを付ける
//	log << "frame,";
//	for (int i = 0; i < classNum; i++) {
//		log << i << ",";
//	}
//	log << endl;
//	/*動画判別*/
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
//		//colorチャンネルの調節
//		if (frame.channels() == 4) cvtColor(frame, frame, COLOR_BGRA2BGR);
//
//		//検証
//		//cv::resize(img1, img1, cv::Size(), 0.3, 0.3);
//		//imshow("hoge", img1);
//		//imshow("hoge1", img);
//
//		//画像の変形
//		Mat dark_inputBlob = blobFromImage(frame, 1 / 255.F, Size(416, 416), Scalar(), true, false);
//		//cout << frame.cols << "," << frame.rows << endl;
//		//Convert Mat to batch of images
//		//ネットのセッティング
//		// 入力層に画像を入力
//		dark_net.setInput(dark_inputBlob, "data");
//
//		// フォワードパス(順伝播)の計算ここにdetection結果が入っている
//		Mat detectionMat = dark_net.forward("detection_out");   //compute output 以前は、getOutputsNames(net)[0]);
//		ostringstream ss;
//		//vector<double> layersTimings;
//		//double freq = getTickFrequency() / 1000;
//		//double time = dark_net.getPerfProfile(layersTimings) / freq;
//		//ss << "FPS: " << 1000 / time << "; time: " << time << " ms";
//		//putText(frame, ss.str(), Point(20, 20), 0, 0.5, Scalar(0, 0, 255));
//
//		//ROIの描画個数指定
//		float confidenceThreshold = 0.25;//ここ変える必要がある何パーセント以下のROIをはじく
//		//1*5行列
//		Mat surgicaldetection_area = Mat_<float>(1, 5);
//		surgicaldetection_area.resize(10);//10*3
//		int threshold_truecount = 0;
//		//img画像への重畳　detectionMat.rowsはおそらく生成されたROIの個数
//		for (int i = 0; i < detectionMat.rows; i++)
//		{
//			//個数？
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
//				//画像上における座標(xLeftButtom,yLeftBottom,xRightTop - xLeftBottom,yRightTop - yLeftBottom)
//				int xLeftBottom = static_cast<int>((x - width / 2) * frame.cols);
//				int yLeftBottom = static_cast<int>((y - height / 2) * frame.rows);
//				int xRightTop = static_cast<int>((x + width / 2) * frame.cols);
//				int yRightTop = static_cast<int>((y + height / 2) * frame.rows);
//				Rect object(xLeftBottom, yLeftBottom,
//					xRightTop - xLeftBottom,
//					yRightTop - yLeftBottom);
//				//Caffe入れるならここ
//				//画像切り取り
//				if (xLeftBottom >= frame.cols) xLeftBottom = frame.cols;
//				if (yRightTop >= frame.rows) yRightTop = frame.rows;
//				if (xRightTop >= frame.cols) xRightTop = frame.cols;
//				if (yLeftBottom >= frame.rows) yLeftBottom = frame.rows;
//
//				int xwidth = xRightTop - xLeftBottom - 1;
//				int yheight = yRightTop - yLeftBottom - 1;
//
//				//格納
//				surgicaldetection_area.at<float>(threshold_truecount - 1, 0) = xLeftBottom;
//				surgicaldetection_area.at<float>(threshold_truecount - 1, 1) = yLeftBottom;
//				surgicaldetection_area.at<float>(threshold_truecount - 1, 2) = xwidth;
//				surgicaldetection_area.at<float>(threshold_truecount - 1, 3) = yheight;
//				surgicaldetection_area.at<float>(threshold_truecount - 1, 4) = detectionMat.at<float>(i, 0);//ここでバックで確認
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
//				// 出力層(Softmax)の出力を取得, ここに予測結果が格納されている
//				Mat prob = caffe_net.forward("prob");
//				int classId;
//				double classProb;
//				getMaxClass(prob, &classId, &classProb);
//				//Caffeでの回答結果 a:(自信パーセンテージ)classNames.at(classID):回答した術具の名前
//				double a = classProb;
//				vector<String> classNames = readClassNames(labelTxt);
//				String Caffe_classname = classNames.at(classId);
//				String Caffe_classprob = to_string(classProb * 100);
//				//this->setWindowTitle("Answer!!");
//				//ui.label->setText("Class is " + classname + "/" + "Acuracy is" + classprob);
//
//				rectangle(frame, object, Scalar(0, 255, 0));
//				//画像上へのROI指定された部分の名前指定
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
//		//EXCELへの出力処理
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
//		//動画への書き込み
//		writer << frame;
//		if (waitKey(1) >= 0) exit(-1);
//	}//for(;;)
//	log.close();
//	waitKey(0);
//}