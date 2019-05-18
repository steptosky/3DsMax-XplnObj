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

#include "ConverterATTR.h"

#pragma warning(push, 0)
#include <max.h>
#pragma warning(pop)

#include <xpln/obj/ObjAbstract.h>
#include <xpln/obj/ObjMesh.h>
#include "models/MdManip.h"
#include "models/MdObjAttr.h"
#include "ExportParams.h"
#include "ImportParams.h"

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

struct ManipGetter : ManipIO::IManipIo {

    explicit ManipGetter(xobj::ObjMesh * mesh) { mMesh = mesh; }
    void gotAttrManip(const xobj::AttrManipAxisKnob & inManip) override { mMesh->mAttr.mManip = xobj::AttrManip(inManip); }
    void gotAttrManip(const xobj::AttrManipAxisSwitchLeftRight & inManip) override { mMesh->mAttr.mManip = xobj::AttrManip(inManip); }
    void gotAttrManip(const xobj::AttrManipAxisSwitchUpDown & inManip) override { mMesh->mAttr.mManip = xobj::AttrManip(inManip); }
    void gotAttrManip(const xobj::AttrManipCmd & inManip) override { mMesh->mAttr.mManip = xobj::AttrManip(inManip); }
    void gotAttrManip(const xobj::AttrManipCmdAxis & inManip) override { mMesh->mAttr.mManip = xobj::AttrManip(inManip); }
    void gotAttrManip(const xobj::AttrManipCmdKnob & inManip) override { mMesh->mAttr.mManip = xobj::AttrManip(inManip); }
    void gotAttrManip(const xobj::AttrManipCmdSwitchLeftRight & inManip) override { mMesh->mAttr.mManip = xobj::AttrManip(inManip); }
    void gotAttrManip(const xobj::AttrManipCmdSwitchUpDown & inManip) override { mMesh->mAttr.mManip = xobj::AttrManip(inManip); }
    void gotAttrManip(const xobj::AttrManipDelta & inManip) override { mMesh->mAttr.mManip = xobj::AttrManip(inManip); }
    void gotAttrManip(const xobj::AttrManipDragAxis & inManip) override { mMesh->mAttr.mManip = xobj::AttrManip(inManip); }
    void gotAttrManip(const xobj::AttrManipDragAxisPix & inManip) override { mMesh->mAttr.mManip = xobj::AttrManip(inManip); }
    void gotAttrManip(const xobj::AttrManipDragXy & inManip) override { mMesh->mAttr.mManip = xobj::AttrManip(inManip); }
    void gotAttrManip(const xobj::AttrManipNoop & inManip) override { mMesh->mAttr.mManip = xobj::AttrManip(inManip); }
    void gotAttrManip(const xobj::AttrManipPanel & inManip) override { mMesh->mAttr.mManip = xobj::AttrManip(inManip); }
    void gotAttrManip(const xobj::AttrManipPush & inManip) override { mMesh->mAttr.mManip = xobj::AttrManip(inManip); }
    void gotAttrManip(const xobj::AttrManipRadio & inManip) override { mMesh->mAttr.mManip = xobj::AttrManip(inManip); }
    void gotAttrManip(const xobj::AttrManipToggle & inManip) override { mMesh->mAttr.mManip = xobj::AttrManip(inManip); }
    void gotAttrManip(const xobj::AttrManipWrap & inManip) override { mMesh->mAttr.mManip = xobj::AttrManip(inManip); }
    void gotNoManip() override { mMesh->mAttr.mManip = std::nullopt; }

private:
    xobj::ObjMesh * mMesh;
};

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void ConverterAttr::toXpln(xobj::ObjAbstract & outXObj, INode * inNode, const ExportParams &) {
    const auto xType = outXObj.objType();
    if (xType != xobj::OBJ_MESH) {
        return;
    }
    auto & m = static_cast<xobj::ObjMesh&>(outXObj);
    MdObjAttr mdBaseAttr;
    if (mdBaseAttr.linkNode(inNode)) {
        mdBaseAttr.loadFromNode(m.mAttr);
    }
    MdManip mdManip;
    if (mdManip.linkNode(inNode)) {
        ManipGetter mg(&m);
        mdManip.loadFromNode(&mg);
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void ConverterAttr::toMax(INode * inNode, const xobj::ObjAbstract & inXObj, const ImportParams &) {
    const auto xType = inXObj.objType();
    if (xType != xobj::OBJ_MESH) {
        return;
    }
    const auto & m = static_cast<const xobj::ObjMesh&>(inXObj);
    MdObjAttr mdBaseAttr;
    if (mdBaseAttr.linkNode(inNode)) {
        mdBaseAttr.saveToNode(m.mAttr);
    }
    MdManip mdManip;
    if (mdManip.linkNode(inNode)) {
        if (m.mAttr.mManip) {
            mdManip.saveToNode(m.mAttr.mManip);
        }
    }
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
