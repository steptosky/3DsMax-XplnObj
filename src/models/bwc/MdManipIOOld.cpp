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

#include "MdManipIOOld.h"
#include "common/Logger.h"
#include "models/bwc/eClassesOld.h"
#include "gup/ObjCommon.h"
#include "common/String.h"
#include "SerializationId.h"
#include "stsu_data_stream.h" // backward compatibility
#include "models/io/NodeIO.h"

namespace bcw {
// backward compatibility

#define MdManipClassId SerializationId(0x24b2011e, 0x30017957)

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

// id from 1.8.5 version
enum eObjManipTypes {
    OBJ_MANIP_NONE = 0,
    OBJ_MANIP_DRAG_XY = 1 << 1,
    OBJ_MANIP_DRAG_AXIS = 1 << 2,
    OBJ_MANIP_COMMAND = 1 << 3,
    OBJ_MANIP_COMMAND_AXIS = 1 << 4,
    OBJ_MANIP_NOOP = 1 << 5,
    OBJ_MANIP_PUSH = 1 << 6,
    OBJ_MANIP_RADIO = 1 << 7,
    OBJ_MANIP_TOGGLE = 1 << 8,
    OBJ_MANIP_DELTA = 1 << 9,
    OBJ_MANIP_WRAP = 1 << 10,
    OBJ_MANIP_DRAG_AXIS_PIX = 1 << 11,
};

/**************************************************************************************************/
/////////////////////////* FROM X-PLANE LIBRARY, BACKWARD COMPATIBILITY *///////////////////////////
/**************************************************************************************************/

// id from 1.8.6 version
namespace xpln_bwc {
    // from x-plane library, backward compatibility

    /***************************************************************************************/

    enum class eManipType {
        NONE = 0,
        NO_MANIP = 1,
        DRAG_XY = 2,
        DRAG_AXIS = 3,
        COMMAND = 4,
        COMMAND_AXIS = 5,
        NOOP = 6,
        PUSH = 7,
        RADIO = 8,
        TOGGLE = 9,
        DELTA = 10,
        WRAP = 11,
        DRAG_AXIS_PIX = 12,
    };

    enum class eCursorType : int32_t {
        NONE,
        FOUR_ARROWS,
        HAND,
        BUTTON,
        ROTATE_SMALL,
        ROTATE_SMALL_LEFT,
        ROTATE_SMALL_RIGHT,
        ROTATE_MEDIUM,
        ROTATE_MEDIUM_LEFT,
        ROTATE_MEDIUM_RIGHT,
        ROTATE_LARGE,
        ROTATE_LARGE_LEFT,
        ROTATE_LARGE_RIGHT,
        UP_DOWN,
        DOWN,
        UP,
        LEFT_RIGHT,
        RIGHT,
        LEFT,
        ARROW,
    };

    std::map<eCursorType, std::string> gCursor({
            {eCursorType::NONE, "none"},
            {eCursorType::FOUR_ARROWS, "four_arrows"},
            {eCursorType::HAND, "hand"},
            {eCursorType::BUTTON, "button"},
            {eCursorType::ROTATE_SMALL, "rotate_small"},
            {eCursorType::ROTATE_SMALL_LEFT, "rotate_small_left"},
            {eCursorType::ROTATE_SMALL_RIGHT, "rotate_small_right"},
            {eCursorType::ROTATE_MEDIUM, "rotate_medium"},
            {eCursorType::ROTATE_MEDIUM_LEFT, "rotate_medium_left"},
            {eCursorType::ROTATE_MEDIUM_RIGHT, "rotate_medium_right"},
            {eCursorType::ROTATE_LARGE, "rotate_large"},
            {eCursorType::ROTATE_LARGE_LEFT, "rotate_large_left"},
            {eCursorType::ROTATE_LARGE_RIGHT, "rotate_large_right"},
            {eCursorType::UP_DOWN, "up_down"},
            {eCursorType::DOWN, "down"},
            {eCursorType::UP, "up"},
            {eCursorType::LEFT_RIGHT, "left_right"},
            {eCursorType::RIGHT, "right"},
            {eCursorType::LEFT, "left"},
            {eCursorType::ARROW, "arrow"}
    });

    /**************************************************************************************************/
    //////////////////////////////////////////* Static area *///////////////////////////////////////////
    /**************************************************************************************************/

