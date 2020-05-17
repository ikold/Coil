#include "Coil.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"


class ApplicationLayer final : public Coil::Layer
{
public:
	ApplicationLayer()
		: Camera(-1.6f, 1.6f, -0.9f, 0.9f),
		  FrameTime(Coil::PString("%8f ms", 0.f)),
		  MousePosition(Coil::PString("x: %6d y: %6d", 0, 0)),
		  FrameIterator(FrameTimeArray->begin())
	{
		Coil::GUI::LogWindow({ "Log" })->BindBuffer(Coil::Logger::GetBuffer());

		Coil::GUI::Overlay({ "frame time" })->BindTextBuffer(FrameTime);

		SquareColor = std::make_shared<glm::vec3>(0.2f);

		Coil::RString buffer = Coil::BString("Input test");
		Coil::Logger::Info(buffer);
		buffer->Expand(1);
		Coil::RString multiLineBuffer = Coil::BString("Multiline\nInput\ntest");
		Coil::Logger::Info(multiLineBuffer);

		Coil::GUI::ComponentWindow({ "Name" }, {
			Coil::GUI::Text("Square Color"),
			Coil::GUI::ColorPicker(SquareColor),
			Coil::GUI::TextInput(buffer),
			Coil::GUI::MultiLineTextInput({"", -1.f}, multiLineBuffer),
			Coil::GUI::Text("Camera Rotation"),
			Coil::GUI::FloatSlider(CameraRotation, -180.f, 180.f),
			Coil::GUI::Text("Frame Time"),
			Coil::GUI::PlotLine({ "", 0, 32.f }, FrameTimeArray, 0.f, 50.f),
			Coil::GUI::Button({ "Button" }, [] { Coil::Logger::Info("First Button pressed!"); }),
			Coil::GUI::Button({ "Button", 0, 0, true }, [] { Coil::Logger::Info("Second Button pressed!"); }),
			Coil::GUI::Button({ "Button", 0, 0, true }, [] { Coil::Logger::Info("Third Button pressed!"); }),
		});

		Coil::Logger::Trace(MousePosition);

		{
			VertexArray.reset(Coil::VertexArray::Create());

			float32 vertices[3 * 7] = {
				-0.5f, -0.5f, 0.0f, 1.f, 0.f, 1.f, 1.f,
				0.5f, -0.5f, 0.0f, 0.f, 0.f, 1.f, 1.f,
				0.0f, 0.5f, 0.0f, 1.f, 1.f, 0.f, 1.f
			};

			Coil::Ref<Coil::VertexBuffer> vertexBuffer;
			vertexBuffer.reset(Coil::VertexBuffer::Create(vertices, sizeof(vertices)));

			vertexBuffer->SetLayout({
				{ Coil::ShaderDataType::Float3, "position" },
				{ Coil::ShaderDataType::Float4, "color" }
			});

			VertexArray->AddVertexBuffer(vertexBuffer);

			uint32 indices[3] = { 0, 1, 2 };

			Coil::Ref<Coil::IndexBuffer> indexBuffer;
			indexBuffer.reset(Coil::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32)));

