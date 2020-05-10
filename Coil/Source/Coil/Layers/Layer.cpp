#include "pch.h"
#include "Layer.h"

#include <utility>

namespace Coil
{
	Layer::Layer(RString<String> name)
		: DebugName(std::move(name))
	{}
}
