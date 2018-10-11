#include"surgicalposition.h"
//-------------------------------------------------------------------------
// 腫瘍の輝度値設定
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkResampleImageFilter.h"
#include <itkCastImageFilter.h>
#include "itkBinaryThresholdImageFilter.h"
#include "itkConfidenceConnectedImageFilter.h"
#include <itkConnectedThresholdImageFilter.h>
#include <itkGrayscaleDilateImageFilter.h>
#include "itkMaskImageFilter.h"
#include "itkMeanImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkAddImageFilter.h"
#include "itkAndImageFilter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkImageBase.h"

// トラッカー
#include <iostream>
#include <math.h>
#include <cstdlib>
#include "igtlOSUtil.h"
#include "igtlTransformMessage.h"
#include "igtlClientSocket.h"
#include "fstream"
#include "direct.h"

#include "vcl_iostream.h"
#include "vcl_fstream.h"
#include "direct.h"
#include "time.h"
#include <String>


using namespace std;

SurgicalPosition::SurgicalPosition()
{

}

string Get_file_name(const string &pathname);
string Get_directory_name(const string &pathname);
void Cmdp_msg(time_t time, igtl::Matrix4x4& matrix, double &fps);
int Brain_Discrimination(double maskmax);
void create_sulci_list(string *Lsulci_list, string *Rsulci_list);

const unsigned int Dimension = 3;