			VertexArray->SetIndexBuffer(indexBuffer);
		}

		{
			SquareVertexArray.reset(Coil::VertexArray::Create());

			float32 vertices[4 * 5] = {
				-0.5f, -0.5f, 0.f, 0.f, 0.f,
				0.5f, -0.5f, 0.f, 1.f, 0.f,
				0.5f, 0.5f, 0.f, 1.f, 1.f,
				-0.5f, 0.5f, 0.f, 0.f, 1.f
			};

			Coil::Ref<Coil::VertexBuffer> vertexBuffer;
			vertexBuffer.reset(Coil::VertexBuffer::Create(vertices, sizeof(vertices)));

			vertexBuffer->SetLayout({
				{ Coil::ShaderDataType::Float3, "position" },
				{ Coil::ShaderDataType::Float2, "TextureCoordinates" }
			});

			SquareVertexArray->AddVertexBuffer(vertexBuffer);

			uint32 indices[6] = { 0, 1, 2, 0, 2, 3 };

			std::shared_ptr<Coil::IndexBuffer> indexBuffer;
			indexBuffer.reset(Coil::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32)));

			SquareVertexArray->SetIndexBuffer(indexBuffer);
		}

		VertexColorShader.reset(Coil::Shader::Create(
			Coil::File::Load("Resources/Shaders/VertexColor.vert"),
			Coil::File::Load("Resources/Shaders/VertexColor.frag")
		));

		RainbowShader.reset(Coil::Shader::Create(
			Coil::File::Load("Resources/Shaders/Rainbow.vert"),
			Coil::File::Load("Resources/Shaders/Rainbow.frag")
		));

		ColorShader.reset(Coil::Shader::Create(
			Coil::File::Load("Resources/Shaders/Color.vert"),
			Coil::File::Load("Resources/Shaders/Color.frag")
		));

		TextureShader.reset(Coil::Shader::Create(
			Coil::File::Load("Resources/Shaders/Texture.vert"),
			Coil::File::Load("Resources/Shaders/Texture.frag")
		));
	}

	~ApplicationLayer() = default;

	void OnUpdate() override
	{
		if (!Coil::Input::IsKeyPressed(CL_KEY_TAB))
		{
			auto [x, y] = Coil::Input::GetMousePosition();
			MousePosition->Set(0, static_cast<int32>(x));
			MousePosition->Set(1, static_cast<int32>(y));
		}
		else
		{
			Coil::Logger::Info("Tab is pressed!");
		}

		if (Coil::Input::IsKeyPressed(CL_KEY_LEFT))
			CameraPosition.x += CameraSpeed * Coil::Time::DeltaTime();

		if (Coil::Input::IsKeyPressed(CL_KEY_RIGHT))
			CameraPosition.x -= CameraSpeed * Coil::Time::DeltaTime();

		if (Coil::Input::IsKeyPressed(CL_KEY_DOWN))
			CameraPosition.y += CameraSpeed * Coil::Time::DeltaTime();

		if (Coil::Input::IsKeyPressed(CL_KEY_UP))
			CameraPosition.y -= CameraSpeed * Coil::Time::DeltaTime();

		Camera.SetPosition(CameraPosition);
		Camera.SetRotation(*CameraRotation);


		*FrameIterator = Coil::Time::DeltaTime();

		if (++FrameIterator >= FrameTimeArray->end())
			FrameIterator = FrameTimeArray->begin();

		FrameTime->Set(0, Coil::Time::DeltaTime());


		{
			Coil::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
			Coil::RenderCommand::Clear();

			Coil::Renderer::BeginScene(Camera);

			static Coil::RString<Coil::String> uColor = "uColor";

			ColorShader->Bind();
			std::dynamic_pointer_cast<Coil::OpenGLShader>(ColorShader)->UploadUniformFloat3(uColor, *SquareColor);

			for (int32 x = 0; x < 20; ++x)
			{
				for (int32 y = 0; y < 20; ++y)
				{
					static glm::mat4 scale = glm::scale(glm::mat4(1.f), glm::vec3(0.1f));

					glm::mat4 squareTransform = glm::translate(
						glm::mat4(1.f), glm::vec3((x - 10) * 0.11f, (y - 10) * 0.11f, 0.f)) * scale;

					Coil::Renderer::Submit(ColorShader, SquareVertexArray, squareTransform);
				}
			}

			Coil::Renderer::Submit(TextureShader, SquareVertexArray);

			//Coil::Renderer::Submit(VertexColorShader, VertexArray);

			Coil::Renderer::EndScene();
		}
	}

	void OnEvent(Coil::Event& event) override
	{
		Coil::EventDispatcher dispatcher(event);
	}
	
private:
	Coil::Ref<Coil::VertexArray> VertexArray, SquareVertexArray;
	Coil::Ref<Coil::Shader> RainbowShader, VertexColorShader, ColorShader, TextureShader;
	Coil::OrthographicCamera Camera;

	Coil::RString<Coil::PString> FrameTime, MousePosition;

	glm::vec3 CameraPosition = glm::vec3(0);
	float32 CameraSpeed      = 0.001f;

	Coil::Ref<float32> CameraRotation = std::make_shared<float32>(0);

	int32 Counter                                  = 0;
	Coil::Ref<std::vector<float32>> FrameTimeArray = std::make_shared<std::vector<float32>>(120, 0);
	std::vector<float32>::iterator FrameIterator;

	Coil::Ref<glm::vec3> SquareColor;
};


class App final : public cl::Application
{
public:
	App()
	{
		PushLayer(new ApplicationLayer());
	}

	~App() = default;
};


BindApplication(App)
