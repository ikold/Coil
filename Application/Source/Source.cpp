#include "Coil.h"

class App : public cl::Application
{
public:
	App()
	{
		PushOverlay(new cl::ImGuiLayer());
	}

	~App()
	{
	}
};

BindApplication(App)