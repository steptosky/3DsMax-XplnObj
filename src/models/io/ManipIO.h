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
#include <xpln/obj/manipulators/AttrManip.h>

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
    static void saveToNode(INode * outNode, const std::optional<xobj::AttrManip> & manip);

    //-------------------------------------------------------------------------

private:

    //-------------------------------------------------------------------------

    class ManipSaver;
    friend ManipSaver;

    template<typename T>
    static void saveToNodeInternal(INode * outNode, const T & inManip) {
        if (!outNode) {
            XLError << "INode is nullptr";
            return;
        }

        std::stringstream buf;
        sts::DataStream stream(&buf);
        stream.writeFormatVersion();
        stream.setValue<uint8_t>(uint8_t(1)); // node io version
        stream.setValue<std::string>(inManip.mType.toString());
        save(stream, inManip);
        saveDataToNode(outNode, buf);
    }

    //-------------------------------------------------------------------------

    static void save(sts::DataStreamO & stream, const xobj::AttrManipAxisKnob & manip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipAxisSwitchLeftRight & manip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipAxisSwitchUpDown & manip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipCmd & manip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipCmdAxis & manip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipCmdKnob & manip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipCmdKnob2 & manip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipCmdSwitchLeftRight & manip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipCmdSwitchLeftRight2 & manip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipCmdSwitchUpDown & manip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipCmdSwitchUpDown2 & manip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipDelta & manip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipDragAxis & manip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipDragAxisPix & manip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipDragXy & manip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipDragRotate & manip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipNoop & manip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipPanel & inManip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipPush & manip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipRadio & manip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipToggle & manip);
    static void save(sts::DataStreamO & stream, const xobj::AttrManipWrap & manip);

    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipAxisKnob & outManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipAxisSwitchLeftRight & outManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipAxisSwitchUpDown & outManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipCmd & outManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipCmdAxis & outManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipCmdKnob & inManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipCmdKnob2 & inManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipCmdSwitchLeftRight & inManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipCmdSwitchLeftRight2 & inManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipCmdSwitchUpDown & inManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipCmdSwitchUpDown2 & inManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipDelta & outManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipDragAxis & outManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipDragAxisPix & outManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipDragXy & outManip);
    static bool load(INode * node, sts::DataStreamI & stream, xobj::AttrManipDragRotate & outManip);
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
