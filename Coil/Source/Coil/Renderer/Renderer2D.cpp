#include "pch.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"
#include <glm/ext/matrix_transform.hpp>


namespace Coil
{
	struct Renderer2DData
	{
		static constexpr uint32 MaxQuads        = 32 * 1024;
		static constexpr uint32 MaxVertices     = MaxQuads * 4;
		static constexpr uint32 MaxIndices      = MaxQuads * 6;
		static constexpr uint32 MaxTextureSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> ColorTextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32 QuadIndexCount                            = 0;
		Renderer2D::QuadVertex* QuadVertexBufferData     = nullptr;
		Renderer2D::QuadVertex* QuadVertexBufferIterator = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32 TextureSlotIndex = 1; // 0: White texture

		glm::vec4 QuadVertexPositions[4]{};

		Renderer2D::Statistics Stats;
	};


	static Renderer2DData Data;


	Renderer2D::QuadBuilder::QuadBuilder()
	{
		CL_PROFILE_FUNCTION_LOW()

		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		for (uint32 i = 0; i < 4; ++i)
		{
			// Emplaced position transformation for better performance
			QuadData[i].Position           = { 0.f, 0.f, 0.f };
			QuadData[i].Color              = glm::vec4(1.f);
			QuadData[i].TextureCoordinates = textureCoords[i];
			QuadData[i].TextureIndex       = 0.f;
		}

		Texture = Data.TextureSlots[0];
	}

	void Renderer2D::QuadBuilder::SetPosition(const glm::vec3& position)
	{
		CL_PROFILE_FUNCTION_LOW()

		Position = position;

		for (uint32 i = 0; i < 4; ++i)
		{
			QuadData[i].Position = {
				Position.x + TransformedQVP[i].x,
				Position.y + TransformedQVP[i].y,
				Position.z
			};
		}
	}

	void Renderer2D::QuadBuilder::SetPosition(const glm::vec2& position)
	{
		CL_PROFILE_FUNCTION_LOW()

		Position.x = position.x;
		Position.y = position.y;

		for (uint32 i = 0; i < 4; ++i)
		{
			QuadData[i].Position.x = Position.x + TransformedQVP[i].x;
			QuadData[i].Position.y = Position.y + TransformedQVP[i].y;
		}
	}

	void Renderer2D::QuadBuilder::SetZPosition(float32 zPosition)
	{
		CL_PROFILE_FUNCTION_LOW()

		Position.z = zPosition;

		for (auto& vertex : QuadData)
			vertex.Position.z = zPosition;
	}

	void Renderer2D::QuadBuilder::SetScale(const glm::vec2& scale)
	{
		CL_PROFILE_FUNCTION_LOW()

		Scale = scale;

		// Setting Rotation to itself to recalculate TransformedQVP
		SetRotation(Rotation);
	}

	void Renderer2D::QuadBuilder::SetRotation(float32 radians)
	{
		CL_PROFILE_FUNCTION_LOW()

		Rotation = radians;

		const float32 c = cos(Rotation);
		const float32 s = sin(Rotation);

		glm::mat2 rotationMatrix = {
			{ c * Scale.x, s * Scale.x },
			{ -s * Scale.y, c * Scale.y }
		};

		for (uint32 i = 0; i < 4; ++i)
		{
			const auto& qvp = Data.QuadVertexPositions[i];

			TransformedQVP[i].x = qvp.x * rotationMatrix[0][0] + qvp.y * rotationMatrix[1][0];
			TransformedQVP[i].y = qvp.x * rotationMatrix[0][1] + qvp.y * rotationMatrix[1][1];

			QuadData[i].Position.x = Position.x + TransformedQVP[i].x;
			QuadData[i].Position.y = Position.y + TransformedQVP[i].y;
		}
	}

