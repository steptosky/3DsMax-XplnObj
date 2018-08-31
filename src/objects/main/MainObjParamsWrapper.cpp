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

#include "MainObjParamsWrapper.h"
#include "MainObjectParams.h"
#include "common/Logger.h"
#include "common/String.h"
#include "classes-desc/ClassesDescriptions.h"

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

MainObjParamsWrapper::MainObjParamsWrapper(INode * node, const TimeValue t, const Interval & interval)
    : mInterval(interval),
      mT(t),
      mNode(node) {

    DbgAssert(node);
    DbgAssert(isMainObj(node));

    mPbExp = node->GetObjectRef()->GetParamBlockByID(MainObjExpParams);
    mPbAttr = node->GetObjectRef()->GetParamBlockByID(MainObjAttrParams);
    DbgAssert(mPbExp);
    DbgAssert(mPbAttr);
}

MainObjParamsWrapper::MainObjParamsWrapper(IParamBlock2 * pbAttr, IParamBlock2 * pbExp, const TimeValue t, const Interval & interval)
    : mInterval(interval),
      mT(t),
      mNode(nullptr) {

    mPbExp = pbExp;
    mPbAttr = pbAttr;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool MainObjParamsWrapper::isMainObj(INode * inNode) {
    Object * obj = inNode->GetObjectRef();
    if (obj != nullptr) {
        return (obj->ClassID() == ClassesDescriptions::mainObj()->ClassID()) == TRUE;
    }
    return false;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool MainObjParamsWrapper::isExportEnable() {
    BOOL val = TRUE;
    if (mPbExp) {
        if (!mPbExp->GetValue(MainObjAttr_ExportEnable, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_ExportEnable);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val == TRUE;
}

bool MainObjParamsWrapper::isMeshExport() {
    BOOL val = TRUE;
    if (mPbExp) {
        if (!mPbExp->GetValue(MainObjExp_Meshes, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjExp_Meshes);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val == TRUE;
}

bool MainObjParamsWrapper::isLinesExport() {
    BOOL val = TRUE;
    if (mPbExp) {
        if (!mPbExp->GetValue(MainObjExp_Lines, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjExp_Lines);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val == TRUE;
}

bool MainObjParamsWrapper::isLightsExport() {
    BOOL val = TRUE;
    if (mPbExp) {
        if (!mPbExp->GetValue(MainObjExp_Lights, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjExp_Lights);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val == TRUE;
}

bool MainObjParamsWrapper::isAnimationExport() {
    BOOL val = TRUE;
    if (mPbExp) {
        if (!mPbExp->GetValue(MainObjExp_Animation, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjExp_Animation);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val == TRUE;
}

bool MainObjParamsWrapper::isOptimisation() {
    BOOL val = TRUE;
    if (mPbExp) {
        if (!mPbExp->GetValue(MainObjExp_Optimisation, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjExp_Optimisation);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val == TRUE;
}

bool MainObjParamsWrapper::isInstancing() {
    BOOL val = FALSE;
    if (mPbExp) {
        if (!mPbExp->GetValue(MainObjExp_Instancing, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjExp_Instancing);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val == TRUE;
}

bool MainObjParamsWrapper::isDebug() {
    BOOL val = FALSE;
    if (mPbExp) {
        if (!mPbExp->GetValue(MainObjExp_Debug, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjExp_Debug);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val == TRUE;
}

//-------------------------------------------------------------------------

void MainObjParamsWrapper::setExportEnable(bool inState) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjAttr_ExportEnable, mT, int(inState))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_ExportEnable);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setMeshExport(bool inState) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjExp_Meshes, mT, int(inState))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjExp_Meshes);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setLinesExport(bool inState) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjExp_Lines, mT, int(inState))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjExp_Lines);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setLightsExport(bool inState) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjExp_Lights, mT, int(inState))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjExp_Lights);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setAnimationExport(bool inState) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjExp_Animation, mT, int(inState))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjExp_Animation);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setOptimisation(bool inState) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjExp_Optimisation, mT, int(inState))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjExp_Optimisation);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setInstancing(bool inState) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjExp_Instancing, mT, int(inState))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjExp_Instancing);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setDebug(bool inState) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjExp_Debug, mT, int(inState))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjExp_Debug);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool MainObjParamsWrapper::isNameMesh() {
    BOOL val = TRUE;
    if (mPbExp) {
        if (!mPbExp->GetValue(MainObjExp_NameMeshes, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjExp_NameMeshes);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val == TRUE;
}

bool MainObjParamsWrapper::isNameLines() {
    BOOL val = TRUE;
    if (mPbExp) {
        if (!mPbExp->GetValue(MainObjExp_NameLines, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjExp_NameLines);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val == TRUE;
}

bool MainObjParamsWrapper::isNameLights() {
    BOOL val = TRUE;
    if (mPbExp) {
        if (!mPbExp->GetValue(MainObjExp_NameLights, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjExp_NameLights);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val == TRUE;
}

bool MainObjParamsWrapper::isNameDummies() {
    BOOL val = TRUE;
    if (mPbExp) {
        if (!mPbExp->GetValue(MainObjExp_NameDummies, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjExp_NameDummies);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val == TRUE;
}

bool MainObjParamsWrapper::isTreeHierarchy() {
    BOOL val = TRUE;
    if (mPbExp) {
        if (!mPbExp->GetValue(MainObjExp_TreeHierarchy, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjExp_TreeHierarchy);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val == TRUE;
}

//-------------------------------------------------------------------------

void MainObjParamsWrapper::setNameMesh(bool inState) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjExp_NameMeshes, mT, int(inState))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjExp_NameMeshes);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setNameLines(bool inState) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjExp_NameLines, mT, int(inState))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjExp_NameLines);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setNameLights(bool inState) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjExp_NameLights, mT, int(inState))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjExp_NameLights);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setNameDummies(bool inState) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjExp_NameDummies, mT, int(inState))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjExp_NameDummies);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setTreeHierarchy(bool inState) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjExp_TreeHierarchy, mT, int(inState))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjExp_TreeHierarchy);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool MainObjParamsWrapper::isManualScale() {
    BOOL val = TRUE;
    if (mPbExp) {
        if (!mPbExp->GetValue(MainObj_ScaleEnabled, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObj_ScaleEnabled);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val == TRUE;
}

float MainObjParamsWrapper::scale() {
    float val = TRUE;
    if (mPbExp) {
        if (!mPbExp->GetValue(MainObj_ScaleValue, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObj_ScaleValue);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val;
}

//-------------------------------------------------------------------------

void MainObjParamsWrapper::setScale(bool inManual, float inValue) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObj_ScaleEnabled, mT, int(inManual))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObj_ScaleEnabled);
        }
        if (!mPbExp->SetValue(MainObj_ScaleValue, mT, inValue)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObj_ScaleValue);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

std::string MainObjParamsWrapper::pathPrefix() {
#if MAX_VERSION_MAJOR < 12
	TCHAR * val = nullptr;
#else
    const MCHAR * val = nullptr;
#endif
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_PathPrefix, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_PathPrefix);
            return "";
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val ? std::string(sts::toMbString(val)) : "";
}

std::string MainObjParamsWrapper::texture() {
#if MAX_VERSION_MAJOR < 12
	TCHAR * val = nullptr;
#else
    const MCHAR * val = nullptr;
#endif
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_Texture, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_Texture);
            return "";
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val ? std::string(sts::toMbString(val)) : "";
}

std::string MainObjParamsWrapper::textureLit() {
#if MAX_VERSION_MAJOR < 12
	TCHAR * val = nullptr;
#else
    const MCHAR * val = nullptr;
#endif
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_TextureLit, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_TextureLit);
            return "";
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val ? std::string(sts::toMbString(val)) : "";
}