void SurgicalPosition::Detect_Position(string fn_mri, string fn_log, string fn_sulci, bool slicer_check)
{
	time_t start_time = time(NULL);
	char *input_navilog;
	char *input_mri;
	char *input_sulci;
	string pre_file;  // 元ファイル
	string name_navilog;
	string name_mri;
	string name_sulcus_mri;
	string directory_name;
	string input_directory_name;
	string temp_directory_name = "E:/DEVEL/SurgicalProcessIdentificationSystem/Build/src/temp/";
	string log_file; //術具ログファイル名
	ofstream ofs("test.csv");
	int sulcus = 0;//脳溝判定するか否か

	string sulci_left[] =
	{
		"F.C.L.a._left",//1
		"F.C.L.r.ant._left",
		"F.C.L.r.diag._left",
		"F.C.L.r.sc.ant._left",
		"F.C.M.ant._left",
		"F.Cal.ant.-Sc.Cal._left",
		"F.I.P.Po.C.inf._left",
		"F.I.P.r.int.1_left",
		"F.P.O._left",
		"OCCIPITAL_left",
		"S.C._left",//11
		"S.Call._left",
		"S.F.inf._left",
		"S.F.int._left",
		"S.F.marginal._left",
		"S.F.orbitaire._left",
		"S.F.sup._left",
		"S.Li.ant._left",
		"S.O.T.lat.ant._left",
		"S.O.T.lat.med._left",//20
		"S.O.p._left",
		"S.Or._left",
		"S.Pa.sup._left",//23
		"S.Pe.C.inf._left",
		"S.Pe.C.marginal._left",
		"S.Pe.C.sup._left",
		"S.R.inf._left",
		"S.T.i.ant._left",
		"S.T.pol._left",
		"S.T.s.ter.asc.ant._left",//30
		"S.p.C._left",
		"ventricle_left",
		"F.C.L.p._left",
		"F.C.L.r.asc._left",
		"F.C.L.r.retroC.tr._left",
		"F.C.L.r.sc.post._left",
		"F.C.M.post._left",
		"F.Coll._left",
		"F.I.P._left",//39
		"F.I.P.r.int.2_left",
		"INSULA_left",
		"S.C.LPC._left",
		"S.C.sylvian._left",//43
		"S.Cu._left",
		"S.F.inf.ant._left",
		"S.F.inter._left",
		"S.F.median._left",
		"S.F.polaire.tr._left",
		"S.GSM._left",
		"S.Li.post._left",//50
		"S.O.T.lat.int._left",
		"S.O.T.lat.post._left",
		"S.Olf._left",
		"S.Pa.int._left",
		"S.Pa.t._left",
		"S.Pe.C.inter._left",
		"S.Pe.C.median._left",
		"S.Po.C.sup._left",
		"S.Rh._left",
		"S.T.i.post._left",//60
		"S.T.s._left",
		"S.T.s.ter.asc.post._left",
		"S.s.P._left",
		"unknown"//64
	};

	string sulci_right[] =
	{
		"F.C.L.a._right",
		"F.C.L.r.ant._right",
		"F.C.L.r.diag._right",
		"F.C.L.r.sc.ant._right",
		"F.C.M.ant._right",
		"F.Cal.ant.-Sc.Cal._right",
		"F.I.P.Po.C.inf._right",
		"F.I.P.r.int.1_right",
		"F.P.O._right",
		"OCCIPITAL_right",//10
		"S.C._right",//11
		"S.Call._right",
		"S.F.inf._right",
		"S.F.int._right",
		"S.F.marginal._right",
		"S.F.orbitaire._right",
		"S.F.sup._right",
		"S.Li.ant._right",
		"S.O.T.lat.ant._right",
		"S.O.T.lat.med._right",//20
		"S.O.p._right",
		"S.Or._right",
		"S.Pa.sup._right",
		"S.Pe.C.inf._right",
		"S.Pe.C.marginal._right",
		"S.Pe.C.sup._right",
		"S.R.inf._right",
		"S.T.i.ant._right",
		"S.T.pol._right",
		"S.T.s.ter.asc.ant._right",//30
		"S.p.C._right",
		"ventricle_right",
		"F.C.L.p._right",
		"F.C.L.r.asc._right",
		"F.C.L.r.retroC.tr._right",
		"F.C.L.r.sc.post._right",
		"F.C.M.post._right",
		"F.Coll._right",
		"F.I.P._right",
		"F.I.P.r.int.2_right",//40
		"INSULA_right",
		"S.C.LPC._right",
		"S.C.sylvian._right",//43
		"S.Cu._right",
		"S.F.inf.ant._right",
		"S.F.inter._right",
		"S.F.median._right",
		"S.F.polaire.tr._right",
		"S.GSM._right",
		"S.Li.post._right",//50
		"S.O.T.lat.int._right",
		"S.O.T.lat.post._right",
		"S.Olf._right",
		"S.Pa.int._right",
		"S.Pa.t._right",
		"S.Pe.C.inter._right",
		"S.Pe.C.median._right",
		"S.Po.C.sup._right",
		"S.Rh._right",
		"S.T.i.post._right",//60
		"S.T.s._right",
		"S.T.s.ter.asc.post._right",
		"S.s.P._right",
		"unknown"//64
	};
	cout << fn_mri << fn_log << fn_sulci << endl;
	cout << slicer_check << endl;

	/*Slicerのポート開放*/
	char*  hostname = "localhost";
	int    port = 18944;
	// 顕微鏡画像に合わせるためfpsを29.97に設定
	double fps = 29.97;
	// 元は20
	// double fps = 20;
	int interval = (int)(1000.0 / fps);
	int r = -1;
	//脳溝探索アルゴリズムカウンター
	int sulcus_count = 0;

	time_t time_Value[99];
	time_t time_Delay;
	int time_output_counter = 0;
	double maskmax_Value[99];
	double masksulcus_Value[99];

	typedef itk::Image<float, Dimension> ImageType2;
	typedef itk::ImageFileReader<ImageType2> ReaderType2;
	ReaderType2::Pointer reader2 = ReaderType2::New();

	//脳溝用
	typedef itk::Image<float, Dimension> ImageType3;
	typedef itk::ImageFileReader<ImageType3> ReaderType3;
	ReaderType3::Pointer reader3 = ReaderType3::New();
	
	try {
		reader2->SetFileName(fn_mri);
		reader2->Update();
		reader3->SetFileName(fn_sulci);
		reader3->Update();
	}
	catch (...) {
		exit(-1);
	}

	//--------------------------------------------------------
	// 画像内の最大輝度値と最小輝度値を求める
	//maskMinimumMaximumImageCalculator
	//typedef double MinMaxCalculatorPixelType;
	typedef itk::Image< float, Dimension > MinMaxCalculatorImageType;
	typedef itk::MinimumMaximumImageCalculator<MinMaxCalculatorImageType> MinMaxCalculator;

	typedef itk::Image< float, Dimension > MinMaxCalculatorImageType3;
	typedef itk::MinimumMaximumImageCalculator<MinMaxCalculatorImageType> MinMaxCalculator3;


	MinMaxCalculator::Pointer maskminmaxCalculator = MinMaxCalculator::New();
	MinMaxCalculator3::Pointer maskminmaxCalculator3 = MinMaxCalculator3::New();
	double maskmin, maskmax;
	double maskmin_sulcus, maskmax_sulcus;

	//tumor_mri用
	try {
		maskminmaxCalculator->SetImage(reader2->GetOutput());
		maskminmaxCalculator->Compute();
		// 腫瘍部分：77，脳領域：255，脳の外：0
		maskmin = maskminmaxCalculator->GetMinimum();  // maskmin = 0
		maskmax = maskminmaxCalculator->GetMaximum();  // maskmaxmaskMinimumMaximumImageCalculator error" << endl; = 777
	}
	catch (...) {
		cerr << "Not Get mask MIN MAX Image Caluclator";
	}
	cout << "maskMinimumMaximumImageCalculator done" << maskmax << endl;


	igtl::ClientSocket::Pointer socket;
	socket = igtl::ClientSocket::New();
	if (slicer_check == 1) {
		r = socket->ConnectToServer(hostname, port);
		if (r != 0)
		{
			std::cerr << "Cannot connect to the server." << std::endl;
			while (1) {};
			exit(0);
		}
	}
	//------------------------------------------------------------
	// Allocate Transform Message Class

	igtl::TransformMessage::Pointer transMsg;
	transMsg = igtl::TransformMessage::New();
	transMsg->SetDeviceName("Tracker");

	//------------------------------------------------------------
	// Allocate TimeStamp class
	igtl::TimeStamp::Pointer ts;
	ts = igtl::TimeStamp::New();

	//---------------------------------
	/*ここまで*/

	//import file

	//string input_file;//術具ログデータ
	//input_file = input_directory_name + log_file;
	ifstream ifs(fn_log);
	ifstream ifs1(fn_log);

	string output_file;//時間のみ
	output_file = temp_directory_name + "temp.csv";
	ofstream temp_ofs(output_file);

	string count_temp_file;//時間のみ
	count_temp_file = temp_directory_name + "temp.csv";
	ifstream temp_ifs(count_temp_file);

	string count_output_file;//術具ログの数+次のログまでの秒数
	count_output_file = temp_directory_name + "count.csv";
	ofstream log_ofs(count_output_file);

	//string input_navilog;//術具ログデータ
	//input_navilog = input_directory_name + log_file;
	
	string input_time_data;//ログデータの数+次のログまでの秒数
	input_time_data =  temp_directory_name + "count.csv";
	ifstream ifs_time(input_time_data);

	//------------------------------------------------------------
	// make temp
	double  end_time, delay_time;
	delay_time = 0;//動画の時間に対するtimeデータの遅延(tracker_patient_log.140723_1241.csv : 93s)

	end_time = 1000000000000000000 + delay_time;
	//文字列格納個
	string str;
	auto flug = false;
	string Inst = "Instrument";
	string Inst_M = "InstrumentML";
	string Inst_ML = "InstrumentL";


	//ML:バイポーラ，L：電極
	//temp_ofs（ログの時間のみを記述したもの）を出力
	while (getline(ifs, str))
	{
		if (str.rfind(string(Inst)) < 62025) {
			time_t time = 0;
			time = stod(str.substr(7, str.rfind(string(Inst)) - 9));
			//if (time >= start_time2 && time < end_time){
			temp_ofs << time << endl;
			//}
		}
	}

	//------------------------------------------------------
	// count

	string count_temp_str = "";
	string count_temp_pre_str = "";
	int count = 0;//1秒あたりの術具ログの数
	int log_interval = 1;//前の術具ログとの時間差(s)
	bool first_count = true;

	while (getline(temp_ifs, count_temp_str))
	{
		if (count_temp_pre_str == count_temp_str) {
			count++;
		}
		else if (count_temp_pre_str != "") {
			if (count != 0) {
				log_ofs << count_temp_pre_str << ',' << count << ',' << log_interval << endl;
				log_interval = stod(count_temp_str) - stod(count_temp_pre_str);
			}
			count = 1;
		}
		count_temp_pre_str = count_temp_str;
	}

	//Check Feature Point
	const     unsigned int   Dimension = 3;
	typedef   float  PixelType;
	typedef   itk::Image< PixelType, Dimension > ImageType;
	typedef   itk::Image< PixelType, Dimension > outputImageType;
	//脳溝用
	typedef   itk::Image< PixelType, Dimension > ImageType3;
	typedef   itk::Image< PixelType, Dimension > outputImageType3;


	ImageType::RegionType region2;
	ImageType::IndexType start;
	start.Fill(0);
	region2.SetIndex(start);
	region2.SetSize(reader2->GetOutput()->GetLargestPossibleRegion().GetSize());
	ImageType::Pointer image = ImageType::New();
	image->SetRegions(region2);


	//脳溝用
	ImageType::RegionType region3;
	ImageType::IndexType start_3;
	start_3.Fill(0);
	region3.SetIndex(start_3);
	region3.SetSize(reader2->GetOutput()->GetLargestPossibleRegion().GetSize());
	ImageType::Pointer image_3 = ImageType::New();
	image_3->SetRegions(region3);

	/*MRI画像の物理座標（origin）とボクセルとボクセルの間の間隔（mm），画像の傾き（mm）*/
	image->SetOrigin(reader2->GetOutput()->GetOrigin());
	image->SetSpacing(reader2->GetOutput()->GetSpacing());
	image->SetDirection(reader2->GetOutput()->GetDirection());

	image->Allocate();
	image->FillBuffer(itk::NumericTraits< PixelType >::Zero);

	/*MRI画像の物理座標（origin）とボクセルとボクセルの間の間隔（mm），画像の傾き（mm）*/
	image_3->SetOrigin(reader3->GetOutput()->GetOrigin());
	image_3->SetSpacing(reader3->GetOutput()->GetSpacing());
	image_3->SetDirection(reader3->GetOutput()->GetDirection());

	image_3->Allocate();
	image_3->FillBuffer(itk::NumericTraits< PixelType >::Zero);

	/*書き込み*/
	typedef itk::AddImageFilter <ImageType, ImageType >
		AddImageFilterType;

	AddImageFilterType::Pointer addFilter
		= AddImageFilterType::New();

	typedef itk::ImageFileWriter<  ImageType  > WriterType;
	WriterType::Pointer writer = WriterType::New();

	//------------------------------------------------------
	//main loop
	//エクセル横最大幅:62025

	str = "";
	string time_str;
	time_t pre_time = 1;
	int output_count = 0;
	int Inst_num = 0;//1:ML,M:2,other:0

	cout << "ここからメインループ" << endl;
	while (getline(ifs1, str))
	{
		Inst_num = 0;
		if (str.rfind(string(Inst)) < 62025) {
			//timeの取得および術具の識別
			time_t time = 0;
			time = stod(str.substr(7, str.rfind(string(Inst)) - 9));
			cout << time << endl;
			//substr:str文字列7番目(TDATA, )からstr.rfind("Instrument__"-9要素だけの文字列を抜き取る(.xxxxxxxxxx)xは数値)
			//ログとブレインラボ（Slicer），ログと顕微鏡画像の時間を合わせるための補正
			//if (time >= start_time2 && time < end_time){
			//術具パラメータ抽出
			if (str.find(string(Inst_ML)) != string::npos) {
				str.erase(0, str.rfind(string(Inst_ML)) + 17);//+16or17
				Inst_num = 1;
			}
			else {
				str.erase(0, str.rfind(string(Inst_M)) + 17);//+16or17
				Inst_num = 2;
			}
			size_t index = 0;//術具のパラメータ(positionまたはorientation)の先頭位置
			size_t size = 0; //術具のパラメータのサイズ
			float temp_matrix[12];//術具のパラメータを一時的に格納

			//float型で術具のパラメータを取得
			for (int i = 0; i < 12; i++) {
				size = str.find(',', index);
				string temp_str = "";
				temp_str = str.substr(index, size);
				temp_matrix[i] = 0;
				temp_matrix[i] = stof(temp_str);
				index = size + 1;
			}
		
			igtl::Matrix4x4 matrix;
			/*
			matrix[0][3] = temp_matrix[0];
			matrix[1][3] = temp_matrix[1];
			matrix[2][3] = temp_matrix[2];
			matrix[0][0] = temp_matrix[3];
			matrix[0][1] = temp_matrix[4];
			matrix[0][2] = temp_matrix[5];
			matrix[1][0] = temp_matrix[6];
			matrix[1][1] = temp_matrix[7];
			matrix[1][2] = temp_matrix[8];
			matrix[2][0] = temp_matrix[9];
			matrix[2][1] = temp_matrix[10];
			matrix[2][2] = temp_matrix[11];
			*/

			matrix[0][3] = temp_matrix[0];
			matrix[1][3] = temp_matrix[1];
			matrix[2][3] = temp_matrix[2];
			matrix[0][0] = temp_matrix[3];
			matrix[0][1] = temp_matrix[4];
			matrix[0][2] = temp_matrix[5];
			matrix[1][0] = temp_matrix[6];
			matrix[1][1] = temp_matrix[7];
			matrix[1][2] = temp_matrix[8];
			matrix[2][0] = temp_matrix[9];
			matrix[2][1] = temp_matrix[10];
			matrix[2][2] = temp_matrix[11];
			//}//ifowari

			//log_ofs << time << ',' 
			//	<< matrix[0][3] << ',' << matrix[1][3] << ',' << matrix[2][3] << ','
			//	<< matrix[0][0] << ' ' << matrix[0][1] << ' ' << matrix[0][2] << ','
			//	<< matrix[1][0] << ' ' << matrix[1][1] << ' ' << matrix[1][2] << ','
			//	<< matrix[2][0] << ' ' << matrix[2][1] << ' ' << matrix[2][2] << endl;


			//originの値取得
			const MinMaxCalculatorImageType::PointType & origin = reader2->GetOutput()->GetOrigin();
			MinMaxCalculatorImageType::PointType origin_ras = reader2->GetOutput()->GetOrigin();
			const MinMaxCalculatorImageType::SpacingType & spacing = reader2->GetOutput()->GetSpacing();
			MinMaxCalculatorImageType::SizeType image_size = reader2->GetOutput()->GetLargestPossibleRegion().GetSize();
			MinMaxCalculatorImageType::DirectionType direction = reader2->GetOutput()->GetDirection();

			cout << "size " << image_size << endl;
			cout << "direction" << endl;
			cout << direction << endl;
			cout << "origin" << origin << endl;

			//MinMaxCalculatorImageType::PointType new_origin;
			//double new_origin[3] = {0, 0, 0};
			//MinMaxCalculatorImageType::DirectionType temp_origin;

			//emp_origin = { 1, 1, 1 };

			// new_origin[0] = origin[0] * direction(0,0) + origin[1] * direction(1,0) + origin[2] * direction(2,0);

			// cout << "new_origin: " << new_origin << endl;
			// cout << "direction(0,0): " << direction(0, 1) << endl;
			ts->GetTime();
			transMsg->SetMatrix(matrix);
			transMsg->SetTimeStamp(ts);
			transMsg->Pack();
			

			//fps可変、ログの時間が飛んだ場合の対処
			if (time != pre_time) {
				getline(ifs_time, time_str);
				size_t temp_index = 0;//術具のパラメータ(positionまたはorientation)の先頭位置
				size_t temp_size = 0; //術具のパラメータのサイズ
				int temp_time_matrix[3];//術具のパラメータを一時的に格納
				for (int i = 0; i < 3; i++) {
					cout << time_str << endl;
					temp_size = time_str.find(',', temp_index);
					string temp_str = "";
					temp_str = time_str.substr(temp_index, temp_size);
					temp_time_matrix[i] = 0;
					temp_time_matrix[i] = stoi(temp_str);
					temp_index = temp_size + 1;
				}
				for (int i = 0; i < temp_time_matrix[2] - 1; i++) {
					//cout << 1000 * (temp_time_matrix[2] - 1);
					cout << "nodata : stop time " << temp_time_matrix[2] - 1 - i << "s" << endl;
					//igtl::Sleep(1000);
				}
				fps = temp_time_matrix[1];
				interval = (int)(1000.0 / fps);
			}
			pre_time = time;
		
			typedef itk::MinimumMaximumImageCalculator<MinMaxCalculatorImageType> MinMaxCalculator;
			MinMaxCalculatorImageType::IndexType index2;
			MinMaxCalculatorImageType::SizeType size2;


			//脳溝判別用画像に使用
			/*どれが変数名かわからん*/

			typedef itk::MinimumMaximumImageCalculator<MinMaxCalculatorImageType> MinMaxCalculator3;
			MinMaxCalculatorImageType::IndexType index3;
			MinMaxCalculatorImageType::SizeType size3;

			int size_x = 15, size_y = 15, size_z = 10;

			////http://public.kitware.com/pipermail/insight-users/2010-June/037400.html
			//typedef itk::Matrix<double, 3, 3> MatrixType;
			//MatrixType tempM, tempMinv;
			//tempM(0, 0) = direction[0][0] * spacing[0];
			//tempM(0, 1) = direction[0][1] * spacing[1];
			//tempM(0, 2) = direction[0][2] * spacing[2];
			//tempM(1, 0) = direction[1][0] * spacing[0];
			//tempM(1, 1) = direction[1][1] * spacing[1];
			//tempM(1, 2) = direction[1][2] * spacing[2];
			//tempM(2, 0) = direction[2][0] * spacing[0];
			//tempM(2, 1) = direction[2][1] * spacing[1];
			//tempM(2, 2) = direction[2][2] * spacing[2];
			//cout << "tempM" << tempM << endl;
			//tempMinv = tempM.GetInverse();

			//index2[0] = tempMinv(0, 0) * (matrix[0][3] - origin[0]) +
			//			tempMinv(0, 1) * (matrix[1][3] - origin[1]) +
			//		    tempMinv(0, 2) * (matrix[2][3] - origin[2]);
			//index2[1] = tempMinv(1, 0) * (matrix[0][3] - origin[0]) + 
			//			tempMinv(1, 1) * (matrix[1][3] - origin[1]) + 
			//			tempMinv(1, 2) * (matrix[2][3] - origin[2]);
			//index2[2] = tempMinv(2, 0) * (matrix[0][3] - origin[0]) + 
			//			tempMinv(2, 1) * (matrix[1][3] - origin[1]) +
			//			tempMinv(2, 2) * (matrix[2][3] - origin[2]);


			//物理座標->画像座標変換
			ImageType::Pointer testimage = ImageType::New();
			testimage->CopyInformation(reader2->GetOutput());
			ImageType::PointType testPoint, testPoint2;
			testPoint[0] = (-1) * matrix[0][3];//実際の座標値にマイナス必須
			testPoint[1] = (-1) * matrix[1][3];
			testPoint[2] = matrix[2][3];
			ImageType::IndexType testindex;
			testimage->TransformPhysicalPointToIndex(testPoint, testindex);
			cout << "変換後index: " << testindex << endl;

			index2 = testindex;


			cout << "変換後index: " << index2 << endl;

			/*軌跡描画*/
			if (index2[0]>0 && index2[0]<512 && index2[1]>0 && index2[1]<512 && index2[2]>0 && index2[2]<96) {
				//cout<<"zoi"<< endl;
				image->SetPixel(index2, 10000);
			}

			cout << "index2:" << index2 << endl;

			size2[0] = 20;
			size2[1] = 20;
			size2[2] = 12;

			size3[0] = size_x;
			size3[1] = size_y;
			size3[2] = size_z;

			MinMaxCalculatorImageType::RegionType region;
			region.SetIndex(index2);
			region.SetSize(size2);


			MinMaxCalculatorImageType::RegionType region3;
			region3.SetIndex(index2);
			region3.SetSize(size3);

			MinMaxCalculator::Pointer maskminmaxCalculator2 = MinMaxCalculator::New();
			//脳溝用

			//double maskmin2, maskmax2;
			double  maskmax2;
			double  maskmax3;

			try {
				maskminmaxCalculator2->SetImage(reader2->GetOutput());
				maskminmaxCalculator2->SetRegion(region);
				maskminmaxCalculator2->Compute();
				// 腫瘍部分：777，脳領域：255，脳の外：0
				//maskmin2 = maskminmaxCalculator2->GetMinimum();  // maskmin = 0
				maskmax2 = maskminmaxCalculator2->GetMaximum();  // maskmax = 255
			}
			catch (...) {
				cerr << "maskMinimumMaximumImageCalculator error--------------hoge" << endl;
			}
			cout << "maskMinimumMaximumImageCalculator done" << endl;
			cout << "max: " << maskmax2 << endl;

			//	cout << "min pixel value : " << min << endl << "max pixel value : " << max << endl;

			Cmdp_msg(time - delay_time, matrix, fps);


			//術具位置判別処理
			//	Brain_Discrimination(maskmax2);

			//脳溝判別処理
			//術具位置判別処理

			//sulcus_mri用


			//脳溝における最大きどち取得

			if (Brain_Discrimination(maskmax2) == 1) {
				MinMaxCalculator3::Pointer maskminmaxCalculator3 = MinMaxCalculator3::New();
				sulcus = 0;
				sulcus_count = 0;
				while (sulcus == 0) {
					try {
						maskminmaxCalculator3->SetImage(reader3->GetOutput());
						maskminmaxCalculator3->SetRegion(region3);
						maskminmaxCalculator3->Compute();
						// 腫瘍部分：77，脳領域：255，脳の外：0
						maskmax_sulcus = maskminmaxCalculator3->GetMaximum();  // maskmaxmaskMinimumMaximumImageCalculator error" << endl; = 777
					}
					catch (...) {
						cerr << "--------------------------------Not Get sulcus_mask MIN MAX Image Caluclator--------------------------";
						sulcus = -1;
						//sulci_left[63];
					}
					if (maskmax_sulcus == 0) {
						//cout << "aaaa" << endl;
						size3[0] = size3[0] + 5;
						size3[1] = size3[1] + 5;
						size3[2] = size3[2] + 5;
						region3.SetSize(size3);
						if (sulcus_count >= 5) {
							sulcus = -2;
						}
						else {
							sulcus_count = sulcus_count + 1;
						}
					}
					else {
						sulcus = 1;
						sulcus_count = 0;
						size3[0] = size_x;
						size3[1] = size_y;
						size3[2] = size_z;
						//cout << "iiii" << endl;
					}
				}
				cout << "--------------------術具に近接している脳溝名----------------------" << endl;
				cout << "max: " << sulci_left[int(maskmax_sulcus)] << endl;
				//cout <<"sulcus:"<<sulci_left[(int)maskmax_sulcus]<<endl;;
				//Sulcus_Discrimination(maskmax_sulcus,*Lsulci_list,*Rsulci_list);
			}
			else {
				maskmax_sulcus = 0;
			}


			/*csvファイルにて保存*/
			time_Value[time_output_counter] = time - delay_time;
			maskmax_Value[time_output_counter] = maskmax2;
			masksulcus_Value[time_output_counter] = maskmax_sulcus;
			time_output_counter++;

			//フレームレート調整可能以下コメントアウト文は1秒間に20切り出します
			/*if(time_output_counter==100){
			for(int i=0;i<100;i++){
			ofs<<"No"<<i<<","<<time_Value[i]<<","<<maskmax_Value[i]<<","<<masksulcus_Value[i]<<endl;
			}
			time_t time_Value[99]={};
			int maskmax_Value[99]={};
			double masksulcus_Value[99]={};

			time_output_counter=0;
			}*/
			/*ここまで*/

			//1秒間に1切り出し
			if (time_output_counter == 100) {
				for (int i = 0; i<100; i = i + 20) {
					if (time_Value[i] == time_Value[i + 20] - 1) {
						ofs << "No" << i << "," << time_Value[i] << "," << Inst_num << "," << maskmax_Value[i] << "," << masksulcus_Value[i] << endl;
					}
					else {
						ofs << "No" << i << "," << time_Value[i] << "," << Inst_num << "," << maskmax_Value[i] << "," << masksulcus_Value[i] << endl;
						time_Delay = time_Value[i + 20] - time_Value[i];
						for (int k = 1; k<time_Delay; k++) {
							ofs << "NO_LOGERROR" << "," << time_Value[i] + k << Inst_num << "," << '-1' << "," << "-1" << endl;
						}
					}
				}
				time_t time_Value[99] = {};
				int maskmax_Value[99] = {};
				double masksulcus_Value[99] = {};

				time_output_counter = 0;
			}



			/*Slicer*/
			if (slicer_check == 1) {
				socket->Send(transMsg->GetPackPointer(), transMsg->GetPackSize());
				igtl::Sleep(interval); // wait
				if (output_count % 200 == 0) {
					addFilter->SetInput1(image);
					addFilter->SetInput2(reader2->GetOutput());
					try {
						addFilter->Update();
					}
					catch (...) {
						cerr << "add" << endl;
						getchar();
					}
					writer->SetInput(addFilter->GetOutput());
					writer->SetFileName(directory_name + "point_zoi.nii");
					try {
						writer->Update();
					}
					catch (...) {
						cerr << "hogehoge" << endl;
						getchar();
					}
					writer->SetInput(image);
					writer->SetFileName(directory_name + "point_ganbaru.nii");
					writer->Update();
					output_count = 0;
				}//output_count%200のif文終了部分
			}//if()終了部分				
		}//追加
		output_count++;


	}

	//------------------------------------------------------------
	// Close connection
	if (slicer_check == 1) socket->CloseSocket();

}


