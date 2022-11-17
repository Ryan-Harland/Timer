#pragma once
#include <Windows.h>
#include "FixedVector.h"
#include <string>

/*
	Uses system queries to get the given time between two given points in code.
	Can represent from seconds to nanoseconds, and can output in plain text, or as a .csv, for graphing in Excel
*/
class Timer {

public:
	// Custom enum class that holds all needed time scales, and the integers needed to convert between them.
	enum class Timepoint { seconds = 1, milliseconds = 1000, microseconds = 1000000, nanoseconds = 1000000000 };

	/*
	Initialises all vars to 0, including custom types such as LARGE_INTEGER. 
	Also caches reference to the internal vector of mPreviousFrames
	*/
	Timer(int _framesToTrack);

	// Uses Windows.h to query the CPU frequency and the number of passed clock cycles
	void StartTimer();

	// Further uses Windows.h to get the new number of cycles. The two results are compared to get a timeframe, which is added to mPreviousFrames
	void StopTimer();

	// Uses Timepoint as a parameter to know which time frame to return mLastFrame as.
	double const GetElapsedTime(Timepoint);

	// Returns the average of all frame times in mPreviousFrames as whichever type is specified as a parameter.
	double const GetAverageElapsedTime(Timepoint);

	// Finds the highest frame time in mPreviousFrames and returns it as the specified time scale
	double const GetMaxFrametime(Timepoint);

	// Finds the lowest frame time in mPreviousFrames and returns it as the specified time scale
	double const GetMinFrametime(Timepoint);

	// Logs all entries in mPreviousFrames to a .csv file, ready for use in various graphing applications
	void logTimeToCSV(Timepoint, std::string);

	// Logs all entries as plain text, for pure logging purposes
	void logTimeToTXT(Timepoint, std::string);

private:
	// Member vars to store the result of querying the performance counter
	LARGE_INTEGER mStartTime;
	LARGE_INTEGER mEndTime;
	LARGE_INTEGER mClockFreq;

	double mLastFrame = 0;
	double mMaxFrameTime;
	double mMinFrameTime;
	FixedVector<double> mPreviousFrames;

	bool mTimerStarted;
	
	// Constant pointer to the internal vector of mPreviousFrames, constant as we don't want to risk reallocating it.
	const std::vector<double>* mInternalVec = nullptr;

	// Internal functions to calculate the difference in time points, as well as convert between the desired time scales.
	double calcTime();
	double convertTime(double,Timepoint);
};
