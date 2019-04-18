#include "Texture.h"

Texture::Texture(const std::string& path) :
	filePath(path),
	localBuffer(nullptr),
	width(0),
	height(0),
	bpp(0)
{
	stbi_set_flip_vertically_on_load(1);
	localBuffer = stbi_load(filePath.c_str(), &width, &height, &bpp, 4);
	glGenTextures(1, &texObj);  // actually this is still a buffer which is a texture buffer
	glBindBuffer(GL_TEXTURE_2D, texObj);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
	unbind();

	if (localBuffer)
		stbi_image_free(localBuffer);

}

Texture::~Texture()
{
	glDeleteTextures(1, &texObj);
}

void Texture::bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texObj);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
