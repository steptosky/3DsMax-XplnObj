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
#include <iparamb2.h>
#pragma warning(pop)

#include <xpln/obj/attributes/AttrWetDry.h>
#include <xpln/obj/attributes/AttrBlend.h>
#include <xpln/obj/attributes/AttrLayerGroup.h>
#include <xpln/obj/attributes/AttrDrapedLayerGroup.h>
#include <xpln/obj/attributes/AttrLodDrap.h>
#include <xpln/obj/attributes/AttrSlungLoadWeight.h>
#include <xpln/obj/attributes/AttrSpecular.h>
#include <xpln/obj/attributes/AttrTint.h>
#include <xpln/obj/attributes/AttrSlopeLimit.h>
#include <xpln/obj/attributes/AttrCockpitRegion.h>

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

/*!
 * \details This class is for easy read and write x-plane and other data of 3d max object.
 */
class MainObjParamsWrapper {
public:

    MainObjParamsWrapper(IParamBlock2 * pbAttr, IParamBlock2 * pbExp, const TimeValue t, const Interval & interval);
    MainObjParamsWrapper(INode * node, const TimeValue t, const Interval & interval);
    ~MainObjParamsWrapper() = default;

    //-------------------------------------------------------------------------

    static bool isMainObj(INode * inNode);

    /***************************************************************************************/

    void setExportEnable(bool inState);
    void setMeshExport(bool inState);
    void setLinesExport(bool inState);
    void setLightsExport(bool inState);
    void setAnimationExport(bool inState);
    void setOptimisation(bool inState);
    void setInstancing(bool inState);
    void setDebug(bool inState);

    bool isExportEnable();
    bool isMeshExport();
    bool isLinesExport();
    bool isLightsExport();
    bool isAnimationExport();
    bool isOptimisation();
    bool isInstancing();
    bool isDebug();
    //-------------------------------------------------------------------------

    void setNameMesh(bool inState);
    void setNameLines(bool inState);
    void setNameLights(bool inState);
    void setNameDummies(bool inState);
    void setTreeHierarchy(bool inState);

    bool isNameMesh();
    bool isNameLines();
    bool isNameLights();
    bool isNameDummies();
    bool isTreeHierarchy();

    //-------------------------------------------------------------------------

    void setScale(bool inManual, float inValue);
    bool isManualScale();
    float scale();

    /***************************************************************************************/

    void setPathPrefix(const std::string & inStr);
    void setTexture(const std::string & inStr);
    void setTextureLit(const std::string & inStr);
    void setTextureNormal(const std::string & inStr);

    std::string pathPrefix();
    std::string texture();
    std::string textureLit();
    std::string textureNormal();

    //-------------------------------------------------------------------------

    void setBlendGlass(bool inState);
    void setNormalMetalness(bool inState);
    void setNoShadow(bool inState);
    void setTilted(bool inState);
    void setCockpitLit(bool inState);

    bool isBlendGlass();
    bool isNormalMetalness();
    bool isNoShadow();
    bool isTilted();
    bool isCockpitLit();

    //-------------------------------------------------------------------------

    void setWetDry(const xobj::AttrWetDry & inAttr);
    void setBlend(const xobj::AttrBlend & inAttr);
    void setLayerGroup(const xobj::AttrLayerGroup & inAttr);
    void setDrapedLayerGroup(const xobj::AttrDrapedLayerGroup & inAttr);
    void setLodDrap(const xobj::AttrLodDrap & inAttr);
    void setSlungWeight(const xobj::AttrSlungLoadWeight & inAttr);
    void setSpecular(const xobj::AttrSpecular & inAttr);
    void setTint(const xobj::AttrTint & inAttr);
    void setSlopeLimit(const xobj::AttrSlopeLimit & inAttr);
    void setCockpitRegion(const xobj::AttrCockpitRegion & inAttr, xobj::AttrCockpitRegion::eNum inIdx);

    xobj::AttrWetDry wetDry();
    xobj::AttrBlend blend();
    xobj::AttrLayerGroup layerGroup();
    xobj::AttrDrapedLayerGroup drapedLayerGroup();
    xobj::AttrLodDrap lodDrap();
    xobj::AttrSlungLoadWeight slungWeight();
    xobj::AttrSpecular specular();
    xobj::AttrTint tint();
    xobj::AttrSlopeLimit slopeLimit();
    xobj::AttrCockpitRegion cockpitRegion(xobj::AttrCockpitRegion::eNum inIdx);

    /***************************************************************************************/

    INode * node() const {
        return mNode;
    }

    TimeValue timeValue() const {
        return mT;
    }

private:

    Interval mInterval;
    TimeValue mT = 0;
    IParamBlock2 * mPbExp = nullptr;
    IParamBlock2 * mPbAttr = nullptr;
    INode * mNode = nullptr;

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
