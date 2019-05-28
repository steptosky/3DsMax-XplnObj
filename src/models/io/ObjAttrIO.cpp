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

#include "ObjAttrIO.h"
#include "additional/stsio/DataStream.h"
#include "common/String.h"
#include "common/Logger.h"
#include "NodeIO.h"
#include "models/eDataId.h"
#include "models/bwc/MdObjAttrIOOld.h"
#include "classes-desc/ClassesDescriptions.h"

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool ObjAttrIO::canApply(INode * node) {
    assert(node);
    const Class_ID id = node->GetObjectRef()->ClassID();
    if (id == ClassesDescriptions::mainObj()->ClassID() || id == ClassesDescriptions::lodObj()->ClassID()) {
        return false;
    }
    SClass_ID sid = node->GetObjectRef()->SuperClassID();
    if (sid == GEOMOBJECT_CLASS_ID) {
        return true;
    }
    sid = node->EvalWorldState(GetCOREInterface()->GetTime()).obj->SuperClassID();
    return sid == GEOMOBJECT_CLASS_ID;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void ObjAttrIO::save(sts::DataStreamO & stream, const xobj::AttrSet & attrSet) {
    stream.setValue<uint8_t>(uint8_t(1)); // manip io version

    // the method was changed from isSunLight() to isTree(), that is why it is inverted.
    stream.setValue<bool>(!attrSet.mIsTree);

    stream.setValue<bool>(attrSet.mIsTwoSided);
    stream.setValue<bool>(attrSet.mIsDraw);
    stream.setValue<bool>(attrSet.mIsDraped);
    stream.setValue<bool>(attrSet.mIsCastShadow);
    stream.setValue<bool>(attrSet.mIsSolidForCamera);
}

bool ObjAttrIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrSet & outAttr) {
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }

    // the method was changed from setSunLight() to setTree(), that is why it is inverted.
    outAttr.mIsTree = !stream.value<bool>();

    outAttr.mIsTwoSided = stream.value<bool>();
    outAttr.mIsDraw = stream.value<bool>();
    outAttr.mIsDraped = stream.value<bool>();
    outAttr.mIsCastShadow = stream.value<bool>();
    outAttr.mIsSolidForCamera = stream.value<bool>();
    return true;
}

/***************************************************************************************/

void ObjAttrIO::save(sts::DataStreamO & stream, const std::optional<xobj::AttrPolyOffset> & attr) {
    stream.setValue<uint8_t>(uint8_t(1)); // io version
    const xobj::AttrPolyOffset actual(attr.value_or(xobj::AttrPolyOffset()));
    stream.setValue<float>(actual.mOffset);
    stream.setValue<bool>(attr.has_value());
}

bool ObjAttrIO::loadPolyOffset(INode * node, sts::DataStreamI & stream, xobj::AttrSet & outAttr) {
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    xobj::AttrPolyOffset out(stream.value<float>());
    outAttr.mPolyOffset = stream.value<bool>() ? std::optional(out) : std::nullopt;
    return true;
}

/***************************************************************************************/

void ObjAttrIO::save(sts::DataStreamO & stream, const std::optional<xobj::AttrHard> & attr) {
    stream.setValue<uint8_t>(uint8_t(1)); // io version
    const xobj::AttrHard actual(attr.value_or(xobj::AttrHard()));
    stream.setValue<std::string>(actual.mSurface.toString());
    stream.setValue<bool>(actual.mIsDeck);
    stream.setValue<bool>(attr.has_value());
}

bool ObjAttrIO::loadHard(INode * node, sts::DataStreamI & stream, xobj::AttrSet & outAttr) {
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    std::string str;
    stream.value<std::string>(str);

    xobj::AttrHard out(xobj::ESurface::fromString(str.c_str()), stream.value<bool>());
    outAttr.mHard = stream.value<bool>() ? std::optional(out) : std::nullopt;
    return true;
}

/***************************************************************************************/

void ObjAttrIO::save(sts::DataStreamO & stream, const std::optional<xobj::AttrShiny> & attr) {
    stream.setValue<uint8_t>(uint8_t(1)); // io version
    const xobj::AttrShiny actual(attr.value_or(xobj::AttrShiny()));
    stream.setValue<float>(actual.mRatio);
    stream.setValue<bool>(attr.has_value());
}

bool ObjAttrIO::loadShiny(INode * node, sts::DataStreamI & stream, xobj::AttrSet & outAttr) {
    const auto version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    xobj::AttrShiny out(stream.value<float>());
    outAttr.mShiny = stream.value<bool>() ? std::optional(out) : std::nullopt;
    return true;
}

/***************************************************************************************/

void ObjAttrIO::save(sts::DataStreamO & stream, const std::optional<xobj::AttrBlend> & attr) {
    stream.setValue<uint8_t>(uint8_t(1)); // io version
    const xobj::AttrBlend actual(attr.value_or(xobj::AttrBlend()));
    stream.setValue<uint8_t>(actual.mType);
    stream.setValue<float>(actual.mRatio);
    stream.setValue<bool>(attr.has_value());
}

