#pragma once
#include <cstdint>

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

	constexpr RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff)
		: r(r), g(g), b(b), a(a)
	{
	}
	
	constexpr RGBA(uint32_t val = 0xff000000)
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
	Image(unsigned int width, unsigned int height);

	Image(const Image& other);

	Image(Image&& other) noexcept;

	Image& operator=(const Image& other);

	Image& operator=(Image&& other) noexcept;

	~Image();

	unsigned int Width() const
	{
		return _width;
	}

	unsigned int Height() const
	{
		return _height;
	}

	RGBA GetPixel(unsigned int x, unsigned int y) const;

	void SetPixel(unsigned int x, unsigned int y, RGBA color) const;

	void Clear(RGBA color = 0xff000000) const;

	void Copy(const Image& image, unsigned int x, unsigned int y);

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