#include <cassert>
#include <cstring>
#include "Image.h"

Image::Image(unsigned int width, unsigned int height)
	: _width(width), _height(height), _data(new RGBA[(size_t)width * (size_t)height])
{
}

Image::Image(const Image& other)
{
	_width = other._width;
	_height = other._height;
	_data = new RGBA[(size_t)(other._width) * (size_t)other._height]; // Fight me
	memcpy(_data, other._data, (size_t)(_width) * (size_t)(_height) * sizeof(RGBA));
}

Image::Image(Image&& other) noexcept
{
	_width = other._width;
	_height = other._height;
	_data = other._data;

	other._data = nullptr;
}

Image& Image::operator=(const Image& other)
{
	if (this == &other) return *this;
	delete[] _data;

	_width = other._width;
	_height = other._height;
	_data = new RGBA[(size_t)(other._width) * (size_t)(other._height)];
	memcpy(_data, other._data, (size_t)(_width) * (size_t)(_height) * sizeof(RGBA));
	return *this;
}


Image& Image::operator=(Image&& other) noexcept
{
	if (this == &other) return *this;

	delete[] _data;

	_width = other._width;
	_height = other._height;
	_data = other._data;

	other._data = nullptr;
	return *this;
}

Image::~Image()
{
	delete[] _data;
}

RGBA Image::GetPixel(unsigned int x, unsigned int y) const
{
	assert(x < _width && y < _height);
	return _data[y * _width + x];
}

void Image::SetPixel(unsigned int x, unsigned int y, RGBA color) const
{
	assert(x < _width && y < _height);
	_data[y * _width + x] = color;
}

void Image::Clear(RGBA color) const
{
	for (unsigned int i = 0; i < _width * _height; ++i)
	{
		_data[i] = color;
	}
}

void Image::Copy(const Image& image, unsigned int x, unsigned int y)
{
	unsigned int imageX = 0;
	unsigned int imageY = 0;
	for (unsigned int posY = y; posY < y + image.Height(); ++posY)
	{
		imageX = 0;
		for (unsigned int posX = x; posX < x + image.Width(); ++posX)
		{
			SetPixel(posX, posY, image.GetPixel(imageX++, imageY));
		}
		++imageY;
	}
}