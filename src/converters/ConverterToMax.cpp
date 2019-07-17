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

#include "ConverterToMax.h"

#include "ConverterDummy.h"
#include "ConverterLine.h"
#include "ConverterMesh.h"
#include "ConverterMain.h"
#include "ConverterLight.h"
#include "ConverterAnimVisibility.h"
#include "ConverterAnimTranslate.h"
#include "ConverterAnimRotate.h"
#include "ConverterUtils.h"
#include "ConverterSmoke.h"
#include "ConverterLod.h"

#include <xpln/obj/ObjMain.h>
#include <xpln/obj/ObjAbstractLight.h>
#include "ImportParams.h"

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

ConverterToMax::ConverterToMax() {
    mIp = GetCOREInterface();

    mXObjMain = nullptr;
    mMainObj = nullptr;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool ConverterToMax::toMax(xobj::ObjMain & xObjMain) {
    INode * main = ConverterMain::toMax(xObjMain);
    if (!main) {
        return false;
    }

    const ImportParams importPrams;

    for (size_t i = 0; i < xObjMain.lods().size(); ++i) {
        auto & lod = xObjMain.lods().at(i);
        INode * maxLod = ConverterLod::toMax(*lod, importPrams);
        if (maxLod == nullptr) {
            return false;
        }
        main->AttachChild(maxLod);
        if (!processXTransformHierarchy(maxLod, &lod->transform(), importPrams)) {
            return false;
        }
    }
    return false;
}

bool ConverterToMax::processXTransformHierarchy(INode * parent, xobj::Transform * xTransform, const ImportParams & params) {
    ConverterUtils::toMaxTransform(*xTransform, parent);
    if (xTransform->hasAnimRotate() || xTransform->hasAnimTrans()) {
        INode * animNode = ConverterMain::createBone(xTransform);
        if (!animNode) {
            return false;
        }
        DbgAssert(parent);
        DbgAssert(xTransform);
        if (!ConverterAnimVisibility::toMax(*animNode, *xTransform, params) &&
            !ConverterAnimTranslate::toMax(*animNode, *xTransform, params) &&
            !ConverterAnimRotate::toMax(*animNode, *xTransform, params)) {
            return false;
        }
        ConverterUtils::toMaxTransform(*xTransform, animNode);
        parent->AttachChild(animNode);
        parent = animNode;
    }
    //---------------------------
    processXTransformObjects(parent, xTransform, params);
    //---------------------------
    for (size_t i = 0; i < xTransform->childrenNum(); ++i) {
        if (!processXTransformHierarchy(parent, xTransform->childAt(i), params)) {
            return false;
        }
    }
    return true;
}

void ConverterToMax::processXTransformObjects(INode * parent, xobj::Transform * xTransform, const ImportParams & params) {
    for (auto & obj : xTransform->objList()) {
        INode * node = processXObjects(*obj, params);
        if (node) {
            ConverterUtils::toMaxTransform(*xTransform, node);
            parent->AttachChild(node);
        }
    }
}

INode * ConverterToMax::processXObjects(const xobj::ObjAbstract & xObj, const ImportParams & params) {
    INode * node = ConverterMesh::toMax(&xObj, params);
    if (!node) {
        node = ConverterLight::toMax(&xObj, params);
    }
    else if (!node) {
        node = ConverterSmoke::toMax(&xObj, params);
    }
    else if (!node) {
        node = ConverterDummy::toMax(&xObj, params);
    }
    else if (!node) {
        node = ConverterLine::toMax(&xObj, params);
    }
    return node;
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
