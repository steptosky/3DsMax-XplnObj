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

#include "LightIO.h"
#include <sstream>
#include "models/eDataId.h"
#include "gup/ObjCommon.h"
#include "common/String.h"
#include "NodeIO.h"
#include "models/bwc/MdLightIOOld.h"

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

namespace bcw {
// backward compatibility
// It is used for migration from previous data
struct LightGetter : LightIO::ILightIO {

    virtual ~LightGetter() {
        delete mObj;
    }

    void gotLight(const xobj::ObjLightCustom & inLight) override {
        mObj = inLight.clone();
    }

    void gotLight(const xobj::ObjLightNamed & inLight) override {
        mObj = inLight.clone();
    }

    void gotLight(const xobj::ObjLightParam & inLight) override {
        mObj = inLight.clone();
    }

    void gotLight(const xobj::ObjLightPoint & inLight) override {
        mObj = inLight.clone();
    }

    void gotLight(const xobj::ObjLightSpillCust & inLight) override {
        mObj = inLight.clone();
    }

    void gotNoLight() override {
        delete mObj;
        mObj = nullptr;
    }

    xobj::ObjAbstract * mObj = nullptr;

};
}

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

void LightIO::save(sts::DataStreamO & stream, const xobj::ObjLightCustom & inLight) {
    stream.setValue<uint8_t>(uint8_t(1)); // io version

    stream.setValue<float>(inLight.size());
    stream.setValue<std::string>(inLight.dataRef());

    const xobj::Color & color = inLight.color();
    stream.setValue<float>(color.red());
    stream.setValue<float>(color.green());
    stream.setValue<float>(color.blue());
    stream.setValue<float>(color.alpha());

    const xobj::RectangleI & rect = inLight.textureRect();
    stream.setValue<float>(rect.point1().x);
    stream.setValue<float>(rect.point1().y);
    stream.setValue<float>(rect.point2().x);
    stream.setValue<float>(rect.point2().y);
}

bool LightIO::load(INode * node, sts::DataStreamI & stream, xobj::ObjLightCustom & outLight) {
    uint8_t version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }

    outLight.setSize(stream.value<float>());
    outLight.setDataRef(stream.value<std::string>());

    xobj::Color c;
    c.setRed(stream.value<float>());
    c.setGreen(stream.value<float>());
    c.setBlue(stream.value<float>());
    c.setAlpha(stream.value<float>());
    outLight.setColor(c);

    xobj::Point2 p1;
    p1.x = stream.value<float>();
    p1.y = stream.value<float>();
    xobj::Point2 p2;
    p2.x = stream.value<float>();
    p2.y = stream.value<float>();
    outLight.setTextureRect(xobj::RectangleI(p1, p2));
    return true;
}

/***************************************************************************************/

void LightIO::save(sts::DataStreamO & stream, const xobj::ObjLightNamed & inLight) {
    stream.setValue<uint8_t>(uint8_t(1)); // io version
    stream.setValue<std::string>(inLight.name());
}

bool LightIO::load(INode * node, sts::DataStreamI & stream, xobj::ObjLightNamed & outLight) {
    uint8_t version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outLight.setName(stream.value<std::string>());
    return true;
}

/***************************************************************************************/

void LightIO::save(sts::DataStreamO & stream, const xobj::ObjLightParam & inLight) {
    stream.setValue<uint8_t>(uint8_t(2)); // io version
    stream.setValue<std::string>(inLight.name());
    stream.setValue<std::string>(inLight.params());
}

bool LightIO::load(INode * node, sts::DataStreamI & stream, xobj::ObjLightParam & outLight) {
    const std::uint8_t version = stream.value<std::uint8_t>();
    if (version == 2) {
        outLight.setName(stream.value<std::string>());
        outLight.setRawParams(stream.value<std::string>());
        return true;
    }

    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }

    const auto lightId = stream.value<std::string>();
    auto lightName = stream.value<std::string>();
    if (lightName.empty()) {
        lightName = lightId;
    }
    outLight.setName(lightName);
    outLight.setRawParams(stream.value<std::string>());
    return true;
}

/***************************************************************************************/

void LightIO::save(sts::DataStreamO & stream, const xobj::ObjLightPoint & inLight) {
    stream.setValue<uint8_t>(uint8_t(1)); // io version

    const xobj::Color & color = inLight.color();
    stream.setValue<float>(color.red());
    stream.setValue<float>(color.green());
    stream.setValue<float>(color.blue());
    stream.setValue<float>(color.alpha());
}

bool LightIO::load(INode * node, sts::DataStreamI & stream, xobj::ObjLightPoint & outLight) {
    uint8_t version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    xobj::Color c;
    c.setRed(stream.value<float>());
    c.setGreen(stream.value<float>());
    c.setBlue(stream.value<float>());
    c.setAlpha(stream.value<float>());
    outLight.setColor(c);
    return true;
}

/***************************************************************************************/

void LightIO::save(sts::DataStreamO & stream, const xobj::ObjLightSpillCust & inLight) {
    stream.setValue<uint8_t>(uint8_t(1)); // io version

    stream.setValue<float>(inLight.size());
    stream.setValue<std::string>(inLight.dataRef());

    const xobj::Color & color = inLight.color();
    stream.setValue<float>(color.red());
    stream.setValue<float>(color.green());
    stream.setValue<float>(color.blue());
    stream.setValue<float>(color.alpha());
}

