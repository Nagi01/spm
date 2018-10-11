#pragma once
#include <Eigen/Sparse>
using namespace std;
using namespace Eigen;

class SurgicalIdentification
{
private:
	int *x;
	int x_num;
	int *rand_s, *rand_x;
	int *s;

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		SurgicalIdentification(const int x_num);
	~SurgicalIdentification() { delete[] x; };
	MatrixXd vec;
	void setmatrix(const Eigen::MatrixXd &a);
	vector<vector<int>> generateSample(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, const Eigen::MatrixXd &rho, int n);
	int sample(const Eigen::MatrixXd &p);
	int Viterbi(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, const Eigen::MatrixXd &rho, const int n, vector<vector<int>> x);
	MatrixXd bsxfun(const Eigen::MatrixXd &A, const Eigen::MatrixXd &P, const Eigen::MatrixXd &rho, int i);
	MatrixXd argmax(const Eigen::MatrixXd &Z);
	MatrixXd diag(const Eigen::MatrixXd &Z, const Eigen::MatrixXd &I, int i);
};