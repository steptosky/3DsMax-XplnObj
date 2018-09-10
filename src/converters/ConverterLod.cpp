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

#include "ConverterLod.h"

#pragma warning(push, 0)
#include <max.h>
#pragma warning(pop)

#include <xpln/obj/ObjLodGroup.h>

#include "common/String.h"
#include "common/Logger.h"

#include "objects/lod/LodObjParamsWrapper.h"
#include "classes-desc/ClassesDescriptions.h"

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool ConverterLod::toXpln(INode * inNode, xobj::ObjLodGroup & outLod, const ExportParams & params) {
    if (!LodObjParamsWrapper::isLodObj(inNode)) {
        return false;
    }

    LodObjParamsWrapper values(inNode, params.mCurrTime, FOREVER);
    outLod.setNearVal(values.nearValue());
    outLod.setFarVal(values.farValue());
    outLod.setObjectName(sts::toMbString(inNode->GetName()));
    return true;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

INode * ConverterLod::toMax(const xobj::ObjLodGroup & inXObj, const ImportParams & params) {
    const auto lodObj = reinterpret_cast<Object*>(params.mCoreInterface->CreateInstance(HELPER_CLASS_ID, ClassesDescriptions::lodObj()->ClassID()));
    if (lodObj == nullptr) {
        LCritical << "Lod object <" << inXObj.objectName() << "> couldn't be created.";
        return nullptr;
    }

    auto node = params.mCoreInterface->CreateObjectNode(lodObj);
    if (node == nullptr) {
        LCritical << "Max node for the object <" << inXObj.objectName() << "> couldn't be created.";
        return nullptr;
    }

    LodObjParamsWrapper values(node, params.mCurrTime, FOREVER);
    values.setNearValue(inXObj.nearVal());
    values.setFarValue(inXObj.nearVal());
    node->SetName(toTSTR(inXObj.objectName().c_str()));
    return node;
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
