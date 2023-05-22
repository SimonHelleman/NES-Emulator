#pragma once
#include <cstdint>
#include <cstring>
#include <cassert>

struct RGBA
{
	union
	{
		uint32_t val;
		struct
		{
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
		};
	};

	RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff)
		: r(r), g(g), b(b), a(a)
	{
	}
	
	RGBA(uint32_t val = 0xff000000)
		: val(val)
	{
	}
	
	bool operator==(const RGBA& other) const
	{
		return val == other.val;
	}
	
	bool operator!=(const RGBA& other) const
	{
		return val != other.val;
	}
};

class Image
{
public:
	Image(unsigned int width, unsigned int height)
		: _width(width), _height(height), _data(new RGBA[(size_t)width * (size_t)height])
	{
	}

	Image(const Image& other)
	{
		_width = other._width;
		_height = other._height;
		_data = new RGBA[(size_t)(other._width) * (size_t)other._height]; // Fight me
		memcpy(_data, other._data, (size_t)(_width) * (size_t)(_height) * sizeof(RGBA));
	}

	Image(Image&& other) noexcept
	{
		_width = other._width;
		_height = other._height;
		_data = other._data;

		other._data = nullptr;
	}

	Image& operator=(const Image& other)
	{
		if (this == &other) return *this;
		delete[] _data;

		_width = other._width;
		_height = other._height;
		_data = new RGBA[(size_t)(other._width) * (size_t)(other._height)];
		memcpy(_data, other._data, (size_t)(_width) * (size_t)(_height) * sizeof(RGBA));
		return *this;
	}

	Image& operator=(Image&& other) noexcept
	{
		if (this == &other) return *this;

		delete[] _data;

		_width = other._width;
		_height = other._height;
		_data = other._data;

		other._data = nullptr;

		return *this;
	}

	~Image()
	{
		delete[] _data;
	}

	unsigned int Width() const
	{
		return _width;
	}

	unsigned int Height() const
	{
		return _height;
	}

	RGBA GetPixel(unsigned int x, unsigned int y) const
	{
		assert(x < _width && y < _height);
		return _data[y * _width + x];
	}

	void SetPixel(unsigned int x, unsigned int y, RGBA color) const
	{
		assert(x < _width && y < _height);
		_data[y * _width + x] = color;
	}

	void Clear() const
	{
		for (unsigned int i = 0; i < _width * _height; ++i)
		{
			_data[i] = { 0, 0, 0 };
		}
	}

	const void* Data() const
	{
		return _data;
	}

	RGBA* Pixels() const
	{
		return _data;
	}

private:	
	unsigned int _width;
	unsigned int _height;
	
	RGBA* _data;
};