bool LightIO::load(INode * node, sts::DataStreamI & stream, xobj::ObjLightSpillCust & outLight) {
    uint8_t version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }

    outLight.setSize(stream.value<float>());
    outLight.setDataRef(stream.value<std::string>());

    xobj::Color c;
    c.setRed(stream.value<float>());
    c.setGreen(stream.value<float>());
    c.setBlue(stream.value<float>());
    c.setAlpha(stream.value<float>());
    outLight.setColor(c);
    return true;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool LightIO::canApply(INode * node) {
    return (node->GetObjectRef()->SuperClassID() == LIGHT_CLASS_ID);
}

bool LightIO::cloneData(INode * NodeFrom, INode * NodeTo) {
    if (!canApply(NodeTo)) {
        return false;
    }
    NodeIO::cloneData(NodeFrom, NodeTo, static_cast<DWORD>(eLightIOID::LIGHT));
    return true;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void LightIO::saveDataToNode(INode * outNode, const std::stringstream & indata) {
    NodeIO::saveData(outNode, static_cast<DWORD>(eLightIOID::LIGHT), indata.str());
}

void LightIO::removeFromNode(INode * node) {
    NodeIO::removeData(node, static_cast<DWORD>(eLightIOID::LIGHT));
}

bool LightIO::loadFromNode(INode * inNode, ILightIO * inCallback) {
    if (!inNode || !inCallback) {
        LError << "One or more of the input parameters is nullptr";
        return false;
    }

    AppDataChunk * chunk = NodeIO::dataChunk(inNode, static_cast<DWORD>(eLightIOID::LIGHT));
    if (!chunk) {
        chunk = bcw::MdLightIOOld::data(inNode);
        if (!chunk) {
            inCallback->gotNoLight();
            return true;
        }
        if (bcw::MdLightIOOld::loadFromNode(inNode, chunk, inCallback)) {
            bcw::LightGetter gettr;
            bcw::MdLightIOOld::loadFromNode(inNode, chunk, &gettr);
            if (gettr.mObj) {
                saveToNode(inNode, gettr.mObj);
                bcw::MdLightIOOld::removeDataFromNode(inNode);
            }
            return true;
        }
        return false;
    }

    if (chunk->length == 0) {
        log_unexpected_data_length(inNode, chunk->length);
        inCallback->gotNoLight();
        return false;
    }

    //-------------------------------------------------------------------------

    std::stringstream buf(std::string(reinterpret_cast<const char *>(chunk->data), chunk->length));
    sts::DataStream stream(&buf);
    stream.readAndSetFormatVersion();
    uint8_t version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(inNode, version);
        inCallback->gotNoLight();
        return false;
    }

    //-------------------------------------------------------------------------

    xobj::eObjectType id(static_cast<xobj::eObjectType>(stream.value<int32_t>()));
    switch (id) {
        case xobj::eObjectType::OBJ_NO: {
            inCallback->gotNoLight();
            return true;
        }
        case xobj::eObjectType::OBJ_LIGHT_CUSTOM: {
            xobj::ObjLightCustom light;
            if (!load(inNode, stream, light)) {
                inCallback->gotNoLight();
                return false;
            }
            inCallback->gotLight(light);
            break;
        }
        case xobj::eObjectType::OBJ_LIGHT_NAMED: {
            xobj::ObjLightNamed light;
            if (!load(inNode, stream, light)) {
                inCallback->gotNoLight();
                return false;
            }
            inCallback->gotLight(light);
            break;
        }
        case xobj::eObjectType::OBJ_LIGHT_PARAM: {
            xobj::ObjLightParam light;
            if (!load(inNode, stream, light)) {
                inCallback->gotNoLight();
                return false;
            }
            inCallback->gotLight(light);
            break;
        }
        case xobj::eObjectType::OBJ_LIGHT_POINT: {
            xobj::ObjLightPoint light;
            if (!load(inNode, stream, light)) {
                inCallback->gotNoLight();
                return false;
            }
            inCallback->gotLight(light);
            break;
        }
        case xobj::eObjectType::OBJ_LIGHT_SPILL_CUSTOM: {
            xobj::ObjLightSpillCust light;
            if (!load(inNode, stream, light)) {
                inCallback->gotNoLight();
                return false;
            }
            inCallback->gotLight(light);
            break;
        }
        default: return false;
    }

    //-------------------------------------------------------------------------

    return true;
}

void LightIO::saveToNode(INode * outNode, const xobj::ObjAbstract * inLight) {
    if (inLight == nullptr) {
        removeFromNode(outNode);
        return;
    }
    switch (inLight->objType()) {
        case xobj::eObjectType::OBJ_LIGHT_CUSTOM:
            saveToNodeInternal(outNode, *static_cast<const xobj::ObjLightCustom *>(inLight));
            break;
        case xobj::eObjectType::OBJ_LIGHT_NAMED:
            saveToNodeInternal(outNode, *static_cast<const xobj::ObjLightNamed *>(inLight));
            break;
        case xobj::eObjectType::OBJ_LIGHT_PARAM:
            saveToNodeInternal(outNode, *static_cast<const xobj::ObjLightParam *>(inLight));
            break;
        case xobj::eObjectType::OBJ_LIGHT_POINT:
            saveToNodeInternal(outNode, *static_cast<const xobj::ObjLightPoint *>(inLight));
            break;
        case xobj::eObjectType::OBJ_LIGHT_SPILL_CUSTOM:
            saveToNodeInternal(outNode, *static_cast<const xobj::ObjLightSpillCust *>(inLight));
            break;
        default:
            LError << LogNode(outNode) << "has got incorrect x-plane object.";
            removeFromNode(outNode);
            break;
    }
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
