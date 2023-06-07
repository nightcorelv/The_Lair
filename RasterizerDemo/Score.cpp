#include "Score.h"
#include <fstream>

float Score::data = 0;

bool Score::Add(float score)
{
	bool ranked = false;
	if (score < data || data == 0)
	{
		data = score;
		ranked = true;
	}
	return ranked;
}

float Score::Get()
{
	return data;
}

void Score::Save()
{
	std::ofstream out("../data/score.txt");
	if (out.is_open())
	{
		out << data;
		out.close();
	}

}

void Score::Load()
{
	std::ifstream in("../data/score.txt");
	if (in.is_open())
	{
		in >> data;
		in.close();
	}
}