void GetRandomTestMatrix(igtl::Matrix4x4& matrix)
{
	float position[3];
	float orientation[4];

	// random position
	static float phi = 0.0;
	position[0] = 50.0 * cos(phi);
	position[1] = 50.0 * sin(phi);
	position[2] = 50.0 * cos(phi);
	phi = phi + 0.2;

	// random orientation
	static float theta = 0.0;
	orientation[0] = 0.0;
	orientation[1] = 0.6666666666*cos(theta);
	orientation[2] = 0.577350269189626;
	orientation[3] = 0.6666666666*sin(theta);
	theta = theta + 0.1;

	//igtl::Matrix4x4 matrix;
	igtl::QuaternionToMatrix(orientation, matrix);

	matrix[0][3] = position[0];
	matrix[1][3] = position[1];
	matrix[2][3] = position[2];

	igtl::PrintMatrix(matrix);
}


//メッセージ表示
void Cmdp_msg(time_t time, igtl::Matrix4x4& matrix, double &fps)
{
	cout << "fps : " << fps << endl;
	cout << "time : " << ctime(&time);
	//	cout << "time : " << asctime(localtime(&time));
	cout << "position : ("
		<< matrix[0][3] << ',' << matrix[1][3] << ',' << matrix[2][3] << ')' << endl;
	cout << "orientation : " << endl
		<< matrix[0][0] << ' ' << matrix[0][1] << ' ' << matrix[0][2] << ' ' << endl
		<< matrix[1][0] << ' ' << matrix[1][1] << ' ' << matrix[1][2] << ' ' << endl
		<< matrix[2][0] << ' ' << matrix[2][1] << ' ' << matrix[2][2] << endl << endl;
}


