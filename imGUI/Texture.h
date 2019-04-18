#ifndef  _TEXTURE_H_
#define _TEXTURE_H_
#include<GL/glew.h>	
#include<iostream>
#include"stbimage.h"
class Texture {
private:
	GLuint texObj;
	std::string filePath;
	unsigned char* localBuffer;
	int width;
	int height;
	int bpp;
public:
	Texture(const std::string& path);
	~Texture();
	void bind(unsigned int slot);
	void unbind();
	inline int getWidth() const{ return width; }
	inline int getHeight() const { return height; }
};

#endif // ! _TEXTURE_H_
