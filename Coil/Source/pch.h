#pragma once

#include "Coil/Core/Core.h"


#ifdef CL_PLATFORM_WINDOWS
#include <windows.h>
#endif

#include <memory>

#include <functional>

#include <vector>

#include <array>

#include <unordered_map>

#include <numeric>


#include <glm/glm.hpp>


#include "Coil/Utilities/Types.h"

#include "Coil/Utilities/Math.h"

#include "Coil/Utilities/String/String.h"
#include "Coil/Utilities/String/BString.h"
#include "Coil/Utilities/String/PString.h"
#include "Coil/Utilities/String/RString.h"

#include "Coil/Utilities/Time.h"

#include "Coil/Utilities/Logger.h"

#include "Coil/Core/Input.h"

#include "Coil/Utilities/File.h"

#include "Coil/Debug/Instrumentor.h"


#include "Coil/Core/KeyCodes.h"

#include "Coil/Core/MouseButtons.h"


#include "Coil/Events/Event.h"

#include "Coil/Events/ApplicationEvent.h"

#include "Coil/Events/MouseEvent.h"

#include "Coil/Events/KeyEvent.h"
