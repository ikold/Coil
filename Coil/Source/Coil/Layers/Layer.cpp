#include "pch.h"
#include "Layer.h"

namespace Coil
{
	Layer::Layer(RString<String> name)
		: DebugName(name)
	{}

	Layer::~Layer()
	{}
}