    inline eManipType manipIdOldAssociation(uint64_t oldId) {
        switch (oldId) {
            case OBJ_MANIP_NONE: return eManipType::NONE;
            case OBJ_MANIP_DRAG_XY: return eManipType::DRAG_XY;
            case OBJ_MANIP_DRAG_AXIS: return eManipType::DRAG_AXIS;
            case OBJ_MANIP_COMMAND: return eManipType::COMMAND;
            case OBJ_MANIP_COMMAND_AXIS: return eManipType::COMMAND_AXIS;
            case OBJ_MANIP_NOOP: return eManipType::NOOP;
            case OBJ_MANIP_PUSH: return eManipType::PUSH;
            case OBJ_MANIP_RADIO: return eManipType::RADIO;
            case OBJ_MANIP_TOGGLE: return eManipType::TOGGLE;
            case OBJ_MANIP_DELTA: return eManipType::DELTA;
            case OBJ_MANIP_WRAP: return eManipType::WRAP;
            case OBJ_MANIP_DRAG_AXIS_PIX: return eManipType::DRAG_AXIS_PIX;
            default: return eManipType::NONE;
        }
    }

    /**************************************************************************************************/
    //////////////////////////////////////////* Static area *///////////////////////////////////////////
    /**************************************************************************************************/

    struct AbstractCommand {

        AbstractCommand(INode * inNode)
            : node(inNode) {}

        virtual ~AbstractCommand() = default;

        INode * node;
        std::string mCustomData;
        std::vector<std::string> mValues;
        bool mIsActive = false;

        virtual SerializationId getSerializationId() const = 0;
        virtual uint8_t getVersion() const = 0;

        virtual bool load(sts_bwc::DataStreamI & stream) {
            SerializationId id;
            id.load(stream.getStdIStream());
            if (id != getSerializationId()) {
                log_unexpected_data(node, id);
                return false;
            }

            uint8_t version;
            stream >> version;
            if (version != getVersion()) {
                log_unexpected_version(node, version);
                return false;
            }

            stream >> mCustomData;
            stream >> mIsActive;

            uint8_t size = 0;
            stream >> size;
            if (size) {
                mValues.resize(static_cast<size_t>(size));
                for (auto & curr : mValues) {
                    stream >> curr;
                }
            }
            else {
                mValues.clear();
            }

            return true;
        }
    };

    /***************************************************************************************/

    struct ObjManipStateCmd : AbstractCommand {

        ObjManipStateCmd(INode * inNode)
            : AbstractCommand(inNode) {}

        virtual ~ObjManipStateCmd() = default;

        SerializationId getSerializationId() const override {
            return SerializationId(0x39250e56, 0x1ce9182e);
        }

        uint8_t getVersion() const override {
            return 2;
        }

        bool load(sts_bwc::DataStreamI & stream) override {
            std::streampos begPos = stream.getStdIStream().tellg();

            SerializationId id1;
            id1.load(stream.getStdIStream());
            if (id1 != getSerializationId()) {
                log_unexpected_data(node, id1);
                return false;
            }

            unsigned char version;
            stream >> version;
            if (version == 1) {
                //--------------------------------------------
                // Certain manip data (version 1)
                std::string str;
                stream >> str;
                mManip.setCmd(str);
                //--------------------------------------------
                // From parent class (ObjAbstractManip sts x-plane lib 0.1.0)
                SerializationId id;
                id.load(stream.getStdIStream());
                if (id != SerializationId(0x538a3874, 0x19b23b35)) {
                    log_unexpected_data(node, id);
                    return false;
                }
                unsigned char versionOld;
                stream >> versionOld;
                if (versionOld != 1) {
                    log_unexpected_version(node, versionOld);
                    return false;
                }

                int32_t ct = 0;
                stream >> ct;
                mManip.setCursor(xobj::ECursor::fromString(gCursor[static_cast<eCursorType>(ct)].c_str()));
                stream >> str;
                mManip.setToolTip(str);
                return true;
                //--------------------------------------------
            }
            //--------------------------------------------
            // Certain manip data (version 2)
            mValues.resize(3);
            stream.getStdIStream().seekg(begPos);
            if (! AbstractCommand::load(stream)) {
                return false;
            }
            mManip.setCursor(xobj::ECursor::fromString(mValues[0].c_str()));
            mManip.setCmd(mValues[1]);
            mManip.setToolTip(mValues[2]);
            return true;
            //--------------------------------------------
        }

        xobj::AttrManipCmd mManip;
    };

    /***************************************************************************************/

    struct ObjManipStateCmdAxis : AbstractCommand {

        ObjManipStateCmdAxis(INode * inNode)
            : AbstractCommand(inNode) {}

        virtual ~ObjManipStateCmdAxis() = default;

        SerializationId getSerializationId() const override {
            return SerializationId(0x7eba769d, 0x5b8c4e29);
        }

        uint8_t getVersion() const override {
            return 2;
        }

