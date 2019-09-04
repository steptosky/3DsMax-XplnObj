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
#include <xpln/obj/ObjMesh.h>

#include "ManipIO.h"

#include "models/eDataId.h"
#include "gup/ObjCommon.h"
#include "common/String.h"
#include "models/bwc/MdManipIOOld.h"
#include "NodeIO.h"

/**************************************************************************************************/
/////////////////////////////////////////* Static area *////////////////////////////////////////////
/**************************************************************************************************/

namespace bcw {
// backward compatibility

// It is used for migration from previous data
struct ManipGetter : ManipIO::IManipIo {

    void gotAttrManip(const xobj::AttrManipAxisKnob & inManip) override {
        mAttr.mManip = inManip;
    }

    void gotAttrManip(const xobj::AttrManipAxisSwitchLeftRight & inManip) override {
        mAttr.mManip = inManip;
    }

    void gotAttrManip(const xobj::AttrManipAxisSwitchUpDown & inManip) override {
        mAttr.mManip = inManip;
    }

    void gotAttrManip(const xobj::AttrManipCmd & inManip) override {
        mAttr.mManip = inManip;
    }

    void gotAttrManip(const xobj::AttrManipCmdAxis & inManip) override {
        mAttr.mManip = inManip;
    }

    void gotAttrManip(const xobj::AttrManipCmdKnob & inManip) override {
        mAttr.mManip = inManip;
    }

    void gotAttrManip(const xobj::AttrManipCmdSwitchLeftRight & inManip) override {
        mAttr.mManip = inManip;
    }

    void gotAttrManip(const xobj::AttrManipCmdSwitchUpDown & inManip) override {
        mAttr.mManip = inManip;
    }

    void gotAttrManip(const xobj::AttrManipDelta & inManip) override {
        mAttr.mManip = inManip;
    }

    void gotAttrManip(const xobj::AttrManipDragAxis & inManip) override {
        mAttr.mManip = inManip;
    }

    void gotAttrManip(const xobj::AttrManipDragAxisPix & inManip) override {
        mAttr.mManip = inManip;
    }

    void gotAttrManip(const xobj::AttrManipDragXy & inManip) override {
        mAttr.mManip = inManip;
    }

    void gotAttrManip(const xobj::AttrManipNoop & inManip) override {
        mAttr.mManip = inManip;
    }

    void gotAttrManip(const xobj::AttrManipPanel & inManip) override {
        mAttr.mManip = inManip;
    }

    void gotAttrManip(const xobj::AttrManipPush & inManip) override {
        mAttr.mManip = inManip;
    }

    void gotAttrManip(const xobj::AttrManipRadio & inManip) override {
        mAttr.mManip = inManip;
    }

    void gotAttrManip(const xobj::AttrManipToggle & inManip) override {
        mAttr.mManip = inManip;
    }

    void gotAttrManip(const xobj::AttrManipWrap & inManip) override {
        mAttr.mManip = inManip;
    }

    void gotNoManip() override {
        mAttr.mManip = std::nullopt;
    }

