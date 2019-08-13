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

#include <unordered_set>

#pragma warning(push, 0)
#include <max.h>
#pragma warning(pop)

#include <xpln/obj/attributes/AttrWetDry.h>
#include <xpln/obj/attributes/AttrBlend.h>
#include <xpln/obj/attributes/AttrLayerGroup.h>
#include <xpln/obj/attributes/AttrDrapedLayerGroup.h>
#include <xpln/obj/attributes/AttrDrapedLod.h>
#include <xpln/obj/attributes/AttrSlungLoadWeight.h>
#include <xpln/obj/attributes/AttrSpecular.h>
#include <xpln/obj/attributes/AttrTint.h>
#include <xpln/obj/attributes/AttrSlopeLimit.h>
#include <xpln/obj/attributes/AttrCockpitRegion.h>

class IParamBlock2;
class ILayer;

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

/*!
 * \details This class is for easy read and write x-plane and other data of 3d max object.
 */
class MainObjParamsWrapper {
public:

    MainObjParamsWrapper(IParamBlock2 * pbAttr, IParamBlock2 * pbGeom, TimeValue t, const Interval & interval);
    MainObjParamsWrapper(INode * node, TimeValue t, const Interval & interval);
    ~MainObjParamsWrapper() = default;

    //-------------------------------------------------------------------------

    static bool isMainObj(INode * inNode);

    /***************************************************************************************/

    void setExportEnable(bool state);
    void setMeshExport(bool state);
    void setLinesExport(bool state);
    void setLightsExport(bool state);
    void setAnimationExport(bool state);
    void setOptimization(bool state);
    void setInstancing(bool state);
    void setDebug(bool state);

    bool isExportEnable();
    bool isMeshExport();
    bool isLinesExport();
    bool isLightsExport();
    bool isAnimationExport();
    bool isOptimization();
    bool isInstancing();
    bool isDebug();
    //-------------------------------------------------------------------------

    void setNameMesh(bool state);
    void setNameLines(bool state);
    void setNameLights(bool state);
    void setNameDummies(bool state);
    void setTreeHierarchy(bool state);

    bool isNameMesh();
    bool isNameLines();
    bool isNameLights();
    bool isNameDummies();
    bool isTreeHierarchy();

    //-------------------------------------------------------------------------

    void setScale(bool manual, float value);
    bool isManualScale();
    float scale();

    /***************************************************************************************/

    void setPathPrefix(const std::string & str);
    void setTexture(const std::optional<std::string> & str);
    void setTextureLit(const std::optional<std::string> & str);
    void setTextureNormal(const std::optional<std::string> & str);

    std::string pathPrefix();
    std::optional<std::string> texture();
    std::optional<std::string> textureLit();
    std::optional<std::string> textureNormal();

    //-------------------------------------------------------------------------

    void setBlendGlass(bool state);
    void setNormalMetalness(bool state);
    void setNoShadow(bool state);
    void setTilted(bool state);
    void setCockpitLit(bool state);

    bool isBlendGlass();
    bool isNormalMetalness();
    bool isNoShadow();
    bool isTilted();
    bool isCockpitLit();

    //-------------------------------------------------------------------------

    void setWetDry(const std::optional<xobj::AttrWetDry> & attr);
    void setBlend(const std::optional<xobj::AttrBlend> & attr);
    void setLayerGroup(const std::optional<xobj::AttrLayerGroup> & attr);
    void setDrapedLayerGroup(const std::optional<xobj::AttrDrapedLayerGroup> & attr);
    void setLodDrap(const std::optional<xobj::AttrDrapedLod> & attr);
    void setSlungWeight(const std::optional<xobj::AttrSlungLoadWeight> & attr);
    void setSpecular(const std::optional<xobj::AttrSpecular> & attr);
    void setTint(const std::optional<xobj::AttrTint> & attr);
    void setSlopeLimit(const std::optional<xobj::AttrSlopeLimit> & attr);
    void setCockpitRegion(const std::optional<xobj::AttrCockpitRegion> & attr, xobj::AttrCockpitRegion::eNum idx);

    std::optional<xobj::AttrWetDry> wetDry();
    std::optional<xobj::AttrBlend> blend();
    std::optional<xobj::AttrLayerGroup> layerGroup();
    std::optional<xobj::AttrDrapedLayerGroup> drapedLayerGroup();
    std::optional<xobj::AttrDrapedLod> lodDrap();
    std::optional<xobj::AttrSlungLoadWeight> slungWeight();
    std::optional<xobj::AttrSpecular> specular();
    std::optional<xobj::AttrTint> tint();
    std::optional<xobj::AttrSlopeLimit> slopeLimit();
    std::optional<xobj::AttrCockpitRegion> cockpitRegion(xobj::AttrCockpitRegion::eNum idx);

    /***************************************************************************************/

    bool useLayersObjects();
    static std::unordered_set<ILayer*> geometryLayers(const MStr & startsWith);

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
    IParamBlock2 * mPbGeom = nullptr;
    IParamBlock2 * mPbAttr = nullptr;
    INode * mNode = nullptr;

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