        bool load(sts_bwc::DataStreamI & stream) override {
            std::streampos begPos = stream.getStdIStream().tellg();

            SerializationId id1;
            id1.load(stream.getStdIStream());
            if (id1 != getSerializationId()) {
                log_unexpected_data(node, id1);
                return false;
            }

            unsigned char version;
            stream >> version;

            if (version == 1) {
                //--------------------------------------------
                // Certain manip data (version 1)
                float val = 0.0f;
                stream >> val;
                mManip.setDirectionX(val);
                stream >> val;
                mManip.setDirectionY(val);
                stream >> val;
                mManip.setDirectionZ(val);

                std::string str;
                stream >> str;
                mManip.setCmdNegative(str);
                stream >> str;
                mManip.setCmdPositive(str);
                //--------------------------------------------
                // From parent class (ObjAbstractManip sts x-plane lib 0.1.0)
                SerializationId id;
                id.load(stream.getStdIStream());
                if (id != SerializationId(0x538a3874, 0x19b23b35)) {
                    log_unexpected_data(node, id);
                    return false;
                }

                unsigned char versionOld;
                stream >> versionOld;
                if (versionOld != 1) {
                    log_unexpected_version(node, versionOld);
                    return false;
                }

                int32_t ct = 0;
                stream >> ct;
                mManip.setCursor(xobj::ECursor::fromString(gCursor[static_cast<eCursorType>(ct)].c_str()));
                stream >> str;
                mManip.setToolTip(str);
                return true;
                //--------------------------------------------
            }
            //--------------------------------------------
            // Certain manip data (version 2)
            mValues.resize(7);
            stream.getStdIStream().seekg(begPos);
            if (!AbstractCommand::load(stream)) {
                return false;
            }
            mManip.setCursor(xobj::ECursor::fromString(mValues[0].c_str()));
            mManip.setDirectionX(sts::toFloat(mValues[1]));
            mManip.setDirectionY(sts::toFloat(mValues[2]));
            mManip.setDirectionZ(sts::toFloat(mValues[3]));
            mManip.setCmdNegative(mValues[5]);
            mManip.setCmdPositive(mValues[4]);
            mManip.setToolTip(mValues[6]);
            return true;
            //--------------------------------------------
        }

        xobj::AttrManipCmdAxis mManip;
    };

    /***************************************************************************************/

    struct ObjManipStateDelta : AbstractCommand {

        ObjManipStateDelta(INode * inNode)
            : AbstractCommand(inNode) {}

        virtual ~ObjManipStateDelta() = default;

        SerializationId getSerializationId() const override {
            return SerializationId(0x2380734b, 0x589a2189);
        }

        uint8_t getVersion() const override {
            return 2;
        }

        bool load(sts_bwc::DataStreamI & stream) override {
            std::streampos begPos = stream.getStdIStream().tellg();

            SerializationId id1;
            id1.load(stream.getStdIStream());
            if (id1 != getSerializationId()) {
                log_unexpected_data(node, id1);
                return false;
            }

            unsigned char version;
            stream >> version;

            if (version == 1) {
                //--------------------------------------------
                // Certain manip data (version 1)
                float val = 0.0f;
                stream >> val;
                mManip.setDown(val);
                stream >> val;
                mManip.setHold(val);
                stream >> val;
                mManip.setMinimum(val);
                stream >> val;
                mManip.setMaximum(val);
                std::string str;
                stream >> str;
                mManip.setDataref(str);
                //--------------------------------------------
                // From parent class (ObjAbstractManip sts x-plane lib 0.1.0)
                SerializationId id;
                id.load(stream.getStdIStream());
                if (id != SerializationId(0x538a3874, 0x19b23b35)) {
                    log_unexpected_data(node, id);
                    return false;
                }

                unsigned char versionOld;
                stream >> versionOld;
                if (versionOld != 1) {
                    log_unexpected_version(node, versionOld);
                    return false;
                }

                int32_t ct = 0;
                stream >> ct;
                mManip.setCursor(xobj::ECursor::fromString(gCursor[static_cast<eCursorType>(ct)].c_str()));
                stream >> str;
                mManip.setToolTip(str);
                return true;
                //--------------------------------------------
            }
            //--------------------------------------------
            // Certain manip data (version 2)
            mValues.resize(7);
            stream.getStdIStream().seekg(begPos);
            if (!AbstractCommand::load(stream)) {
                return false;
            }
            mManip.setCursor(xobj::ECursor::fromString(mValues[0].c_str()));
            mManip.setDown(sts::toFloat(mValues[1]));
            mManip.setHold(sts::toFloat(mValues[2]));
            mManip.setMinimum(sts::toFloat(mValues[3]));
            mManip.setMaximum(sts::toFloat(mValues[4]));
            mManip.setDataref(mValues[5]);
            mManip.setToolTip(mValues[6]);
            return true;
            //--------------------------------------------
        }

        xobj::AttrManipDelta mManip;
    };

    /***************************************************************************************/

    struct ObjManipStateDragAxis : AbstractCommand {

