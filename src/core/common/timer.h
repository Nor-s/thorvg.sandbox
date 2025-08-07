#ifndef _CORE_COMMON_TIME_H_
#define _CORE_COMMON_TIME_H_

#include <chrono>

namespace core
{
class Timer
{
	using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

public:
	Timer()
	{
		reset();
	}
	~Timer()
	{
	}
	double duration()
	{
		return duration(mStart);
	}
	double duration(TimePoint& start)
	{
		TimePoint end = std::chrono::system_clock::now();
		return duration(start, end);
	}
	double duration(const TimePoint& start, const TimePoint& end)
	{
		auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		return elapsed.count() / 1000.0;
	}
	double elapsed()
	{
		TimePoint end = std::chrono::system_clock::now();
		double ret = duration(mBefore, end);
		mBefore = end;
		return ret;
	}
	void reset()
	{
		mStart = std::chrono::system_clock::now();
		mBefore = mStart;
	}

private:
	TimePoint mStart;
	TimePoint mBefore;
};
}	 // namespace core

#endif