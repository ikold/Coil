#include "Coil.h"

#include <glm/gtc/matrix_transform.hpp>


class ApplicationLayer final : public Coil::Layer
{
public:
	ApplicationLayer()
		: Camera(-1.6f, 1.6f, -0.9f, 0.9f),
		  FrameTime(Coil::PString("%8f ms", 0.f)),
		  MousePosition(Coil::PString("x: %6d y: %6d", 0, 0))
	{
		Coil::ImGuiInterface::Create<Coil::ImGuiLogWindow>("Log").BindBuffer(Coil::Logger::GetBuffer());

		Coil::ImGuiInterface::Create<Coil::ImGuiOverlay>("frame time").BindTextBuffer(FrameTime);

		Coil::Logger::Trace(MousePosition);

		{
			VertexArray.reset(Coil::VertexArray::Create());

			float32 vertices[3 * 7] = {
				-0.5f, -0.5f, 0.0f, 1.f, 0.f, 1.f, 1.f,
				0.5f, -0.5f, 0.0f, 0.f, 0.f, 1.f, 1.f,
				0.0f, 0.5f, 0.0f, 1.f, 1.f, 0.f, 1.f
			};

			std::shared_ptr<Coil::VertexBuffer> vertexBuffer;
			vertexBuffer.reset(Coil::VertexBuffer::Create(vertices, sizeof(vertices)));

			vertexBuffer->SetLayout({
				{ Coil::ShaderDataType::Float3, "position" },
				{ Coil::ShaderDataType::Float4, "color" }
			});

			VertexArray->AddVertexBuffer(vertexBuffer);

			uint32 indices[3] = { 0, 1, 2 };

			std::shared_ptr<Coil::IndexBuffer> indexBuffer;
			indexBuffer.reset(Coil::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32)));

			VertexArray->SetIndexBuffer(indexBuffer);
		}

		{
			SquareVertexArray.reset(Coil::VertexArray::Create());

			float32 vertices[4 * 3] = {
				-0.5f, -0.5f, 0.0f,
				0.5f, -0.5f, 0.0f,
				0.5f, 0.5f, 0.0f,
				-0.5f, 0.5f, 0.0f
			};

			std::shared_ptr<Coil::VertexBuffer> vertexBuffer;
			vertexBuffer.reset(Coil::VertexBuffer::Create(vertices, sizeof(vertices)));

			vertexBuffer->SetLayout({
				{ Coil::ShaderDataType::Float3, "position" }
			});

			SquareVertexArray->AddVertexBuffer(vertexBuffer);

			uint32 indices[6] = { 0, 1, 2, 0, 2, 3 };

			std::shared_ptr<Coil::IndexBuffer> indexBuffer;
			indexBuffer.reset(Coil::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32)));

			SquareVertexArray->SetIndexBuffer(indexBuffer);
		}

		VertexColorShader = std::make_shared<Coil::Shader>(
			Coil::File::Load("Resources/Shaders/VertexColor.vert"),
			Coil::File::Load("Resources/Shaders/VertexColor.frag")
		);

		RainbowShader = std::make_shared<Coil::Shader>(
			Coil::File::Load("Resources/Shaders/Rainbow.vert"),
			Coil::File::Load("Resources/Shaders/Rainbow.frag")
		);
	}

	~ApplicationLayer() = default;

	void OnUpdate() override
	{
		if (!Coil::Input::IsKeyPressed(CL_KEY_TAB))
		{
			auto [x, y] = Coil::Input::GetMousePosition();
			MousePosition->Set(0, static_cast<int32>(x));
			MousePosition->Set(1, static_cast<int32>(y));

			//Camera.SetPosition({
			//	x / Coil::Application::Get().GetWindow().GetWidth() * -2 + 1,
			//	y / Coil::Application::Get().GetWindow().GetHeight() * 2 - 1,
			//	0.f
			//});
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


		if (Coil::Input::IsKeyPressed(CL_KEY_Q))
			CameraRotation -= CameraRotationSpeed * (Coil::Time::DeltaTime() / 1000);

		if (Coil::Input::IsKeyPressed(CL_KEY_E))
			CameraRotation += CameraRotationSpeed * (Coil::Time::DeltaTime() / 1000);


		Camera.SetPosition(CameraPosition);
		Camera.SetRotation(CameraRotation);


		FrameTimeArray[Counter] = Coil::Time::DeltaTime();

		if (++Counter > 59)
		{
			Counter     = 0;
			float32 sum = 0.f;
			for (auto i : FrameTimeArray)
				sum += i;

			FrameTime->Set(0, sum / 60);
		}


		{
			Coil::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
			Coil::RenderCommand::Clear();

			Coil::Renderer::BeginScene(Camera);


			static glm::mat4 scale = glm::scale(glm::mat4(1.f), glm::vec3(0.1f));

			for (int32 x = -10; x < 10; ++x)
			{
				for (int32 y = -10; y < 10; ++y)
				{
					glm::mat4 squareTransform = glm::translate(
						glm::mat4(1.f), glm::vec3(x * 0.11f, y * 0.11f, 0.f)) * scale;
					Coil::Renderer::Submit(RainbowShader, SquareVertexArray, squareTransform);
				}
			}


			Coil::Renderer::Submit(VertexColorShader, VertexArray);

			Coil::Renderer::EndScene();
		}
	}

	void OnEvent(Coil::Event& event) override
	{
		Coil::EventDispatcher dispatcher(event);
	}

private:
	std::shared_ptr<Coil::VertexArray> VertexArray, SquareVertexArray;
	std::shared_ptr<Coil::Shader> RainbowShader, VertexColorShader;
	Coil::OrthographicCamera Camera;

	Coil::RString<Coil::PString> FrameTime, MousePosition;

	glm::vec3 CameraPosition = glm::vec3(0);
	float32 CameraSpeed      = 0.01f;

	float32 CameraRotation      = 0;
	float32 CameraRotationSpeed = 50.f;

	int32 Counter = 0;
	float32 FrameTimeArray[60]{};
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