        ObjManipStateDragAxis(INode * inNode)
            : AbstractCommand(inNode) {}

        virtual ~ObjManipStateDragAxis() = default;

        SerializationId getSerializationId() const override {
            return SerializationId(0x12250d88, 0x40fa0648);
        }

        uint8_t getVersion() const override {
            return 2;
        }

        bool load(sts_bwc::DataStreamI & stream) override {
            std::streampos begPos = stream.getStdIStream().tellg();

            SerializationId id1;
            id1.load(stream.getStdIStream());
            if (id1 != getSerializationId()) {
                log_unexpected_data(node, id1);
                return false;
            }

            unsigned char version;
            stream >> version;

            if (version == 1) {
                //--------------------------------------------
                // Certain manip data (version 1)
                float val = 0.0f;
                stream >> val;
                mManip.setDirectionX(val);
                stream >> val;
                mManip.setDirectionY(val);
                stream >> val;
                mManip.setDirectionZ(val);
                stream >> val;
                mManip.setVal1(val);
                stream >> val;
                mManip.setVal2(val);
                std::string str;
                stream >> str;
                mManip.setDataref(str);
                //--------------------------------------------
                // From parent class (ObjAbstractManip sts x-plane lib 0.1.0)
                SerializationId id;
                id.load(stream.getStdIStream());
                if (id != SerializationId(0x538a3874, 0x19b23b35)) {
                    log_unexpected_data(node, id);
                    return false;
                }

                unsigned char versionOld;
                stream >> versionOld;
                if (versionOld != 1) {
                    log_unexpected_version(node, versionOld);
                    return false;
                }

                int32_t ct = 0;
                stream >> ct;
                mManip.setCursor(xobj::ECursor::fromString(gCursor[static_cast<eCursorType>(ct)].c_str()));
                stream >> str;
                mManip.setToolTip(str);
                return true;
                //--------------------------------------------
            }
            //--------------------------------------------
            // Certain manip data (version 2)
            mValues.resize(8);
            stream.getStdIStream().seekg(begPos);
            if (!AbstractCommand::load(stream)) {
                return false;
            }
            mManip.setCursor(xobj::ECursor::fromString(mValues[0].c_str()));
            mManip.setDirectionX(sts::toFloat(mValues[1]));
            mManip.setDirectionY(sts::toFloat(mValues[2]));
            mManip.setDirectionZ(sts::toFloat(mValues[3]));
            mManip.setVal1(sts::toFloat(mValues[4]));
            mManip.setVal2(sts::toFloat(mValues[5]));
            mManip.setDataref(mValues[6]);
            mManip.setToolTip(mValues[7]);
            return true;
            //--------------------------------------------
        }

        xobj::AttrManipDragAxis mManip;
    };

    /***************************************************************************************/

    struct ObjManipStateDragAxisPix : AbstractCommand {

        ObjManipStateDragAxisPix(INode * inNode)
            : AbstractCommand(inNode) {}

        virtual ~ObjManipStateDragAxisPix() = default;

        SerializationId getSerializationId() const override {
            return SerializationId(0x724359c2, 0x2fbe3415);
        }

        uint8_t getVersion() const override {
            return 2;
        }

        bool load(sts_bwc::DataStreamI & stream) override {
            std::streampos begPos = stream.getStdIStream().tellg();

            SerializationId id1;
            id1.load(stream.getStdIStream());
            if (id1 != getSerializationId()) {
                log_unexpected_data(node, id1);
                return false;
            }

            unsigned char version;
            stream >> version;

            if (version == 1) {
                //--------------------------------------------
                // Certain manip data (version 1)
                float val = 0.0f;
                int val_int = 0;
                stream >> val_int;
                mManip.setDxPix(val_int);
                stream >> val_int;
                mManip.setStep(val_int);

                stream >> val;
                mManip.setExp(val);
                stream >> val;
                mManip.setVal1(val);
                stream >> val;
                mManip.setVal2(val);
                std::string str;
                stream >> str;
                mManip.setDataref(str);
                //--------------------------------------------
                // From parent class (ObjAbstractManip sts x-plane lib 0.1.0)
                SerializationId id;
                id.load(stream.getStdIStream());
                if (id != SerializationId(0x538a3874, 0x19b23b35)) {
                    log_unexpected_data(node, id);
                    return false;
                }

                unsigned char versionOld;
                stream >> versionOld;
                if (versionOld != 1) {
                    log_unexpected_version(node, versionOld);
                    return false;
                }

                int32_t ct = 0;
                stream >> ct;
                mManip.setCursor(xobj::ECursor::fromString(gCursor[static_cast<eCursorType>(ct)].c_str()));
                stream >> str;
                mManip.setToolTip(str);
                return true;
                //--------------------------------------------
            }
            //--------------------------------------------
            // Certain manip data (version 2)
            mValues.resize(8);
            stream.getStdIStream().seekg(begPos);
            if (!AbstractCommand::load(stream)) {
                return false;
            }
            mManip.setCursor(xobj::ECursor::fromString(mValues[0].c_str()));
            mManip.setDxPix(sts::toInt32(mValues[1]));
            mManip.setStep(sts::toInt32(mValues[2]));
            mManip.setExp(sts::toFloat(mValues[3]));
            mManip.setVal1(sts::toFloat(mValues[4]));
            mManip.setVal2(sts::toFloat(mValues[5]));
            mManip.setDataref(mValues[6]);
            mManip.setToolTip(mValues[7]);
            return true;
            //--------------------------------------------
        }

