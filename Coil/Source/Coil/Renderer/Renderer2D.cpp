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
		Ref<Shader> ColorShader;
		Ref<Shader> TextureShader;
	};


	static Scope<Renderer2DStorage> Data;


	void Renderer2D::Init()
	{
		Data                  = CreateScope<Renderer2DStorage>();
		Data->QuadVertexArray = VertexArray::Create();

		float32 squareVertices[4 * 5] = {
			-0.5f, -0.5f, 0.f, 0.f, 0.f,
			0.5f, -0.5f, 0.f, 1.f, 0.f,
			0.5f, 0.5f, 0.f, 1.f, 1.f,
			-0.5f, 0.5f, 0.f, 0.f, 1.f
		};

		Ref<VertexBuffer> squareVB = VertexBuffer::Create(squareVertices, sizeof squareVertices);

		squareVB->SetLayout({
			{ ShaderDataType::Float3, "Position" },
			{ ShaderDataType::Float2, "TextureCoordinates" }
		});

		Data->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32 squareIndices[6]   = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB = IndexBuffer::Create(squareIndices, sizeof squareIndices / sizeof uint32);
		Data->QuadVertexArray->SetIndexBuffer(squareIB);

		Data->ColorShader   = Shader::Create("Resources/Shaders/Color.glsl");
		Data->TextureShader = Shader::Create("Resources/Shaders/Texture.glsl");

		Data->TextureShader->Bind();
		Data->TextureShader->SetInt("uTexture", 0);
	}

	void Renderer2D::ShoutDown()
	{
		Data.reset();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		static RString uViewProjection = "uViewProjection";

		Data->ColorShader->Bind();
		Data->ColorShader->SetMat4(uViewProjection, camera.GetViewProjectionMatrix());

		Data->TextureShader->Bind();
		Data->TextureShader->SetMat4(uViewProjection, camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{}

	void Renderer2D::DrawQuad(const glm::vec2& position, float32 rotation, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.f }, rotation, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float32 rotation, const glm::vec2& size, const glm::vec4& color)
	{
		static RString uColor = "uColor";

		Data->ColorShader->Bind();
		Data->ColorShader->SetFloat4(uColor, color);

		DrawQuad(position, rotation, size, Data->ColorShader);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float32 rotation, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.f }, rotation, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float32 rotation, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		texture->Bind();
		Data->TextureShader->Bind();

		DrawQuad(position, rotation, size, Data->TextureShader);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float32 rotation, const glm::vec2& size, const Ref<Shader>& shader)
	{
		static RString uTransform = "uTransform";

		glm::mat4 transform = translate(glm::mat4(1.f), position) * rotate(glm::mat4(1.f), rotation, { 0.f, 0.f, 1.f }) * scale(glm::mat4(1.f), { size.x, size.y, 1.f });

		shader->Bind();
		shader->SetMat4(uTransform, transform);

		Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(Data->QuadVertexArray);
	}
}
