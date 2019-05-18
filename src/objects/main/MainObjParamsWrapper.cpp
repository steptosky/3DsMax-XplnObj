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

#pragma warning(push, 0)
#include <iparamb2.h>
#pragma warning(pop)

#include "objects/main/param-blocks/MainObjParams.h"
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

    mPbExp = node->GetObjectRef()->GetParamBlockByID(static_cast<BlockID>(eMainObjParamsBlocks::MainObjGeometryParams));
    mPbAttr = node->GetObjectRef()->GetParamBlockByID(static_cast<BlockID>(eMainObjParamsBlocks::MainObjAttrParams));
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
        if (!mPbExp->GetValue(MainObjGeom_Meshes, mT, val, mInterval)) {
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
        if (!mPbExp->GetValue(MainObjGeom_Lines, mT, val, mInterval)) {
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
        if (!mPbExp->GetValue(MainObjGeom_Lights, mT, val, mInterval)) {
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
        if (!mPbExp->GetValue(MainObjGeom_Animation, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjExp_Animation);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val == TRUE;
}

bool MainObjParamsWrapper::isOptimization() {
    BOOL val = TRUE;
    if (mPbExp) {
        if (!mPbExp->GetValue(MainObjGeom_Optimization, mT, val, mInterval)) {
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
        if (!mPbExp->GetValue(MainObjGeom_Instancing, mT, val, mInterval)) {
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
        if (!mPbExp->GetValue(MainObjGeom_Debug, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjExp_Debug);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val == TRUE;
}

//-------------------------------------------------------------------------

void MainObjParamsWrapper::setExportEnable(const bool state) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjAttr_ExportEnable, mT, int(state))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_ExportEnable);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setMeshExport(const bool state) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjGeom_Meshes, mT, int(state))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjExp_Meshes);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setLinesExport(const bool state) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjGeom_Lines, mT, int(state))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjExp_Lines);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setLightsExport(const bool state) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjGeom_Lights, mT, int(state))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjExp_Lights);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setAnimationExport(const bool state) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjGeom_Animation, mT, int(state))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjExp_Animation);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setOptimization(const bool state) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjGeom_Optimization, mT, int(state))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjExp_Optimisation);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setInstancing(const bool state) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjGeom_Instancing, mT, int(state))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjExp_Instancing);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setDebug(const bool state) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjGeom_Debug, mT, int(state))) {
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
        if (!mPbExp->GetValue(MainObjGeom_NameMeshes, mT, val, mInterval)) {
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
        if (!mPbExp->GetValue(MainObjGeom_NameLines, mT, val, mInterval)) {
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
        if (!mPbExp->GetValue(MainObjGeom_NameLights, mT, val, mInterval)) {
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
        if (!mPbExp->GetValue(MainObjGeom_NameDummies, mT, val, mInterval)) {
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
        if (!mPbExp->GetValue(MainObjGeom_TreeHierarchy, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjExp_TreeHierarchy);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val == TRUE;
}

//-------------------------------------------------------------------------

void MainObjParamsWrapper::setNameMesh(const bool state) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjGeom_NameMeshes, mT, int(state))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjExp_NameMeshes);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setNameLines(const bool state) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjGeom_NameLines, mT, int(state))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjExp_NameLines);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setNameLights(const bool state) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjGeom_NameLights, mT, int(state))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjExp_NameLights);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setNameDummies(const bool state) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjGeom_NameDummies, mT, int(state))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjExp_NameDummies);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setTreeHierarchy(const bool state) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjGeom_TreeHierarchy, mT, int(state))) {
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
        if (!mPbExp->GetValue(MainObjGeom_ScaleEnabled, mT, val, mInterval)) {
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
        if (!mPbExp->GetValue(MainObjGeom_ScaleValue, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObj_ScaleValue);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    return val;
}

//-------------------------------------------------------------------------

void MainObjParamsWrapper::setScale(const bool manual, const float value) {
    if (mPbExp) {
        if (!mPbExp->SetValue(MainObjGeom_ScaleEnabled, mT, int(manual))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObj_ScaleEnabled);
        }
        if (!mPbExp->SetValue(MainObjGeom_ScaleValue, mT, value)) {
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

std::optional<std::string> MainObjParamsWrapper::texture() {
#if MAX_VERSION_MAJOR < 12
    TCHAR * val = nullptr;
#else
    const MCHAR * val = nullptr;
#endif
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_Texture, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_Texture);
            return std::nullopt;
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    const auto valStr = val ? sts::toMbString(val) : std::string();
    return !valStr.empty() ? std::optional(valStr) : std::nullopt;
}

std::optional<std::string> MainObjParamsWrapper::textureLit() {
#if MAX_VERSION_MAJOR < 12
    TCHAR * val = nullptr;
#else
    const MCHAR * val = nullptr;
#endif
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_TextureLit, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_TextureLit);
            return std::nullopt;
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    const auto valStr = val ? sts::toMbString(val) : std::string();
    return !valStr.empty() ? std::optional(valStr) : std::nullopt;
}

std::optional<std::string> MainObjParamsWrapper::textureNormal() {
#if MAX_VERSION_MAJOR < 12
    TCHAR * val = nullptr;
#else
    const MCHAR * val = nullptr;
#endif
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_TextureNormal, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_TextureNormal);
            return std::nullopt;
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
    const auto valStr = val ? sts::toMbString(val) : std::string();
    return !valStr.empty() ? std::optional(valStr) : std::nullopt;
}

//-------------------------------------------------------------------------

void MainObjParamsWrapper::setPathPrefix(const std::string & str) {
    if (mPbAttr) {
        sts::Str convertedStr = sts::toString(str);
#if MAX_VERSION_MAJOR < 12
        const auto strVal = const_cast<TCHAR*>(convertedStr.c_str());
#else
        const auto strVal = convertedStr.c_str();
#endif

        if (!mPbAttr->SetValue(MainObjAttr_PathPrefix, mT, strVal)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_PathPrefix);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setTexture(const std::optional<std::string> & str) {
    if (mPbAttr) {
        const auto convertedStr = sts::toString(str.value_or(std::string()));
#if MAX_VERSION_MAJOR < 12
        const auto strVal = const_cast<TCHAR*>(convertedStr.c_str());
#else
        const auto strVal = convertedStr.c_str();
#endif

        if (!mPbAttr->SetValue(MainObjAttr_Texture, mT, strVal)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_Texture);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setTextureLit(const std::optional<std::string> & str) {
    if (mPbAttr) {
        const auto convertedStr = sts::toString(str.value_or(std::string()));
#if MAX_VERSION_MAJOR < 12
        const auto strVal = const_cast<TCHAR*>(convertedStr.c_str());
#else
        const auto strVal = convertedStr.c_str();
#endif

        if (!mPbAttr->SetValue(MainObjAttr_TextureLit, mT, strVal)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_TextureLit);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setTextureNormal(const std::optional<std::string> & str) {
    if (mPbAttr) {
        const auto convertedStr = sts::toString(str.value_or(std::string()));
#if MAX_VERSION_MAJOR < 12
        const auto strVal = const_cast<TCHAR*>(convertedStr.c_str());
#else
        const auto strVal = convertedStr.c_str();
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

void MainObjParamsWrapper::setBlendGlass(const bool state) {
    if (mPbAttr) {
        if (!mPbAttr->SetValue(MainObjAttr_BlendGlass, mT, int(state))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_BlendGlass);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setNormalMetalness(const bool state) {
    if (mPbAttr) {
        if (!mPbAttr->SetValue(MainObjAttr_NormalMetalness, mT, int(state))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_NormalMetalness);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setNoShadow(const bool state) {
    if (mPbAttr) {
        if (!mPbAttr->SetValue(MainObjAttr_NoShadow, mT, int(state))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_NoShadow);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setTilted(const bool state) {
    if (mPbAttr) {
        if (!mPbAttr->SetValue(MainObjAttr_Tilted, mT, int(state))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_Tilted);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setCockpitLit(const bool state) {
    if (mPbAttr) {
        if (!mPbAttr->SetValue(MainObjAttr_CockpitLit, mT, int(state))) {
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

std::optional<xobj::AttrWetDry> MainObjParamsWrapper::wetDry() {
    int val = 0;
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_DryWet, mT, val, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_DryWet);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
        return std::nullopt;
    }
    return val == 0 ? std::nullopt : std::optional(xobj::AttrWetDry(static_cast<xobj::AttrWetDry::eState>(val)));
}

std::optional<xobj::AttrBlend> MainObjParamsWrapper::blend() {
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
        return std::nullopt;
    }

    if (type == 0) {
        return std::nullopt;
    }

    return xobj::AttrBlend(static_cast<xobj::AttrBlend::eType>(type), ratio);
}

std::optional<xobj::AttrLayerGroup> MainObjParamsWrapper::layerGroup() {
#if MAX_VERSION_MAJOR < 12
    TCHAR * layer = nullptr;
#else
    const MCHAR * layer = nullptr;
#endif
    int offset = 0;
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_LayerGroupLayer, mT, layer, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_LayerGroupLayer);
            return std::nullopt;
        }
        if (!mPbAttr->GetValue(MainObjAttr_LayerGroupOffset, mT, offset, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_LayerGroupOffset);
            return std::nullopt;
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
        return std::nullopt;
    }

    const auto l = layer ? xobj::ELayer::fromString(sts::toMbString(layer).c_str()) : xobj::ELayer(xobj::ELayer::none);
    const xobj::AttrLayerGroup out(l, offset);
    return l.id() == xobj::ELayer::none ? std::nullopt : std::optional(out);
}

std::optional<xobj::AttrDrapedLayerGroup> MainObjParamsWrapper::drapedLayerGroup() {
#if MAX_VERSION_MAJOR < 12
    TCHAR * layer = nullptr;
#else
    const MCHAR * layer = nullptr;
#endif
    int offset = 0;
    if (mPbAttr) {
        if (!mPbAttr->GetValue(MainObjAttr_LayerGroupDrapedLayer, mT, layer, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_LayerGroupDrapedLayer);
            return std::nullopt;
        }
        if (!mPbAttr->GetValue(MainObjAttr_LayerGroupDrapedOffset, mT, offset, mInterval)) {
            LError << LogNode(mNode) << "Can't retrieve value:" << TOTEXT(MainObjAttr_LayerGroupDrapedOffset);
            return std::nullopt;
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
        return std::nullopt;
    }
    const auto l = layer ? xobj::ELayer::fromString(sts::toMbString(layer).c_str()) : xobj::ELayer(xobj::ELayer::none);
    const xobj::AttrDrapedLayerGroup out(l, offset);
    return l.id() == xobj::ELayer::none ? std::nullopt : std::optional(out);
}

std::optional<xobj::AttrDrapedLod> MainObjParamsWrapper::lodDrap() {
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
        return std::nullopt;
    }
    const xobj::AttrDrapedLod out(dist);
    return enabled == TRUE ? std::optional(out) : std::nullopt;
}

std::optional<xobj::AttrSlungLoadWeight> MainObjParamsWrapper::slungWeight() {
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
        return std::nullopt;
    }
    const xobj::AttrSlungLoadWeight out(weight);
    return enabled == TRUE ? std::optional(out) : std::nullopt;
}

std::optional<xobj::AttrSpecular> MainObjParamsWrapper::specular() {
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
        return std::nullopt;
    }
    const xobj::AttrSpecular out(ratio);
    return enabled == TRUE ? std::optional(out) : std::nullopt;
}

std::optional<xobj::AttrTint> MainObjParamsWrapper::tint() {
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
        return std::nullopt;
    }
    const xobj::AttrTint out(albedo, emissive);
    return enabled == TRUE ? std::optional(out) : std::nullopt;
}

std::optional<xobj::AttrSlopeLimit> MainObjParamsWrapper::slopeLimit() {
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
        return std::nullopt;
    }
    const xobj::AttrSlopeLimit out(minPitch, maxPitch, minRoll, maxRoll);
    return enabled == TRUE ? std::optional(out) : std::nullopt;
}

std::optional<xobj::AttrCockpitRegion> MainObjParamsWrapper::cockpitRegion(xobj::AttrCockpitRegion::eNum idx) {
    ParamID pEnabled = 0;
    ParamID pLeft = 0;
    ParamID pBottom = 0;
    ParamID pRight = 0;
    ParamID pTop = 0;

    if (idx > 3) {
        LError << "Cockpit region index must be between 0-3 inclusive";
    }

    switch (idx) {
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
        default: return std::nullopt;
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
        return std::nullopt;
    }
    const xobj::AttrCockpitRegion out(left, bottom, right, top);
    return enabled == TRUE ? std::optional(out) : std::nullopt;
}

//-------------------------------------------------------------------------

void MainObjParamsWrapper::setWetDry(const std::optional<xobj::AttrWetDry> & attr) {
    if (mPbAttr) {
        const auto actual = attr.value_or(xobj::AttrWetDry());
        if (!mPbAttr->SetValue(MainObjAttr_DryWet, mT, attr ? int(actual.mState) : FALSE)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_DryWet);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setBlend(const std::optional<xobj::AttrBlend> & attr) {
    if (mPbAttr) {
        const auto actual = attr.value_or(xobj::AttrBlend());
        if (!mPbAttr->SetValue(MainObjAttr_BlendingType, mT, attr ? int(actual.mType) : FALSE)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_BlendingType);
        }
        if (!mPbAttr->SetValue(MainObjAttr_BlendingRatio, mT, actual.mRatio)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_BlendingRatio);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setLayerGroup(const std::optional<xobj::AttrLayerGroup> & attr) {
    if (mPbAttr) {
        const auto actual = attr.value_or(xobj::AttrLayerGroup());

        const auto str = sts::toString(actual.mLayer.toString());
#if MAX_VERSION_MAJOR < 12
        TCHAR * strVal = const_cast<TCHAR*>(str.c_str());
#else
        const MCHAR * strVal = str.c_str();
#endif

        if (!mPbAttr->SetValue(MainObjAttr_LayerGroupLayer, mT, strVal)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_LayerGroupLayer);
        }
        if (!mPbAttr->SetValue(MainObjAttr_LayerGroupOffset, mT, actual.mOffset)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_LayerGroupOffset);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setDrapedLayerGroup(const std::optional<xobj::AttrDrapedLayerGroup> & attr) {
    if (mPbAttr) {
        const auto actual = attr.value_or(xobj::AttrDrapedLayerGroup());

        const auto str = sts::toString(actual.mLayer.toString());
#if MAX_VERSION_MAJOR < 12
        TCHAR * strVal = const_cast<TCHAR*>(str.c_str());
#else
        const MCHAR * strVal = str.c_str();
#endif

        if (!mPbAttr->SetValue(MainObjAttr_LayerGroupDrapedLayer, mT, strVal)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_LayerGroupDrapedLayer);
        }
        if (!mPbAttr->SetValue(MainObjAttr_LayerGroupDrapedOffset, mT, actual.mOffset)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_LayerGroupDrapedOffset);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setLodDrap(const std::optional<xobj::AttrDrapedLod> & attr) {
    if (mPbAttr) {
        const auto actual = attr.value_or(xobj::AttrDrapedLod());
        if (!mPbAttr->SetValue(MainObjAttr_LodDrapEnable, mT, int(static_cast<bool>(attr)))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_LodDrapEnable);
        }
        if (!mPbAttr->SetValue(MainObjAttr_LodDrapDistance, mT, actual.mDistance)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_LodDrapDistance);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setSlungWeight(const std::optional<xobj::AttrSlungLoadWeight> & attr) {
    if (mPbAttr) {
        const auto actual = attr.value_or(xobj::AttrSlungLoadWeight());
        if (!mPbAttr->SetValue(MainObjAttr_SlungLoadWeightEnable, mT, int(static_cast<bool>(attr)))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_SlungLoadWeightEnable);
        }
        if (!mPbAttr->SetValue(MainObjAttr_SlungLoadWeight, mT, actual.mWeight)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_SlungLoadWeight);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setSpecular(const std::optional<xobj::AttrSpecular> & attr) {
    if (mPbAttr) {
        const auto actual = attr.value_or(xobj::AttrSpecular());
        if (!mPbAttr->SetValue(MainObjAttr_SpecularEnable, mT, int(static_cast<bool>(attr)))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_SpecularEnable);
        }
        if (!mPbAttr->SetValue(MainObjAttr_SpecularRatio, mT, actual.mRatio)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_SpecularRatio);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setTint(const std::optional<xobj::AttrTint> & attr) {
    if (mPbAttr) {
        const auto actual = attr.value_or(xobj::AttrTint());
        if (!mPbAttr->SetValue(MainObjAttr_TintEnable, mT, int(static_cast<bool>(attr)))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_TintEnable);
        }
        if (!mPbAttr->SetValue(MainObjAttr_TintAlbedo, mT, actual.mAlbedo)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_TintAlbedo);
        }
        if (!mPbAttr->SetValue(MainObjAttr_TintEmissive, mT, actual.mEmissive)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_TintEmissive);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setSlopeLimit(const std::optional<xobj::AttrSlopeLimit> & attr) {
    if (mPbAttr) {
        const auto actual = attr.value_or(xobj::AttrSlopeLimit());
        if (!mPbAttr->SetValue(MainObjAttr_SlopeLimitEnable, mT, int(static_cast<bool>(attr)))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_SlopeLimitEnable);
        }
        if (!mPbAttr->SetValue(MainObjAttr_SlopeLimitMinPitch, mT, actual.mMinPitch)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_SlopeLimitMinPitch);
        }
        if (!mPbAttr->SetValue(MainObjAttr_SlopeLimitMaxPitch, mT, actual.mMaxPitch)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_SlopeLimitMaxPitch);
        }
        if (!mPbAttr->SetValue(MainObjAttr_SlopeLimitMinRoll, mT, actual.mMinRoll)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_SlopeLimitMinRoll);
        }
        if (!mPbAttr->SetValue(MainObjAttr_SlopeLimitMaxRoll, mT, actual.mMaxRoll)) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(MainObjAttr_SlopeLimitMaxRoll);
        }
    }
    else {
        LError << "Pointer to IParamBlock2 is nullptr";
    }
}

void MainObjParamsWrapper::setCockpitRegion(const std::optional<xobj::AttrCockpitRegion> & attr, const xobj::AttrCockpitRegion::eNum idx) {
    ParamID pEnabled = 0;
    ParamID pLeft = 0;
    ParamID pBottom = 0;
    ParamID pRight = 0;
    ParamID pTop = 0;
    const auto actual = attr.value_or(xobj::AttrCockpitRegion());

    if (idx > 3) {
        LError << "Cockpit region index must be between 0-3 inclusive";
    }

    switch (idx) {
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
        if (!mPbAttr->SetValue(pEnabled, mT, int(static_cast<bool>(attr)))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(pEnabled);
        }
        if (!mPbAttr->SetValue(pLeft, mT, int(actual.mLeft))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(pLeft);
        }
        if (!mPbAttr->SetValue(pBottom, mT, int(actual.mBottom))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(pBottom);
        }
        if (!mPbAttr->SetValue(pRight, mT, int(actual.mRight))) {
            LError << LogNode(mNode) << "Can't save value:" << TOTEXT(pRight);
        }
        if (!mPbAttr->SetValue(pTop, mT, int(actual.mTop))) {
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