        xobj::AttrManipDragAxisPix mManip;
    };

    /***************************************************************************************/

    struct ObjManipStateDragXy : AbstractCommand {

        ObjManipStateDragXy(INode * inNode)
            : AbstractCommand(inNode) {}

        virtual ~ObjManipStateDragXy() = default;

        SerializationId getSerializationId() const override {
            return SerializationId(0x6a432bb5, 0x66433cdd);
        }

        uint8_t getVersion() const override {
            return 2;
        }

        bool load(sts_bwc::DataStreamI & stream) override {
            std::streampos begPos = stream.getStdIStream().tellg();

            SerializationId id1;
            id1.load(stream.getStdIStream());
            if (id1 != getSerializationId()) {
                log_unexpected_data(node, id1);
                return false;
            }

            unsigned char version;
            stream >> version;

            if (version == 1) {
                //--------------------------------------------
                // Certain manip data (version 1)
                float val = 0.0f;
                stream >> val;
                mManip.setX(val);
                stream >> val;
                mManip.setXMin(val);
                stream >> val;
                mManip.setXMax(val);
                std::string str;
                stream >> str;
                mManip.setXDataref(str);

                stream >> val;
                mManip.setY(val);
                stream >> val;
                mManip.setYMin(val);
                stream >> val;
                mManip.setYMax(val);
                stream >> str;
                mManip.setYDataref(str);
                //--------------------------------------------
                // From parent class (ObjAbstractManip sts x-plane lib 0.1.0)
                SerializationId id;
                id.load(stream.getStdIStream());
                if (id != SerializationId(0x538a3874, 0x19b23b35)) {
                    log_unexpected_data(node, id);
                    return false;
                }

                unsigned char versionOld;
                stream >> versionOld;
                if (versionOld != 1) {
                    log_unexpected_version(node, versionOld);
                    return false;
                }

                int32_t ct = 0;
                stream >> ct;
                mManip.setCursor(xobj::ECursor::fromString(gCursor[static_cast<eCursorType>(ct)].c_str()));
                stream >> str;
                mManip.setToolTip(str);
                return true;
                //--------------------------------------------
            }
            //--------------------------------------------
            // Certain manip data (version 2)
            mValues.resize(10);
            stream.getStdIStream().seekg(begPos);
            if (!AbstractCommand::load(stream)) {
                return false;
            }
            mManip.setCursor(xobj::ECursor::fromString(mValues[0].c_str()));
            mManip.setX(sts::toFloat(mValues[1]));
            mManip.setXMin(sts::toFloat(mValues[3]));
            mManip.setXMax(sts::toFloat(mValues[4]));
            mManip.setXDataref(mValues[7]);
            mManip.setY(sts::toFloat(mValues[2]));
            mManip.setYMin(sts::toFloat(mValues[5]));
            mManip.setYMax(sts::toFloat(mValues[6]));
            mManip.setYDataref(mValues[8]);
            mManip.setToolTip(mValues[9]);
            return true;
            //--------------------------------------------
        }

        xobj::AttrManipDragXy mManip;
    };

    /***************************************************************************************/

    struct ObjManipStateNoop : AbstractCommand {

        ObjManipStateNoop(INode * inNode)
            : AbstractCommand(inNode) {}

        virtual ~ObjManipStateNoop() = default;

        SerializationId getSerializationId() const override {
            return SerializationId(0x4163c9, 0x5ba32071);
        }

        uint8_t getVersion() const override {
            return 2;
        }

        bool load(sts_bwc::DataStreamI & stream) override {
            std::streampos begPos = stream.getStdIStream().tellg();

            SerializationId id1;
            id1.load(stream.getStdIStream());
            if (id1 != getSerializationId()) {
                log_unexpected_data(node, id1);
                return false;
            }

            unsigned char versionOld;
            stream >> versionOld;
            if (versionOld != 1) {
                log_unexpected_version(node, versionOld);
                return false;
            }

            //--------------------------------------------
            // Certain manip data (version 2)
            mValues.resize(10);
            stream.getStdIStream().seekg(begPos);
            return AbstractCommand::load(stream);
            //--------------------------------------------
        }