bool ObjAttrIO::loadBlend(INode * node, sts::DataStreamI & stream, xobj::AttrSet & outAttr) {
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    const auto type = static_cast<xobj::AttrBlend::eType>(stream.value<uint8_t>());
    xobj::AttrBlend out(type, stream.value<float>());
    outAttr.mBlend = stream.value<bool>() ? std::optional(out) : std::nullopt;
    return true;
}

/***************************************************************************************/

void ObjAttrIO::save(sts::DataStreamO & stream, const std::optional<xobj::AttrLightLevel> & attr) {
    stream.setValue<uint8_t>(uint8_t(1)); // io version
    const xobj::AttrLightLevel actual(attr.value_or(xobj::AttrLightLevel()));
    stream.setValue<float>(actual.mVal1);
    stream.setValue<float>(actual.mVal2);
    stream.setValue<std::string>(actual.mDataref);
    stream.setValue<bool>(attr.has_value());
}

bool ObjAttrIO::loadLightLevel(INode * node, sts::DataStreamI & stream, xobj::AttrSet & outAttr) {
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    xobj::AttrLightLevel out;
    out.mVal1 = stream.value<float>();
    out.mVal2 = stream.value<float>();
    out.mDataref = stream.value<std::string>();
    outAttr.mLightLevel = stream.value<bool>() ? std::optional(out) : std::nullopt;
    return true;
}

/***************************************************************************************/

void ObjAttrIO::save(sts::DataStreamO & stream, const std::optional<xobj::AttrCockpit> & attr) {
    stream.setValue<uint8_t>(uint8_t(1)); // io version
    const xobj::AttrCockpit actual(attr.value_or(xobj::AttrCockpit()));
    stream.setValue<uint8_t>(actual.mType);
    stream.setValue<bool>(attr.has_value());
}

bool ObjAttrIO::loadCockpit(INode * node, sts::DataStreamI & stream, xobj::AttrSet & outAttr) {
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    xobj::AttrCockpit out(static_cast<xobj::AttrCockpit::eType>(stream.value<uint8_t>()));
    outAttr.mCockpit = stream.value<bool>() ? std::optional(out) : std::nullopt;
    return true;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool ObjAttrIO::cloneData(INode * NodeFrom, INode * NodeTo) {
    if (!canApply(NodeTo)) {
        return false;
    }
    NodeIO::cloneData(NodeFrom, NodeTo, static_cast<DWORD>(eAttribuesIOID::OBJ_ATTRIBUTES));
    return true;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool ObjAttrIO::loadFromNode(INode * inNode, xobj::AttrSet & outAttrSet) {
    outAttrSet.reset();
    if (!inNode) {
        XLError << "INode is nullptr";
        return false;
    }

    AppDataChunk * chunk = NodeIO::dataChunk(inNode, static_cast<DWORD>(eAttribuesIOID::OBJ_ATTRIBUTES));
    if (!chunk) {
        chunk = bcw::MdObjAttrIOOld::data(inNode);
        if (!chunk) {
            return true;
        }
        if (bcw::MdObjAttrIOOld::loadFromNode(inNode, chunk, outAttrSet)) {
            saveToNode(inNode, outAttrSet);
            bcw::MdObjAttrIOOld::removeFromNode(inNode);
            return true;
        }
        return false;
    }

    if (chunk->length == 0) {
        log_unexpected_data_length(inNode, chunk->length);
        return false;
    }

    //-------------------------------------------------------------------------

    std::stringstream buf(std::string(reinterpret_cast<const char *>(chunk->data), chunk->length));
    sts::DataStream stream(&buf);
    stream.readAndSetFormatVersion();
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(inNode, version);
        return false;
    }
    return load(inNode, stream, outAttrSet) &&
           loadPolyOffset(inNode, stream, outAttrSet) &&
           loadHard(inNode, stream, outAttrSet) &&
           loadShiny(inNode, stream, outAttrSet) &&
           loadBlend(inNode, stream, outAttrSet) &&
           loadLightLevel(inNode, stream, outAttrSet) &&
           loadCockpit(inNode, stream, outAttrSet);
}

void ObjAttrIO::removeFromNode(INode * node) {
    NodeIO::removeData(node, static_cast<DWORD>(eAttribuesIOID::OBJ_ATTRIBUTES));
}

void ObjAttrIO::saveToNode(INode * outNode, const xobj::AttrSet & inAttrSet) {
    if (!outNode) {
        XLError << "INode is nullptr";
        return;
    }

    std::stringstream buf;
    sts::DataStream stream(&buf);
    stream.writeFormatVersion();
    stream.setValue<uint8_t>(uint8_t(1)); // node io version
    save(stream, inAttrSet);
    save(stream, inAttrSet.mPolyOffset);
    save(stream, inAttrSet.mHard);
    save(stream, inAttrSet.mShiny);
    save(stream, inAttrSet.mBlend);
    save(stream, inAttrSet.mLightLevel);
    save(stream, inAttrSet.mCockpit);
    NodeIO::saveData(outNode, static_cast<DWORD>(eAttribuesIOID::OBJ_ATTRIBUTES), buf.str());
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
