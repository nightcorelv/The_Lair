#include "Time.h"
#include <chrono>
#include <thread>

long long Time::frameTime = 1000000.0f / 60;

std::chrono::steady_clock::time_point lap;
std::chrono::steady_clock::time_point start;

int Time::fps = 0;

float Time::DeltaTimeInSeconds()
{
	return std::chrono::duration<float, std::ratio<1, 1>>(std::chrono::steady_clock::now() - lap).count();
}

float Time::DeltaTimeInMiliseconds()
{
	return std::chrono::duration<float, std::milli>(std::chrono::steady_clock::now() - lap).count();
}

float Time::DeltaTimeInMicroseconds()
{
	return std::chrono::duration<float, std::micro>(std::chrono::steady_clock::now() - lap).count();
}

float Time::DeltaTimeInNanoseconds()
{
	return std::chrono::duration<float, std::nano>(std::chrono::steady_clock::now() - lap).count();
}

float Time::CurrentTime()
{
	return std::chrono::duration<float, std::ratio<1, 1>>(std::chrono::steady_clock::now() - start).count();
}

void Time::Start()
{
	lap = std::chrono::steady_clock::now();

	static const bool once = []()
	{
		start = lap;
		return true;
	} ();

}

void Time::Reset()
{
	start = std::chrono::steady_clock::now();
	lap = start;
}

void Time::Update()
{
	//long long elapsedTime = DeltaTimeInMicroseconds();

	//if (frameTime >= elapsedTime)
	//{
	//   std::this_thread::sleep_for(std::chrono::microseconds(frameTime - elapsedTime));
	//}

	static float lapTime = 0;
	static int fpsCount = 0;
	if (CurrentTime() - lapTime >= 1.0f)
	{
		fps = fpsCount;
		fpsCount = 0;
		lapTime = CurrentTime();
	}
	fpsCount++;

	//spin lock
	while (DeltaTimeInMicroseconds() < frameTime);

	Start(); //restart time

}

void Time::SetFPS(unsigned char fps)
{
	fps = (fps == 0) ? 60 : fps;
	frameTime = 1000000.0f / fps;
}

int Time::GetFPS()
{
	return fps;
}
