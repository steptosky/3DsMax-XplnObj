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

#include "MainObj.h"

#include <xpln/enums/ELayer.h>
#include <xpln/obj/attributes/AttrBlend.h>
#include <xpln/obj/attributes/AttrWetDry.h>

#include "objects/main/param-blocks/MainObjParams.h"
#include "common/String.h"
#include "models/bwc/SerializationId.h"
#include "ui-win/DlgMessageBox.h"
#include "MainObjParamsWrapper.h"
#include "additional/math/Compare.h"

// The file contains backward compatibility data loading methods

using namespace std::string_literals;

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void MainObject::loadMdDisplayObj(sts_bwc::DataStream & stream) const {
    bcw::SerializationId id;
    id.load(stream.getStdStream());

    if (id != bcw::SerializationId(0x7f084ca4, 0x1f19134e)) {
        LError << "Unexpected data input: " << id.toString();
        return;
    }
    uint8_t version;
    stream >> version;
    if (version != 1) {
        LError << "Unexpected data version: " << version;
        return;
    }

    float iconSize;
    stream >> iconSize;
    if (!mDisplayPb->SetValue(MainObjDisp_IconScale, mIp ? mIp->GetTime() : 0, iconSize)) {
        DLError << "Can't save " << TOTEXT(MainObjDisp_IconScale) << " value to the param block";
    }
}

