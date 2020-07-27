#include "Coil.h"
#include "Layers/ApplicationLayer.h"
#include "Coil/Core/EntryPoint.h"


class App final : public cl::Application
{
public:
	App()
	{
		CL_PROFILE_FUNCTION_HIGH()

		PushLayer(new ApplicationLayer());
	}
};


BindApplication(App)
