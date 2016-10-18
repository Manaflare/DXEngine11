#include "PreCompiled.h"
#include "NANATimer.h"



NANATimer::NANATimer()
{
}


NANATimer::~NANATimer() 
{

}

void NANATimer::StartTimer()
{
	m_startTime = GetTime();
}

double NANATimer::EndTimer()
{
	return GetTime() - m_startTime;
}

double NANATimer::GetTime()
{
	high_resolution_clock::time_point currentTime = high_resolution_clock::now();
	duration<double> timeDuration = currentTime.time_since_epoch();
	return timeDuration.count();
}

