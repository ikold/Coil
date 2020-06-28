#include "Coil.h"
#include "Coil/EntryPoint.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"


class ApplicationLayer final : public Coil::Layer
{
public:
	ApplicationLayer()
		: Layer("Application"),
		  CameraController(1280.0f / 720.0f, true),
		  FrameTime(Coil::PString("%8f ms", 0.f)),
		  MousePosition(Coil::PString("mouse x: %6d y: %6d", 0, 0)),
		  SquareColor(Coil::CreateRef<glm::vec3>(0.2f))
	{
		Coil::GUI::LogWindow({ "Log" })->BindBuffer(Coil::Logger::GetBuffer());

		Coil::GUI::Overlay({ "frame time" })->BindTextBuffer(FrameTime);
		Coil::GUI::ComponentWindow({ "Name" }, {
			Coil::GUI::Text("Square Color"),
			Coil::GUI::ColorPicker(SquareColor)
		});

		Coil::Logger::Trace(MousePosition);

		{
			SquareVertexArray = Coil::VertexArray::Create();

			float32 vertices[4 * 5] = {
				-0.5f, -0.5f, 0.f, 0.f, 0.f,
				0.5f, -0.5f, 0.f, 1.f, 0.f,
				0.5f, 0.5f, 0.f, 1.f, 1.f,
				-0.5f, 0.5f, 0.f, 0.f, 1.f
			};

			Coil::Ref<Coil::VertexBuffer> vertexBuffer = Coil::VertexBuffer::Create(vertices, sizeof vertices);

			vertexBuffer->SetLayout({
				{ Coil::ShaderDataType::Float3, "position" },
				{ Coil::ShaderDataType::Float2, "TextureCoordinates" }
			});

			SquareVertexArray->AddVertexBuffer(vertexBuffer);

			uint32 indices[6] = { 0, 1, 2, 0, 2, 3 };

			Coil::Ref<Coil::IndexBuffer> indexBuffer = Coil::IndexBuffer::Create(indices, sizeof indices / sizeof(uint32));

			SquareVertexArray->SetIndexBuffer(indexBuffer);
		}

		ColorShader   = Coil::Shader::Create("Resources/Shaders/Color.glsl");
		TextureShader = Coil::Shader::Create("Resources/Shaders/Texture.glsl");

		Texture = Coil::Texture2D::Create("Resources/Textures/Colorgrid.png");

		TextureShader->Bind();
		std::dynamic_pointer_cast<Coil::OpenGLShader>(TextureShader)->UploadUniformInt("uTexture", 0);
	}

	void OnUpdate() override
	{
		FrameTime->Set(0, Coil::Time::DeltaTime());

		CameraController.OnUpdate();

		auto [mouseX, mouseY] = Coil::Input::GetMousePosition();
		MousePosition->Set(0, static_cast<int32>(mouseX));
		MousePosition->Set(1, static_cast<int32>(mouseY));


		{
			Coil::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
			Coil::RenderCommand::Clear();

			Coil::Renderer::BeginScene(CameraController.GetCamera());

			static Coil::RString<Coil::String> uColor = "uColor";

			ColorShader->Bind();
			std::dynamic_pointer_cast<Coil::OpenGLShader>(ColorShader)->UploadUniformFloat3(uColor, *SquareColor);

			const int32 gridHeight = 20;
			const int32 gridWidth  = 40;

			TimeIteration += Coil::Time::DeltaTime() / 1000.f * 0.5f;

			while (TimeIteration > 4.f)
				TimeIteration -= 4.f;

			for (int32 x = -gridWidth / 2; x < gridWidth - gridWidth / 2; ++x)
			{
				for (int32 y = -gridHeight / 2; y < gridHeight - gridHeight / 2; ++y)
				{
					static glm::mat4 scale = glm::scale(glm::mat4(1.f), glm::vec3(0.1f));

					glm::mat4 squareTransform = glm::translate(
						glm::mat4(1.f), glm::vec3((x - TimeIteration) * 0.11f, (y + TimeIteration / 4) * 0.11f, 0.f)) * scale;

					Coil::Renderer::Submit(ColorShader, SquareVertexArray, squareTransform);
				}
			}

			Texture->Bind();
			Coil::Renderer::Submit(TextureShader, SquareVertexArray);

			Coil::Renderer::EndScene();
		}
	}

	void OnEvent(Coil::Event& event) override
	{
		CameraController.OnEvent(event);
	}

private:
	Coil::Ref<Coil::VertexArray> VertexArray, SquareVertexArray;
	Coil::Ref<Coil::Shader> ColorShader, TextureShader;
	Coil::Ref<Coil::Texture2D> Texture;

	Coil::OrthographicCameraController CameraController;

	Coil::RString<Coil::PString> FrameTime, MousePosition;

	Coil::Ref<glm::vec3> SquareColor;

	float32 TimeIteration = 0.f;
};


class App final : public cl::Application
{
public:
	App()
	{
		PushLayer(new ApplicationLayer());
	}
};


BindApplication(App)
