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

#include "MdAnimIoOld.h"
#include "common/Logger.h"
#include "gup/ObjCommon.h"
#include "models/io/NodeIO.h"
#include "models/bwc/eClassesOld.h"
#include "SerializationId.h"
#include "models/bwc/stsu_data_stream.h"
#include "common/String.h"

#define MdAnimRotClassId SerializationId(0x74002ccb, 0xbc806e3)
#define MdAnimTransClassId SerializationId(0x41762f41, 0x9e869d0)
#define MdAnimVisClassId SerializationId(0x43243b67, 0x6f2a652b)

namespace bcw {
// backward compatibility

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool MdAnimIoOld::loadRotateFromNode(INode * inNode, AppDataChunk * data, MdAnimRot & outAnimRot) {
    outAnimRot.reset();
    if (!inNode) {
        LError << "Node is nullptr";
        return false;
    }

    if (data->length == 0) {
        log_unexpected_data_length(inNode, data->length);
        return false;
    }

    //-------------------------------------------------------------------------

    std::stringstream buf(std::string(reinterpret_cast<const char *>(data->data), data->length));
    sts_bwc::DataStreamI stream(buf);

    SerializationId id;
    id.load(stream.getStdIStream());
    if (id != MdAnimRotClassId) {
        log_unexpected_data(inNode, id);
        return false;
    }
    unsigned char version;
    stream >> version;
    if (version != 1) {
        log_unexpected_version(inNode, version);
        return false;
    }

    //-------------------------------------------------------------------------

    struct {
        std::string dataref;
        float loopValue;
        bool enable;
        bool reverse;
        bool loobEnable;
        std::vector<float> keyValueList;
    } animData;

    stream >> animData.dataref;
    stream >> animData.loopValue;
    stream >> animData.enable;
    stream >> animData.reverse;
    stream >> animData.loobEnable;

    unsigned size = 0;
    stream >> size;
    animData.keyValueList.resize(size, 0.0f);
    if (size) {
        stream.readRawData(reinterpret_cast<char*>(animData.keyValueList.data()), size * sizeof(float));
    }

    outAnimRot.mDataref = std::move(animData.dataref);
    outAnimRot.mLoopValue = animData.loopValue;
    outAnimRot.mEnable = animData.enable;
    outAnimRot.mReverse = animData.reverse;
    outAnimRot.mLoopEnable = animData.loobEnable;
    outAnimRot.mKeyList = std::move(animData.keyValueList);
    return true;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool MdAnimIoOld::loadTransFromNode(INode * inNode, AppDataChunk * data, MdAnimTrans & outAnimTrans) {
    outAnimTrans.reset();
    if (!inNode) {
        LError << "Node is nullptr";
        return false;
    }

    if (data->length == 0) {
        log_unexpected_data_length(inNode, data->length);
        return false;
    }

    //-------------------------------------------------------------------------

    std::stringstream buf(std::string(reinterpret_cast<const char *>(data->data), data->length));
    sts_bwc::DataStreamI stream(buf);

    SerializationId id;
    id.load(stream.getStdIStream());
    if (id != MdAnimTransClassId) {
        log_unexpected_data(inNode, id);
        return false;
    }
    unsigned char version;
    stream >> version;
    if (version != 1) {
        log_unexpected_version(inNode, version);
        return false;
    }

    //-------------------------------------------------------------------------

    struct AnimData {
        std::string dataref;
        float loopValue;
        bool enable;
        bool reverse;
        bool loobEnable;
        std::vector<float> keyValueList;
    } animData;

    stream >> animData.dataref;
    stream >> animData.loopValue;
    stream >> animData.enable;
    stream >> animData.reverse;
    stream >> animData.loobEnable;

    unsigned size = 0;
    stream >> size;
    animData.keyValueList.resize(size, 0.0f);
    if (size) {
        stream.readRawData(reinterpret_cast<char*>(animData.keyValueList.data()), size * sizeof(float));
    }

    outAnimTrans.mDataref = std::move(animData.dataref);
    outAnimTrans.mLoopValue = animData.loopValue;
    outAnimTrans.mEnable = animData.enable;
    outAnimTrans.mReverse = animData.reverse;
    outAnimTrans.mLoopEnable = animData.loobEnable;
    outAnimTrans.mKeyList = std::move(animData.keyValueList);
    return true;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool MdAnimIoOld::loadVisibilityFromNode(INode * inNode, AppDataChunk * data, MdAnimVis & outAnimVis) {
    outAnimVis.reset();
    if (!inNode) {
        LError << "Node is nullptr";
        return false;
    }

    if (data->length == 0) {
        log_unexpected_data_length(inNode, data->length);
        return false;
    }

    //-------------------------------------------------------------------------

    std::stringstream buf(std::string(reinterpret_cast<const char *>(data->data), data->length));
    sts_bwc::DataStream stream(buf);

    SerializationId id;
    id.load(stream.getStdStream());
    if (id != MdAnimVisClassId) {
        log_unexpected_data(inNode, id);
        return false;
    }
    unsigned char version;
    stream >> version;
    if (version != MdAnimVis::mVersion) {
        log_unexpected_version(inNode, version);
        return false;
    }

    struct AnimData {

        enum eType {
            UNDEFINED = 0,
            SHOW = 'S',
            HIDE = 'H'
        };

        struct Key {
            eType mType;
            float mValue1;
            float mValue2;
            std::string mDataref;
        };

        typedef std::vector<Key> KeyList;
        bool mEnable;
        KeyList mKeyList;
    } animData;

    stream >> animData.mEnable;

    unsigned size = 0;
    stream >> size;
    animData.mKeyList.resize(size);
    for (auto & curr : animData.mKeyList) {
        {
            // from sts x-xplane obj library 0.1.1
            std::streampos begPos = stream.getStdStream().tellg();
            unsigned char ver;
            stream.getStdStream().read(reinterpret_cast<char*>(&ver), 1);
            stream.getStdStream().seekg(begPos);
            if (ver > 1) {
                SerializationId id1;
                id1.load(stream.getStdStream());
                if (id1 != SerializationId(0x4d816bfe, 0x1b146c7)) {
                    log_unexpected_data(inNode, id1);
                    return false;
                }

                stream >> ver;
                if (ver != 2) {
                    log_unexpected_version(inNode, ver);
                    return false;
                }
            }
            else {
                stream >> ver;
                SerializationId id2;
                id2.load(stream.getStdStream());
                if (id2 != SerializationId(0x4d816bfe, 0x1b146c7)) {
                    log_unexpected_data(inNode, id2);
                    return false;
                }
            }

            char type = 0;
            stream >> type;
            curr.mType = static_cast<AnimData::eType>(type);
            stream >> curr.mValue1;
            stream >> curr.mValue2;
            stream >> curr.mDataref;
        }
    }
    outAnimVis.mEnable = animData.mEnable;
    outAnimVis.mKeyList.resize(size);
    for (size_t i = 0; i < outAnimVis.mKeyList.size(); ++i) {
        outAnimVis.mKeyList[i].pDrf = animData.mKeyList[i].mDataref;
        outAnimVis.mKeyList[i].pValue1 = animData.mKeyList[i].mValue1;
        outAnimVis.mKeyList[i].pValue2 = animData.mKeyList[i].mValue2;

        switch (animData.mKeyList[i].mType) {
            case AnimData::UNDEFINED: outAnimVis.mKeyList[i].pType = xobj::AnimVisibilityKey::UNDEFINED;
                break;
            case AnimData::SHOW: outAnimVis.mKeyList[i].pType = xobj::AnimVisibilityKey::SHOW;
                break;
            case AnimData::HIDE: outAnimVis.mKeyList[i].pType = xobj::AnimVisibilityKey::HIDE;
                break;
            default: break;
        }
    }
    return true;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void MdAnimIoOld::removeRotateFromNode(INode * node) {
    if (!node) {
        node->RemoveAppDataChunk(COMMON_CLASS_ID, GUP_CLASS_ID, static_cast<DWORD>(bwc::ANIM_DATA_ROTATE_ID));
    }
}

void MdAnimIoOld::removeAnimTransFromNode(INode * node) {
    if (!node) {
        node->RemoveAppDataChunk(COMMON_CLASS_ID, GUP_CLASS_ID, static_cast<DWORD>(bwc::ANIM_DATA_TRANS_ID));
    }
}

void MdAnimIoOld::removeVisibilityFromNode(INode * node) {
    if (!node) {
        node->RemoveAppDataChunk(COMMON_CLASS_ID, GUP_CLASS_ID, static_cast<DWORD>(bwc::ANIM_DATA_VIS_ID));
    }
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

AppDataChunk * MdAnimIoOld::dataRotate(INode * node) {
    return NodeIO::dataChunk(node, static_cast<DWORD>(bwc::ANIM_DATA_ROTATE_ID));
}

AppDataChunk * MdAnimIoOld::dataTrans(INode * node) {
    return NodeIO::dataChunk(node, static_cast<DWORD>(bwc::ANIM_DATA_TRANS_ID));
}

AppDataChunk * MdAnimIoOld::dataVisibility(INode * node) {
    return NodeIO::dataChunk(node, static_cast<DWORD>(bwc::ANIM_DATA_VIS_ID));
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
}
