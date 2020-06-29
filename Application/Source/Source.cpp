#include "Coil.h"
#include "Layers//ApplicationLayer.h"
#include "Coil/Core/EntryPoint.h"


class App final : public cl::Application
{
public:
	App()
	{
		PushLayer(new ApplicationLayer());
	}
};


BindApplication(App)
