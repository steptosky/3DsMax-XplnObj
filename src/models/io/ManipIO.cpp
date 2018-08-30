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
        mAttr.setManipulator(inManip.clone());
    }

    void gotAttrManip(const xobj::AttrManipAxisSwitchLeftRight & inManip) override {
        mAttr.setManipulator(inManip.clone());
    }

    void gotAttrManip(const xobj::AttrManipAxisSwitchUpDown & inManip) override {
        mAttr.setManipulator(inManip.clone());
    }

    void gotAttrManip(const xobj::AttrManipCmd & inManip) override {
        mAttr.setManipulator(inManip.clone());
    }

    void gotAttrManip(const xobj::AttrManipCmdAxis & inManip) override {
        mAttr.setManipulator(inManip.clone());
    }

    void gotAttrManip(const xobj::AttrManipCmdKnob & inManip) override {
        mAttr.setManipulator(inManip.clone());
    }

    void gotAttrManip(const xobj::AttrManipCmdSwitchLeftRight & inManip) override {
        mAttr.setManipulator(inManip.clone());
    }

    void gotAttrManip(const xobj::AttrManipCmdSwitchUpDown & inManip) override {
        mAttr.setManipulator(inManip.clone());
    }

    void gotAttrManip(const xobj::AttrManipDelta & inManip) override {
        mAttr.setManipulator(inManip.clone());
    }

    void gotAttrManip(const xobj::AttrManipDragAxis & inManip) override {
        mAttr.setManipulator(inManip.clone());
    }

    void gotAttrManip(const xobj::AttrManipDragAxisPix & inManip) override {
        mAttr.setManipulator(inManip.clone());
    }

    void gotAttrManip(const xobj::AttrManipDragXy & inManip) override {
        mAttr.setManipulator(inManip.clone());
    }

    void gotAttrManip(const xobj::AttrManipNoop & inManip) override {
        mAttr.setManipulator(inManip.clone());
    }

    void gotAttrManip(const xobj::AttrManipPanel & inManip) override {
        mAttr.setManipulator(inManip.clone());
    }

    void gotAttrManip(const xobj::AttrManipPush & inManip) override {
        mAttr.setManipulator(inManip.clone());
    }

    void gotAttrManip(const xobj::AttrManipRadio & inManip) override {
        mAttr.setManipulator(inManip.clone());
    }

    void gotAttrManip(const xobj::AttrManipToggle & inManip) override {
        mAttr.setManipulator(inManip.clone());
    }

    void gotAttrManip(const xobj::AttrManipWrap & inManip) override {
        mAttr.setManipulator(inManip.clone());
    }

    void gotNoManip() override {
        mAttr.setManipulator(nullptr);
    }

    xobj::AttrSet mAttr;

};
}

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipAxisKnob & inManip) {
    stream.setValue<uint8_t>(uint8_t(1)); // manip io version
    stream.setValue<int32_t>(inManip.cursor().id());
    stream.setValue<std::string>(inManip.toolTip());
    stream.setValue<float>(inManip.clickDelta());
    stream.setValue<float>(inManip.holdDelta());
    stream.setValue<float>(inManip.minimum());
    stream.setValue<float>(inManip.maximum());
    stream.setValue<bool>(inManip.isWheelEnabled());
    stream.setValue<float>(inManip.wheelDelta());
    stream.setValue<std::string>(inManip.dataref());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipAxisKnob & outManip) {
    uint8_t version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.setCursor(xobj::ECursor(xobj::ECursor::eId(stream.value<int32_t>())));
    outManip.setToolTip(stream.value<std::string>());
    outManip.setClickDelta(stream.value<float>());
    outManip.setHoldDelta(stream.value<float>());
    outManip.setMinimum(stream.value<float>());
    outManip.setMaximum(stream.value<float>());
    outManip.setWheelEnabled(stream.value<bool>());
    outManip.setWheelDelta(stream.value<float>());
    outManip.setDataref(stream.value<std::string>());
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipAxisSwitchLeftRight & inManip) {
    stream.setValue<uint8_t>(uint8_t(1)); // manip io version
    stream.setValue<int32_t>(inManip.cursor().id());
    stream.setValue<std::string>(inManip.toolTip());
    stream.setValue<float>(inManip.clickDelta());
    stream.setValue<float>(inManip.holdDelta());
    stream.setValue<float>(inManip.minimum());
    stream.setValue<float>(inManip.maximum());
    stream.setValue<bool>(inManip.isWheelEnabled());
    stream.setValue<float>(inManip.wheelDelta());
    stream.setValue<std::string>(inManip.dataref());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipAxisSwitchLeftRight & outManip) {
    uint8_t version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.setCursor(xobj::ECursor(xobj::ECursor::eId(stream.value<int32_t>())));
    outManip.setToolTip(stream.value<std::string>());
    outManip.setClickDelta(stream.value<float>());
    outManip.setHoldDelta(stream.value<float>());
    outManip.setMinimum(stream.value<float>());
    outManip.setMaximum(stream.value<float>());
    outManip.setWheelEnabled(stream.value<bool>());
    outManip.setWheelDelta(stream.value<float>());
    outManip.setDataref(stream.value<std::string>());
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipAxisSwitchUpDown & inManip) {
    stream.setValue<uint8_t>(uint8_t(1)); // manip io version
    stream.setValue<int32_t>(inManip.cursor().id());
    stream.setValue<std::string>(inManip.toolTip());
    stream.setValue<float>(inManip.clickDelta());
    stream.setValue<float>(inManip.holdDelta());
    stream.setValue<float>(inManip.minimum());
    stream.setValue<float>(inManip.maximum());
    stream.setValue<bool>(inManip.isWheelEnabled());
    stream.setValue<float>(inManip.wheelDelta());
    stream.setValue<std::string>(inManip.dataref());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipAxisSwitchUpDown & outManip) {
    uint8_t version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.setCursor(xobj::ECursor(xobj::ECursor::eId(stream.value<int32_t>())));
    outManip.setToolTip(stream.value<std::string>());
    outManip.setClickDelta(stream.value<float>());
    outManip.setHoldDelta(stream.value<float>());
    outManip.setMinimum(stream.value<float>());
    outManip.setMaximum(stream.value<float>());
    outManip.setWheelEnabled(stream.value<bool>());
    outManip.setWheelDelta(stream.value<float>());
    outManip.setDataref(stream.value<std::string>());
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipCmd & inManip) {
    stream.setValue<uint8_t>(uint8_t(1)); // manip manip io version
    stream.setValue<int32_t>(inManip.cursor().id());
    stream.setValue<std::string>(inManip.toolTip());
    stream.setValue<std::string>(inManip.command());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipCmd & outManip) {
    uint8_t version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.setCursor(xobj::ECursor(xobj::ECursor::eId(stream.value<int32_t>())));
    outManip.setToolTip(stream.value<std::string>());
    outManip.setCommand(stream.value<std::string>());
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipCmdAxis & inManip) {
    stream.setValue<uint8_t>(uint8_t(1)); // manip io version
    stream.setValue<int32_t>(inManip.cursor().id());
    stream.setValue<std::string>(inManip.toolTip());
    stream.setValue<float>(inManip.x());
    stream.setValue<float>(inManip.y());
    stream.setValue<float>(inManip.z());
    stream.setValue<std::string>(inManip.cmdPositive());
    stream.setValue<std::string>(inManip.cmdNegative());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipCmdAxis & outManip) {
    uint8_t version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.setCursor(xobj::ECursor(xobj::ECursor::eId(stream.value<int32_t>())));
    outManip.setToolTip(stream.value<std::string>());
    outManip.setX(stream.value<float>());
    outManip.setY(stream.value<float>());
    outManip.setZ(stream.value<float>());
    outManip.setCmdPositive(stream.value<std::string>());
    outManip.setCmdNegative(stream.value<std::string>());
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipCmdKnob & inManip) {
    stream.setValue<uint8_t>(uint8_t(1)); // manip io version
    stream.setValue<int32_t>(inManip.cursor().id());
    stream.setValue<std::string>(inManip.toolTip());
    stream.setValue<std::string>(inManip.cmdPositive());
    stream.setValue<std::string>(inManip.cmdNegative());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipCmdKnob & outManip) {
    uint8_t version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.setCursor(xobj::ECursor(xobj::ECursor::eId(stream.value<int32_t>())));
    outManip.setToolTip(stream.value<std::string>());
    outManip.setCmdPositive(stream.value<std::string>());
    outManip.setCmdNegative(stream.value<std::string>());
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipCmdSwitchLeftRight & inManip) {
    stream.setValue<uint8_t>(uint8_t(1)); // manip io version
    stream.setValue<int32_t>(inManip.cursor().id());
    stream.setValue<std::string>(inManip.toolTip());
    stream.setValue<std::string>(inManip.cmdPositive());
    stream.setValue<std::string>(inManip.cmdNegative());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipCmdSwitchLeftRight & outManip) {
    uint8_t version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.setCursor(xobj::ECursor(xobj::ECursor::eId(stream.value<int32_t>())));
    outManip.setToolTip(stream.value<std::string>());
    outManip.setCmdPositive(stream.value<std::string>());
    outManip.setCmdNegative(stream.value<std::string>());
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipCmdSwitchUpDown & inManip) {
    stream.setValue<uint8_t>(uint8_t(1)); // manip io version
    stream.setValue<int32_t>(inManip.cursor().id());
    stream.setValue<std::string>(inManip.toolTip());
    stream.setValue<std::string>(inManip.cmdPositive());
    stream.setValue<std::string>(inManip.cmdNegative());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipCmdSwitchUpDown & outManip) {
    uint8_t version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.setCursor(xobj::ECursor(xobj::ECursor::eId(stream.value<int32_t>())));
    outManip.setToolTip(stream.value<std::string>());
    outManip.setCmdPositive(stream.value<std::string>());
    outManip.setCmdNegative(stream.value<std::string>());
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipDelta & inManip) {
    stream.setValue<uint8_t>(uint8_t(1)); // manip io version
    stream.setValue<int32_t>(inManip.cursor().id());
    stream.setValue<std::string>(inManip.toolTip());
    stream.setValue<float>(inManip.down());
    stream.setValue<float>(inManip.hold());
    stream.setValue<float>(inManip.minimum());
    stream.setValue<float>(inManip.maximum());
    stream.setValue<bool>(inManip.isWheelEnabled());
    stream.setValue<float>(inManip.wheelDelta());
    stream.setValue<std::string>(inManip.dataref());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipDelta & outManip) {
    uint8_t version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.setCursor(xobj::ECursor(xobj::ECursor::eId(stream.value<int32_t>())));
    outManip.setToolTip(stream.value<std::string>());
    outManip.setDown(stream.value<float>());
    outManip.setHold(stream.value<float>());
    outManip.setMinimum(stream.value<float>());
    outManip.setMaximum(stream.value<float>());
    outManip.setWheelEnabled(stream.value<bool>());
    outManip.setWheelDelta(stream.value<float>());
    outManip.setDataref(stream.value<std::string>());
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipDragAxis & inManip) {
    stream.setValue<uint8_t>(uint8_t(1)); // manip io version
    stream.setValue<int32_t>(inManip.cursor().id());
    stream.setValue<std::string>(inManip.toolTip());
    stream.setValue<float>(inManip.x());
    stream.setValue<float>(inManip.y());
    stream.setValue<float>(inManip.z());
    stream.setValue<float>(inManip.val1());
    stream.setValue<float>(inManip.val2());
    stream.setValue<bool>(inManip.isWheelEnabled());
    stream.setValue<float>(inManip.wheelDelta());
    stream.setValue<std::string>(inManip.dataref());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipDragAxis & outManip) {
    uint8_t version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.setCursor(xobj::ECursor(xobj::ECursor::eId(stream.value<int32_t>())));
    outManip.setToolTip(stream.value<std::string>());
    outManip.setX(stream.value<float>());
    outManip.setY(stream.value<float>());
    outManip.setZ(stream.value<float>());
    outManip.setVal1(stream.value<float>());
    outManip.setVal2(stream.value<float>());
    outManip.setWheelEnabled(stream.value<bool>());
    outManip.setWheelDelta(stream.value<float>());
    outManip.setDataref(stream.value<std::string>());
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipDragAxisPix & inManip) {
    stream.setValue<uint8_t>(uint8_t(1)); // manip io version
    stream.setValue<int32_t>(inManip.cursor().id());
    stream.setValue<std::string>(inManip.toolTip());
    stream.setValue<int>(inManip.dxPix());
    stream.setValue<int>(inManip.step());
    stream.setValue<float>(inManip.exp());
    stream.setValue<float>(inManip.val1());
    stream.setValue<float>(inManip.val2());
    stream.setValue<bool>(inManip.isWheelEnabled());
    stream.setValue<float>(inManip.wheelDelta());
    stream.setValue<std::string>(inManip.dataref());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipDragAxisPix & outManip) {
    uint8_t version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.setCursor(xobj::ECursor(xobj::ECursor::eId(stream.value<int32_t>())));
    outManip.setToolTip(stream.value<std::string>());
    outManip.setDxPix(stream.value<int>());
    outManip.setStep(stream.value<int>());
    outManip.setExp(stream.value<float>());
    outManip.setVal1(stream.value<float>());
    outManip.setVal2(stream.value<float>());
    outManip.setWheelEnabled(stream.value<bool>());
    outManip.setWheelDelta(stream.value<float>());
    outManip.setDataref(stream.value<std::string>());
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipDragXy & inManip) {
    stream.setValue<uint8_t>(uint8_t(1)); // manip io version
    stream.setValue<int32_t>(inManip.cursor().id());
    stream.setValue<std::string>(inManip.toolTip());
    stream.setValue<float>(inManip.x());
    stream.setValue<float>(inManip.xMin());
    stream.setValue<float>(inManip.xMax());
    stream.setValue<std::string>(inManip.xDataref());
    stream.setValue<float>(inManip.y());
    stream.setValue<float>(inManip.yMin());
    stream.setValue<float>(inManip.yMax());
    stream.setValue<std::string>(inManip.yDataref());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipDragXy & outManip) {
    uint8_t version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.setCursor(xobj::ECursor(xobj::ECursor::eId(stream.value<int32_t>())));
    outManip.setToolTip(stream.value<std::string>());
    outManip.setX(stream.value<float>());
    outManip.setXMin(stream.value<float>());
    outManip.setXMax(stream.value<float>());
    outManip.setXDataref(stream.value<std::string>());
    outManip.setY(stream.value<float>());
    outManip.setYMin(stream.value<float>());
    outManip.setYMax(stream.value<float>());
    outManip.setYDataref(stream.value<std::string>());
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipNoop & /*inManip*/) {
    stream.setValue<uint8_t>(uint8_t(1)); // manip io version
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipNoop & /*outManip*/) {
    uint8_t version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipPanel & /*inManip*/) {
    stream.setValue<uint8_t>(uint8_t(1)); // manip io version
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipPanel & /*outManip*/) {
    uint8_t version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipPush & inManip) {
    stream.setValue<uint8_t>(uint8_t(1)); // manip io version
    stream.setValue<int32_t>(inManip.cursor().id());
    stream.setValue<std::string>(inManip.toolTip());
    stream.setValue<float>(inManip.down());
    stream.setValue<float>(inManip.up());
    stream.setValue<bool>(inManip.isWheelEnabled());
    stream.setValue<float>(inManip.wheelDelta());
    stream.setValue<std::string>(inManip.dataref());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipPush & outManip) {
    uint8_t version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.setCursor(xobj::ECursor(xobj::ECursor::eId(stream.value<int32_t>())));
    outManip.setToolTip(stream.value<std::string>());
    outManip.setDown(stream.value<float>());
    outManip.setUp(stream.value<float>());
    outManip.setWheelEnabled(stream.value<bool>());
    outManip.setWheelDelta(stream.value<float>());
    outManip.setDataref(stream.value<std::string>());
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipRadio & inManip) {
    stream.setValue<uint8_t>(uint8_t(1)); // manip io version
    stream.setValue<int32_t>(inManip.cursor().id());
    stream.setValue<std::string>(inManip.toolTip());
    stream.setValue<float>(inManip.down());
    stream.setValue<bool>(inManip.isWheelEnabled());
    stream.setValue<float>(inManip.wheelDelta());
    stream.setValue<std::string>(inManip.dataref());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipRadio & outManip) {
    uint8_t version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.setCursor(xobj::ECursor(xobj::ECursor::eId(stream.value<int32_t>())));
    outManip.setToolTip(stream.value<std::string>());
    outManip.setDown(stream.value<float>());
    outManip.setWheelEnabled(stream.value<bool>());
    outManip.setWheelDelta(stream.value<float>());
    outManip.setDataref(stream.value<std::string>());
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipToggle & inManip) {
    stream.setValue<uint8_t>(uint8_t(1)); // manip io version
    stream.setValue<int32_t>(inManip.cursor().id());
    stream.setValue<std::string>(inManip.toolTip());
    stream.setValue<float>(inManip.on());
    stream.setValue<float>(inManip.off());
    stream.setValue<bool>(inManip.isWheelEnabled());
    stream.setValue<float>(inManip.wheelDelta());
    stream.setValue<std::string>(inManip.dataref());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipToggle & outManip) {
    uint8_t version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.setCursor(xobj::ECursor(xobj::ECursor::eId(stream.value<int32_t>())));
    outManip.setToolTip(stream.value<std::string>());
    outManip.setOn(stream.value<float>());
    outManip.setOff(stream.value<float>());
    outManip.setWheelEnabled(stream.value<bool>());
    outManip.setWheelDelta(stream.value<float>());
    outManip.setDataref(stream.value<std::string>());
    return true;
}

