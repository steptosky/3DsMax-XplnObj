#pragma once

/*
**  Copyright(C) 2019, StepToSky
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

#pragma warning(push, 0)
#include <max.h>
#pragma warning(pop)

#include <xpln/obj/Transform.h>
#include "models/MdAnimRot.h"

class ExportParams;
class ImportParams;

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

class ConverterAnimRotate {
public:

    ConverterAnimRotate() = delete;

    static bool toXpln(INode & node, xobj::Transform & transform, const ExportParams & params);
    static bool toMax(INode & node, xobj::Transform & transform, const ImportParams & params);

private:

    static void processLinearRotate(INode & node, xobj::Transform & transform, Control & control, const ExportParams & params);

    static void objAnimRotate(INode * node, xobj::Transform & transform, Control & control, const ExportParams & params);
    static void objAnimRotateAxis(INode * node, Control * control, char axis,
                                  xobj::AnimRotate & outXAnim, const ExportParams & params);
    static xobj::AnimRotate::KeyList * getRotateAxisAnimation(Control * ctrlAxis, const MdAnimRot::KeyValueList & keys,
                                                              int isReversed, const ExportParams & params);

    static bool rotAnimValidation(INode * node, Control * control, const char * inCtrlName, char axis);
    static bool checkRotateKeysValue(INode * node, const xobj::AnimRotate::KeyList & keys, const char * ctrlName, char axis);
    static float rotateValue(Control * inAxis, TimeValue t);

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
