#ifndef _LOADOBJ_H_
#define _LOADOBJ_H_
#include<iostream>	
#include<vector>

class LoadObj {
private:
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<unsigned int> indices;
public:
	LoadObj(const char* filepath);
	std::vector<float> getVectorPositions();
	std::vector<unsigned int> getIndices();
};


#endif