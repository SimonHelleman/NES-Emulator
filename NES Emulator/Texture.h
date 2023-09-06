#pragma once
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

	Texture() = default;

	Texture(const Image& image, Wrapping wrapping = Wrapping::Repeat,
		Filtering filter = Filtering::Nearest);
	
	Texture(const Texture&) = delete;
	
	Texture(Texture&& other) noexcept;

	Texture& operator=(const Texture&) = delete;

	Texture& operator=(Texture&& other) noexcept;

	~Texture();


	constexpr Filtering Filter() const
	{
		return _filter;
	}

	constexpr Wrapping Wrap() const
	{
		return _wrapping;
	}

	unsigned int TextureId() const
	{
		return _texture;
	}

	void Bind();

	void Unbind();

	void Update(const Image& image);

private:
	unsigned int _texture;
	Wrapping _wrapping;
	Filtering _filter;
};