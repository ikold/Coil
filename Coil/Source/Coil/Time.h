#pragma once

#include "Coil/Core.h"

namespace Coil
{
	#define Timestamp long

	/* TO BE IMPLEMENTED */
	class COIL_API Time
	{
	public:
		Time();
		~Time();

		/* Placeholder method for geting current timestamp*/
		static Timestamp Now() { return 0; };
	};

}

