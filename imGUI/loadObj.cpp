#include"loadObj.h"	
#include<sstream>
#include<iostream>
#include<fstream>
LoadObj::LoadObj(const char* filepath) {
	std::ifstream in(filepath, std::ios::in);
	if (!in) {
		std::cerr << "can not openfile" << filepath << std::endl;
		exit(1);
	}

	std::string line;
	while (std::getline(in, line)) {
		if (line.substr(0, 2) == "v ") {
			std::istringstream s(line.substr(2));
			for (int i = 0; i < 3; i++) {
				float position;
				s >> position;
				this->positions.push_back(position);
			}
			this->positions.push_back(1.0f);
		}
		else if (line.substr(0, 2) == "f ") {
			//std::istringstream s(line.substr(2));
			//for (int i = 0; i < 3; i++) {
			//	unsigned int index;
			//	s >> index;
			//	this->indices.push_back(index);
			//}
			int i1, i2, i3;
			int n1, n2, n3;
			sscanf_s(line.c_str(),
				"f %i//%i %i//%i %i//%i",
				&i1, &n1, &i2, &n2, &i3, &n3);
			indices.push_back((unsigned int)(i1 - 1));
			indices.push_back((unsigned int)(i2 - 1));
			indices.push_back((unsigned int)(i3 - 1));
		}
	}
}

std::vector<float> LoadObj::getVectorPositions() {
	std::cout << positions.size() << " vertices position attribute imported" << std::endl;
	return this->positions;
}

std::vector<unsigned int> LoadObj::getIndices()
{
	std::cout << indices.size() << "number of indices read";
	return this->indices;
}