    xobj::AttrSet mAttr;

};
}

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipAxisKnob & manip) {
    stream.setValue<std::uint8_t>(std::uint8_t(1)); // manip io version
    stream.setValue<std::int32_t>(manip.mCursor.id());
    stream.setValue<std::string>(manip.mToolType.str());
    stream.setValue<float>(manip.mClickDelta);
    stream.setValue<float>(manip.mHoldDelta);
    stream.setValue<float>(manip.mMin);
    stream.setValue<float>(manip.mMax);
    stream.setValue<bool>(manip.mWheel.has_value());
    stream.setValue<float>(manip.mWheel.value_or(xobj::AttrManipWheel()).mWheelDelta);
    stream.setValue<std::string>(manip.mDataref.str());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipAxisKnob & outManip) {
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.mCursor = xobj::ECursor(xobj::ECursor::eId(stream.value<std::int32_t>()));
    outManip.mToolType = stream.value<std::string>();
    outManip.mClickDelta = stream.value<float>();
    outManip.mHoldDelta = stream.value<float>();
    outManip.mMin = stream.value<float>();
    outManip.mMax = stream.value<float>();
    //-------------------
    const auto wheelEnabled = stream.value<bool>();
    xobj::AttrManipWheel wheel(stream.value<float>()); // we always have to read the value from the stream
    outManip.mWheel = wheelEnabled ? std::optional(wheel) : std::nullopt;
    //-------------------
    outManip.mDataref = stream.value<std::string>();
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipAxisSwitchLeftRight & manip) {
    stream.setValue<std::uint8_t>(std::uint8_t(1)); // manip io version
    stream.setValue<std::int32_t>(manip.mCursor.id());
    stream.setValue<std::string>(manip.mToolType.str());
    stream.setValue<float>(manip.mClickDelta);
    stream.setValue<float>(manip.mHoldDelta);
    stream.setValue<float>(manip.mMin);
    stream.setValue<float>(manip.mMax);
    stream.setValue<bool>(manip.mWheel.has_value());
    stream.setValue<float>(manip.mWheel.value_or(xobj::AttrManipWheel()).mWheelDelta);
    stream.setValue<std::string>(manip.mDataref.str());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipAxisSwitchLeftRight & outManip) {
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.mCursor = xobj::ECursor(xobj::ECursor::eId(stream.value<std::int32_t>()));
    outManip.mToolType = stream.value<std::string>();
    outManip.mClickDelta = stream.value<float>();
    outManip.mHoldDelta = stream.value<float>();
    outManip.mMin = stream.value<float>();
    outManip.mMax = stream.value<float>();
    //-------------------
    const auto wheelEnabled = stream.value<bool>();
    xobj::AttrManipWheel wheel(stream.value<float>()); // we always have to read the value from the stream
    outManip.mWheel = wheelEnabled ? std::optional(wheel) : std::nullopt;
    //-------------------
    outManip.mDataref = stream.value<std::string>();
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipAxisSwitchUpDown & manip) {
    stream.setValue<std::uint8_t>(std::uint8_t(1)); // manip io version
    stream.setValue<std::int32_t>(manip.mCursor.id());
    stream.setValue<std::string>(manip.mToolType.str());
    stream.setValue<float>(manip.mClickDelta);
    stream.setValue<float>(manip.mHoldDelta);
    stream.setValue<float>(manip.mMin);
    stream.setValue<float>(manip.mMax);
    stream.setValue<bool>(manip.mWheel.has_value());
    stream.setValue<float>(manip.mWheel.value_or(xobj::AttrManipWheel()).mWheelDelta);
    stream.setValue<std::string>(manip.mDataref.str());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipAxisSwitchUpDown & outManip) {
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.mCursor = xobj::ECursor(xobj::ECursor::eId(stream.value<std::int32_t>()));
    outManip.mToolType = stream.value<std::string>();
    outManip.mClickDelta = stream.value<float>();
    outManip.mHoldDelta = stream.value<float>();
    outManip.mMin = stream.value<float>();
    outManip.mMax = stream.value<float>();
    //-------------------
    const auto wheelEnabled = stream.value<bool>();
    xobj::AttrManipWheel wheel(stream.value<float>()); // we always have to read the value from the stream
    outManip.mWheel = wheelEnabled ? std::optional(wheel) : std::nullopt;
    //-------------------
    outManip.mDataref = stream.value<std::string>();
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipCmd & manip) {
    stream.setValue<std::uint8_t>(std::uint8_t(1)); // manip manip io version
    stream.setValue<std::int32_t>(manip.mCursor.id());
    stream.setValue<std::string>(manip.mToolType.str());
    stream.setValue<std::string>(manip.mCommand.str());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipCmd & outManip) {
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.mCursor = xobj::ECursor(xobj::ECursor::eId(stream.value<std::int32_t>()));
    outManip.mToolType = stream.value<std::string>();
    outManip.mCommand = stream.value<std::string>();
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipCmdAxis & manip) {
    stream.setValue<std::uint8_t>(std::uint8_t(1)); // manip io version
    stream.setValue<std::int32_t>(manip.mCursor.id());
    stream.setValue<std::string>(manip.mToolType.str());
    stream.setValue<float>(manip.mDirX);
    stream.setValue<float>(manip.mDirY);
    stream.setValue<float>(manip.mDirZ);
    stream.setValue<std::string>(manip.mPosCommand.str());
    stream.setValue<std::string>(manip.mNegCommand.str());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipCmdAxis & outManip) {
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.mCursor = xobj::ECursor(xobj::ECursor::eId(stream.value<std::int32_t>()));
    outManip.mToolType = stream.value<std::string>();
    outManip.mDirX = stream.value<float>();
    outManip.mDirY = stream.value<float>();
    outManip.mDirZ = stream.value<float>();
    outManip.mPosCommand = stream.value<std::string>();
    outManip.mNegCommand = stream.value<std::string>();
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipCmdKnob & manip) {
    stream.setValue<std::uint8_t>(std::uint8_t(1)); // manip io version
    stream.setValue<std::int32_t>(manip.mCursor.id());
    stream.setValue<std::string>(manip.mToolType.str());
    stream.setValue<std::string>(manip.mPosCommand.str());
    stream.setValue<std::string>(manip.mNegCommand.str());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipCmdKnob & outManip) {
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.mCursor = xobj::ECursor(xobj::ECursor::eId(stream.value<std::int32_t>()));
    outManip.mToolType = stream.value<std::string>();
    outManip.mPosCommand = stream.value<std::string>();
    outManip.mNegCommand = stream.value<std::string>();
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & /*stream*/, const xobj::AttrManipCmdKnob2 & /*manip*/) {
    // todo implementation
}