void MainObject::loadRawGlobAttr(sts_bwc::DataStream & stream) const {
    bcw::SerializationId id;
    id.load(stream.getStdStream());
    if (id != bcw::SerializationId(0x37af3f56, 0x7eae24ae)) {
        LError << "Unexpected data input: " << id.toString();
        return;
    }
    unsigned char version;
    stream >> version;
    if (version != 1) {
        LError << "Unexpected data version: " << version;
        return;
    }

    struct {
        double noBlendRatio;
        double shadowBlendRatio;
        double specularRatio;

        double tintAlbedo;
        double tintEmissive;

        double slopePitchMax;
        double slopePitchMin;
        double slopeRollMax;
        double slopeRollMin;

        double slungLoad;
        int layerGroup;
        double lodDrapedDist;
        int drapedLayerGroup;

        std::wstring strLayerGroup;
        std::wstring strDrapedLayerGroup;

        bool chkNoShadow;
        bool chkTilted;
        bool chkCockpitLit;
        bool chkDry;
        bool chkWet;

        bool chkNoBlend;
        bool chkShadowBlend;
        bool chkSpecular;
        bool chkTint;
        bool chkSlopeLimit;
        bool chkSlungLoad;
        bool chkLodDraped;

        bool chkLayerGroup;
        bool chkDrapedLayerGroup;

        std::wstring strDiffuse;
        std::wstring strLit;
        std::wstring strBump;
    } data;

    stream >> data.noBlendRatio;
    stream >> data.shadowBlendRatio;
    stream >> data.specularRatio;

    stream >> data.tintAlbedo;
    stream >> data.tintEmissive;

    stream >> data.slopePitchMax;
    stream >> data.slopePitchMin;
    stream >> data.slopeRollMax;
    stream >> data.slopeRollMin;

    stream >> data.slungLoad;
    stream >> data.layerGroup;
    stream >> data.lodDrapedDist;
    stream >> data.drapedLayerGroup;

    stream >> data.strLayerGroup;
    stream >> data.strDrapedLayerGroup;

    stream >> data.chkNoShadow;
    stream >> data.chkTilted;
    stream >> data.chkCockpitLit;
    stream >> data.chkDry;
    stream >> data.chkWet;

    stream >> data.chkNoBlend;
    stream >> data.chkShadowBlend;
    stream >> data.chkSpecular;
    stream >> data.chkTint;
    stream >> data.chkSlopeLimit;
    stream >> data.chkSlungLoad;
    stream >> data.chkLodDraped;

    stream >> data.chkLayerGroup;
    stream >> data.chkDrapedLayerGroup;

    stream >> data.strDiffuse;
    stream >> data.strLit;
    stream >> data.strBump;

    //-------------------------------------------------------------------------
    if (data.chkNoBlend && data.chkShadowBlend) {
        const std::string msg(R"("shadow blend" and "no blend" can not be enabled at same time. Only data about "no blend" will be loaded.)");
        ui::win::DlgMessageBox::warning(nullptr, "[Backward Compatibility]", msg);
        LWarning << msg;
    }
    MainObjParamsWrapper wrapper(mAttrParamsPb, mExpPb, TimeValue(0), FOREVER);
    if (data.chkNoBlend) {
        wrapper.setBlend(xobj::AttrBlend(xobj::AttrBlend::no_blend, float(data.noBlendRatio)));
    }
    else if (data.chkShadowBlend) {
        wrapper.setBlend(xobj::AttrBlend(xobj::AttrBlend::shadow_blend, float(data.noBlendRatio)));
    }
    else {
        wrapper.setBlend(xobj::AttrBlend());
    }
    //-------------------------------------------------------------------------
    if (data.chkSpecular) {
        wrapper.setSpecular(xobj::AttrSpecular(float(data.specularRatio)));
    }
    else {
        wrapper.setSpecular(xobj::AttrSpecular());
    }
    //-------------------------------------------------------------------------
    if (data.chkTint) {
        wrapper.setTint(xobj::AttrTint(float(data.tintAlbedo), float(data.tintEmissive)));
    }
    else {
        wrapper.setTint(xobj::AttrTint());
    }
    //-------------------------------------------------------------------------
    if (data.chkSlopeLimit) {
        wrapper.setSlopeLimit(xobj::AttrSlopeLimit(float(data.slopePitchMin), float(data.slopePitchMax),
                                                   float(data.slopeRollMin), float(data.slopeRollMax)));
    }
    else {
        wrapper.setSlopeLimit(xobj::AttrSlopeLimit());
    }
    //-------------------------------------------------------------------------
    if (data.chkSlungLoad) {
        wrapper.setSlungWeight(xobj::AttrSlungLoadWeight(float(data.slungLoad)));
    }
    else {
        wrapper.setSlungWeight(xobj::AttrSlungLoadWeight());
    }
    //-------------------------------------------------------------------------
    if (data.chkLodDraped) {
        wrapper.setLodDrap(xobj::AttrDrapedLod(float(data.lodDrapedDist)));
    }
    else {
        wrapper.setLodDrap(xobj::AttrDrapedLod());
    }
    //-------------------------------------------------------------------------
    if (data.chkLayerGroup) {
        wrapper.setLayerGroup(xobj::AttrLayerGroup(xobj::ELayer::fromString(sts::toMbString(data.strLayerGroup).c_str()),
                                                   int(data.layerGroup)));
    }
    else {
        wrapper.setLayerGroup(xobj::AttrLayerGroup());
    }
    //-------------------------------------------------------------------------
    if (data.chkDrapedLayerGroup) {
        wrapper.setDrapedLayerGroup(xobj::AttrDrapedLayerGroup(xobj::ELayer::fromString(sts::toMbString(data.strDrapedLayerGroup).c_str()),
                                                               int(data.drapedLayerGroup)));
    }
    else {
        wrapper.setDrapedLayerGroup(xobj::AttrDrapedLayerGroup());
    }
    //-------------------------------------------------------------------------
    wrapper.setNoShadow(data.chkNoShadow);
    wrapper.setTilted(data.chkTilted);
    wrapper.setCockpitLit(data.chkCockpitLit);
    //-------------------------------------------------------------------------
    if (data.chkDry) {
        wrapper.setWetDry(xobj::AttrWetDry(xobj::AttrWetDry::dry));
    }
    else if (data.chkWet) {
        wrapper.setWetDry(xobj::AttrWetDry(xobj::AttrWetDry::wet));
    }
    else {
        wrapper.setWetDry(xobj::AttrWetDry());
    }
    //-------------------------------------------------------------------------
    wrapper.setTexture(sts::toMbString(data.strDiffuse));
    wrapper.setTextureLit(sts::toMbString(data.strLit));
    wrapper.setTextureNormal(sts::toMbString(data.strBump));
    //-------------------------------------------------------------------------
}

