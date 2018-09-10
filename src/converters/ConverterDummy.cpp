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

#include "ConverterDummy.h"
#include "ConverterUtils.h"
#include "common/String.h"
#include "common/Logger.h"

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

INode * ConverterDummy::toMax(const xobj::ObjAbstract * inXObj, const ImportParams & params) {
    if (inXObj->objType() != xobj::OBJ_DUMMY) {
        return nullptr;
    }
    HelperObject * pobj = reinterpret_cast<HelperObject*>(params.mCoreInterface->CreateInstance(HELPER_CLASS_ID, Class_ID(POINTHELP_CLASS_ID, 0)));
    if (pobj == nullptr) {
        LError << "Object couldn't be created.";
        return nullptr;
    }

    INode * pnode = params.mCoreInterface->CreateObjectNode(pobj);
    if (pnode == nullptr) {
        LError << "Node couldn't be created.";
        return nullptr;
    }

    pnode->SetName(toTSTR(inXObj->objectName().c_str()));
    return pnode;
}

xobj::ObjDummy * ConverterDummy::toXpln(INode * inNode, const ExportParams & params) {
    assert(inNode);
    const ObjectState & obsState = inNode->EvalWorldState(params.mCurrTime);
    if (obsState.obj->SuperClassID() != SClass_ID(HELPER_CLASS_ID)) {
        return nullptr;
    }
    xobj::ObjDummy * xObj = new xobj::ObjDummy;
    xObj->setObjectName(sts::toMbString(inNode->GetName()));
    return xObj;
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