bool ManipIO::load(INode * /*node*/, sts::DataStreamI & /*stream*/, xobj::AttrManipCmdKnob2 & /*outManip*/) {
    // todo implementation
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipCmdSwitchLeftRight & manip) {
    stream.setValue<std::uint8_t>(std::uint8_t(1)); // manip io version
    stream.setValue<std::int32_t>(manip.mCursor.id());
    stream.setValue<std::string>(manip.mToolType.str());
    stream.setValue<std::string>(manip.mPosCommand.str());
    stream.setValue<std::string>(manip.mNegCommand.str());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipCmdSwitchLeftRight & outManip) {
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.mCursor = xobj::ECursor(xobj::ECursor::eId(stream.value<std::int32_t>()));
    outManip.mToolType = stream.value<std::string>();
    outManip.mPosCommand = stream.value<std::string>();
    outManip.mNegCommand = stream.value<std::string>();
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & /*stream*/, const xobj::AttrManipCmdSwitchLeftRight2 & /*manip*/) {
    // todo implementation
}

bool ManipIO::load(INode * /*node*/, sts::DataStreamI & /*stream*/, xobj::AttrManipCmdSwitchLeftRight2 & /*outManip*/) {
    // todo implementation
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipCmdSwitchUpDown & manip) {
    stream.setValue<std::uint8_t>(std::uint8_t(1)); // manip io version
    stream.setValue<std::int32_t>(manip.mCursor.id());
    stream.setValue<std::string>(manip.mToolType.str());
    stream.setValue<std::string>(manip.mPosCommand.str());
    stream.setValue<std::string>(manip.mNegCommand.str());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipCmdSwitchUpDown & outManip) {
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.mCursor = xobj::ECursor(xobj::ECursor::eId(stream.value<std::int32_t>()));
    outManip.mToolType = stream.value<std::string>();
    outManip.mPosCommand = stream.value<std::string>();
    outManip.mNegCommand = stream.value<std::string>();
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & /*stream*/, const xobj::AttrManipCmdSwitchUpDown2 & /*manip*/) {
    // todo implementation
}

