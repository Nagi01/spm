#pragma once
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

class SurgicalPosition
{
public:
	SurgicalPosition();
	void Detect_Position(string fn_mri, string fn_log, string fn_sulci, bool slicer_check);
	~SurgicalPosition() {
	};
private:

};

