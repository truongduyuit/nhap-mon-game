#pragma once
#include <iostream>
#include <random>

using namespace std;

class CMathHelper
{
public:

	int static RandomInt(int begin, int end);
	float static GetBezier3P(float p0, float p1, float p2, float t);
	vector<int> static TachSo(int so, int length);

	CMathHelper() { ; }
	~CMathHelper() { ; }
};