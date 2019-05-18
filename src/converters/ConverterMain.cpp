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

#include "ConverterMain.h"

#pragma warning(push, 0)
#include <max.h>
#pragma warning(pop)

#include "common/String.h"
#include "common/Logger.h"

#include <xpln/obj/ObjMain.h>
#include "objects/main/MainObjParamsWrapper.h"
#include "classes-desc/ClassesDescriptions.h"
#include "ExportParams.h"
/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool ConverterMain::toXpln(INode * inNode, xobj::ObjMain & outMain, const ExportParams & params) {
    if (!MainObjParamsWrapper::isMainObj(inNode)) {
        return false;
    }

    MainObjParamsWrapper attr(inNode, params.mCurrTime, FOREVER);
    auto & xop = outMain.mExportOptions;

    xop.enable(xobj::XOBJ_EXP_APPLY_LOD_TM);

    attr.isOptimization() ? xop.enable(xobj::XOBJ_EXP_OPTIMIZATION) : xop.disable(xobj::XOBJ_EXP_OPTIMIZATION);
    attr.isInstancing() ? xop.enable(xobj::XOBJ_EXP_CHECK_INSTANCE) : xop.disable(xobj::XOBJ_EXP_CHECK_INSTANCE);
    attr.isDebug() ? xop.enable(xobj::XOBJ_EXP_DEBUG) : xop.disable(xobj::XOBJ_EXP_DEBUG);
    attr.isNameMesh() ? xop.enable(xobj::XOBJ_EXP_MARK_MESH) : xop.disable(xobj::XOBJ_EXP_MARK_MESH);
    attr.isNameLines() ? xop.enable(xobj::XOBJ_EXP_MARK_LINE) : xop.disable(xobj::XOBJ_EXP_MARK_LINE);
    attr.isNameLights() ? xop.enable(xobj::XOBJ_EXP_MARK_LIGHT) : xop.disable(xobj::XOBJ_EXP_MARK_LIGHT);
    attr.isNameDummies() ? xop.enable(xobj::XOBJ_EXP_MARK_DUMMY) : xop.disable(xobj::XOBJ_EXP_MARK_DUMMY);
    attr.isTreeHierarchy() ? xop.enable(xobj::XOBJ_EXP_MARK_TREE_HIERARCHY) : xop.disable(xobj::XOBJ_EXP_MARK_TREE_HIERARCHY);

    outMain.mAttr.mDebug = xop.isEnabled(xobj::XOBJ_EXP_DEBUG);

    const std::string prefix = attr.pathPrefix();
    const auto texture = attr.texture();
    if (texture) {
        outMain.mAttr.mTexture = makeTexturePath(*texture, prefix);
    }
    const auto textureLit = attr.textureLit();
    if (textureLit) {
        outMain.mAttr.mTextureLit = makeTexturePath(*textureLit, prefix);
    }
    const auto textureNormal = attr.textureNormal();
    if (textureNormal) {
        outMain.mAttr.mTextureNormal = makeTexturePath(*textureNormal, prefix);
    }

    outMain.mAttr.mDropShadow = !attr.isNoShadow();
    outMain.mAttr.mTilted = attr.isTilted();
    outMain.mAttr.mCockpitLit = attr.isCockpitLit();

    outMain.mAttr.mBlendClass = attr.isBlendGlass();
    outMain.mAttr.mNormalMetalness = attr.isNormalMetalness();
    outMain.mAttr.mWetDry = attr.wetDry();
    outMain.mAttr.mBlend = attr.blend();
    outMain.mAttr.mLayerGroup = attr.layerGroup();
    outMain.mDraped.mAttr.mLayerGroup = attr.drapedLayerGroup();
    outMain.mDraped.mAttr.mLod = attr.lodDrap();
    outMain.mAttr.mSlungLoadWeight = attr.slungWeight();
    outMain.mAttr.mSpecular = attr.specular();
    outMain.mAttr.mTint = attr.tint();
    outMain.mAttr.mSlopeLimit = attr.slopeLimit();
    outMain.mAttr.mCockpitRegion1 = attr.cockpitRegion(xobj::AttrCockpitRegion::r1);
    outMain.mAttr.mCockpitRegion2 = attr.cockpitRegion(xobj::AttrCockpitRegion::r2);
    outMain.mAttr.mCockpitRegion3 = attr.cockpitRegion(xobj::AttrCockpitRegion::r3);
    outMain.mAttr.mCockpitRegion4 = attr.cockpitRegion(xobj::AttrCockpitRegion::r4);

    outMain.setObjectName(sts::toMbString(inNode->GetName()));
    return true;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

INode * ConverterMain::toMax(const xobj::ObjMain & inXObj) {
    auto coreInterface = GetCOREInterface();
    const auto mainObj = reinterpret_cast<Object*>(coreInterface->CreateInstance(HELPER_CLASS_ID, ClassesDescriptions::mainObj()->ClassID()));
    if (mainObj == nullptr) {
        LCritical << "Main object <" << inXObj.objectName() << "> couldn't be created.";
        return nullptr;
    }

    auto node = coreInterface->CreateObjectNode(mainObj);
    if (node == nullptr) {
        LCritical << "Max node for the object <" << inXObj.objectName() << "> couldn't be created.";
        return nullptr;
    }

    MainObjParamsWrapper attr(node, GetCOREInterface()->GetTime(), FOREVER);
    auto & xop = inXObj.mExportOptions;

    attr.setOptimization(xop.isEnabled(xobj::XOBJ_EXP_OPTIMIZATION));
    attr.setInstancing(xop.isEnabled(xobj::XOBJ_EXP_CHECK_INSTANCE));
    attr.setDebug(xop.isEnabled(xobj::XOBJ_EXP_DEBUG) || inXObj.mAttr.mDebug);
    attr.setNameMesh(xop.isEnabled(xobj::XOBJ_EXP_MARK_MESH));
    attr.setNameLines(xop.isEnabled(xobj::XOBJ_EXP_MARK_LINE));
    attr.setNameLights(xop.isEnabled(xobj::XOBJ_EXP_MARK_LIGHT));
    attr.setNameDummies(xop.isEnabled(xobj::XOBJ_EXP_MARK_DUMMY));
    attr.setTreeHierarchy(xop.isEnabled(xobj::XOBJ_EXP_MARK_TREE_HIERARCHY));

    std::string resTexture;
    std::string resPrefix1;
    const std::string resPrefix2;
    const std::string resPrefix3;

    if (inXObj.mAttr.mTexture) {
        makeTexturePath(*inXObj.mAttr.mTexture, resTexture, resPrefix1);
        attr.setPathPrefix(resPrefix1);
        attr.setTexture(resTexture);
    }

    if (inXObj.mAttr.mTextureLit) {
        makeTexturePath(*inXObj.mAttr.mTextureLit, resTexture, resPrefix1);
        attr.setTextureLit(resTexture);
    }

    if (inXObj.mAttr.mTextureNormal) {
        makeTexturePath(*inXObj.mAttr.mTextureNormal, resTexture, resPrefix1);
        attr.setTextureNormal(resTexture);
    }

    if (resPrefix1 != resPrefix2 && resPrefix2 != resPrefix3) {
        CLWarning << "Textures have different prefix, only one <"
                << resPrefix1 << "> prefix will be used.";
    }

    attr.setNoShadow(!inXObj.mAttr.mDropShadow);
    attr.setTilted(inXObj.mAttr.mTilted);
    attr.setCockpitLit(inXObj.mAttr.mCockpitLit);

    attr.setBlendGlass(inXObj.mAttr.mBlendClass);
    attr.setNormalMetalness(inXObj.mAttr.mNormalMetalness);
    attr.setWetDry(inXObj.mAttr.mWetDry);
    attr.setBlend(inXObj.mAttr.mBlend);
    attr.setLayerGroup(inXObj.mAttr.mLayerGroup);
    attr.setDrapedLayerGroup(inXObj.mDraped.mAttr.mLayerGroup);
    attr.setLodDrap(inXObj.mDraped.mAttr.mLod);
    attr.setSlungWeight(inXObj.mAttr.mSlungLoadWeight);
    attr.setSpecular(inXObj.mAttr.mSpecular);
    attr.setTint(inXObj.mAttr.mTint);
    attr.setSlopeLimit(inXObj.mAttr.mSlopeLimit);
    attr.setCockpitRegion(inXObj.mAttr.mCockpitRegion1, xobj::AttrCockpitRegion::r1);
    attr.setCockpitRegion(inXObj.mAttr.mCockpitRegion2, xobj::AttrCockpitRegion::r2);
    attr.setCockpitRegion(inXObj.mAttr.mCockpitRegion3, xobj::AttrCockpitRegion::r3);
    attr.setCockpitRegion(inXObj.mAttr.mCockpitRegion4, xobj::AttrCockpitRegion::r4);

    node->SetName(xobj::toMStr(inXObj.objectName()));
    return node;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

std::string ConverterMain::makeTexturePath(const std::string & texture, const std::string & prefix) {
    if (texture == "none" || texture.empty()) {
        return "";
    }

    auto res = sts::MbStrUtils::splitToList(texture, "\\/");
    DbgAssert(!res.empty());
    std::string out(prefix);
    if (!prefix.empty()) {
        out.append("/");
    }
    out.append(res.back());
    return out;
}

void ConverterMain::makeTexturePath(const std::string & texture, std::string & outTextureName, std::string & outPrefix) {
    outTextureName.clear();
    outPrefix.clear();
    if (texture == "none" || texture.empty()) {
        return;
    }

    const std::size_t pos = texture.find_last_of("\\/");
    if (pos != std::string::npos) {
        outPrefix = texture.substr(0, pos);
        outTextureName = texture.substr(pos, texture.size() - pos);
    }
    else {
        outTextureName = texture;
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

INode * ConverterMain::createBone(const xobj::Transform * xTransform) {
    auto coreInterface = GetCOREInterface();
    const auto boneObj = reinterpret_cast<Object*>(coreInterface->CreateInstance(BONE_CLASS_ID, BONE_OBJ_CLASSID));
    if (boneObj == nullptr) {
        LCritical << "Bone object <" << xTransform->mName << "> couldn't be created.";
        return nullptr;
    }

    auto node = coreInterface->CreateObjectNode(boneObj);
    if (node == nullptr) {
        LCritical << "Max node for the object <" << xTransform->mName << "> couldn't be created.";
        return nullptr;
    }
    node->SetBoneNodeOnOff(TRUE, GetCOREInterface()->GetTime());
    return node;
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
