#pragma once

namespace Coil
{
	/**
	 * @brief Base class for Texture
	 *
	 * @todo Write a documentation
	 */
	class Texture
	{
	public:
		Texture()                   = default;
		Texture(const Texture& rhs) = default;
		Texture(Texture&& rhs)      = default;

		virtual ~Texture() = default;

		Texture& operator=(const Texture& rhs) = default;
		Texture& operator=(Texture&& rhs)      = default;

		[[nodiscard]] virtual uint32 GetWidth() const = 0;
		[[nodiscard]] virtual uint32 GetHeight() const = 0;

		virtual void SetData(void* data, uint32 size) = 0;

		virtual void Bind(uint32 slot = 0) const = 0;
	};


	/**
	 * @brief Base class for specialize 2D Texture
	 */
	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32 width, uint32 height);
		static Ref<Texture2D> Create(const RString<>& path);
	};
}