	void Renderer2D::QuadBuilder::SetTexture(const Ref<Texture2D>& texture)
	{
		CL_PROFILE_FUNCTION_LOW()

		Texture              = texture;
		float32 textureIndex = -1.f;

		for (uint32 i = 0; i < Data.TextureSlotIndex; ++i)
		{
			if (*Data.TextureSlots[i] == *Texture)
			{
				textureIndex = static_cast<float32>(i);
				break;
			}
		}

		if (textureIndex == -1.f)
		{
			if (Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				Flush();

			textureIndex                               = static_cast<float32>(Data.TextureSlotIndex);
			Data.TextureSlots[Data.TextureSlotIndex++] = Texture;
		}

		for (auto& vertex : QuadData)
			vertex.TextureIndex = textureIndex;
	}

	void Renderer2D::QuadBuilder::ResetTexture()
	{
		CL_PROFILE_FUNCTION_LOW()

		Texture = nullptr;

		for (auto& vertex : QuadData)
			vertex.TextureIndex = 0;
	}

	void Renderer2D::QuadBuilder::SetColor(const glm::vec4& color)
	{
		CL_PROFILE_FUNCTION_LOW()

		Color = color;

		for (auto& vertex : QuadData)
			vertex.Color = Color;
	}

	void Renderer2D::QuadBuilder::Move(const glm::vec3& position)
	{
		CL_PROFILE_FUNCTION_LOW()

		Position += position;

		for (auto& vertex : QuadData)
			vertex.Position += position;
	}

	void Renderer2D::QuadBuilder::Move(const glm::vec2& position)
	{
		CL_PROFILE_FUNCTION_LOW()

		Position.x += position.x;
		Position.y += position.y;

		for (auto& vertex : QuadData)
		{
			vertex.Position.x += position.x;
			vertex.Position.y += position.y;
		}
	}

	void Renderer2D::QuadBuilder::Revalidate()
	{
		CL_PROFILE_FUNCTION_LOW()

		SetTexture(Texture);
	}

	void Renderer2D::QuadBuilder::Draw() const
	{
		CL_PROFILE_FUNCTION_LOW()

		if (Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			Flush();

		memcpy(Data.QuadVertexBufferIterator, QuadData, sizeof QuadData);

		Data.QuadVertexBufferIterator += 4;
		Data.QuadIndexCount += 6;

		++Data.Stats.QuadCount;
	}


	void Renderer2D::Init()
	{
		CL_PROFILE_FUNCTION_HIGH()

		Data.QuadVertexArray = VertexArray::Create();

		Data.QuadVertexBuffer = VertexBuffer::Create(Data.MaxVertices * sizeof QuadVertex);

		Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "Position" },
			{ ShaderDataType::Float4, "Color" },
			{ ShaderDataType::Float2, "TextureCoordinates" },
			{ ShaderDataType::Float, "TextureIndex" }
		});

		Data.QuadVertexArray->AddVertexBuffer(Data.QuadVertexBuffer);

		Data.QuadVertexBufferData     = new QuadVertex[Data.MaxVertices];
		Data.QuadVertexBufferIterator = Data.QuadVertexBufferData;

		auto* quadIndices = new uint32[Renderer2DData::MaxIndices];