bool ManipIO::load(INode * /*node*/, sts::DataStreamI & /*stream*/, xobj::AttrManipCmdSwitchUpDown2 & /*outManip*/) {
    // todo implementation
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipDelta & manip) {
    stream.setValue<std::uint8_t>(std::uint8_t(1)); // manip io version
    stream.setValue<std::int32_t>(manip.mCursor.id());
    stream.setValue<std::string>(manip.mToolType.str());
    stream.setValue<float>(manip.mDown);
    stream.setValue<float>(manip.mHold);
    stream.setValue<float>(manip.mMin);
    stream.setValue<float>(manip.mMax);
    stream.setValue<bool>(manip.mWheel.has_value());
    stream.setValue<float>(manip.mWheel.value_or(xobj::AttrManipWheel()).mWheelDelta);
    stream.setValue<std::string>(manip.mDataref.str());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipDelta & outManip) {
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.mCursor = xobj::ECursor(xobj::ECursor::eId(stream.value<std::int32_t>()));
    outManip.mToolType = stream.value<std::string>();
    outManip.mDown = stream.value<float>();
    outManip.mHold = stream.value<float>();
    outManip.mMin = stream.value<float>();
    outManip.mMax = stream.value<float>();
    //-------------------
    const auto wheelEnabled = stream.value<bool>();
    xobj::AttrManipWheel wheel(stream.value<float>()); // we always have to read the value from the stream
    outManip.mWheel = wheelEnabled ? std::optional(wheel) : std::nullopt;
    //-------------------
    outManip.mDataref = stream.value<std::string>();
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipDragAxis & manip) {
    stream.setValue<std::uint8_t>(std::uint8_t(1)); // manip io version
    stream.setValue<std::int32_t>(manip.mCursor.id());
    stream.setValue<std::string>(manip.mToolType.str());
    stream.setValue<float>(manip.mDirX);
    stream.setValue<float>(manip.mDirY);
    stream.setValue<float>(manip.mDirZ);
    stream.setValue<float>(manip.mVal1);
    stream.setValue<float>(manip.mVal2);
    stream.setValue<bool>(manip.mWheel.has_value());
    stream.setValue<float>(manip.mWheel.value_or(xobj::AttrManipWheel()).mWheelDelta);
    stream.setValue<std::string>(manip.mDataref.str());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipDragAxis & outManip) {
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.mCursor = xobj::ECursor(xobj::ECursor::eId(stream.value<std::int32_t>()));
    outManip.mToolType = stream.value<std::string>();
    outManip.mDirX = stream.value<float>();
    outManip.mDirY = stream.value<float>();
    outManip.mDirZ = stream.value<float>();
    outManip.mVal1 = stream.value<float>();
    outManip.mVal2 = stream.value<float>();
    //-------------------
    const auto wheelEnabled = stream.value<bool>();
    xobj::AttrManipWheel wheel(stream.value<float>()); // we always have to read the value from the stream
    outManip.mWheel = wheelEnabled ? std::optional(wheel) : std::nullopt;
    //-------------------
    outManip.mDataref = stream.value<std::string>();
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipDragAxisPix & manip) {
    stream.setValue<std::uint8_t>(std::uint8_t(1)); // manip io version
    stream.setValue<std::int32_t>(manip.mCursor.id());
    stream.setValue<std::string>(manip.mToolType.str());
    stream.setValue<int>(manip.mDxPix);
    stream.setValue<int>(manip.mStep);
    stream.setValue<float>(manip.mExp);
    stream.setValue<float>(manip.mVal1);
    stream.setValue<float>(manip.mVal2);
    stream.setValue<bool>(manip.mWheel.has_value());
    stream.setValue<float>(manip.mWheel.value_or(xobj::AttrManipWheel()).mWheelDelta);
    stream.setValue<std::string>(manip.mDataref.str());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipDragAxisPix & outManip) {
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.mCursor = xobj::ECursor(xobj::ECursor::eId(stream.value<std::int32_t>()));
    outManip.mToolType = stream.value<std::string>();
    outManip.mDxPix = stream.value<int>();
    outManip.mStep = stream.value<int>();
    outManip.mExp = stream.value<float>();
    outManip.mVal1 = stream.value<float>();
    outManip.mVal2 = stream.value<float>();
    //-------------------
    const auto wheelEnabled = stream.value<bool>();
    xobj::AttrManipWheel wheel(stream.value<float>()); // we always have to read the value from the stream
    outManip.mWheel = wheelEnabled ? std::optional(wheel) : std::nullopt;
    //-------------------
    outManip.mDataref = stream.value<std::string>();
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipDragXy & manip) {
    stream.setValue<std::uint8_t>(std::uint8_t(1)); // manip io version
    stream.setValue<std::int32_t>(manip.mCursor.id());
    stream.setValue<std::string>(manip.mToolType.str());
    stream.setValue<float>(manip.mX);
    stream.setValue<float>(manip.mXMin);
    stream.setValue<float>(manip.mXMax);
    stream.setValue<std::string>(manip.mXDataref.str());
    stream.setValue<float>(manip.mY);
    stream.setValue<float>(manip.mYMin);
    stream.setValue<float>(manip.mYMax);
    stream.setValue<std::string>(manip.mYDataref.str());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipDragXy & outManip) {
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.mCursor = xobj::ECursor(xobj::ECursor::eId(stream.value<std::int32_t>()));
    outManip.mToolType = stream.value<std::string>();
    outManip.mX = stream.value<float>();
    outManip.mXMin = stream.value<float>();
    outManip.mXMax = stream.value<float>();
    outManip.mXDataref = stream.value<std::string>();
    outManip.mY = stream.value<float>();
    outManip.mYMin = stream.value<float>();
    outManip.mYMax = stream.value<float>();
    outManip.mYDataref = stream.value<std::string>();
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & /*stream*/, const xobj::AttrManipDragRotate & /*manip*/) {
    // todo implementation
}