        xobj::AttrManipNoop mManip;
    };

    /***************************************************************************************/

    struct ObjManipStatePush : AbstractCommand {

        ObjManipStatePush(INode * inNode)
            : AbstractCommand(inNode) {}

        virtual ~ObjManipStatePush() = default;

        SerializationId getSerializationId() const override {
            return SerializationId(0x1c7853e2, 0x7107139d);
        }

        uint8_t getVersion() const override {
            return 2;
        }

        bool load(sts_bwc::DataStreamI & stream) override {
            std::streampos begPos = stream.getStdIStream().tellg();

            SerializationId id1;
            id1.load(stream.getStdIStream());
            if (id1 != getSerializationId()) {
                log_unexpected_data(node, id1);
                return false;
            }

            unsigned char version;
            stream >> version;

            if (version == 1) {
                //--------------------------------------------
                // Certain manip data (version 1)
                float val = 0.0f;
                stream >> val;
                mManip.setDown(val);
                stream >> val;
                mManip.setUp(val);
                std::string str;
                stream >> str;
                mManip.setDataref(str);
                //--------------------------------------------
                // From parent class (ObjAbstractManip sts x-plane lib 0.1.0)
                SerializationId id;
                id.load(stream.getStdIStream());
                if (id != SerializationId(0x538a3874, 0x19b23b35)) {
                    log_unexpected_data(node, id);
                    return false;
                }

                unsigned char versionOld;
                stream >> versionOld;
                if (versionOld != 1) {
                    log_unexpected_version(node, versionOld);
                    return false;
                }

                int32_t ct = 0;
                stream >> ct;
                mManip.setCursor(xobj::ECursor::fromString(gCursor[static_cast<eCursorType>(ct)].c_str()));
                stream >> str;
                mManip.setToolTip(str);
                return true;
                //--------------------------------------------
            }
            //--------------------------------------------
            // Certain manip data (version 2)
            mValues.resize(5);
            stream.getStdIStream().seekg(begPos);
            if (!AbstractCommand::load(stream)) {
                return false;
            }
            mManip.setCursor(xobj::ECursor::fromString(mValues[0].c_str()));
            mManip.setDown(sts::toFloat(mValues[1]));
            mManip.setUp(sts::toFloat(mValues[2]));
            mManip.setDataref(mValues[3]);
            mManip.setToolTip(mValues[4]);
            return true;
            //--------------------------------------------
        }

        xobj::AttrManipPush mManip;
    };

    /***************************************************************************************/

    struct ObjManipStateRadio : AbstractCommand {

        ObjManipStateRadio(INode * inNode)
            : AbstractCommand(inNode) {}

        virtual ~ObjManipStateRadio() = default;

        SerializationId getSerializationId() const override {
            return SerializationId(0x3a122473, 0x3ca50c7f);
        }

        uint8_t getVersion() const override {
            return 2;
        }

        bool load(sts_bwc::DataStreamI & stream) override {
            std::streampos begPos = stream.getStdIStream().tellg();

            SerializationId id1;
            id1.load(stream.getStdIStream());
            if (id1 != getSerializationId()) {
                log_unexpected_data(node, id1);
                return false;
            }

            unsigned char version;
            stream >> version;

            if (version == 1) {
                //--------------------------------------------
                // Certain manip data (version 1)
                float val = 0.0f;
                stream >> val;
                mManip.setDown(val);
                std::string str;
                stream >> str;
                mManip.setDataref(str);
                //--------------------------------------------
                // From parent class (ObjAbstractManip sts x-plane lib 0.1.0)
                SerializationId id;
                id.load(stream.getStdIStream());
                if (id != SerializationId(0x538a3874, 0x19b23b35)) {
                    log_unexpected_data(node, id);
                    return false;
                }

                unsigned char versionOld;
                stream >> versionOld;
                if (versionOld != 1) {
                    log_unexpected_version(node, versionOld);
                    return false;
                }

                int32_t ct = 0;
                stream >> ct;
                mManip.setCursor(xobj::ECursor::fromString(gCursor[static_cast<eCursorType>(ct)].c_str()));
                stream >> str;
                mManip.setToolTip(str);
                return true;
                //--------------------------------------------
            }
            //--------------------------------------------
            // Certain manip data (version 2)
            mValues.resize(4);
            stream.getStdIStream().seekg(begPos);
            if (!AbstractCommand::load(stream)) {
                return false;
            }
            mManip.setCursor(xobj::ECursor::fromString(mValues[0].c_str()));
            mManip.setDown(sts::toFloat(mValues[1]));
            mManip.setDataref(mValues[2]);
            mManip.setToolTip(mValues[3]);
            return true;
            //--------------------------------------------
        }

