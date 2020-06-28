#include "pch.h"
#include "Layer.h"


namespace Coil
{
	Layer::Layer(RString<> name)
		: DebugName(Move(name))
	{}
}
