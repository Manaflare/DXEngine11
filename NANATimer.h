#pragma once

#include <chrono>

using namespace std;
using namespace chrono;

class NANATimer
{
public:
	NANATimer();
	~NANATimer();


	void StartTimer();
	double EndTimer();
	static double GetTime();
	double GetFrameTime();
private:
	double m_startTime;
};

