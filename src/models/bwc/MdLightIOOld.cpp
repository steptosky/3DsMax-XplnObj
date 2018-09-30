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

#include "MdLightIOOld.h"
#include "models/bwc/eClassesOld.h"
#include "models/io/NodeIO.h"
#include "SerializationId.h"
#include "common/String.h"
#include "stsu_data_stream.h"

namespace bcw {
// backward compatibility

#define MdLightIOOldClassId SerializationId(0x42ac7391, 0x41e71cc5)

/**************************************************************************************************/
/////////////////////////* FROM X-PLANE LIBRARY, BACKWARD COMPATIBILITY *///////////////////////////
/**************************************************************************************************/

// id from 1.8.5, 1.8.6 version
namespace xpln_bwc {
    // from x-plane library, backward compatibility

    enum eObjectType {
        OBJ_NO = 0,
        //!< Not correct object
        OBJ_MESH = 1 << 1,
        //!< Geometric mesh object
        OBJ_LINE = 1 << 2,
        //!< Geometric line object
        OBJ_LIGHT = 1 << 3,
        //!< Light any
        OBJ_LIGHT_NAMED = 1 << 4 | OBJ_LIGHT,
        //!< Light named
        OBJ_LIGHT_CUSTOM = 1 << 5 | OBJ_LIGHT,
        //!< Light custom
        OBJ_LIGHT_PARAM = 1 << 6 | OBJ_LIGHT,
        //!< Light param
        OBJ_LIGHT_SPILL_CUSTOM = 1 << 7 | OBJ_LIGHT,
        //!< Light spill custom
        OBJ_DUMMY = 1 << 8,
        //!< Dummy object (For example: can be used as animation point)
        OBJ_LOD = 1 << 9,
        //!< Lod object
    };

}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

struct ObjLightCustom {

    ObjLightCustom(INode * inNode)
        : node(inNode) {}

    static SerializationId getSerializationId() {
        return SerializationId(0x24482140, 0x591f6edd);
    }

    bool load(sts_bwc::DataStreamI & stream) {
        SerializationId id;
        id.load(stream.getStdIStream());
        if (id != getSerializationId()) {
            log_unexpected_data(node, id);
            LError << "Incorrect input data";
            return false;
        }
        unsigned char version;
        stream >> version;
        if (version != 1) {
            log_unexpected_version(node, version);
            return false;
        }

        double mR;
        double mG;
        double mB;
        double mA;

        double mSize;

        double mS1;
        double mT1;
        double mS2;
        double mT2;

        std::string mDataref;

        stream >> mR;
        stream >> mG;
        stream >> mB;
        stream >> mA;

        stream >> mSize;

        stream >> mS1;
        stream >> mT1;
        stream >> mS2;
        stream >> mT2;

        stream >> mDataref;

        mLight.setColor(xobj::Color(float(mR), float(mG), float(mB), float(mA)));
        mLight.setSize(float(mSize));
        mLight.setDataRef(mDataref);
        mLight.setTextureRect(xobj::RectangleI(xobj::Point2(float(mS1), float(mT1)),
                                               xobj::Point2(float(mS2), float(mT2))));
        return true;
    }

    INode * node;
    xobj::ObjLightCustom mLight;
};

/***************************************************************************************/

struct ObjLightNamed {

    ObjLightNamed(INode * inNode)
        : node(inNode) {}

    static SerializationId getSerializationId() {
        return SerializationId(0x48146d91, 0x63ab3cd8);
    }

    bool load(sts_bwc::DataStreamI & stream) {
        SerializationId id;
        id.load(stream.getStdIStream());
        if (id != getSerializationId()) {
            log_unexpected_data(node, id);
            return false;
        }
        unsigned char version;
        stream >> version;
        if (version != 1) {
            log_unexpected_version(node, version);
            return false;
        }
        std::string mName;
        stream >> mName;
        mLight.setName(mName);
        return true;
    }

    INode * node;
    xobj::ObjLightNamed mLight;
};

/***************************************************************************************/

struct ObjLightParam {

    ObjLightParam(INode * inNode)
        : node(inNode) {}

    static SerializationId getSerializationId() {
        return SerializationId(0x409e7410, 0x7298509d);
    }