        xobj::AttrManipRadio mManip;
    };

    /***************************************************************************************/

    struct ObjManipStateToggle : AbstractCommand {

        ObjManipStateToggle(INode * inNode)
            : AbstractCommand(inNode) {}

        virtual ~ObjManipStateToggle() = default;

        SerializationId getSerializationId() const override {
            return SerializationId(0x2d4c1fa6, 0xba4248);
        }

        uint8_t getVersion() const override {
            return 2;
        }

        bool load(sts_bwc::DataStreamI & stream) override {
            std::streampos begPos = stream.getStdIStream().tellg();

            SerializationId id1;
            id1.load(stream.getStdIStream());
            if (id1 != getSerializationId()) {
                log_unexpected_data(node, id1);
                return false;
            }

            unsigned char version;
            stream >> version;

            if (version == 1) {
                //--------------------------------------------
                // Certain manip data (version 1)
                float val = 0.0f;
                stream >> val;
                mManip.setOn(val);
                stream >> val;
                mManip.setOff(val);
                std::string str;
                stream >> str;
                mManip.setDataref(str);
                //--------------------------------------------
                // From parent class (ObjAbstractManip sts x-plane lib 0.1.0)
                SerializationId id;
                id.load(stream.getStdIStream());
                if (id != SerializationId(0x538a3874, 0x19b23b35)) {
                    log_unexpected_data(node, id);
                    return false;
                }

                unsigned char versionOld;
                stream >> versionOld;
                if (versionOld != 1) {
                    log_unexpected_version(node, versionOld);
                    return false;
                }

                int32_t ct = 0;
                stream >> ct;
                mManip.setCursor(xobj::ECursor::fromString(gCursor[static_cast<eCursorType>(ct)].c_str()));
                stream >> str;
                mManip.setToolTip(str);
                return true;
                //--------------------------------------------
            }
            //--------------------------------------------
            // Certain manip data (version 2)
            mValues.resize(5);
            stream.getStdIStream().seekg(begPos);
            if (!AbstractCommand::load(stream)) {
                return false;
            }
            mManip.setCursor(xobj::ECursor::fromString(mValues[0].c_str()));
            mManip.setOn(sts::toFloat(mValues[1]));
            mManip.setOff(sts::toFloat(mValues[2]));
            mManip.setDataref(mValues[3]);
            mManip.setToolTip(mValues[4]);
            return true;
            //--------------------------------------------
        }

        xobj::AttrManipToggle mManip;
    };

    /***************************************************************************************/

    struct ObjManipStateWrap : AbstractCommand {

        ObjManipStateWrap(INode * inNode)
            : AbstractCommand(inNode) {}

        virtual ~ObjManipStateWrap() = default;

        SerializationId getSerializationId() const override {
            return SerializationId(0x2d4c1fa6, 0xba4248);
        }

        uint8_t getVersion() const override {
            return 2;
        }

        bool load(sts_bwc::DataStreamI & stream) override {
            std::streampos begPos = stream.getStdIStream().tellg();

            SerializationId id1;
            id1.load(stream.getStdIStream());
            if (id1 != getSerializationId()) {
                log_unexpected_data(node, id1);
                return false;
            }

            unsigned char version;
            stream >> version;

            if (version == 1) {
                //--------------------------------------------
                // Certain manip data (version 1)
                float val = 0.0f;
                stream >> val;
                mManip.setDown(val);
                stream >> val;
                mManip.setHold(val);
                stream >> val;
                mManip.setMinimum(val);
                stream >> val;
                mManip.setMaximum(val);
                std::string str;
                stream >> str;
                mManip.setDataref(str);
                //--------------------------------------------
                // From parent class (ObjAbstractManip sts x-plane lib 0.1.0)
                SerializationId id;
                id.load(stream.getStdIStream());
                if (id != SerializationId(0x538a3874, 0x19b23b35)) {
                    log_unexpected_data(node, id);
                    return false;
                }

                unsigned char versionOld;
                stream >> versionOld;
                if (versionOld != 1) {
                    log_unexpected_version(node, versionOld);
                    return false;
                }

                int32_t ct = 0;
                stream >> ct;
                mManip.setCursor(xobj::ECursor::fromString(gCursor[static_cast<eCursorType>(ct)].c_str()));
                stream >> str;
                mManip.setToolTip(str);
                return true;
                //--------------------------------------------
            }
            //--------------------------------------------
            // Certain manip data (version 2)
            mValues.resize(7);
            stream.getStdIStream().seekg(begPos);
            if (!AbstractCommand::load(stream)) {
                return false;
            }
            mManip.setCursor(xobj::ECursor::fromString(mValues[0].c_str()));
            mManip.setDown(sts::toFloat(mValues[1]));
            mManip.setHold(sts::toFloat(mValues[2]));
            mManip.setMinimum(sts::toFloat(mValues[3]));
            mManip.setMaximum(sts::toFloat(mValues[4]));
            mManip.setDataref(mValues[5]);
            mManip.setToolTip(mValues[6]);
            return true;
            //--------------------------------------------
        }

