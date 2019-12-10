#include "MathHelper.h"
#include <iostream>
#include <random>

using namespace std;

int CMathHelper::RandomInt(int begin, int end)
{
	random_device rd;
	mt19937 rng(rd());
	uniform_int_distribution<int> uni(begin, end);
	auto n = uni(rng);

	return n;
}

float CMathHelper::GetBezier3P(float p0, float p1, float p2, float t)
{
	return (1 - t)*(1 - t)*p0 + 2 * t*(1 - t)*p1 + t * t*p2;
}