void MainObject::loadRawExpOption(sts_bwc::DataStream & stream) const {
    bcw::SerializationId id;
    id.load(stream.getStdStream());
    if (id != bcw::SerializationId(0xf34588f, 0x1863592b)) {
        LError << "Unexpected data input: " << id.toString();
        return;
    }
    unsigned char version;
    stream >> version;
    if (version != 1) {
        LError << "Unexpected data version: " << version;
        return;
    }

    //--------------------------
    uint64_t flags = 0;
    double scale = 0.0;
    stream >> flags;
    stream >> scale;
    //--------------------------
    {
        bcw::SerializationId idx;
        idx.load(stream.getStdStream());
        if (idx != bcw::SerializationId(0x3b0318d2, 0x7b7e3761)) {
            LError << "Unexpected data input: " << idx.toString();
            return;
        }
        unsigned char ver;
        stream >> ver;
        if (version != 1) {
            LError << "Unexpected data version: " << version;
            return;
        }

        int32_t xVersion = 0;
        stream >> xVersion;

        std::string signature;
        uint64_t oFlags = 0;
        stream >> oFlags;
        stream >> signature;

        enum {
            EXP_NO_OPT = 0,
            EXP_MARK_MESH = 1 << 0,
            EXP_MARK_LINE = 1 << 1,
            EXP_MARK_DUMMY = 1 << 2,
            EXP_MARK_LIGHT = 1 << 3,
            EXP_MARK_TREE = 1 << 4,
            EXP_MARK_COMMENTS = 1 << 5,
            EXP_MARK_TRANSFORM = 1 << 6,
            EXP_MARK_VERTEX = 1 << 7,
            EXP_OBJ_MESH = 1 << 11,
            EXP_OBJ_LINE = 1 << 12,
            EXP_OBJ_DUMMY = 1 << 13,
            EXP_OBJ_LIGHT = 1 << 14,
            EXP_ANIMATION = 1 << 21,
            EXP_OPTIMIZATION = 1 << 22,
            EXP_CHECK_INSTANCE = 1 << 23,
            EXP_DEBUG = 1 << 24,
        };

        if (mExpPb) {
            MainObjParamsWrapper wrapper(mAttrParamsPb, mExpPb, TimeValue(0), FOREVER);
            //-------------------------------------------------------------------------
            wrapper.setMeshExport((oFlags & EXP_OBJ_MESH) == EXP_OBJ_MESH);
            wrapper.setLinesExport(false);
            wrapper.setLightsExport((oFlags & EXP_OBJ_LIGHT) == EXP_OBJ_LIGHT);
            wrapper.setAnimationExport((oFlags & EXP_ANIMATION) == EXP_ANIMATION);
            //-------------------------------------------------------------------------
            wrapper.setNameMesh((oFlags & EXP_MARK_MESH) == EXP_MARK_MESH);
            wrapper.setNameLines((oFlags & EXP_MARK_LINE) == EXP_MARK_LINE);
            wrapper.setNameLights((oFlags & EXP_MARK_LIGHT) == EXP_MARK_LIGHT);
            wrapper.setNameDummies((oFlags & EXP_MARK_DUMMY) == EXP_MARK_DUMMY);
            wrapper.setTreeHierarchy((oFlags & EXP_MARK_TREE) == EXP_MARK_TREE);
            //-------------------------------------------------------------------------
            if (!stsff::math::isEqual(scale, 1.0)) {
                wrapper.setScale(true, float(scale));
            }
            //-------------------------------------------------------------------------
        }
    }
}

void MainObject::load186(std::vector<char> & inByteArray) {
    LMessage << "Object: <" << sts::toMbString(GetObjectName()) << "> has got data from previous version.";
    std::stringbuf buf(std::string(reinterpret_cast<char*>(inByteArray.data()), inByteArray.size()));
    sts_bwc::DataStream stream(buf);

    const auto pos = stream.getStdStream().tellg();
    uint8_t version;
    stream >> version; // 64 >= when incorrect version
    stream.getStdStream().seekg(pos);
    if (version != 0) {
        loadRawExpOption(stream);
        loadRawGlobAttr(stream);
    }
    else {
        uint8_t ver;
        stream >> ver;
        loadRawExpOption(stream);
        loadRawGlobAttr(stream);
        loadMdDisplayObj(stream);
    }
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
