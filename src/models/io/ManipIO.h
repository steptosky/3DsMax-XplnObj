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

#pragma warning(push, 0)
#include <max.h>
#pragma warning(pop)

#include "common/Logger.h"
#include "additional/stsio/DataStream.h"
#include <xpln/enums/EManipulator.h>
#include <xpln/obj/manipulators/AttrManipAxisKnob.h>
#include <xpln/obj/manipulators/AttrManipAxisSwitchLeftRight.h>
#include <xpln/obj/manipulators/AttrManipAxisSwitchUpDown.h>
#include <xpln/obj/manipulators/AttrManipCmd.h>
#include <xpln/obj/manipulators/AttrManipCmdKnob.h>
#include <xpln/obj/manipulators/AttrManipCmdSwitchLeftRight.h>
#include <xpln/obj/manipulators/AttrManipCmdSwitchUpDown.h>
#include <xpln/obj/manipulators/AttrManipDragXy.h>
#include <xpln/obj/manipulators/AttrManipDragAxisPix.h>
#include <xpln/obj/manipulators/AttrManipCmdAxis.h>
#include <xpln/obj/manipulators/AttrManipDragAxis.h>
#include <xpln/obj/manipulators/AttrManipDelta.h>
#include <xpln/obj/manipulators/AttrManipPanel.h>
#include <xpln/obj/manipulators/AttrManipPush.h>
#include <xpln/obj/manipulators/AttrManipRadio.h>
#include <xpln/obj/manipulators/AttrManipToggle.h>
#include <xpln/obj/manipulators/AttrManipWrap.h>
#include <xpln/obj/manipulators/AttrManipNoop.h>

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

class ManipIO {

    ManipIO() = default;
    ~ManipIO() = default;

public:

    //-------------------------------------------------------------------------

    class IManipIo {
    public:
        virtual ~IManipIo() = default;
        virtual void gotAttrManip(const xobj::AttrManipAxisKnob & inManip) = 0;
        virtual void gotAttrManip(const xobj::AttrManipAxisSwitchLeftRight & inManip) = 0;
        virtual void gotAttrManip(const xobj::AttrManipAxisSwitchUpDown & inManip) = 0;
        virtual void gotAttrManip(const xobj::AttrManipCmd & inManip) = 0;
        virtual void gotAttrManip(const xobj::AttrManipCmdAxis & inManip) = 0;
        virtual void gotAttrManip(const xobj::AttrManipCmdKnob & inManip) = 0;
        virtual void gotAttrManip(const xobj::AttrManipCmdSwitchLeftRight & inManip) = 0;
        virtual void gotAttrManip(const xobj::AttrManipCmdSwitchUpDown & inManip) = 0;
        virtual void gotAttrManip(const xobj::AttrManipDelta & inManip) = 0;
        virtual void gotAttrManip(const xobj::AttrManipDragAxis & inManip) = 0;
        virtual void gotAttrManip(const xobj::AttrManipDragAxisPix & inManip) = 0;
        virtual void gotAttrManip(const xobj::AttrManipDragXy & inManip) = 0;
        virtual void gotAttrManip(const xobj::AttrManipNoop & inManip) = 0;
        virtual void gotAttrManip(const xobj::AttrManipPanel & inManip) = 0;
        virtual void gotAttrManip(const xobj::AttrManipPush & inManip) = 0;
        virtual void gotAttrManip(const xobj::AttrManipRadio & inManip) = 0;
        virtual void gotAttrManip(const xobj::AttrManipToggle & inManip) = 0;
        virtual void gotAttrManip(const xobj::AttrManipWrap & inManip) = 0;
        virtual void gotNoManip() = 0;
    };

    //-------------------------------------------------------------------------

    static bool canApply(INode * node);
    static bool cloneData(INode * from, INode * to);

    //-------------------------------------------------------------------------

    static bool loadFromNode(INode * inNode, IManipIo * inCallback);
    static void removeFromNode(INode * node);
    static void saveToNode(INode * outNode, const xobj::AttrManipBase * inManip);

    //-------------------------------------------------------------------------

private:

    //-------------------------------------------------------------------------

    template<typename T>
    static void saveToNodeInternal(INode * outNode, const T & inManip) {
        if (!outNode) {
            LError << "INode is nullptr";
            return;
        }

        std::stringstream buf;
        sts::DataStream stream(&buf);
        stream.writeFormatVersion();
        stream.setValue<uint8_t>(uint8_t(1)); // node io version
        xobj::EManipulator type = inManip.type();
        stream.setValue<std::string>(type.toString());
        save(stream, inManip);
        saveDataToNode(outNode, buf);
    }

    //-------------------------------------------------------------------------

    static void save(sts::DataStreamO & stream, const xobj::AttrManipAxisKnob & inManip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipAxisSwitchLeftRight & inManip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipAxisSwitchUpDown & inManip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipCmd & inManip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipCmdAxis & inManip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipCmdKnob & inManip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipCmdSwitchLeftRight & inManip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipCmdSwitchUpDown & inManip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipDelta & inManip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipDragAxis & inManip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipDragAxisPix & inManip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipDragXy & inManip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipNoop & inManip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipPanel & inManip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipPush & inManip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipRadio & inManip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipToggle & inManip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipWrap & inManip);

    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipAxisKnob & outManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipAxisSwitchLeftRight & outManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipAxisSwitchUpDown & outManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipCmd & outManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipCmdAxis & outManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipCmdKnob & inManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipCmdSwitchLeftRight & inManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipCmdSwitchUpDown & inManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipDelta & outManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipDragAxis & outManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipDragAxisPix & outManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipDragXy & outManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipNoop & outManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipPanel & outManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipPush & outManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipRadio & outManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipToggle & outManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipWrap & outManip);

    //-------------------------------------------------------------------------

    static void saveDataToNode(INode * outNode, const std::stringstream & indata);

    //-------------------------------------------------------------------------

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
