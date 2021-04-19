#pragma once
#include <chrono>
#include <iostream>
#include <string>
#include <functional>

class TimeStatistics
{
	typedef std::chrono::high_resolution_clock clock_t;
	typedef std::chrono::time_point<clock_t> time_t;


public:
	void Begin(int line_number)
	{
		begin_ = clock_t::now();
		line_number_begin_ = line_number;
	}

	void End(int line_number, std::string function_name)
	{
		time_t end = clock_t::now();
		long double period_ns = (end - begin_).count() * ((double)std::chrono::high_resolution_clock::period::num / std::chrono::high_resolution_clock::period::den);
		std::cout << function_name << " line_number:" << line_number_begin_ << "-" << line_number << " cost time(s): " << period_ns << std::endl;
	}

private:
	time_t begin_;
	int line_number_begin_ = 0;
};

template<class T, class... Args>
long double
run_time_statistics(T func, Args... args)
{
	typedef std::chrono::high_resolution_clock clock_t;
	typedef std::chrono::time_point<clock_t> time_t;
	time_t start;
	time_t end;

	start = clock_t::now();
	auto c = func(args...);
	std::cout << c;
	end = clock_t::now();

	return
		(end - start).count()
		* ((double)std::chrono::high_resolution_clock::period::num
			/ std::chrono::high_resolution_clock::period::den);
}

#define TIMECOUNTBEGIN TimeStatistics ts; ts.Begin(__LINE__);
#define TIMECOUNTEND ts.End(__LINE__, __FUNCTION__);