std::string MainObjParamsWrapper::textureNormal() {
#if MAX_VERSION_MAJOR < 12
	TCHAR * val = nullptr;
#else
    const MCHAR * val = nullptr;
#endif
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_TextureNormal, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_TextureNormal);
            return "";
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val ? std::string(sts::toMbString(val)) : "";
}

//-------------------------------------------------------------------------

void MainObjParamsWrapper::setPathPrefix(const std::string & inStr) {
    if (mPbAttr) {
        sts::Str str = sts::toString(inStr);
#if MAX_VERSION_MAJOR < 12
		TCHAR * strVal = const_cast<TCHAR*>(str.c_str());
#else
        const MCHAR * strVal = str.c_str();
#endif

        if (!mPbAttr->SetValue(MainObjAttr_PathPrefix, mT, strVal)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_PathPrefix);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setTexture(const std::string & inStr) {
    if (mPbAttr) {
        sts::Str str = sts::toString(inStr);
#if MAX_VERSION_MAJOR < 12
		TCHAR * strVal = const_cast<TCHAR*>(str.c_str());
#else
        const MCHAR * strVal = str.c_str();
#endif

        if (!mPbAttr->SetValue(MainObjAttr_Texture, mT, strVal)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_Texture);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setTextureLit(const std::string & inStr) {
    if (mPbAttr) {
        sts::Str str = sts::toString(inStr);
#if MAX_VERSION_MAJOR < 12
		TCHAR * strVal = const_cast<TCHAR*>(str.c_str());
#else
        const MCHAR * strVal = str.c_str();
#endif

        if (!mPbAttr->SetValue(MainObjAttr_TextureLit, mT, strVal)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_TextureLit);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setTextureNormal(const std::string & inStr) {
    if (mPbAttr) {
        sts::Str str = sts::toString(inStr);
#if MAX_VERSION_MAJOR < 12
		TCHAR * strVal = const_cast<TCHAR*>(str.c_str());
#else
        const MCHAR * strVal = str.c_str();
#endif

        if (!mPbAttr->SetValue(MainObjAttr_TextureNormal, mT, strVal)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_TextureNormal);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool MainObjParamsWrapper::isBlendGlass() {
    BOOL val = TRUE;
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_BlendGlass, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_BlendGlass);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val == TRUE;
}

bool MainObjParamsWrapper::isNormalMetalness() {
    BOOL val = TRUE;
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_NormalMetalness, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_NormalMetalness);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val == TRUE;
}

bool MainObjParamsWrapper::isNoShadow() {
    BOOL val = TRUE;
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_NoShadow, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_NoShadow);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val == TRUE;
}

bool MainObjParamsWrapper::isTilted() {
    BOOL val = TRUE;
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_Tilted, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_Tilted);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val == TRUE;
}

bool MainObjParamsWrapper::isCockpitLit() {
    BOOL val = TRUE;
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_CockpitLit, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_CockpitLit);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val == TRUE;
}

//-------------------------------------------------------------------------

void MainObjParamsWrapper::setBlendGlass(bool inState) {
    if (mPbAttr) {
        if (!mPbAttr->SetValue(MainObjAttr_BlendGlass, mT, int(inState))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_BlendGlass);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setNormalMetalness(bool inState) {
    if (mPbAttr) {
        if (!mPbAttr->SetValue(MainObjAttr_NormalMetalness, mT, int(inState))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_NormalMetalness);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setNoShadow(bool inState) {
    if (mPbAttr) {
        if (!mPbAttr->SetValue(MainObjAttr_NoShadow, mT, int(inState))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_NoShadow);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setTilted(bool inState) {
    if (mPbAttr) {
        if (!mPbAttr->SetValue(MainObjAttr_Tilted, mT, int(inState))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_Tilted);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setCockpitLit(bool inState) {
    if (mPbAttr) {
        if (!mPbAttr->SetValue(MainObjAttr_CockpitLit, mT, int(inState))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_CockpitLit);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

xobj::AttrWetDry MainObjParamsWrapper::wetDry() {
    int val = 0;
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_DryWet, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_DryWet);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
        return xobj::AttrWetDry();
    }
    return val == 0 ? xobj::AttrWetDry() : xobj::AttrWetDry(static_cast<xobj::AttrWetDry::eState>(val));
}

xobj::AttrBlend MainObjParamsWrapper::blend() {
    float ratio = 0.0f;
    int type = 0;
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_BlendingType, mT, type, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_BlendingType);
        }
        if (!mPbAttr->GetValue(MainObjAttr_BlendingRatio, mT, ratio, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_BlendingRatio);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
        return xobj::AttrBlend();
    }
    xobj::AttrBlend out(static_cast<xobj::AttrBlend::eType>(type), ratio);
    out.setEnabled(type != 0);
    return out;
}

xobj::AttrLayerGroup MainObjParamsWrapper::layerGroup() {
#if MAX_VERSION_MAJOR < 12
	TCHAR * layer = nullptr;
#else
    const MCHAR * layer = nullptr;
#endif
    int offset = 0;
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_LayerGroupLayer, mT, layer, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_LayerGroupLayer);
            return xobj::AttrLayerGroup();
        }
        if (!mPbAttr->GetValue(MainObjAttr_LayerGroupOffset, mT, offset, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_LayerGroupOffset);
            return xobj::AttrLayerGroup();
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
        return xobj::AttrLayerGroup();
    }
    xobj::ELayer l = layer ? xobj::ELayer::fromString(sts::toMbString(layer).c_str()) : xobj::ELayer(xobj::ELayer::none);
    xobj::AttrLayerGroup out(l, offset);
    out.setEnabled(l.id() != xobj::ELayer::none);
    return out;
}

xobj::AttrDrapedLayerGroup MainObjParamsWrapper::drapedLayerGroup() {
#if MAX_VERSION_MAJOR < 12
	TCHAR * layer = nullptr;
#else
    const MCHAR * layer = nullptr;
#endif
    int offset = 0;
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_LayerGroupDrapedLayer, mT, layer, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_LayerGroupDrapedLayer);
            return xobj::AttrDrapedLayerGroup();
        }
        if (!mPbAttr->GetValue(MainObjAttr_LayerGroupDrapedOffset, mT, offset, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_LayerGroupDrapedOffset);
            return xobj::AttrDrapedLayerGroup();
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
        return xobj::AttrDrapedLayerGroup();
    }
    xobj::ELayer l = layer ? xobj::ELayer::fromString(sts::toMbString(layer).c_str()) : xobj::ELayer(xobj::ELayer::none);
    xobj::AttrDrapedLayerGroup out(l, offset);
    out.setEnabled(l.id() != xobj::ELayer::none);
    return out;
}

xobj::AttrLodDrap MainObjParamsWrapper::lodDrap() {
    float dist = 0;
    BOOL enabled = TRUE;
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_LodDrapEnable, mT, enabled, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_LodDrapEnable);
        }
        if (!mPbAttr->GetValue(MainObjAttr_LodDrapDistance, mT, dist, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_LodDrapDistance);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
        return xobj::AttrLodDrap();
    }
    xobj::AttrLodDrap out(dist);
    out.setEnabled(enabled == TRUE);
    return out;
}

xobj::AttrSlungLoadWeight MainObjParamsWrapper::slungWeight() {
    float weight = 0;
    BOOL enabled = TRUE;
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_SlungLoadWeightEnable, mT, enabled, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_SlungLoadWeightEnable);
        }
        if (!mPbAttr->GetValue(MainObjAttr_SlungLoadWeight, mT, weight, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_SlungLoadWeight);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
        return xobj::AttrSlungLoadWeight();
    }
    xobj::AttrSlungLoadWeight out(weight);
    out.setEnabled(enabled == TRUE);
    return out;
}

xobj::AttrSpecular MainObjParamsWrapper::specular() {
    float ratio = 0;
    BOOL enabled = TRUE;
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_SpecularEnable, mT, enabled, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_SpecularEnable);
        }
        if (!mPbAttr->GetValue(MainObjAttr_SpecularRatio, mT, ratio, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_SpecularRatio);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
        return xobj::AttrSpecular();
    }
    xobj::AttrSpecular out(ratio);
    out.setEnabled(enabled == TRUE);
    return out;
}

