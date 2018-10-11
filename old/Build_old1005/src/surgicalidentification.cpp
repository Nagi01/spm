#include "surgicalidentification.h"
#include <Eigen/Core>
#include <Eigen/Sparse>
#include <Eigen/Dense>
#include <numeric>
#include <random>
#include<iostream>
#include <time.h>
#include <tuple>
#include <vector>

using namespace std;
using namespace Eigen;

SurgicalIdentification::SurgicalIdentification(const int nnum)
{
	x = new int[nnum];
	MatrixXd vec(3, 3);

}

void SurgicalIdentification::setmatrix(const Eigen::MatrixXd &a)
{
	vec = a + a;
	//cout <<"hoge"<< a.row(1) << endl;
}


vector<vector<int>> SurgicalIdentification::generateSample(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, const Eigen::MatrixXd &rho, int n)
{
	vector<vector <int>> x_s(2, vector<int>(n));
	x_s[0][0] = sample(rho);//x����i
	x_s[1][0] = sample(B.row(x_s[0][0]));//s�����i
	cout << "---generateSample---" << endl;
	//cout << x_s[0][0] << x_s[1][0] << endl;
	for (int t = 1; t < n; t++)
	{
		x_s[1][t] = sample(A.row(x_s[0][t - 1]));
		x_s[0][t] = sample(B.row(x_s[1][t]));
	}
	//�m�F�e�X�g�R�[�h
	for (int i = 0; i < n; i++) {
		cout << x_s[0][i];
	}
	cout << endl;
	for (int i = 0; i < n; i++) {
		cout << x_s[1][i];
	}
	cout << endl;
	cout << "--------------------" << endl;
	return x_s;
}

int SurgicalIdentification::sample(const Eigen::MatrixXd &p)
{
	int i = 1;
	int ans = 0;
	std::random_device rnd;
	srand((unsigned int)time(NULL));
	//�����_���̐��l�𓾂�
	int random = (rnd() % p.cols());
	double x = double(p.coeff(random, 1) / p.sum());
	//cout << x << endl;
	//double x_min = x.minCoeff();
	for (int i = 0; i < p.cols(); i++)
	{
		if (x == double(p.coeff(i, 1))) {
			ans = i;
		}
	}
	return ans;
}

int SurgicalIdentification::Viterbi(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, const Eigen::MatrixXd &rho, const int n, vector<vector<int>>x_s)
{
	int num = n;
	//x_s�̏�i�ix_s[0][i]�j�́C�������ϑ����ʁi��������ł�������������ǂ��炪�o�����j�ɂȂ�܂��D
	//x_s�̉��i�ix_s[1][i]�j�́C�o�͌��ʁi��������ł����ǂ̂���������g������3��ނ����ABC�̂ǂꂩ�j�ł��D

	//STEP1.������
	MatrixXd P = MatrixXd::Zero(rho.size(), n);
	P.col(0) = log(rho.transpose().array()) + log(B.col(x_s[0][0]).array());//(8.28)
	MatrixXd I = MatrixXd::Ones(rho.size(), n);//(8.29)
	MatrixXd I_1 = MatrixXd::Ones(rho.size(), n);//(8.29)
												 //cout << P << endl;//cout << "P" << endl;cout <<P<<endl;cout<<"I" <<endl;cout <<I<<endl;//�m�F�v�f

												 //STEP2.�ċA�I�v
	MatrixXd Z = MatrixXd::Zero(A.cols(), A.rows());
	for (int i = 0; i < n - 1; ++i)
	{
		Z = bsxfun(A, P, rho, i);
		I.col(i + 1) = P.col(0);
		I.col(i + 1) = argmax(Z);//(8.31)
		P.col(i + 1) = diag(Z, I, i).array() + log(B.col(x_s[0][i + 1]).array());//(8.30)
																				 //getc(stdin);
																				 //cout << "---Z" << endl;
																				 //cout << Z << endl;
																				 //cout << "---I" << endl;
																				 //cout << I.col(i + 1) << endl;
																				 //cout << "---P" << endl;
																				 //cout << P.col(i + 1) << endl;

	}
	//��������I�̒l��0~x�̒l�ɂ��邽��-1���܂��D�i�s��ł̌v�Z�����₷�����邽�߁j
	I = I - I_1;

	//STEP3.�I��
	//cout << P << endl;//�m�F�v�f
	MatrixXd::Index maxRow, maxCol;
	vector <int> S(n);
	P.col(n - 1).maxCoeff(&maxRow, &maxCol);
	//cout << "Min of A is "  << " at " << maxRow << ", " << maxCol << endl;
	S[n - 1] = maxRow;//(8.33)//���ꂪ���Ȃ݂ɐ��茋��

	cout << "------ANS---------" << endl;
	cout << S[n - 1] << endl;
	cout << "------ANS---------" << endl;
	//STEP4.�n��̕���

	for (int t = n - 2; t >= 0; t--)
	{
		S[t] = I.coeff(S[t + 1], t + 1);
	}

	cout << "-------------P-----------------" << endl;
	cout << P << endl;
	cout << "--------------S---------------" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << S[i];
	}
	cout << endl;
	cout << "--------------Send---------------" << endl;
	cout << endl;
	cout << "--------------I---------------" << endl;
	cout << I << endl;
	cout << "--------------Iend---------------" << endl;

	return 0;
}

