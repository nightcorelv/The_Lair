#pragma once
#include <vector>

struct Score
{
	// add to score,r
	static bool Add(float score);

	//get score
	float Get();

	//save score array to a file
	static void Save();
	//load score array from file
	static void Load();

private:

	static float data;
};

