#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <cassert>
#include <limits>

#include <glad/glad.h>

#include <vector>
#include <MiniMath\Core.h>
#include <iostream>

using namespace std;

class LUT
{
public:
	LUT();
	~LUT();

	bool loadLUT(const string& path);
	void bind(int id);
	void applyFilter(istream& stream);

	string name;
	vector<vec3> lut[32][32][32];
	float data;


private:

	float width;
	float height;
	float channel;

	GLuint Textobj = 0;

};