MatrixXd SurgicalIdentification::bsxfun(const Eigen::MatrixXd &A, const Eigen::MatrixXd &P, const Eigen::MatrixXd &rho, int i)
{
	MatrixXd Z = MatrixXd::Zero(A.cols(), A.rows());
	for (int j = 0; j < A.cols(); j++)
	{
		for (int k = 0; k < A.rows(); k++)
		{
			if (A.coeff(k, j) != 0 && rho.coeff(k, 0) != 0)
			{
				Z(k, j) = P.coeff(k, i) + log(A.coeff(k, j));
			}
		}
	}
	return Z;
}

MatrixXd SurgicalIdentification::argmax(const Eigen::MatrixXd &Z)
{
	MatrixXd Z_maxelements = MatrixXd::Zero(Z.cols(), Z.rows());
	vector<int> R;
	vector<int> C;
	vector<int> protoC;

	//cout << "----------Z--------" << endl;
	//cout << Z << endl;
	for (int j = 0; j < Z.cols(); j++)
	{
		for (int k = 0; k < Z.rows(); k++)
		{
			//cout << "----------Z��" <<j << endl;
			//cout << Z.col(j).maxCoeff() << endl;		
			if (Z.col(j).maxCoeff() == Z.coeff(k, j))
			{
				Z_maxelements(k, j) = 1;
				R.push_back(k + 1);
				protoC.push_back(j + 1);
			}
		}
	}//�x�N�g�����Z�����邽�߁C�����ēY������1����n�߂�R��C�ɂ��Ă��܂��D
	 //diff����
	C = protoC;
	for (int i = 1; i < protoC.size(); i++)
	{
		C.at(i) = protoC.at(i) - protoC.at(i - 1);
	}
	//for (int i = 0; i < R.size(); i++) cout << R.at(i) << "," << C.at(i) << endl;

	//cout << "Z_maxelements" << endl;
	//cout << Z_maxelements << endl;
	MatrixXd argmax = MatrixXd::Zero(C.size() - (count(C.begin(), C.end(), 0)), 1);//

	int arg_count = 0;
	for (int i = 0; i < C.size(); i++)
	{
		if (C.at(i) != 0)
			argmax(arg_count++) = R.at(i);
	}
	//cout << "-----------argmax--------" << endl;
	//cout << argmax << endl;
	return argmax;
}
//[row,col]

MatrixXd SurgicalIdentification::diag(const Eigen::MatrixXd &Z, const Eigen::MatrixXd &I, int i)
{
	MatrixXd ZZ = MatrixXd::Zero(Z.cols(), Z.rows());
	for (int j = 0; j < I.rows(); j++)
	{
		ZZ.row(j) = Z.row(I(j, i + 1) - 1);
	}
	return ZZ.diagonal();
}