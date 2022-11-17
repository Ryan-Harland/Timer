#include "Timer.h"
#include <stdexcept>
#include <fstream>
#include <stdio.h>
#include <string>

double Timer::calcTime()
{
	return (double)(mEndTime.QuadPart - mStartTime.QuadPart) / (double)mClockFreq.QuadPart;
}

double Timer::convertTime(double _cycles, Timepoint _time) {
	return _cycles * static_cast<double>(_time);
}

Timer::Timer(int _framesToTrack) : mPreviousFrames(_framesToTrack)
{
	mMaxFrameTime = 0;
	mMinFrameTime = 0;

	mStartTime.LowPart = NULL;
	mStartTime.HighPart = NULL;
	mStartTime.u.LowPart = NULL;
	mStartTime.u.HighPart = NULL;

	mEndTime.LowPart = NULL;
	mEndTime.HighPart = NULL;
	mEndTime.u.LowPart = NULL;
	mEndTime.u.HighPart = NULL;

	mClockFreq.LowPart = NULL;
	mClockFreq.HighPart = NULL;
	mClockFreq.u.LowPart = NULL;
	mClockFreq.u.HighPart = NULL;

	mInternalVec = mPreviousFrames.getInternalVector();
	mTimerStarted = false;
	
}

void Timer::StartTimer()
{
	QueryPerformanceFrequency(&mClockFreq);
	QueryPerformanceCounter(&mStartTime);
	mTimerStarted = true;
}

void Timer::StopTimer()
{
	if (!mTimerStarted) {
		throw std::logic_error("You must start the timer before you can stop it");
	}
	QueryPerformanceCounter(&mEndTime);
	mPreviousFrames.insertElement(calcTime()); 
	mLastFrame = calcTime();
	mTimerStarted = false;
}

double const Timer::GetElapsedTime(Timepoint _time)
{	
	return convertTime(mLastFrame, _time);
}

double const Timer::GetAverageElapsedTime(Timepoint _time)
{

	double totalTime = 0;
	for (unsigned int i = 0; i < mInternalVec->size(); i++) {
		totalTime += mInternalVec->at(i);
	}

	return convertTime(totalTime / (int)mInternalVec->size(), _time);
}

double const Timer::GetMaxFrametime(Timepoint _time)
{
	mMaxFrameTime = mInternalVec->front();
	for (unsigned int i = 0; i < mInternalVec->size(); i++) {
		if (mInternalVec->at(i) > mMaxFrameTime) {
			mMaxFrameTime = mInternalVec->at(i);
		}
	}

	return convertTime(mMaxFrameTime, _time);
}

double const Timer::GetMinFrametime(Timepoint _time)
{
	mMinFrameTime = mInternalVec->front();
	for (unsigned int i = 0; i < mInternalVec->size(); i++) {
		if (mInternalVec->at(i) < mMinFrameTime) {
			mMinFrameTime = mInternalVec->at(i);
		}
	}

	return convertTime(mMinFrameTime, _time);
}

void Timer::logTimeToCSV(Timepoint _time, std::string _filename)
{
	std::fstream stream(_filename, std::ios::out | std::ios::trunc);
	stream << "frame, ";

	switch (_time) {
	case Timepoint::seconds:
		stream << "seconds\n";
		break;
	case Timepoint::milliseconds:
		stream << "milliseconds\n";
		break;
	case Timepoint::microseconds:
		stream << "microseconds\n";
		break;
	case Timepoint::nanoseconds:
		stream << "nanoseconds\n";
		break;
	}

	for (unsigned int i = 0; i < mPreviousFrames.getInternalVector()->size(); i++) {
		stream << i + 1 << ",";
		stream << convertTime(mPreviousFrames.getInternalVector()->at(i), _time);
		stream << "\n";
	}

	stream.close();
}

void Timer::logTimeToTXT(Timepoint _time, std::string _filename)
{
	std::fstream stream(_filename, std::ios::out | std::ios::trunc);
	std::string timepoint;

	switch (_time) {
	case Timepoint::seconds:
		timepoint = "secondsn";
		break;
	case Timepoint::milliseconds:
		timepoint = "milliseconds";
		break;
	case Timepoint::microseconds:
		timepoint = "microseconds";
		break;
	case Timepoint::nanoseconds:
		timepoint = "nanoseconds";
		break;
	}

	for (unsigned int i = 0; i < mPreviousFrames.getInternalVector()->size(); i++) {
		stream << "Frame " << i + 1 <<  ": ";
		stream << convertTime(mPreviousFrames.getInternalVector()->at(i), _time) << " " + timepoint;
		stream << "\n";
	}

	stream.close();
}