xobj::AttrTint MainObjParamsWrapper::tint() {
    float albedo = 0;
    float emissive = 0;
    BOOL enabled = TRUE;
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_TintEnable, mT, enabled, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_TintEnable);
        }
        if (!mPbAttr->GetValue(MainObjAttr_TintAlbedo, mT, albedo, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_TintAlbedo);
        }
        if (!mPbAttr->GetValue(MainObjAttr_TintEmissive, mT, emissive, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_TintEmissive);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
        return xobj::AttrTint();
    }
    xobj::AttrTint out(albedo, emissive);
    out.setEnabled(enabled == TRUE);
    return out;
}

xobj::AttrSlopeLimit MainObjParamsWrapper::slopeLimit() {
    float minPitch = 0;
    float maxPitch = 0;
    float minRoll = 0;
    float maxRoll = 0;
    BOOL enabled = TRUE;
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_SlopeLimitEnable, mT, enabled, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_SlopeLimitEnable);
        }
        if (!mPbAttr->GetValue(MainObjAttr_SlopeLimitMinPitch, mT, minPitch, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_SlopeLimitMinPitch);
        }
        if (!mPbAttr->GetValue(MainObjAttr_SlopeLimitMaxPitch, mT, maxPitch, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_SlopeLimitMaxPitch);
        }
        if (!mPbAttr->GetValue(MainObjAttr_SlopeLimitMinRoll, mT, minRoll, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_SlopeLimitMinRoll);
        }
        if (!mPbAttr->GetValue(MainObjAttr_SlopeLimitMaxRoll, mT, maxRoll, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_SlopeLimitMaxRoll);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
        return xobj::AttrSlopeLimit();
    }
    xobj::AttrSlopeLimit out(minPitch, maxPitch, minRoll, maxRoll);
    out.setEnabled(enabled == TRUE);
    return out;
}