bool ManipIO::load(INode * /*node*/, sts::DataStreamI & /*stream*/, xobj::AttrManipDragRotate & /*outManip*/) {
    // todo implementation
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipNoop & manip) {
    stream.setValue<std::uint8_t>(std::uint8_t(2));
    stream.setValue<std::string>(manip.mToolType.str());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipNoop & outManip) {
    const auto version = stream.value<std::uint8_t>();
    if (version == 2) {
        outManip.mToolType = stream.value<std::string>();
    }
    else if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipPanel & /*inManip*/) {
    stream.setValue<std::uint8_t>(std::uint8_t(1)); // manip io version
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipPanel & /*outManip*/) {
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipPush & manip) {
    stream.setValue<std::uint8_t>(std::uint8_t(1)); // manip io version
    stream.setValue<std::int32_t>(manip.mCursor.id());
    stream.setValue<std::string>(manip.mToolType.str());
    stream.setValue<float>(manip.mDown);
    stream.setValue<float>(manip.mUp);
    stream.setValue<bool>(manip.mWheel.has_value());
    stream.setValue<float>(manip.mWheel.value_or(xobj::AttrManipWheel()).mWheelDelta);
    stream.setValue<std::string>(manip.mDataref.str());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipPush & outManip) {
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.mCursor = xobj::ECursor(xobj::ECursor::eId(stream.value<std::int32_t>()));
    outManip.mToolType = stream.value<std::string>();
    outManip.mDown = stream.value<float>();
    outManip.mUp = stream.value<float>();
    //-------------------
    const auto wheelEnabled = stream.value<bool>();
    xobj::AttrManipWheel wheel(stream.value<float>()); // we always have to read the value from the stream
    outManip.mWheel = wheelEnabled ? std::optional(wheel) : std::nullopt;
    //-------------------
    outManip.mDataref = stream.value<std::string>();
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipRadio & manip) {
    stream.setValue<std::uint8_t>(std::uint8_t(1)); // manip io version
    stream.setValue<std::int32_t>(manip.mCursor.id());
    stream.setValue<std::string>(manip.mToolType.str());
    stream.setValue<float>(manip.mDown);
    stream.setValue<bool>(manip.mWheel.has_value());
    stream.setValue<float>(manip.mWheel.value_or(xobj::AttrManipWheel()).mWheelDelta);
    stream.setValue<std::string>(manip.mDataref.str());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipRadio & outManip) {
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.mCursor = xobj::ECursor(xobj::ECursor::eId(stream.value<std::int32_t>()));
    outManip.mToolType = stream.value<std::string>();
    outManip.mDown = stream.value<float>();
    //-------------------
    const auto wheelEnabled = stream.value<bool>();
    xobj::AttrManipWheel wheel(stream.value<float>()); // we always have to read the value from the stream
    outManip.mWheel = wheelEnabled ? std::optional(wheel) : std::nullopt;
    //-------------------
    outManip.mDataref = stream.value<std::string>();
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipToggle & manip) {
    stream.setValue<std::uint8_t>(std::uint8_t(1)); // manip io version
    stream.setValue<std::int32_t>(manip.mCursor.id());
    stream.setValue<std::string>(manip.mToolType.str());
    stream.setValue<float>(manip.mOn);
    stream.setValue<float>(manip.mOff);
    stream.setValue<bool>(manip.mWheel.has_value());
    stream.setValue<float>(manip.mWheel.value_or(xobj::AttrManipWheel()).mWheelDelta);
    stream.setValue<std::string>(manip.mDataref.str());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipToggle & outManip) {
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.mCursor = xobj::ECursor(xobj::ECursor::eId(stream.value<std::int32_t>()));
    outManip.mToolType = stream.value<std::string>();
    outManip.mOn = stream.value<float>();
    outManip.mOff = stream.value<float>();
    //-------------------
    const auto wheelEnabled = stream.value<bool>();
    xobj::AttrManipWheel wheel(stream.value<float>()); // we always have to read the value from the stream
    outManip.mWheel = wheelEnabled ? std::optional(wheel) : std::nullopt;
    //-------------------
    outManip.mDataref = stream.value<std::string>();
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipWrap & manip) {
    stream.setValue<std::uint8_t>(std::uint8_t(1)); // manip io version
    stream.setValue<std::int32_t>(manip.mCursor.id());
    stream.setValue<std::string>(manip.mToolType.str());
    stream.setValue<float>(manip.mDown);
    stream.setValue<float>(manip.mHold);
    stream.setValue<float>(manip.mMin);
    stream.setValue<float>(manip.mMax);
    stream.setValue<bool>(manip.mWheel.has_value());
    stream.setValue<float>(manip.mWheel.value_or(xobj::AttrManipWheel()).mWheelDelta);
    stream.setValue<std::string>(manip.mDataref.str());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipWrap & outManip) {
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.mCursor = xobj::ECursor(xobj::ECursor::eId(stream.value<std::int32_t>()));
    outManip.mToolType = stream.value<std::string>();
    outManip.mDown = stream.value<float>();
    outManip.mHold = stream.value<float>();
    outManip.mMin = stream.value<float>();
    outManip.mMax = stream.value<float>();
    //-------------------
    const auto wheelEnabled = stream.value<bool>();
    xobj::AttrManipWheel wheel(stream.value<float>()); // we always have to read the value from the stream
    outManip.mWheel = wheelEnabled ? std::optional(wheel) : std::nullopt;
    //-------------------
    outManip.mDataref = stream.value<std::string>();
    return true;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool ManipIO::canApply(INode * node) {
    const auto t = GetCOREInterface()->GetTime();
    ObjectState os = node->EvalWorldState(t);
    if (os.obj->SuperClassID() == SHAPE_CLASS_ID) {
        return false;
    }
    if (os.obj->IsSubClassOf(triObjectClassID)) {
        return true;
    }
    return os.obj->CanConvertToType(triObjectClassID) == TRUE;
}

bool ManipIO::cloneData(INode * NodeFrom, INode * NodeTo) {
    if (!canApply(NodeTo)) {
        return false;
    }
    NodeIO::cloneData(NodeFrom, NodeTo, static_cast<DWORD>(eManipIOID::MANIPULATOR));
    return true;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void ManipIO::saveDataToNode(INode * outNode, const std::stringstream & indata) {
    NodeIO::saveData(outNode, static_cast<DWORD>(eManipIOID::MANIPULATOR), indata.str());
}

void ManipIO::removeFromNode(INode * node) {
    NodeIO::removeData(node, static_cast<DWORD>(eManipIOID::MANIPULATOR));
}

bool ManipIO::loadFromNode(INode * inNode, IManipIo * inCallback) {
    if (!inNode || !inCallback) {
        XLError << "One or more of the input parameters is nullptr";
        return false;
    }

    AppDataChunk * chunk = NodeIO::dataChunk(inNode, static_cast<DWORD>(eManipIOID::MANIPULATOR));
    if (!chunk) {
        chunk = bcw::MdManipIoOld::data(inNode);
        if (!chunk) {
            inCallback->gotNoManip();
            return true;
        }
        if (bcw::MdManipIoOld::loadFromNode(inNode, chunk, inCallback)) {
            bcw::ManipGetter getter;
            bcw::MdManipIoOld::loadFromNode(inNode, chunk, &getter);
            if (getter.mAttr.mManip) {
                saveToNode(inNode, getter.mAttr.mManip);
                bcw::MdManipIoOld::removeDataFromNode(inNode);
            }
            return true;
        }
        return false;
    }

    if (chunk->length == 0) {
        log_unexpected_data_length(inNode, chunk->length);
        inCallback->gotNoManip();
        return false;
    }

    //-------------------------------------------------------------------------

    std::stringstream buf(std::string(reinterpret_cast<const char *>(chunk->data), chunk->length));
    sts::DataStream stream(&buf);
    stream.readAndSetFormatVersion();
    const auto version = stream.value<std::uint8_t>();
    if (version != 1) {
        log_unexpected_version(inNode, version);
        inCallback->gotNoManip();
        return false;
    }

    //-------------------------------------------------------------------------

    xobj::EManipulator id = xobj::EManipulator::fromString(stream.value<std::string>().c_str());
    switch (id.id()) {
        case xobj::EManipulator::eId::none: {
            inCallback->gotNoManip();
            return true;
        }
        case xobj::EManipulator::axis_knob: {
            xobj::AttrManipAxisKnob manip;
            if (!load(inNode, stream, manip)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(manip);
            break;
        }
        case xobj::EManipulator::axis_switch_lr: {
            xobj::AttrManipAxisSwitchLeftRight manip;
            if (!load(inNode, stream, manip)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(manip);
            break;
        }
        case xobj::EManipulator::axis_switch_ud: {
            xobj::AttrManipAxisSwitchUpDown manip;
            if (!load(inNode, stream, manip)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(manip);
            break;
        }
        case xobj::EManipulator::eId::command: {
            xobj::AttrManipCmd manip;
            if (!load(inNode, stream, manip)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(manip);
            break;
        }
        case xobj::EManipulator::eId::command_axis: {
            xobj::AttrManipCmdAxis manip;
            if (!load(inNode, stream, manip)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(manip);
            break;
        }
        case xobj::EManipulator::command_knob: {
            xobj::AttrManipCmdKnob manip;
            if (!load(inNode, stream, manip)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(manip);
            break;
        }
        case xobj::EManipulator::command_switch_lr: {
            xobj::AttrManipCmdSwitchLeftRight manip;
            if (!load(inNode, stream, manip)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(manip);
            break;
        }
        case xobj::EManipulator::command_switch_ud: {
            xobj::AttrManipCmdSwitchUpDown manip;
            if (!load(inNode, stream, manip)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(manip);
            break;
        }
        case xobj::EManipulator::eId::delta: {
            xobj::AttrManipDelta manip;
            if (!load(inNode, stream, manip)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(manip);
            break;
        }
        case xobj::EManipulator::eId::drag_axis: {
            xobj::AttrManipDragAxis manip;
            if (!load(inNode, stream, manip)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(manip);
            break;
        }
        case xobj::EManipulator::eId::drag_axis_pix: {
            xobj::AttrManipDragAxisPix manip;
            if (!load(inNode, stream, manip)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(manip);
            break;
        }
        case xobj::EManipulator::eId::drag_xy: {
            xobj::AttrManipDragXy manip;
            if (!load(inNode, stream, manip)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(manip);
            break;
        }
        case xobj::EManipulator::eId::noop: {
            xobj::AttrManipNoop manip;
            if (!load(inNode, stream, manip)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(manip);
            break;
        }
        case xobj::EManipulator::eId::panel: {
            xobj::AttrManipPanel manip;
            if (!load(inNode, stream, manip)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(manip);
            break;
        }
        case xobj::EManipulator::eId::push: {
            xobj::AttrManipPush manip;
            if (!load(inNode, stream, manip)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(manip);
            break;
        }
        case xobj::EManipulator::eId::radio: {
            xobj::AttrManipRadio manip;
            if (!load(inNode, stream, manip)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(manip);
            break;
        }
        case xobj::EManipulator::eId::toggle: {
            xobj::AttrManipToggle manip;
            if (!load(inNode, stream, manip)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(manip);
            break;
        }
        case xobj::EManipulator::eId::wrap: {
            xobj::AttrManipWrap manip;
            if (!load(inNode, stream, manip)) {
                inCallback->gotNoManip();
                return false;
            }
            inCallback->gotAttrManip(manip);
            break;
        }
        default: return false;
    }

    //-------------------------------------------------------------------------

    return true;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

class ManipIO::ManipSaver {
    template<class V>
    struct always_false : std::false_type {};

public:

    template<typename MANIP>
    void operator()(MANIP && manip) {
        using T = std::decay_t<decltype(manip)>;

        if constexpr (std::is_same_v<T, xobj::AttrManipNone>) {
            ManipIO::removeFromNode(mNode);
        }
        else {
            ManipIO::saveToNodeInternal(mNode, manip);
        }
    }

    INode * mNode = nullptr;
};

void ManipIO::saveToNode(INode * outNode, const std::optional<xobj::AttrManip> & manip) {
    if (!manip) {
        removeFromNode(outNode);
        return;
    }
    std::visit(ManipIO::ManipSaver{outNode}, *manip);
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
