#include <glad/glad.h>
#include "Texture.h"

Texture::Texture(const Image& image, Wrapping wrapping,	Filtering filter)
	: _texture(0), _wrapping(wrapping), _filter(filter)
{
	glGenTextures(1, &_texture);
	Bind();
	switch (wrapping)
	{
	case Wrapping::Repeat:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	case Wrapping::MirroredRepeat:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		break;
	case Wrapping::ClampToEdge:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
	case Wrapping::ClampToBorder:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		break;
	}

	switch (filter)
	{
	case Filtering::Linear:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case Filtering::Nearest:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.Width(), image.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.Data());
	Unbind();
}

Texture::Texture(Texture&& other) noexcept
{

	_texture = other._texture;
	_wrapping = other._wrapping;
	_filter = other._filter;

	other._texture = 0;
	
}

Texture& Texture::operator=(Texture&& other) noexcept
{
	if (this == &other) return *this;
	_texture = other._texture;
	_wrapping = other._wrapping;
	_filter = other._filter;

	other._texture = 0;

	return *this;
}

Texture::~Texture()
{
	glDeleteTextures(1, &_texture);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, _texture);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Update(const Image& image)
{
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.Width(), image.Height(), GL_RGBA, GL_UNSIGNED_BYTE, image.Data());
}
