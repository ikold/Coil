#include "pch.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"
#include <glm/ext/matrix_transform.hpp>


namespace Coil
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> ColorTextureShader;
		Ref<Texture2D> WhiteTexture;
	};


	static Scope<Renderer2DStorage> Data;


	void Renderer2D::Init()
	{
		CL_PROFILE_FUNCTION()

		Data                  = CreateScope<Renderer2DStorage>();
		Data->QuadVertexArray = VertexArray::Create();

		float32 squareVertices[4 * 5] = {
			-0.5f, -0.5f, 0.f, 0.f, 0.f,
			 0.5f, -0.5f, 0.f, 1.f, 0.f,
			 0.5f,  0.5f, 0.f, 1.f, 1.f,
			-0.5f,  0.5f, 0.f, 0.f, 1.f
		};

		Ref<VertexBuffer> squareVB = VertexBuffer::Create(squareVertices, sizeof squareVertices);

		squareVB->SetLayout({
			{ ShaderDataType::Float3, "Position" },
			{ ShaderDataType::Float2, "TextureCoordinates" }
		});

		Data->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32 squareIndices[6]         = { 0, 1, 2, 2, 3, 0 };
		const Ref<IndexBuffer> squareIB = IndexBuffer::Create(squareIndices, sizeof squareIndices / sizeof uint32);
		Data->QuadVertexArray->SetIndexBuffer(squareIB);

		Data->ColorTextureShader = Shader::Create("Resources/Shaders/ColorTextureShader.glsl");

		Data->ColorTextureShader->Bind();
		Data->ColorTextureShader->SetInt("uTexture", 0);

		Data->WhiteTexture      = Texture2D::Create(1, 1);
		uint32 whiteTextureData = 0xffffffff;
		Data->WhiteTexture->SetData(&whiteTextureData, sizeof uint32);
	}

	void Renderer2D::ShutDown()
	{
		CL_PROFILE_FUNCTION()

		Data.reset();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		CL_PROFILE_FUNCTION()

		static RString uViewProjection = "uViewProjection";

		Data->ColorTextureShader->Bind();
		Data->ColorTextureShader->SetMat4(uViewProjection, camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		CL_PROFILE_FUNCTION()
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float32 rotation, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.f }, rotation, size, Data->WhiteTexture, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float32 rotation, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(position, rotation, size, Data->WhiteTexture, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float32 rotation, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.f }, rotation, size, texture, glm::vec4(1.f));
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float32 rotation, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad(position, rotation, size, texture, glm::vec4(1.f));
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float32 rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.f }, rotation, size, texture, glm::vec4(1.f));
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float32 rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		CL_PROFILE_FUNCTION_LOW()

		static RString uColor     = "uColor";
		static RString uTransform = "uTransform";

		float32 const c = cos(rotation);
		float32 const s = sin(rotation);

		glm::mat4 transform = {
			{ c * size.x,	s * size.x,	0.f,		0.f },
			{ -s * size.y,	c * size.y,	0.f,		0.f },
			{ 0.f,			0.f,		1.f,		0.f },
			{ position.x,	position.y,	position.z,	1.f }
		};

		Data->ColorTextureShader->SetMat4(uTransform, transform);
		Data->ColorTextureShader->SetFloat4(uColor, color);
		texture->Bind();

		Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(Data->QuadVertexArray);
	}
}
