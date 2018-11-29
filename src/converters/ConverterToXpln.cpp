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

#include <cassert>
#include <algorithm>

#pragma warning(push, 0)
#include <ilayer.h>
#pragma warning(pop)

#include "ConverterToXpln.h"
#include "common/Logger.h"

#include "ConverterDummy.h"
#include "ConverterLine.h"
#include "ConverterMesh.h"
#include "ConverterMain.h"
#include "ConverterLight.h"
#include "ConverterATTR.h"
#include "ConverterAnim.h"
#include "ConverterUtils.h"
#include "ConverterSmoke.h"
#include "ConverterLod.h"

#include <xpln/obj/ObjMain.h>
#include <xpln/obj/ObjMesh.h>
#include <xpln/obj/ObjAbstractLight.h>
#include <xpln/obj/ObjDummy.h>
#include <xpln/obj/ObjSmoke.h>

#include "objects/lod/LodObjParamsWrapper.h"
#include "objects/main/MainObjParamsWrapper.h"
#include "common/String.h"

#include "common/NodeUtils.h"

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

ConverterToXpln::ConverterToXpln() {
    mXObjMain = nullptr;
    mMainObj = nullptr;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool ConverterToXpln::toXpln(MainObjParamsWrapper * mainNode, xobj::ObjMain & xObjMain) {
    assert(mainNode);
    mXObjMain = &xObjMain;
    mMainObj = mainNode;
    //-----------------------------------------------
    mExportOptions.mCurrTime = mainNode->timeValue();

    if (mainNode->isManualScale()) {
        // TODO Needs implementation of auto-scale value relative system units
        mExportOptions.mScale = mainNode->scale();
    }

    if (mExportOptions.mScale < 0.00001) {
        CLError << "<" << xObjMain.objectName() << "> has too small scale <" << mExportOptions.mScale << ">";
        return false;
    }
    //-----------------------------------------------
    ConverterMain::toXpln(mainNode->node(), xObjMain, mExportOptions);
    if (mainNode->useLayersObjects()) {
        std::unordered_set<ILayer*> geometryLayers = mainNode->geometryLayers();
        if (geometryLayers.empty()) {
            CLError << LogNode(mainNode->node()) << "has enabled using layers objects but hasn't layers specified";
            return false;
        }
        return byLayer(mainNode, geometryLayers, xObjMain);
    }
    return byLink(mainNode, xObjMain);
}

bool ConverterToXpln::byLayer(MainObjParamsWrapper * mainNode, const std::unordered_set<ILayer*> & layers, xobj::ObjMain & xObjMain) {
    const auto rootNodes = rootNodesFromLayers(layers);
    //-----------------------------------------------
    xobj::ObjLodGroup & lod = xObjMain.addLod();
    xobj::Transform & currObjTransform = lod.transform();
    //-----------------------------------------------
    Matrix3 ttm = Inverse(mainNode->node()->GetNodeTM(mExportOptions.mCurrTime));
    ttm.Scale(Point3(mExportOptions.mScale, mExportOptions.mScale, mExportOptions.mScale), TRUE);
    ConverterUtils::toXTransform(ttm, currObjTransform);
    //-----------------------------------------------
    for (const auto rootNode : rootNodes) {
        if (!processNode(rootNode, &currObjTransform, mExportOptions, layers)) {
            return false;
        }
    }
    //-----------------------------------------------
    return true;
}

bool ConverterToXpln::byLink(MainObjParamsWrapper * mainNode, xobj::ObjMain & xObjMain) {
    mLods.clear();
    if (!collectLods(mainNode->node(), mainNode->node(), mLods)) {
        return false;
    }

    if (mLods.empty()) {
        mLods.emplace_back(mainNode->node());
    }

    for (auto currLodNode : mLods) {
        if (currLodNode->NumberOfChildren() == 0) {
            CLWarning << "The lod: <" << sts::toMbString(currLodNode->GetName()) << "> does not have any objects.";
            continue;
        }
        //-----------------------------------------------
        xobj::ObjLodGroup & lod = xObjMain.addLod();
        xobj::Transform & currObjTransform = lod.transform();

        if (mainNode->node() != currLodNode) {
            ConverterLod::toXpln(currLodNode, lod, mExportOptions);
        }
        //-----------------------------------------------
        Matrix3 ttm = Inverse(currLodNode->GetNodeTM(mExportOptions.mCurrTime));
        ttm.Scale(Point3(mExportOptions.mScale, mExportOptions.mScale, mExportOptions.mScale), TRUE);
        ConverterUtils::toXTransform(ttm, currObjTransform);
        //-----------------------------------------------
        const auto numChildren = currLodNode->NumberOfChildren();
        for (int idx = 0; idx < numChildren; ++idx) {
            if (!processNode(currLodNode->GetChildNode(idx), &currObjTransform, mExportOptions, std::unordered_set<ILayer*>())) {
                return false;
            }
        }
        //-----------------------------------------------
    }
    return true;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool ConverterToXpln::processNode(INode * node, xobj::Transform * xTransform,
                                  const ExportParams & params, const std::unordered_set<ILayer*> & layers) const {
    xobj::Transform & tr = xTransform->newChild(sts::toMbString(node->GetName()).c_str());
    ConverterUtils::toXTransform(node->GetNodeTM(mMainObj->timeValue()), tr);
    //-------------------------------------------------------------------------
    // animation
    if (mMainObj->isAnimationExport()) {
        ConverterAnim::toXPLN(node, &tr, params);
    }
    //-------------------------------------------------------------------------
    // transform object
    const auto nodeLayer = layerFromNode(node);
    if (layers.empty() || std::any_of(layers.begin(), layers.end(), [&](const auto layer) { return layer == nodeLayer; })) {
        ObjAbstractList xObjList = xplnObj(node, params);
        for (auto curr : xObjList) {
            tr.addObject(curr);
        }
    }
    //-------------------------------------------------------------------------
    // translate children
    const auto numChildren = node->NumberOfChildren();
    for (int idx = 0; idx < numChildren; ++idx) {
        if (!processNode(node->GetChildNode(idx), &tr, params, layers)) {
            return false;
        }
    }
    //-------------------------------------------------------------------------
    return true;
}

//-------------------------------------------------------------------------

ConverterToXpln::ObjAbstractList ConverterToXpln::xplnObj(INode * inNode, const ExportParams & params) const {
    ObjAbstractList outList;

    if (mMainObj->isMeshExport()) {
        xobj::ObjAbstract * xObj = ConverterMesh::toXpln(inNode, params);
        if (xObj) {
            ConverterAttr::toXpln(*xObj, inNode, params);
            outList.emplace_back(xObj);
            return outList;
        }
    }
    if (mMainObj->isLinesExport()) {
        outList = ConverterLine::toXpln(inNode, Matrix3(1), params);
        if (!outList.empty()) {
            return outList;
        }
    }
    if (mMainObj->isLightsExport()) {
        xobj::ObjAbstract * xObj = ConverterLight::toXpln(inNode, params);
        if (xObj) {
            ConverterAttr::toXpln(*xObj, inNode, params);
            outList.emplace_back(xObj);
            return outList;
        }
    }

    xobj::ObjSmoke * xSmokeObj = ConverterSmoke::toXpln(inNode, params);
    if (xSmokeObj) {
        outList.emplace_back(xSmokeObj);
        return outList;
    }

    xobj::ObjAbstract * xDummyObj = ConverterDummy::toXpln(inNode, params);
    if (xDummyObj) {
        ConverterAttr::toXpln(*xDummyObj, inNode, params);
        outList.emplace_back(xDummyObj);
        return outList;
    }

    return outList;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool ConverterToXpln::collectLods(INode * ownerNode, INode * currNode, std::vector<INode*> & outLods) {
    if (LodObjParamsWrapper::isLodObj(currNode)) {
        if (currNode->GetParentNode() != ownerNode) {
            CLError << "The lod object <"
                    << sts::toMbString(currNode->GetName()) << "> is not linked to <"
                    << sts::toMbString(ownerNode->GetName()) << ">";
            return false;
        }
        outLods.emplace_back(currNode);
    }

    const auto count = currNode->NumberOfChildren();
    for (int i = 0; i < count; ++i) {
        if (!collectLods(ownerNode, currNode->GetChildNode(i), outLods)) {
            return false;
        }
    }
    return true;
}

ILayer * ConverterToXpln::layerFromNode(INode * node) {
    const auto pointer = node->GetReference(NODE_LAYER_REF);
    DbgAssert(pointer);
    DbgAssert(dynamic_cast<const ILayer*>(pointer));
    return static_cast<ILayer*>(pointer);
}

std::unordered_set<INode*> ConverterToXpln::rootNodesFromLayers(const std::unordered_set<ILayer*> & layers) {
    std::unordered_set<INode*> out;
    NodeUtils::visitAll([&](INode * node) ->bool {
        const auto nodeLayer = layerFromNode(node);
        for (const auto l : layers) {
            if (nodeLayer == l) {
                auto root = NodeUtils::root(node);
                DbgAssert(root);
                out.insert(root);
            }
        }
        return true;
    });
    return out;
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
