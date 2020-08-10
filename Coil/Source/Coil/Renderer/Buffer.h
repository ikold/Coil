#pragma once


namespace Coil
{
	enum class ShaderDataType
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};


	/**
	 * @brief Lookup for ShaderDataType size
	 *
	 * @param[in]	type	
	 *
	 * @return		Size in bytes
	 */
	static uint32 ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float: return 4;
		case ShaderDataType::Float2: return 4 * 2;
		case ShaderDataType::Float3: return 4 * 3;
		case ShaderDataType::Float4: return 4 * 4;

		case ShaderDataType::Mat3: return 4 * 3 * 3;
		case ShaderDataType::Mat4: return 4 * 4 * 4;

		case ShaderDataType::Int: return 4;
		case ShaderDataType::Int2: return 4 * 2;
		case ShaderDataType::Int3: return 4 * 3;
		case ShaderDataType::Int4: return 4 * 4;

		case ShaderDataType::Bool: return 1;
		default: CL_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	}


	/**
	 * @brief Stores uniform data for shaders
	 * 
	 * @todo Write a documentation
	 */
	struct BufferElement
	{
		RString<> Name;
		ShaderDataType Type;
		uint32 Size;
		uint32 Offset;
		bool Normalized;

		
		BufferElement()
			: Name(""),
			  Type(ShaderDataType::None),
			  Size(0),
			  Offset(0),
			  Normalized(false) {}

		BufferElement(ShaderDataType type, RString<> name, bool normalized = false)
			: Name(Move(name)),
			  Type(type),
			  Size(ShaderDataTypeSize(type)),
			  Offset(0),
			  Normalized(normalized) {}

		/**
		 * @brief Lookup for number of components in BufferElement
		 *
		 * @return		Number of elements
		 */
		[[nodiscard]] uint32 GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float: return 1;
			case ShaderDataType::Float2: return 2;
			case ShaderDataType::Float3: return 3;
			case ShaderDataType::Float4: return 4;

			case ShaderDataType::Mat3: return 3 * 3;
			case ShaderDataType::Mat4: return 4 * 4;

			case ShaderDataType::Int: return 1;
			case ShaderDataType::Int2: return 2;
			case ShaderDataType::Int3: return 3;
			case ShaderDataType::Int4: return 4;

			case ShaderDataType::Bool: return 1;
			default: CL_CORE_ASSERT(false, "Unknown ShaderDataType!");
				return 0;
			}
		}
	};


	/**
	 * @brief Layout of uniforms
	 *
	 * @todo Write a documentation
	 */
	class BufferLayout
	{
	public:
		BufferLayout() = default;

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		[[nodiscard]] const std::vector<BufferElement>& GetElements() const { return Elements; }

		[[nodiscard]] uint32 GetStride() const { return Stride; }

		std::vector<BufferElement>::iterator begin() { return Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return Elements.end(); }
		[[nodiscard]] std::vector<BufferElement>::const_iterator begin() const { return Elements.begin(); }
		[[nodiscard]] std::vector<BufferElement>::const_iterator end() const { return Elements.end(); }

	private:
		void CalculateOffsetsAndStride()
		{
			uint32 offset = 0;
			Stride        = 0;
			for (auto& element : Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				Stride += element.Size;
			}
		}

	private:
		std::vector<BufferElement> Elements;
		uint32 Stride = 0;
	};


	/**
	 * @brief Vertex buffer
	 *
	 * @todo Write a documentation
	 */
	class VertexBuffer
	{
	public:
		VertexBuffer()                        = default;
		VertexBuffer(const VertexBuffer& rhs) = default;
		VertexBuffer(VertexBuffer&& rhs)      = default;

		virtual ~VertexBuffer() = default;

		VertexBuffer& operator=(const VertexBuffer& rhs) = default;
		VertexBuffer& operator=(VertexBuffer&& rhs)      = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		[[nodiscard]] virtual const BufferLayout& GetLayout() const = 0;

		static Ref<VertexBuffer> Create(float32* vertices, uint32 size);
	};


	/**
	 * @brief Index Buffer
	 *
	 * @todo Write a documentation
	 */
	class IndexBuffer
	{
	public:
		IndexBuffer()                       = default;
		IndexBuffer(const IndexBuffer& rhs) = default;
		IndexBuffer(IndexBuffer&& rhs)      = default;

		virtual ~IndexBuffer() = default;

		IndexBuffer& operator=(const IndexBuffer& rhs) = default;
		IndexBuffer& operator=(IndexBuffer&& rhs)      = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		[[nodiscard]] virtual uint32 GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32* indices, uint32 size);
	};
}
