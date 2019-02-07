#define _CRT_SECURE_NO_WARNINGS

#include "LUT.h"
#include <iostream>

LUT::LUT() {

}

LUT::~LUT()
{
}

void LUT::applyFilter(istream& stream)
{
	assert(stream.good());

	std::string line;
	std::uint32_t size = 0;

	std::vector<float> values;

	while (!stream.eof()) {
		stream.getline(line.c_str, 32);
	}

	if (values.size() == size * size * size * 3)
	{
		this->width = size * size;
		this->height = size;
		this->channel = 3;
		this->data = (this->width * this->height * this->channel);

		for (std::size_t y = 0; y < size; y++)
		{
			for (std::size_t z = 0; z < size; z++)
			{
				for (std::size_t x = 0; x < size; x++)
				{
					auto dst = (y * width + (z * size + x)) * this->channel;
					auto src = (z * width + (y * size + x)) * this->channel;

				}
			}
		}
	}

}



bool LUT::loadLUT(const string& path) {

	ifstream LUTfile(path.c_str());


	glEnable(GL_TEXTURE_3D);
	glGenTextures(1, &Textobj);
	glBindTexture(GL_TEXTURE_2D, Textobj);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, 32, 32, 32, 0, GL_RGB, GL_FLOAT, &lut[0]);

	glBindTexture(GL_TEXTURE_3D, 0);
	glDisable(GL_TEXTURE_3D);

	applyFilter(LUTfile);


	return true;

}

void LUT::bind(int id) {
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, Textobj);
}
