#pragma once

/*
**  Copyright(C) 2018, StepToSky
**
**  Redistribution and use in source and binary forms, with or without
**  modification, are permitted provided that the following conditions are met:
**
**  1.Redistributions of source code must retain the above copyright notice, this
**    list of conditions and the following disclaimer.
**  2.Redistributions in binary form must reproduce the above copyright notice,
**    this list of conditions and the following disclaimer in the documentation
**    and / or other materials provided with the distribution.
**  3.Neither the name of StepToSky nor the names of its contributors
**    may be used to endorse or promote products derived from this software
**    without specific prior written permission.
**
**  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**  DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
**  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**  Contacts: www.steptosky.com
*/

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

#ifdef _MSC_VER
#   define ENABLE_PRECOMPILED_HEADERS
#endif

#ifdef ENABLE_PRECOMPILED_HEADERS

//-------------------------------------------------------------------------

#include <cassert>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <cstdint>
#include <cstddef>
#include <limits>

#include <thread>
#include <mutex>

#include <vector>
#include <map>
#include <list>
#include <unordered_set>

#include <functional>
#include <utility>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <tuple>
#include <regex>
#include <optional>

//-------------------------------------------------------------------------

#include "common/Logger.h"
#include "common/String.h"
#include "ui-win/Utils.h"

//-------------------------------------------------------------------------

// 3d max SDK produces too many warnings,
// So It isn't possible to see the plugin's ones.
#pragma warning(push, 0)
#include <max.h>
#include <strclass.h>
#include <Path.h>
#include <3dsmaxport.h>

#include <imenuman.h>
#include <iparamb2.h>
#include <notify.h>
#pragma warning(pop)

//-------------------------------------------------------------------------

#endif

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
