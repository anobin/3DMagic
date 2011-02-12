/* 
Copyright (c) 2011 Andrew Keating

This file is part of 3DMagic.

3DMagic is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

3DMagic is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with 3DMagic.  If not, see <http://www.gnu.org/licenses/>.

*/
/** Header file for StopWatch class
 *
 * @file StopWatch.h
 * @author Andrew Keating
 */
#ifndef MAGIC3D_STOPWATCH_H
#define MAGIC3D_STOPWATCH_H


// include dependent on OS (Windows or everyone else)
#ifdef _WIN32
#include <windows.h> // a single include for a whole OS, why not?
#else
#include <sys/time.h> // include time functions to get timeofday
#endif

namespace Magic3D
{

/** Base class for Simple stopwatch that allows for counting
 * time in different formats. Should be used for 
 * operations where the time value is relative to
 * the start of something (like the game/simulation)
 */
class StopWatch
{
private:
	// internals dependent on OS
#ifdef WIN32
	LARGE_INTEGER counterFreq;
	LARGE_INTEGER startCount;
#else
	timeval startTime;
#endif

public:
	/// default constructor
	inline StopWatch()
	{
#ifdef WIN32
	QueryPerformanceFrequency(&counterFreq); // get the number of counts per second
	QueryPerformanceCounter(&startCount);	 // get the starting number of counts
#else
	gettimeofday(&startTime, 0); // get the starting time
#endif
	}
	
	/// reset the stopwatch
	inline void reset()
	{
#ifdef WIN32
	QueryPerformanceCounter(&startCount);	 // get the starting number of counts
#else
	gettimeofday(&startTime, 0); // get the starting time
#endif
	}
	
	/** get the current ammount of elapsed time
	 * @return the elapsed time as float seconds
	 */
	inline float getElapsedTime()
	{
	// for windows we get the current count and use the freq to calculate
#ifdef WIN32
	LARGE_INTEGER nowCount;
	QueryPerformanceCounter(&nowCount);

	return float((nowCount.QuadPart - startCount.QuadPart) / 
			double(counterFreq.QuadPart));
	
	
	// for UNIX, we just subtract now time from start time
#else
	timeval nowTime;
	gettimeofday(&nowTime, 0);
	float fSeconds = (float)(nowTime.tv_sec - startTime.tv_sec);
	float fFraction = (float)(nowTime.tv_usec - startTime.tv_usec) * 0.000001f;
	return fSeconds + fFraction;
#endif
	}




};

};


#endif

















