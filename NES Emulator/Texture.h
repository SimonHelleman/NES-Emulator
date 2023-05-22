#pragma once
#include <glad/glad.h>
#include "Image.h"

class Texture
{
public:
	enum class Wrapping
	{
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder
	};

	enum class Filtering
	{
		Nearest,
		Linear
	};

	Texture(const Image& image, Wrapping wrapping = Wrapping::Repeat,
		Filtering filter = Filtering::Nearest)
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
	
	Texture(const Texture&) = delete;
	
	Texture(Texture&& other) noexcept
	{
		_texture = other._texture;
		_wrapping = other._wrapping;
		_filter = other._filter;

		other._texture = 0;
	}

	Texture& operator=(const Texture&) = delete;

	Texture& operator=(Texture&& other) noexcept
	{
		if (this == &other) return *this;
		_texture = other._texture;
		_wrapping = other._wrapping;
		_filter = other._filter;

		other._texture = 0;

		return *this;
	}

	~Texture()
	{
		glDeleteTextures(1, &_texture);
	}


	Filtering Filter() const
	{
		return _filter;
	}

	Wrapping Wrap() const
	{
		return _wrapping;
	}

	GLuint TextureId() const
	{
		return _texture;
	}

	void Bind()
	{
		glBindTexture(GL_TEXTURE_2D, _texture);
	}

	void Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

private:
	GLuint _texture;
	Wrapping _wrapping;
	Filtering _filter;
};