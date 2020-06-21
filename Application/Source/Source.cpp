#include "Coil.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"


class ApplicationLayer final : public Coil::Layer
{
public:
	ApplicationLayer()
		: Camera(-1.6f, 1.6f, -0.9f, 0.9f),
		  FrameTime(Coil::PString("%8f ms", 0.f)),
		  MousePosition(Coil::PString("mouse x: %6d y: %6d", 0, 0)),
		  FrameIterator(FrameTimeArray->begin()),
		  CameraString(Coil::PString("camera x: %6f y: %6f", 0, 0))
	{
		Coil::GUI::LogWindow({ "Log" })->BindBuffer(Coil::Logger::GetBuffer());

		Coil::GUI::Overlay({ "frame time" })->BindTextBuffer(FrameTime);

		Coil::Logger::Debug(Coil::PString("%d, %d", Coil::Application::Get().GetWindow().GetWidth(), Coil::Application::Get().GetWindow().GetHeight()));

		SquareColor = std::make_shared<glm::vec3>(0.2f);

		Coil::RString buffer = Coil::BString("Input test");
		Coil::Logger::Info(buffer);
		buffer->Expand(1);
		Coil::RString multiLineBuffer = Coil::BString("Multiline\nInput\ntest");
		Coil::Logger::Info(multiLineBuffer);

		using Coil::GUI;

		GUI::ComponentWindow({ "Name" }, {
			GUI::Text("Square Color"),
			GUI::ColorPicker(SquareColor),
			GUI::TextInput(buffer),
			GUI::MultiLineTextInput({ "", -1.f }, multiLineBuffer),
			GUI::Text("Camera Rotation"),
			GUI::FloatSlider(CameraRotation, -180.f, 180.f),
			GUI::Text("Camera Zoom"),
			GUI::FloatSlider(CameraScale, 0.1f, 2.f),
			GUI::Text("Frame Time"),
			GUI::PlotLine({ "", 0, 32.f }, FrameTimeArray, 0.f, 50.f),
			GUI::Button({ "Reset camera position" }, [&] { Camera.SetPosition({ 0.f, 0.f, 0.f }); }),
			GUI::Button({ "Button", 0, 0, true }, [] { Coil::Logger::Info("Second Button pressed!"); }),
			GUI::Button({ "Button", 0, 0, true }, [] { Coil::Logger::Info("Third Button pressed!"); }),
			GUI::Button({ "Toggle Log window visibility" }, [] { GUI::ToggleWindowVisibility("Log"); }),
		});

		Coil::Logger::Trace(MousePosition);
		Coil::Logger::Trace(CameraString);

		GUI::GetWindow<Coil::GUIComponentWindow>("Name")->AddComponent(GUI::Text("Component added to retrieved window"));

		{
			VertexArray = Coil::VertexArray::Create();

			float32 vertices[3 * 7] = {
				-0.5f, -0.5f, 0.0f, 1.f, 0.f, 1.f, 1.f,
				0.5f, -0.5f, 0.0f, 0.f, 0.f, 1.f, 1.f,
				0.0f, 0.5f, 0.0f, 1.f, 1.f, 0.f, 1.f
			};

			Coil::Ref<Coil::VertexBuffer> vertexBuffer = Coil::VertexBuffer::Create(vertices, sizeof(vertices));

			vertexBuffer->SetLayout({
				{ Coil::ShaderDataType::Float3, "position" },
				{ Coil::ShaderDataType::Float4, "color" }
			});

			VertexArray->AddVertexBuffer(vertexBuffer);

			uint32 indices[3] = { 0, 1, 2 };

			Coil::Ref<Coil::IndexBuffer> indexBuffer = Coil::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32));

