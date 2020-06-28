#pragma once

namespace Coil
{
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

		virtual void Bind(uint32 slot = 0) const = 0;
	};


	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const RString<>& path);
	};
}