xobj::AttrCockpitRegion MainObjParamsWrapper::cockpitRegion(xobj::AttrCockpitRegion::eNum inIdx) {
    ParamID pEnabled = 0;
    ParamID pLeft = 0;
    ParamID pBottom = 0;
    ParamID pRight = 0;
    ParamID pTop = 0;

    if (inIdx > 3) {
        LError << "Cockpit region index must be between 0-3 inclusive";
    }

    switch (inIdx) {
        case xobj::AttrCockpitRegion::eNum::r1: {
            pEnabled = MainObjAttr_CockpitRegion_1;
            pLeft = MainObjAttr_CockpitRegion_1_L;
            pBottom = MainObjAttr_CockpitRegion_1_B;
            pRight = MainObjAttr_CockpitRegion_1_R;
            pTop = MainObjAttr_CockpitRegion_1_T;
        }
        break;
        case xobj::AttrCockpitRegion::eNum::r2: {
            pEnabled = MainObjAttr_CockpitRegion_2;
            pLeft = MainObjAttr_CockpitRegion_2_L;
            pBottom = MainObjAttr_CockpitRegion_2_B;
            pRight = MainObjAttr_CockpitRegion_2_R;
            pTop = MainObjAttr_CockpitRegion_2_T;
        }
        break;
        case xobj::AttrCockpitRegion::eNum::r3: {
            pEnabled = MainObjAttr_CockpitRegion_3;
            pLeft = MainObjAttr_CockpitRegion_3_L;
            pBottom = MainObjAttr_CockpitRegion_3_B;
            pRight = MainObjAttr_CockpitRegion_3_R;
            pTop = MainObjAttr_CockpitRegion_3_T;
        }
        break;
        case xobj::AttrCockpitRegion::eNum::r4: {
            pEnabled = MainObjAttr_CockpitRegion_4;
            pLeft = MainObjAttr_CockpitRegion_4_L;
            pBottom = MainObjAttr_CockpitRegion_4_B;
            pRight = MainObjAttr_CockpitRegion_4_R;
            pTop = MainObjAttr_CockpitRegion_4_T;
        }
        break;
        default: return xobj::AttrCockpitRegion();
    }

    int left = 0;
    int bottom = 0;
    int right = 0;
    int top = 0;
    BOOL enabled = TRUE;
    if (mPbAttr) {
        if (!mPbAttr->GetValue(pEnabled, mT, enabled, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(pEnabled);
        }
        if (!mPbAttr->GetValue(pLeft, mT, left, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(pLeft);
        }
        if (!mPbAttr->GetValue(pBottom, mT, bottom, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(pBottom);
        }
        if (!mPbAttr->GetValue(pRight, mT, right, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(pRight);
        }
        if (!mPbAttr->GetValue(pTop, mT, top, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(pTop);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
        return xobj::AttrCockpitRegion();
    }
    xobj::AttrCockpitRegion out(left, bottom, right, top);
    out.setEnabled(enabled == TRUE);
    return out;
}

//-------------------------------------------------------------------------

void MainObjParamsWrapper::setWetDry(const xobj::AttrWetDry & inAttr) {
    if (mPbAttr) {
        if (!mPbAttr->SetValue(MainObjAttr_DryWet, mT, inAttr ? int(inAttr.state()) : FALSE)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_DryWet);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setBlend(const xobj::AttrBlend & inAttr) {
    if (mPbAttr) {
        if (!mPbAttr->SetValue(MainObjAttr_BlendingType, mT, inAttr ? int(inAttr.type()) : FALSE)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_BlendingType);
        }
        if (!mPbAttr->SetValue(MainObjAttr_BlendingRatio, mT, inAttr.ratio())) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_BlendingRatio);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setLayerGroup(const xobj::AttrLayerGroup & inAttr) {
    if (mPbAttr) {

        sts::Str str = sts::toString(inAttr.layer().toString());
#if MAX_VERSION_MAJOR < 12
		TCHAR * strVal = const_cast<TCHAR*>(str.c_str());
#else
        const MCHAR * strVal = str.c_str();
#endif

        if (!mPbAttr->SetValue(MainObjAttr_LayerGroupLayer, mT, strVal)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_LayerGroupLayer);
        }
        if (!mPbAttr->SetValue(MainObjAttr_LayerGroupOffset, mT, inAttr.offset())) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_LayerGroupOffset);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setDrapedLayerGroup(const xobj::AttrDrapedLayerGroup & inAttr) {
    if (mPbAttr) {

        sts::Str str = sts::toString(inAttr.layer().toString());
#if MAX_VERSION_MAJOR < 12
		TCHAR * strVal = const_cast<TCHAR*>(str.c_str());
#else
        const MCHAR * strVal = str.c_str();
#endif

        if (!mPbAttr->SetValue(MainObjAttr_LayerGroupDrapedLayer, mT, strVal)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_LayerGroupDrapedLayer);
        }
        if (!mPbAttr->SetValue(MainObjAttr_LayerGroupDrapedOffset, mT, inAttr.offset())) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_LayerGroupDrapedOffset);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setLodDrap(const xobj::AttrLodDrap & inAttr) {
    if (mPbAttr) {
        if (!mPbAttr->SetValue(MainObjAttr_LodDrapEnable, mT, int(static_cast<bool>(inAttr)))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_LodDrapEnable);
        }
        if (!mPbAttr->SetValue(MainObjAttr_LodDrapDistance, mT, inAttr.distance())) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_LodDrapDistance);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setSlungWeight(const xobj::AttrSlungLoadWeight & inAttr) {
    if (mPbAttr) {
        if (!mPbAttr->SetValue(MainObjAttr_SlungLoadWeightEnable, mT, int(static_cast<bool>(inAttr)))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_SlungLoadWeightEnable);
        }
        if (!mPbAttr->SetValue(MainObjAttr_SlungLoadWeight, mT, inAttr.weight())) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_SlungLoadWeight);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setSpecular(const xobj::AttrSpecular & inAttr) {
    if (mPbAttr) {
        if (!mPbAttr->SetValue(MainObjAttr_SpecularEnable, mT, int(static_cast<bool>(inAttr)))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_SpecularEnable);
        }
        if (!mPbAttr->SetValue(MainObjAttr_SpecularRatio, mT, inAttr.ratio())) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_SpecularRatio);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setTint(const xobj::AttrTint & inAttr) {
    if (mPbAttr) {
        if (!mPbAttr->SetValue(MainObjAttr_TintEnable, mT, int(static_cast<bool>(inAttr)))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_TintEnable);
        }
        if (!mPbAttr->SetValue(MainObjAttr_TintAlbedo, mT, inAttr.albedo())) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_TintAlbedo);
        }
        if (!mPbAttr->SetValue(MainObjAttr_TintEmissive, mT, inAttr.emissive())) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_TintEmissive);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setSlopeLimit(const xobj::AttrSlopeLimit & inAttr) {
    if (mPbAttr) {
        if (!mPbAttr->SetValue(MainObjAttr_SlopeLimitEnable, mT, int(static_cast<bool>(inAttr)))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_SlopeLimitEnable);
        }
        if (!mPbAttr->SetValue(MainObjAttr_SlopeLimitMinPitch, mT, inAttr.minPitch())) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_SlopeLimitMinPitch);
        }
        if (!mPbAttr->SetValue(MainObjAttr_SlopeLimitMaxPitch, mT, inAttr.maxPitch())) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_SlopeLimitMaxPitch);
        }
        if (!mPbAttr->SetValue(MainObjAttr_SlopeLimitMinRoll, mT, inAttr.minRoll())) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_SlopeLimitMinRoll);
        }
        if (!mPbAttr->SetValue(MainObjAttr_SlopeLimitMaxRoll, mT, inAttr.maxRoll())) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_SlopeLimitMaxRoll);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setCockpitRegion(const xobj::AttrCockpitRegion & inAttr, xobj::AttrCockpitRegion::eNum inIdx) {
    ParamID pEnabled = 0;
    ParamID pLeft = 0;
    ParamID pBottom = 0;
    ParamID pRight = 0;
    ParamID pTop = 0;

    if (inIdx > 3) {
        LError << "Cockpit region index must be between 0-3 inclusive";
    }

    switch (inIdx) {
        case xobj::AttrCockpitRegion::eNum::r1: {
            pEnabled = MainObjAttr_CockpitRegion_1;
            pLeft = MainObjAttr_CockpitRegion_1_L;
            pBottom = MainObjAttr_CockpitRegion_1_B;
            pRight = MainObjAttr_CockpitRegion_1_R;
            pTop = MainObjAttr_CockpitRegion_1_T;
        }
        break;
        case xobj::AttrCockpitRegion::eNum::r2: {
            pEnabled = MainObjAttr_CockpitRegion_2;
            pLeft = MainObjAttr_CockpitRegion_2_L;
            pBottom = MainObjAttr_CockpitRegion_2_B;
            pRight = MainObjAttr_CockpitRegion_2_R;
            pTop = MainObjAttr_CockpitRegion_2_T;
        }
        break;
        case xobj::AttrCockpitRegion::eNum::r3: {
            pEnabled = MainObjAttr_CockpitRegion_3;
            pLeft = MainObjAttr_CockpitRegion_3_L;
            pBottom = MainObjAttr_CockpitRegion_3_B;
            pRight = MainObjAttr_CockpitRegion_3_R;
            pTop = MainObjAttr_CockpitRegion_3_T;
        }
        break;
        case xobj::AttrCockpitRegion::eNum::r4: {
            pEnabled = MainObjAttr_CockpitRegion_4;
            pLeft = MainObjAttr_CockpitRegion_4_L;
            pBottom = MainObjAttr_CockpitRegion_4_B;
            pRight = MainObjAttr_CockpitRegion_4_R;
            pTop = MainObjAttr_CockpitRegion_4_T;
        }
        break;
        default: return;
    }
    if (mPbAttr) {
        if (!mPbAttr->SetValue(pEnabled, mT, int(static_cast<bool>(inAttr)))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(pEnabled);
        }
        if (!mPbAttr->SetValue(pLeft, mT, int(inAttr.left()))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(pLeft);
        }
        if (!mPbAttr->SetValue(pBottom, mT, int(inAttr.bottom()))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(pBottom);
        }
        if (!mPbAttr->SetValue(pRight, mT, int(inAttr.right()))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(pRight);
        }
        if (!mPbAttr->SetValue(pTop, mT, int(inAttr.top()))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(pTop);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
