/*
**  Copyright(C) 2017, StepToSky
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

#pragma once

#pragma warning(push, 0)
#include <max.h>
#pragma warning(pop)

#include "models/MdAnimRot.h"
#include "models/MdAnimTrans.h"
#include "models/MdAnimVis.h"

namespace bcw {
// backward compatibility

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

/*!
 * \details This class is for backward compatibility.
 *          It loads the animation data from the previous version.
 */
class MdAnimIoOld {

    MdAnimIoOld() = default;
    ~MdAnimIoOld() = default;

public:

    //-------------------------------------------------------------------------

    static bool loadRotateFromNode(INode * inNode, AppDataChunk * data, MdAnimRot & outAnimRot);
    static bool loadTransFromNode(INode * inNode, AppDataChunk * data, MdAnimTrans & outAnimRot);
    static bool loadVisibilityFromNode(INode * inNode, AppDataChunk * data, MdAnimVis & outAnimVis);

    static void removeRotateFromNode(INode * node);
    static void removeAnimTransFromNode(INode * node);
    static void removeVisibilityFromNode(INode * node);

    //-------------------------------------------------------------------------

    static AppDataChunk * dataRotate(INode * node);
    static AppDataChunk * dataTrans(INode * node);
    static AppDataChunk * dataVisibility(INode * node);

    //-------------------------------------------------------------------------

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
}