//ファイル名取得
std::string Get_file_name(const std::string &pathname)
{
	size_t pos1;

	pos1 = pathname.rfind('\\');
	if (pos1 != std::string::npos)
		return pathname.substr(pos1 + 1, pathname.size() - pos1 - 1);

	pos1 = pathname.rfind('/');
	if (pos1 != std::string::npos)
		return pathname.substr(pos1 + 1, pathname.size() - pos1 - 1);

	return pathname;
}



//ディレクトリ名取得
std::string Get_directory_name(const std::string &pathname)
{
	size_t pos1;

	pos1 = pathname.rfind('\\');
	if (pos1 != std::string::npos) {
		return pathname.substr(0, pos1 + 1);

	}

	pos1 = pathname.rfind('/');
	if (pos1 != std::string::npos) {
		return pathname.substr(0, pos1 + 1);
	}

	return "";


}

//拡張子取得
void GetFileExtension(const char *PathName, char* extension)
{
	int i = 0, j = 0;

	while (true) {
		if (PathName[i] == '.') {
			for (j = 0; j < 3; j++)	extension[j] = PathName[i + j + 1];
			break;
		}
		i++;
	}
	return;
}


//脳領域判別処理（脳の外，腫瘍内，脳の表面）
//return1:脳の中という判定return0:脳の外という判定
int Brain_Discrimination(double maskmax) {
	cout << "--------------------術具と脳構造の位置関係--------------------" << endl;
	if (maskmax == 4) {
		//if (maskmax == 255){	
		cout << "脳表面" << endl;
		return 1;
	}
	else if (maskmax >= 1 && maskmax < 4) {
		//else if(maskmax==205){
		cout << "脳内" << endl;
		return 1;
	}
	else if (maskmax >= 5) {
		//else if (maskmax==128){
		cout << "腫瘍内" << endl;
		return 1;
	}
	else {
		cout << "脳の外" << endl;
		return 0;
	}

}

//脳溝判別処理

int Sulcus_Discrimination(double maskmax, string *Lsulci_list, string *Rsulci_list) {

	return 0;

}
