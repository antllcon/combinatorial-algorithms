#pragma once
#include <chrono>
#include <iostream>
#include <string>

class Timer
{
public:
	Timer(const std::string& name = "", bool autoPrint = true);
	~Timer();

	void Reset();
	double ElapsedMs() const;
	void Print() const;

private:
	std::string name_;
	bool autoPrint_;
	std::chrono::high_resolution_clock::time_point start_;
};