		for (uint32 i = 0, offset = 0; i < Renderer2DData::MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		const auto quadIB = IndexBuffer::Create(quadIndices, Renderer2DData::MaxIndices);
		Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;


		int32 samplers[Renderer2DData::MaxTextureSlots];

		for (int32 i    = 0; i < Renderer2DData::MaxTextureSlots; ++i)
			samplers[i] = i;

		Data.ColorTextureShader = Shader::Create("Resources/Shaders/ColorTextureShader.glsl");

		Data.ColorTextureShader->Bind();
		Data.ColorTextureShader->SetIntArray("uTextures", samplers, Renderer2DData::MaxTextureSlots);

		Data.WhiteTexture       = Texture2D::Create(1, 1);
		uint32 whiteTextureData = 0xffffffff;
		Data.WhiteTexture->SetData(&whiteTextureData, sizeof uint32);

		Data.TextureSlots[0] = Data.WhiteTexture;

		Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		Data.QuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		Data.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::ShutDown()
	{
		CL_PROFILE_FUNCTION_HIGH()

		delete[] Data.QuadVertexBufferData;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		CL_PROFILE_FUNCTION_HIGH()

		static RString uViewProjection = "uViewProjection";

		Data.ColorTextureShader->Bind();
		Data.ColorTextureShader->SetMat4(uViewProjection, camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		CL_PROFILE_FUNCTION_HIGH()

		Flush();
	}

	void Renderer2D::Flush()
	{
		CL_PROFILE_FUNCTION_MEDIUM()

		if (Data.QuadIndexCount == 0)
			return;

		const uint32 dataSize = reinterpret_cast<byte*>(Data.QuadVertexBufferIterator) - reinterpret_cast<byte*>(Data.QuadVertexBufferData);
		Data.QuadVertexBuffer->SetData(Data.QuadVertexBufferData, dataSize);

		for (uint32 i = 0; i < Data.TextureSlotIndex; ++i)
			Data.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(Data.QuadVertexArray, Data.QuadIndexCount);

		Data.QuadVertexBufferIterator = Data.QuadVertexBufferData;
		Data.QuadIndexCount           = 0;

		Data.TextureSlotIndex = 1;

		++Data.Stats.DrawCalls;
	}

	void Renderer2D::ResetStatistics()
	{
		CL_PROFILE_FUNCTION_HIGH()

		memset(&Data.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStatistics()
	{
		return Data.Stats;
	}

	uint32 Renderer2D::GetSizeOfQuad()
	{
		return sizeof QuadVertex;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.f }, size, Data.WhiteTexture, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(position, size, Data.WhiteTexture, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.f }, size, texture, glm::vec4(1.f));
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad(position, size, texture, glm::vec4(1.f));
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.f }, size, texture, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		CL_PROFILE_FUNCTION_LOW()

		if (Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			Flush();

		float32 textureIndex = -1.f;

		for (uint32 i = 0; i < Data.TextureSlotIndex; ++i)
		{
			if (*Data.TextureSlots[i] == *texture)
			{
				textureIndex = static_cast<float32>(i);
				break;
			}
		}

		if (textureIndex == -1.f)
		{
			textureIndex                               = static_cast<float32>(Data.TextureSlotIndex);
			Data.TextureSlots[Data.TextureSlotIndex++] = texture;
		}

		constexpr uint32 quadVertexCount    = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };


		for (int32 i = 0; i < quadVertexCount; ++i)
		{
			const auto& qvp = Data.QuadVertexPositions[i];

			// Emplaced position transformation for better performance
			Data.QuadVertexBufferIterator->Position = {
				qvp.w * position.x + qvp.x * size.x,
				qvp.w * position.y + qvp.y * size.y,
				qvp.w * position.z + qvp.z
			};
			Data.QuadVertexBufferIterator->Color              = color;
			Data.QuadVertexBufferIterator->TextureCoordinates = textureCoords[i];
			Data.QuadVertexBufferIterator->TextureIndex       = textureIndex;
			++Data.QuadVertexBufferIterator;
		}

		Data.QuadIndexCount += 6;

		++Data.Stats.QuadCount;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float32 rotation, const glm::vec2& size, const glm::vec4& color)
	{
		DrawRotatedQuad(position, rotation, size, Data.WhiteTexture, color);
	}


	auto Renderer2D::DrawRotatedQuad(const glm::vec3& position, float32 rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color) -> void
	{
		CL_PROFILE_FUNCTION_LOW()

		if (Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			Flush();

		float32 textureIndex = -1.f;

		for (uint32 i = 0; i < Data.TextureSlotIndex; ++i)
		{
			if (*Data.TextureSlots[i] == *texture)
			{
				textureIndex = static_cast<float32>(i);
				break;
			}
		}

		if (textureIndex == -1.f)
		{
			if (Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				Flush();

			textureIndex                               = static_cast<float32>(Data.TextureSlotIndex);
			Data.TextureSlots[Data.TextureSlotIndex++] = texture;
		}

		const float32 c = cos(rotation);
		const float32 s = sin(rotation);

		//const glm::mat4 transform = {
		//	{ c * size.x,	s * size.x, 0.f,		0.f },
		//	{ -s * size.y,	c * size.y, 0.f,		0.f },
		//	{ 0.f,			0.f,		1.f,		0.f },
		//	{ position.x,	position.y, position.z, 1.f }
		//};

		// Fragment that would be affected by rotation in 4x4 matrix
		const glm::mat2 transform = {
			{ c * size.x, s * size.x },
			{ -s * size.y, c * size.y }
		};

		constexpr uint32 quadVertexCount    = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };


		for (int32 i = 0; i < quadVertexCount; ++i)
		{
			const auto& qvp = Data.QuadVertexPositions[i];

			// Emplaced position transformation for better performance
			Data.QuadVertexBufferIterator->Position = {
				qvp.w * position.x + qvp.x * transform[0][0] + qvp.y * transform[1][0],
				qvp.w * position.y + qvp.x * transform[0][1] + qvp.y * transform[1][1],
				qvp.w * position.z + qvp.z
			};
			Data.QuadVertexBufferIterator->Color              = color;
			Data.QuadVertexBufferIterator->TextureCoordinates = textureCoords[i];
			Data.QuadVertexBufferIterator->TextureIndex       = textureIndex;
			++Data.QuadVertexBufferIterator;
		}

		Data.QuadIndexCount += 6;

		++Data.Stats.QuadCount;
	}
}