/***************************************************************************************/

void ManipIO::save(sts::DataStreamO & stream, const xobj::AttrManipWrap & inManip) {
    stream.setValue<uint8_t>(uint8_t(1)); // manip io version
    stream.setValue<int32_t>(inManip.cursor().id());
    stream.setValue<std::string>(inManip.toolTip());
    stream.setValue<float>(inManip.down());
    stream.setValue<float>(inManip.hold());
    stream.setValue<float>(inManip.minimum());
    stream.setValue<float>(inManip.maximum());
    stream.setValue<bool>(inManip.isWheelEnabled());
    stream.setValue<float>(inManip.wheelDelta());
    stream.setValue<std::string>(inManip.dataref());
}

bool ManipIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrManipWrap & outManip) {
    uint8_t version = stream.value<uint8_t>();
    if (version != 1) {
        log_unexpected_version(node, version);
        return false;
    }
    outManip.setCursor(xobj::ECursor(xobj::ECursor::eId(stream.value<int32_t>())));
    outManip.setToolTip(stream.value<std::string>());
    outManip.setDown(stream.value<float>());
    outManip.setHold(stream.value<float>());
    outManip.setMinimum(stream.value<float>());
    outManip.setMaximum(stream.value<float>());
    outManip.setWheelEnabled(stream.value<bool>());
    outManip.setWheelDelta(stream.value<float>());
    outManip.setDataref(stream.value<std::string>());
    return true;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool ManipIO::canApply(INode * node) {
    TimeValue t = GetCOREInterface()->GetTime();
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
        LError << "One or more of the input parameters is nullptr";
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
            bcw::ManipGetter gettr;
            bcw::MdManipIoOld::loadFromNode(inNode, chunk, &gettr);
            if (gettr.mAttr.manipulator()) {
                saveToNode(inNode, gettr.mAttr.manipulator());
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
    uint8_t version = stream.value<uint8_t>();
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

void ManipIO::saveToNode(INode * outNode, const xobj::AttrManipBase * inManip) {
    if (inManip == nullptr) {
        removeFromNode(outNode);
        return;
    }
    switch (inManip->type().id()) {
        case xobj::EManipulator::none:
            removeFromNode(outNode);
            break;
        case xobj::EManipulator::axis_knob:
            saveToNodeInternal(outNode, *static_cast<const xobj::AttrManipAxisKnob *>(inManip));
            break;
        case xobj::EManipulator::axis_switch_lr:
            saveToNodeInternal(outNode, *static_cast<const xobj::AttrManipAxisSwitchLeftRight *>(inManip));
            break;
        case xobj::EManipulator::axis_switch_ud:
            saveToNodeInternal(outNode, *static_cast<const xobj::AttrManipAxisSwitchUpDown *>(inManip));
            break;
        case xobj::EManipulator::command:
            saveToNodeInternal(outNode, *static_cast<const xobj::AttrManipCmd *>(inManip));
            break;
        case xobj::EManipulator::command_axis:
            saveToNodeInternal(outNode, *static_cast<const xobj::AttrManipCmdAxis *>(inManip));
            break;
        case xobj::EManipulator::command_knob:
            saveToNodeInternal(outNode, *static_cast<const xobj::AttrManipCmdKnob *>(inManip));
            break;
        case xobj::EManipulator::command_switch_lr:
            saveToNodeInternal(outNode, *static_cast<const xobj::AttrManipCmdSwitchLeftRight *>(inManip));
            break;
        case xobj::EManipulator::command_switch_ud:
            saveToNodeInternal(outNode, *static_cast<const xobj::AttrManipCmdSwitchUpDown *>(inManip));
            break;
        case xobj::EManipulator::delta:
            saveToNodeInternal(outNode, *static_cast<const xobj::AttrManipDelta *>(inManip));
            break;
        case xobj::EManipulator::drag_axis:
            saveToNodeInternal(outNode, *static_cast<const xobj::AttrManipDragAxis *>(inManip));
            break;
        case xobj::EManipulator::drag_axis_pix:
            saveToNodeInternal(outNode, *static_cast<const xobj::AttrManipDragAxisPix *>(inManip));
            break;
        case xobj::EManipulator::drag_xy:
            saveToNodeInternal(outNode, *static_cast<const xobj::AttrManipDragXy *>(inManip));
            break;
        case xobj::EManipulator::noop:
            saveToNodeInternal(outNode, *static_cast<const xobj::AttrManipNoop *>(inManip));
            break;
        case xobj::EManipulator::panel:
            saveToNodeInternal(outNode, *static_cast<const xobj::AttrManipPanel *>(inManip));
            break;
        case xobj::EManipulator::push:
            saveToNodeInternal(outNode, *static_cast<const xobj::AttrManipPush *>(inManip));
            break;
        case xobj::EManipulator::radio:
            saveToNodeInternal(outNode, *static_cast<const xobj::AttrManipRadio *>(inManip));
            break;
        case xobj::EManipulator::toggle:
            saveToNodeInternal(outNode, *static_cast<const xobj::AttrManipToggle *>(inManip));
            break;
        case xobj::EManipulator::wrap:
            saveToNodeInternal(outNode, *static_cast<const xobj::AttrManipWrap *>(inManip));
            break;
        default: break;
    }
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