    bool load(sts_bwc::DataStreamI & stream) {
        SerializationId id;
        id.load(stream.getStdIStream());
        if (id != getSerializationId()) {
            log_unexpected_data(node, id);
            return false;
        }
        unsigned char version;
        stream >> version;
        if (version != 1) {
            log_unexpected_version(node, version);
            return false;
        }

        bool mIsCustom;
        std::string mName;
        std::string mAdditional;

        stream >> mIsCustom;
        stream >> mName;
        stream >> mAdditional;

        mLight.setName(mName);
        mLight.setParams(mAdditional);
        return true;
    }

    INode * node;
    xobj::ObjLightParam mLight;
};

/***************************************************************************************/

struct ObjLightSpillCust {

    ObjLightSpillCust(INode * inNode)
        : node(inNode) {}

    static SerializationId getSerializationId() {
        return SerializationId(0x221649f2, 0x66062b3b);
    }

    bool load(sts_bwc::DataStreamI & stream) {
        SerializationId id;
        id.load(stream.getStdIStream());
        if (id != getSerializationId()) {
            log_unexpected_data(node, id);
            return false;
        }
        unsigned char version;
        stream >> version;
        if (version != 1) {
            log_unexpected_version(node, version);
            return false;
        }

        double mR;
        double mG;
        double mB;
        double mA;
        double mSize;
        std::string mDataref;

        stream >> mR;
        stream >> mG;
        stream >> mB;
        stream >> mA;
        stream >> mSize;
        stream >> mDataref;

        mLight.setColor(xobj::Color(float(mR), float(mG), float(mB), float(mA)));
        mLight.setSize(float(mSize));;
        mLight.setDataRef(mDataref);
        return true;
    }

    INode * node;
    xobj::ObjLightSpillCust mLight;
};

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

AppDataChunk * MdLightIOOld::data(INode * node) {
    return NodeIO::dataChunk(node, static_cast<DWORD>(bwc::LIGHT_DATA_ID));
}

void MdLightIOOld::removeDataFromNode(INode * node) {
    NodeIO::removeData(node, static_cast<DWORD>(bwc::LIGHT_DATA_ID));
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool MdLightIOOld::loadFromNode(INode * node, AppDataChunk * data, LightIO::ILightIO * inCallback) {
    if (data->length == 0) {
        log_unexpected_data_length(node, data->length);
        inCallback->gotNoLight();
        return false;
    }
    std::stringstream buf(std::string(reinterpret_cast<const char *>(data->data), data->length));
    sts_bwc::DataStream stream(buf);

    SerializationId id;
    id.load(stream.getStdStream());
    if (id != MdLightIOOldClassId) {
        log_unexpected_data(node, id);
        inCallback->gotNoLight();
        return false;
    }

    unsigned char version;
    stream >> version;
    if (version != 1) {
        log_unexpected_version(node, version);
        inCallback->gotNoLight();
        return false;
    }

    uint64_t cm = 0;
    stream >> cm;
    xpln_bwc::eObjectType currLightType = static_cast<xpln_bwc::eObjectType>(cm);
    if (currLightType == xpln_bwc::OBJ_NO) {
        inCallback->gotNoLight();
        return true;
    }

    switch (currLightType) {
        case xpln_bwc::OBJ_LIGHT_NAMED: {
            ObjLightNamed oldLight(node);
            if (!oldLight.load(stream)) {
                inCallback->gotNoLight();
                return false;
            }
            inCallback->gotLight(oldLight.mLight);
            break;
        }
        case xpln_bwc::OBJ_LIGHT_CUSTOM: {
            ObjLightCustom oldLight(node);
            if (!oldLight.load(stream)) {
                inCallback->gotNoLight();
                return false;
            }
            inCallback->gotLight(oldLight.mLight);
            break;
        }
        case xpln_bwc::OBJ_LIGHT_PARAM: {
            ObjLightParam oldLight(node);
            if (!oldLight.load(stream)) {
                inCallback->gotNoLight();
                return false;
            }
            inCallback->gotLight(oldLight.mLight);
            break;
        }
        case xpln_bwc::OBJ_LIGHT_SPILL_CUSTOM: {
            ObjLightSpillCust oldLight(node);
            if (!oldLight.load(stream)) {
                inCallback->gotNoLight();
                return false;
            }
            inCallback->gotLight(oldLight.mLight);
            break;
        };
        default: inCallback->gotNoLight();
    }
    return true;
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

}
