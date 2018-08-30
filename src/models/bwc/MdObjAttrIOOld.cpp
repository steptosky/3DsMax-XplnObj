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

#include <sstream>

#include "MdObjAttrIOOld.h"
#include "models/bwc/eClassesOld.h"
#include "additional/stsio/DataStream.h"
#include "models/io/NodeIO.h"
#include "stsu_data_stream.h"
#include "common/String.h"
#include "SerializationId.h"
#include "common/Logger.h"
#include "ui/DlgMessageBox.h"
#include "classes-desc/ClassesDescriptions.h"

namespace bcw {
// backward compatibility

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool MdObjAttrIOOld::canApply(INode * node) {
    assert(node);
    Class_ID id = node->GetObjectRef()->ClassID();
    if (id == ClassesDescriptions::mainObj()->ClassID() ||
        id == ClassesDescriptions::lodObj()->ClassID())
        return false;
    SClass_ID sid = node->GetObjectRef()->SuperClassID();
    if (sid == GEOMOBJECT_CLASS_ID)
        return true;
    sid = node->EvalWorldState(GetCOREInterface()->GetTime()).obj->SuperClassID();
    if (sid == GEOMOBJECT_CLASS_ID)
        return true;
    return false;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool MdObjAttrIOOld::loadFromNode(INode * node, AppDataChunk * data, xobj::AttrSet & outAttrSet) {
    if (data->length == 0) {
        log_unexpected_data_length(node, data->length);
        return false;
    }
    std::stringstream buf(std::string(reinterpret_cast<const char *>(data->data), data->length));
    sts_bwc::DataStreamI stream(buf);

    SerializationId id;
    id.load(stream.getStdIStream());
    if (id != SerializationId(0x5bfc3adc, 0x53d74b02)) {
        log_unexpected_data(node, id);
        return false;
    }

    unsigned char version;
    stream >> version;
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }

    SerializationId id2;
    id2.load(stream.getStdIStream());
    if (id2 != SerializationId(0x5f96066e, 0x735c55a2)) {
        log_unexpected_data(node, id2);
        return false;
    }

    unsigned char version2;
    stream >> version2;
    if (version2 != 1) {
        log_unexpected_version(node, version2);
        return false;
    }

    struct {
        std::string mLightLevelDataref;
        double mLightLevelVal1;
        double mLightLevelVal2;

        double mNoBlendRatio;
        double mPolyOffsetDist;

        bool mChkNoBlend;
        bool mChkPolyOffset;
        bool mChkLightLevel;

        bool mChkDraped;
        bool mChkNoDepth;
        bool mChkShadeFlat;
        bool mChkNoShadow;
        bool mChkSolidCamera;
        bool mChkDrawDisable;
        bool mChkCockpit;
        bool mChkNoSunLight;
        bool mChkHard;
        bool mChkNoCull;

        bool mChkMatDiffuse;
        bool mChkMatEmission;
        bool mChkMatShiny;
    } d;

    stream >> d.mLightLevelDataref;
    stream >> d.mLightLevelVal1;
    stream >> d.mLightLevelVal2;

    stream >> d.mNoBlendRatio;
    stream >> d.mPolyOffsetDist;

    stream >> d.mChkNoBlend;
    stream >> d.mChkPolyOffset;
    stream >> d.mChkLightLevel;

    stream >> d.mChkDraped;
    stream >> d.mChkNoDepth;
    stream >> d.mChkShadeFlat;
    stream >> d.mChkNoShadow;
    stream >> d.mChkSolidCamera;
    stream >> d.mChkDrawDisable;
    stream >> d.mChkCockpit;
    stream >> d.mChkNoSunLight;
    stream >> d.mChkHard;
    stream >> d.mChkNoCull;

    stream >> d.mChkMatDiffuse;
    stream >> d.mChkMatEmission;
    stream >> d.mChkMatShiny;

    //-------------------------------------------------------------------------

    std::stringstream msgStream;
    if (d.mChkMatDiffuse) {
        msgStream << LogNode(node) << "Material diffuse is no longer supported" << LEOL;
    }

    if (d.mChkMatEmission) {
        msgStream << LogNode(node) << "Material emission is no longer supported" << LEOL;
    }

    if (d.mChkMatShiny) {
        msgStream << LogNode(node) << "Shiny without the value is no longer supported "
                << "and will be converted to shiny with 1.0" << std::endl;
        outAttrSet.setShiny(xobj::AttrShiny(1.0f));
    }

    if (d.mChkNoDepth) {
        msgStream << LogNode(node) << "No-depth is no longer supported" << LEOL;
    }

    if (d.mChkShadeFlat) {
        msgStream << LogNode(node) << "Shader smooth/flat is no longer supported" << LEOL;
    }

    if (d.mChkNoCull) {
        msgStream << LogNode(node) << "No-cull is no longer supported "
                << "and will be rewritten with \"Two sided\"" << LEOL;
        outAttrSet.setTwoSided(true);
    }

    std::string msg = msgStream.str();
    if (!msg.empty()) {
        ui::DlgMessageBox::warning(nullptr, "[Backward Compatibility]", msg);
        LWarning << msg;
    }

    //-------------------------------------------------------------------------

    if (d.mChkNoBlend) {
        outAttrSet.setBlend(xobj::AttrBlend(xobj::AttrBlend::no_blend, float(d.mNoBlendRatio)));
    }
    if (d.mChkPolyOffset) {
        outAttrSet.setPolyOffset(xobj::AttrPolyOffset(float(d.mPolyOffsetDist)));
    }
    if (d.mChkLightLevel) {
        outAttrSet.setLightLevel(xobj::AttrLightLevel(float(d.mLightLevelVal1),
                                                      float(d.mLightLevelVal2),
                                                      d.mLightLevelDataref));
    }
    if (d.mChkCockpit) {
        outAttrSet.setCockpit(xobj::AttrCockpit(xobj::AttrCockpit::cockpit));
    }
    if (d.mChkHard) {
        outAttrSet.setHard(xobj::AttrHard(xobj::ESurface()));
    }
    outAttrSet.setDraped(d.mChkDraped);
    outAttrSet.setCastShadow(!d.mChkNoShadow);
    outAttrSet.setSolidForCamera(d.mChkSolidCamera);
    outAttrSet.setDraw(!d.mChkDrawDisable);
    outAttrSet.setSunLight(!d.mChkNoSunLight);
    return true;
}

AppDataChunk * MdObjAttrIOOld::data(INode * node) {
    return NodeIO::dataChunk(node, static_cast<DWORD>(bwc::BASE_ATTR_DATA_ID));
}

void MdObjAttrIOOld::removeFromNode(INode * node) {
    NodeIO::removeData(node, static_cast<DWORD>(bwc::BASE_ATTR_DATA_ID));
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

}
