#include "pch.h"
#include "Application.h"

#include "Coil/ImGui/ImGuiInterface.h"
#include "Coil/ImGui/ImGuiLogWindow.h"
#include "Coil/ImGui/ImGuiOverlay.h"

#include "Coil/ImGui/ImGuiLayer.h"

#include "Coil/Input.h"

#include "Coil/Renderer/Shader.h"
#include "Coil/Renderer/Buffer.h"
#include "Coil/Renderer/VertexArray.h"

#include "Coil/Renderer/Renderer.h"

#include "Coil/Utilities/File.h"

namespace Coil
{
	Application* Application::Instance = nullptr;

	Application::Application()
		: Running(true)
	{
		CL_ASSERT(!Instance, "Application instance already exist!")
		Instance = this;

		AppWindow = std::unique_ptr<Window>(Window::Create());
		AppWindow->SetEventCallback(BIND_EVENT_METHOD(Application::OnEvent));

		AppWindow->SetVSync(true);

		ImGuiInterface::Create<ImGuiLogWindow>("Log").BindBuffer(Logger::GetBuffer());
		PushOverlay(new ImGuiLayer());
	}

	void Application::Run()
	{
		Logger::Info("Initializing Application");

		Logger::Info("Running Application");

		RString frameTime = PString("%8f ms", 0.f);
		ImGuiInterface::Create<ImGuiOverlay>("frame time").BindTextBuffer(frameTime);


		RString mousePosition = PString("x: %6d y: %6d", 0, 0);
		Logger::Trace(mousePosition);
		Logger::Trace(mousePosition->ToString());

		int32 counter = 0;
		float32 frameTimeArray[60];

		std::shared_ptr<VertexArray> vertexArray;
		std::shared_ptr<VertexArray> squareVertexArray;

		{
			vertexArray.reset(VertexArray::Create());

			float32 vertices[3 * 7] = {
				-0.5f, -0.5f, 0.0f, 1.f, 0.f, 1.f, 1.f,
				 0.5f, -0.5f, 0.0f, 0.f, 0.f, 1.f, 1.f,
				 0.0f,  0.5f, 0.0f, 1.f, 1.f, 0.f, 1.f
			};

			std::shared_ptr<VertexBuffer> vertexBuffer;
			vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

			vertexBuffer->SetLayout({
				{ ShaderDataType::Float3,	"position" },
				{ ShaderDataType::Float4,	"color" }
			});

			vertexArray->AddVertexBuffer(vertexBuffer);

			uint32 indices[3] = { 0, 1, 2 };

			std::shared_ptr<IndexBuffer> indexBuffer;
			indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32)));

			vertexArray->SetIndexBuffer(indexBuffer);
		}


		{
			squareVertexArray.reset(VertexArray::Create());

			float32 vertices[4 * 3] = {
				-0.7f, -0.7f, 0.0f,
				 0.7f, -0.7f, 0.0f,
				 0.7f,  0.7f, 0.0f,
				-0.7f,  0.7f, 0.0f
			};

			std::shared_ptr<VertexBuffer> vertexBuffer;
			vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

			vertexBuffer->SetLayout({
				{ ShaderDataType::Float3,	"position" }
			});

			squareVertexArray->AddVertexBuffer(vertexBuffer);

			uint32 indices[6] = { 0, 1, 2, 0, 2, 3 };

			std::shared_ptr<IndexBuffer> indexBuffer;
			indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32)));

			squareVertexArray->SetIndexBuffer(indexBuffer);
		}

		const Shader vertexColorShader(
			File::Load("Resources/Shaders/VertexColor.vert"),
			File::Load("Resources/Shaders/VertexColor.frag")
		);

		const Shader rainbowShader(
			File::Load("Resources/Shaders/Rainbow.vert"),
			File::Load("Resources/Shaders/Rainbow.frag")
		);

		while (Running)
		{
			// computing of frame time
			Time::Tick();

			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
			RenderCommand::Clear();

			{
				Renderer::BeginScene();

				rainbowShader.Bind();
				Renderer::Submit(squareVertexArray);


				vertexColorShader.Bind();
				Renderer::Submit(vertexArray);

				Renderer::EndScene();
			}

			if (!Input::IsKeyPressed(CL_KEY_TAB))
			{
				auto [x, y] = Input::GetMousePosition();
				mousePosition->Set(0, static_cast<int32>(x));
				mousePosition->Set(1, static_cast<int32>(y));
			}
			else
			{
				Logger::Info("Tab\nis\npressed!");
			}

			frameTimeArray[counter] = Time::DeltaTime();

			if (++counter > 59)
			{
				counter     = 0;
				float32 sum = 0.f;
				for (auto i : frameTimeArray)
					sum += i;

				frameTime->Set(0, sum / 60);
			}

			Update();
		}
	}

	void Application::Update()
	{
		for (auto* layer : AppLayerStack)
			layer->OnUpdate();

		ImGuiLayer::Begin();
		ImGuiInterface::OnImGuiRender();

		for (auto* layer : AppLayerStack)
			layer->OnImGuiRender();
		ImGuiLayer::End();

		AppWindow->OnUpdate();
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_METHOD(Application::OnWindowClosed));

		for (auto it = AppLayerStack.end(); it != AppLayerStack.begin();)
		{
			(*(--it))->OnEvent(event);
			if (event.IsHandled())
				break;
		}
	}

	bool Application::OnWindowClosed([[maybe_unused]] WindowCloseEvent& event)
	{
		Logger::Info("Closing Application");
		Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer)
	{
		AppLayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		AppLayerStack.PushOverlay(overlay);
	}
}
