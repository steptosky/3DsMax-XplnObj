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
#include "models/eDataId.h"

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

class AnimIO {

    AnimIO() = default;
    ~AnimIO() = default;

public:

    //-------------------------------------------------------------------------

    static bool canApply(INode * node);

    //-------------------------------------------------------------------------

    static bool loadRotateFromNode(INode * inNode, MdAnimRot & outAnimRot, eAnimRotateIOID axis);
    static bool loadTransFromNode(INode * inNode, MdAnimTrans & outAnimRot);
    static bool loadVisibilityFromNode(INode * inNode, MdAnimVis & outAnimVis);

    //-------------------------------------------------------------------------

    static void removeRotateFromNode(INode * node, eAnimRotateIOID axis);
    static void removeAnimTransFromNode(INode * node);
    static void removeVisibilityFromNode(INode * node);

    //-------------------------------------------------------------------------

    static void saveRotateToNode(INode * outNode, const MdAnimRot & inAnimRot, eAnimRotateIOID axis);
    static void saveTransToNode(INode * outNode, const MdAnimTrans & inAnimRot);
    static void saveVisibilityToNode(INode * outNode, const MdAnimVis & inAnimVis);

    //-------------------------------------------------------------------------

    static bool cloneTransData(INode * from, INode * to);
    static bool cloneRotateData(INode * from, INode * to);
    static bool cloneVisibilityData(INode * from, INode * to);

private:

    static bool cloneData(INode * from, INode * to, DWORD index);

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
