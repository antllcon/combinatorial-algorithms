#include "Timer.h"

Timer::Timer(const std::string& name, bool autoPrint)
	: name_(name)
	, autoPrint_(autoPrint)
	, start_(std::chrono::high_resolution_clock::now())
{
}

Timer::~Timer()
{
	if (autoPrint_) Print();
}

void Timer::Reset()
{
	start_ = std::chrono::high_resolution_clock::now();
}

double Timer::ElapsedMs() const
{
	auto end = std::chrono::high_resolution_clock::now();
	return std::chrono::duration<double, std::milli>(end - start_).count();
}

void Timer::Print() const
{
	std::cout << (name_.empty() ? "" : name_ + ": ") << ElapsedMs() << " ms\n";
}
