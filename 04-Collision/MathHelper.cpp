#include "MathHelper.h"

int CMathHelper::RandomInt(int begin, int end)
{
	try
	{
		random_device rd;
		mt19937 rng(rd());
		uniform_int_distribution<int> uni(begin, end);
		auto n = uni(rng);

		return n;
	}
	catch (exception e)
	{
		return (begin + end) / 2;
	}
	//return (begin + end) / 2;
}

float CMathHelper::GetBezier3P(float p0, float p1, float p2, float t)
{
	return (1 - t)*(1 - t)*p0 + 2 * t*(1 - t)*p1 + t * t*p2;
}

vector<int> CMathHelper::TachSo(int so, int length)
{
	vector<int> result;

	while (so > 0)
	{
		result.push_back(so % 10);
		so /= 10;
	}

	while (result.size() < length)
	{
		result.push_back(0);
	}

	return result;
}