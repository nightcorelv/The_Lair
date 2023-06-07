#pragma once

struct Time final
{

	static float DeltaTimeInNanoseconds();
	static float DeltaTimeInMicroseconds();
	static float DeltaTimeInMiliseconds();
	static float DeltaTimeInSeconds();

	static float CurrentTime();

	static void Update();
	static void Start();

	static void Reset();

	static void SetFPS(unsigned char value);
	static int GetFPS();

private:

	static int fps;
	static long long frameTime;

};