			VertexArray->SetIndexBuffer(indexBuffer);
		}

		{
			SquareVertexArray = Coil::VertexArray::Create();

			float32 vertices[4 * 5] = {
				-0.5f, -0.5f, 0.f, 0.f, 0.f,
				0.5f, -0.5f, 0.f, 1.f, 0.f,
				0.5f, 0.5f, 0.f, 1.f, 1.f,
				-0.5f, 0.5f, 0.f, 0.f, 1.f
			};

			Coil::Ref<Coil::VertexBuffer> vertexBuffer = Coil::VertexBuffer::Create(vertices, sizeof(vertices));

			vertexBuffer->SetLayout({
				{ Coil::ShaderDataType::Float3, "position" },
				{ Coil::ShaderDataType::Float2, "TextureCoordinates" }
			});

			SquareVertexArray->AddVertexBuffer(vertexBuffer);

			uint32 indices[6] = { 0, 1, 2, 0, 2, 3 };

			std::shared_ptr<Coil::IndexBuffer> indexBuffer = Coil::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32));

			SquareVertexArray->SetIndexBuffer(indexBuffer);
		}

		VertexColorShader = Coil::Shader::Create("Resources/Shaders/VertexColor.glsl");
		RainbowShader     = Coil::Shader::Create("Resources/Shaders/Rainbow.glsl");
		ColorShader       = Coil::Shader::Create("Resources/Shaders/Color.glsl");
		TextureShader     = Coil::Shader::Create("Resources/Shaders/Texture.glsl");

		Texture = Coil::Texture2D::Create("Resources/Textures/Colorgrid.png");

		TextureShader->Bind();
		std::dynamic_pointer_cast<Coil::OpenGLShader>(TextureShader)->UploadUniformInt("uTexture", 0);
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

		float32 relativeCameraSpeed = CameraSpeed * Coil::Time::DeltaTime();

		if (Coil::Input::IsKeyPressed(CL_KEY_LEFT))
		{
			Camera.MoveRelativePosition({ -relativeCameraSpeed, 0.f, 0.f });
		}

		if (Coil::Input::IsKeyPressed(CL_KEY_RIGHT))
		{
			Camera.MoveRelativePosition({ relativeCameraSpeed, 0.f, 0.f });
		}

		if (Coil::Input::IsKeyPressed(CL_KEY_DOWN))
		{
			Camera.MoveRelativePosition({ 0.f, relativeCameraSpeed, 0.f });
		}

		if (Coil::Input::IsKeyPressed(CL_KEY_UP))
		{
			Camera.MoveRelativePosition({ 0.f, -relativeCameraSpeed, 0.f });
		}

		*CameraScale    = Coil::Math::Truncate(*CameraScale, 0.1f, 2.f);
		*CameraRotation = Coil::Math::WrapCycle(*CameraRotation, 360.f, -180.f, 180.f);

		Camera.SetScale(glm::vec3(*CameraScale));
		Camera.SetRotation(*CameraRotation);

		*FrameIterator = Coil::Time::DeltaTime();

		CameraString->Set(0, Camera.GetPosition().x);
		CameraString->Set(1, Camera.GetPosition().y);

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

			const int32 gridHeight = 20;
			const int32 gridWidth  = 40;

			TimeIteration += Coil::Time::DeltaTime() / 1000.f * 0.5f;

			while (TimeIteration > 4.f)
				TimeIteration -= 4.f;

			for (int32 x = -gridWidth / 2; x < (gridWidth - gridWidth / 2); ++x)
			{
				for (int32 y = -gridHeight / 2; y < (gridHeight - gridHeight / 2); ++y)
				{
					static glm::mat4 scale = glm::scale(glm::mat4(1.f), glm::vec3(0.1f));

					glm::mat4 squareTransform = glm::translate(
						glm::mat4(1.f), glm::vec3((x - TimeIteration) * 0.11f, (y + TimeIteration / 4) * 0.11f, 0.f)) * scale;

					Coil::Renderer::Submit(ColorShader, SquareVertexArray, squareTransform);
				}
			}

			Texture->Bind();
			Coil::Renderer::Submit(TextureShader, SquareVertexArray);

			//Coil::Renderer::Submit(VertexColorShader, VertexArray);

			Coil::Renderer::EndScene();
		}
	}

	void OnEvent(Coil::Event& event) override
	{
		Coil::EventDispatcher dispatcher(event);

		dispatcher.Dispatch<Coil::MouseScrolledEvent>([&](Coil::MouseScrolledEvent& event) -> bool
		{
			if (Coil::Input::IsKeyPressed(CL_KEY_LEFT_CONTROL) && Coil::Input::IsKeyPressed(CL_KEY_LEFT_ALT))
				*CameraRotation += event.GetYOffset() * 5.f;
			else
				*CameraScale -= event.GetYOffset() * 0.1f;

			return false;
		});

		dispatcher.Dispatch<Coil::MouseMovedEvent>([&](Coil::MouseMovedEvent& event) -> bool
		{
			if (Coil::Input::IsMouseButtonPressed(CL_MOUSE_BUTTON_MIDDLE))
			{
				const uint32 windowWidth  = Coil::Application::Get().GetWindow().GetWidth();
				const uint32 windowHeight = Coil::Application::Get().GetWindow().GetHeight();

				Camera.MoveRelativePosition({
					(LastMouseEvent.GetX() - event.GetX()) / windowWidth * 2 * 1.6f,
					(LastMouseEvent.GetY() - event.GetY()) / windowHeight * 2 * 0.9f,
					0.f
				});
			}

			LastMouseEvent = event;

			return false;
		});
	}

private:
	Coil::Ref<Coil::VertexArray> VertexArray, SquareVertexArray;
	Coil::Ref<Coil::Shader> RainbowShader, VertexColorShader, ColorShader, TextureShader;
	Coil::Ref<Coil::Texture2D> Texture;

	Coil::OrthographicCamera Camera;

	Coil::RString<Coil::PString> FrameTime, MousePosition;

	float32 CameraSpeed = 0.001f;

	float32 TimeIteration = 0.f;

	Coil::Ref<float32> CameraRotation = std::make_shared<float32>(0);
	Coil::Ref<float32> CameraScale    = std::make_shared<float32>(1);

	Coil::Ref<std::vector<float32>> FrameTimeArray = std::make_shared<std::vector<float32>>(120, 0);
	std::vector<float32>::iterator FrameIterator;

	Coil::Ref<glm::vec3> SquareColor;

	Coil::MouseMovedEvent LastMouseEvent = Coil::MouseMovedEvent(0, 0);

	Coil::RString<Coil::PString> CameraString;
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