        xobj::AttrManipWrap mManip;
    };

    /***************************************************************************************/

}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

AppDataChunk * MdManipIoOld::data(INode * node) {
    return NodeIO::dataChunk(node, static_cast<DWORD>(bwc::MANIP_DATA_ID));
}

void MdManipIoOld::removeDataFromNode(INode * node) {
    NodeIO::removeData(node, static_cast<DWORD>(bwc::MANIP_DATA_ID));
}

bool MdManipIoOld::loadFromNode(INode * node, AppDataChunk * data, ManipIO::IManipIo * inCallback) {
    if (data->length == 0) {
        log_unexpected_data_length(node, data->length);
        inCallback->gotNoManip();
        return false;
    }
    std::stringstream buf(std::string(reinterpret_cast<const char *>(data->data), data->length));
    sts_bwc::DataStream stream(buf);

    SerializationId id;
    id.load(stream.getStdStream());
    if (id != MdManipClassId) {
        log_unexpected_data(node, id);
        inCallback->gotNoManip();
        return false;
    }

    xpln_bwc::eManipType currManip;
    unsigned char version;
    stream >> version;
    if (version == 1) {
        uint64_t cm = 0;
        stream >> cm;
        currManip = xpln_bwc::manipIdOldAssociation(cm);
        if (currManip == xpln_bwc::eManipType::NONE) {
            inCallback->gotNoManip();
            return true;
        }
    }
    else if (version == 2) {
        uint64_t cm = 0;
        stream >> cm;
        currManip = static_cast<xpln_bwc::eManipType>(cm);
        if (currManip == xpln_bwc::eManipType::NONE) {
            inCallback->gotNoManip();
            return true;
        }
    }
    else {
        log_unexpected_version(node, version);
        inCallback->gotNoManip();
        return false;
    }

    //-------------------------------------------------------------------------

    switch (currManip) {
        case xpln_bwc::eManipType::NONE: {
            inCallback->gotNoManip();
            return true;
        }
        case xpln_bwc::eManipType::COMMAND: {
            xpln_bwc::ObjManipStateCmd oldManip(node);
            if (!oldManip.load(stream)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(oldManip.mManip);
            break;
        }
        case xpln_bwc::eManipType::COMMAND_AXIS: {
            xpln_bwc::ObjManipStateCmdAxis oldManip(node);
            if (!oldManip.load(stream)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(oldManip.mManip);
            break;
        }
        case xpln_bwc::eManipType::DELTA: {
            xpln_bwc::ObjManipStateDelta oldManip(node);
            if (!oldManip.load(stream)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(oldManip.mManip);
            break;
        }
        case xpln_bwc::eManipType::DRAG_AXIS: {
            xpln_bwc::ObjManipStateDragAxis oldManip(node);
            if (!oldManip.load(stream)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(oldManip.mManip);
            break;
        }
        case xpln_bwc::eManipType::DRAG_AXIS_PIX: {
            xpln_bwc::ObjManipStateDragAxisPix oldManip(node);
            if (!oldManip.load(stream)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(oldManip.mManip);
            break;
        }
        case xpln_bwc::eManipType::DRAG_XY: {
            xpln_bwc::ObjManipStateDragXy oldManip(node);
            if (!oldManip.load(stream)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(oldManip.mManip);
            break;
        }
        case xpln_bwc::eManipType::NOOP: {
            xpln_bwc::ObjManipStateNoop oldManip(node);
            if (!oldManip.load(stream)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(oldManip.mManip);
            break;
        }
        case xpln_bwc::eManipType::PUSH: {
            xpln_bwc::ObjManipStatePush oldManip(node);
            if (!oldManip.load(stream)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(oldManip.mManip);
            break;
        }
        case xpln_bwc::eManipType::RADIO: {
            xpln_bwc::ObjManipStateRadio oldManip(node);
            if (!oldManip.load(stream)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(oldManip.mManip);
            break;
        }
        case xpln_bwc::eManipType::TOGGLE: {
            xpln_bwc::ObjManipStateToggle oldManip(node);
            if (!oldManip.load(stream)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(oldManip.mManip);
            break;
        }
        case xpln_bwc::eManipType::WRAP: {
            xpln_bwc::ObjManipStateWrap oldManip(node);
            if (!oldManip.load(stream)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(oldManip.mManip);
            break;
        }
        case xpln_bwc::eManipType::NO_MANIP:
        default:
            inCallback->gotNoManip();
            return false;
    }

    //-------------------------------------------------------------------------

    return true;
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

}
