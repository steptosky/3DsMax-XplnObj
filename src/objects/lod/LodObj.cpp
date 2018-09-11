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

#include "LodObj.h"

#include "models/bwc/SerializationId.h"
#include "LodObjParams.h"
#include "common/Logger.h"
#include "common/String.h"
#include "classes-desc/ClassesDescriptions.h"
#include "additional/math/Compare.h"
#include "LodObjParamBlocks.h"
#include "LodIcon-gen.h"

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

class LodObjectPostLoadCallback : public PostLoadCallback {
public:

    explicit LodObjectPostLoadCallback(LodObject * obj)
        : mObj(obj) { }

    virtual ~LodObjectPostLoadCallback() = default;

    //-------------------------------------------------------------------------

    void proc(ILoad *) override {
        mObj->makeIcon();
    }

    //-------------------------------------------------------------------------

    LodObject * mObj = nullptr;
};

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

MouseCallback LodObject::mMouseCallback;
LodObject * LodObject::mEditOb = nullptr;

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

LodObject::LodObject() {
    mDesc = ClassesDescriptions::lodObj();
    mDesc->MakeAutoParamBlocks(this);
    mObjColor = Point3(1.0, 0.7, 0.4);
    makeIcon();
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

CreateMouseCallBack * LodObject::GetCreateMouseCallBack() {
    return &mMouseCallback;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void LodObject::loadMdDisplayObj(sts_bwc::DataStream & stream) const {
    bcw::SerializationId id;
    id.load(stream.getStdStream());

    if (id != bcw::SerializationId(0x7f084ca4, 0x1f19134e)) {
        LError << "Unexpected data input: " << id.toString();
        return;
    }
    uint8_t version;
    stream >> version;
    if (version != 1) {
        LError << "Unexpected data version: " << version;
        return;
    }

    float iconSize;
    stream >> iconSize;
    if (!mDisplayPb->SetValue(PLodObjIconScale, mIp ? mIp->GetTime() : 0, iconSize)) {
        DLError << "Can't save " << TOTEXT(PLodObjIconScale) << " value to the param block";
    }
}

void LodObject::loadRawLod(sts_bwc::DataStream & stream) const {
    bcw::SerializationId id;
    id.load(stream.getStdStream());
    if (id != bcw::SerializationId(0x2b015c93, 0x11327f01)) {
        LError << "Unexpected data input: " << id.toString();
        return;
    }
    unsigned char version;
    stream >> version;
    if (version != 1) {
        LError << "Unexpected data version: " << version;
        return;
    }

    float values[2];
    bool ignoreTransform;

    stream >> values[0];
    stream >> values[1];
    stream >> ignoreTransform;

    if (!mParamsPb->SetValue(PLodObjNear, mIp ? mIp->GetTime() : 0, values[0])) {
        DLError << "Can't save " << TOTEXT(PLodObjNear) << " value to the param block";
        return;
    }

    if (!mParamsPb->SetValue(PLodObjFar, mIp ? mIp->GetTime() : 0, values[1])) {
        DLError << "Can't save " << TOTEXT(PLodObjFar) << " value to the param block";
    }
}

void LodObject::load186(std::vector<char> & inByteArray) {
    LMessage << "Object: <" << sts::toMbString(GetObjectName()) << "> has got data from previous version.";
    std::stringstream buf(std::string(reinterpret_cast<char*>(inByteArray.data()), inByteArray.size()));
    sts_bwc::DataStream stream(buf);

    std::streampos pos = stream.getStdStream().tellg();
    uint8_t version;
    stream >> version; // 64 >= when incorrect version
    stream.getStdStream().seekg(pos);
    if (version != 0) {
        loadRawLod(stream);
    }
    else {
        uint8_t version2;
        stream >> version2;
        loadRawLod(stream);
        loadMdDisplayObj(stream);
    }
}

IOResult LodObject::Load(ILoad * load) {
    ULONG temp;
    uint32_t dataSize = 0;
    IOResult res;
    std::vector<char> ba;
    while ((res = load->OpenChunk()) == IO_OK) {
        if (res == IO_ERROR)
            return res;
        switch (load->CurChunkID()) {
            case 0:
                load->Read(&dataSize, sizeof(dataSize), &temp);
                break;
            case 1:
                if (dataSize != 0) {
                    ba.resize(dataSize);
                    load->Read(ba.data(), dataSize, &temp);
                }
                break;
            default: break;
        }
        load->CloseChunk();
    }
    if (!ba.empty()) {
        try {
            load186(ba);
        }
        catch (std::exception & e) {
            std::string msg("Can't load data of the <");
            msg.append(sts::toMbString(GetObjectName())).append("> reason: <").append(e.what()).append(">.");
            DLError << msg;
        }
        catch (...) {
            std::string msg("Can't load data of the <");
            msg.append(sts::toMbString(GetObjectName())).append("> reason: <unknown exception>.");
            DLError << msg;
        }
    }
    load->RegisterPostLoadCallback(new LodObjectPostLoadCallback(this));
    return IO_OK;
}

IOResult LodObject::Save(ISave *) {
    return IO_OK;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void LodObject::BeginEditParams(IObjParam * ip, const ULONG flags, Animatable * prev) {
    mIp = ip;
    mEditOb = this;
    mDesc->BeginEditParams(ip, this, flags, prev);
}

void LodObject::EndEditParams(IObjParam * ip, const ULONG flags, Animatable * next) {
    mEditOb = nullptr;
    mIp = nullptr;
    mDesc->EndEditParams(ip, this, flags, next);
    ClearAFlag(A_OBJ_CREATING);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

ObjectState LodObject::Eval(TimeValue) {
    return ObjectState(this);
}

Object * LodObject::ConvertToType(TimeValue, Class_ID) {
    return nullptr;
}

int LodObject::CanConvertToType(Class_ID) {
    return FALSE;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void LodObject::GetWorldBoundBox(TimeValue, INode * node, ViewExp *, Box3 & box) {
    const Matrix3 tm = node->GetObjectTM(GetCOREInterface()->GetTime());
    box = mIconMesh.getBoundingBox() * tm;
}

void LodObject::GetLocalBoundBox(TimeValue, INode *, ViewExp *, Box3 & box) {
    box = mIconMesh.getBoundingBox();
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

#if MAX_VERSION_MAJOR > 14
const MCHAR * LodObject::GetObjectName() { return _T("X-Lod"); }
#else
TCHAR * LodObject::GetObjectName() { return _T("X-Lod"); }
#endif

void LodObject::InitNodeName(TSTR & s) { s = _T("X-Lod-"); }

//-------------------------------------------------------------------------

Class_ID LodObject::ClassID() { return mDesc->ClassID(); }
SClass_ID LodObject::SuperClassID() { return mDesc->SuperClassID(); }
void LodObject::GetClassName(TSTR & s) { s = mDesc->ClassName(); }

RefTargetHandle LodObject::Clone(RemapDir & remap) {
    auto newObj = new LodObject();
    newObj->ReplaceReference(LodObjParamBlocks::PbOrderParams, mParamsPb->Clone(remap));
    newObj->ReplaceReference(LodObjParamBlocks::PbOrderDisplay, mDisplayPb->Clone(remap));
    BaseClone(this, newObj, remap);
    return newObj;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

Animatable * LodObject::SubAnim(const int i) {
    return GetParamBlock(i);
}

TSTR LodObject::SubAnimName(const int i) {
    switch (i) {
        case LodObjParamBlocks::PbOrderParams: return _T("Parameters");
        case LodObjParamBlocks::PbOrderDisplay: return _T("Display");
        default: return _T("");
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

int LodObject::GetParamBlockIndex(const int id) {
    return id;
}

int LodObject::NumParamBlocks() {
    return 2;
}

IParamBlock2 * LodObject::GetParamBlock(const int i) {
    switch (i) {
        case LodObjParamBlocks::PbOrderParams: return mParamsPb;
        case LodObjParamBlocks::PbOrderDisplay: return mDisplayPb;
        default: return nullptr;
    }
}

IParamBlock2 * LodObject::GetParamBlockByID(const BlockID id) {
    switch (id) {
        case LodObjParams: return mParamsPb;
        case LodObjDisplay: return mDisplayPb;
        default: return nullptr;
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

int LodObject::NumRefs() {
    return 2;
}

RefTargetHandle LodObject::GetReference(const int i) {
    switch (i) {
        case LodObjParamBlocks::PbOrderParams: return mParamsPb;
        case LodObjParamBlocks::PbOrderDisplay: return mDisplayPb;
        default: return nullptr;
    }
}

void LodObject::SetReference(const int i, const RefTargetHandle target) {
    switch (i) {
        case LodObjParamBlocks::PbOrderParams: {
            mParamsPb = static_cast<IParamBlock2*>(target);
            break;
        }
        case LodObjParamBlocks::PbOrderDisplay: {
            mDisplayPb = static_cast<IParamBlock2*>(target);
            break;
        }
        default: break;
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

#if MAX_VERSION_MAJOR > 16
RefResult LodObject::NotifyRefChanged(const Interval & /*changeInt*/, RefTargetHandle /*hTarget*/,
                                      PartID & /*partID*/, RefMessage message, BOOL /*propagate*/) {
#else
RefResult LodObject::NotifyRefChanged(Interval /*changeInt*/, RefTargetHandle /*hTarget*/,
                                      PartID & /*partID*/, const RefMessage message) {
#endif
    switch (message) {
        case REFMSG_CHANGE:
            if (mEditOb == this) {
                LodObjParamBlocks::mParams.InvalidateUI(mParamsPb->LastNotifyParamID());
                LodObjParamBlocks::mDisplay.InvalidateUI(mDisplayPb->LastNotifyParamID());
            }
            break;
        default: break;
    }
    return REF_SUCCEED;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void LodObject::GetMat(const TimeValue t, INode * node, ViewExp *, Matrix3 & tm) {
    tm = node->GetObjectTM(t);
    tm.NoScale();
}

int LodObject::HitTest(const TimeValue t, INode * node, const int type, const int crossing, const int flags, IPoint2 * p, ViewExp * vpt) {
    HitRegion hitRegion;
    DWORD savedLimits;
    const int res = 0;
    Matrix3 m;
    GraphicsWindow * gw = vpt->getGW();
    Material * mtl = gw->getMaterial();
    MakeHitRegion(hitRegion, type, crossing, 4, p);
    gw->setRndLimits(((savedLimits = gw->getRndLimits()) | GW_PICK) & ~GW_ILLUM);
    GetMat(t, node, vpt, m);
    gw->setTransform(m);
    // if we get a hit on the mIconMesh, we're done
    gw->clearHitCode();
    if (mIconMesh.select(gw, mtl, &hitRegion, flags & HIT_ABORTONHIT))
        return TRUE;
    // if not, check the target line, and set the pair flag if it's hit
    gw->clearHitCode();
    gw->setRndLimits(savedLimits);
    return res;
}

//-------------------------------------------------------------------------

int LodObject::UsesWireColor() {
    return TRUE;
}

//-------------------------------------------------------------------------

int LodObject::Display(const TimeValue t, INode * node, ViewExp * vpt, int /*flags*/) {
    GraphicsWindow * gw = vpt->getGW();
    Material * mtl = gw->getMaterial();
    const Color color(node->GetWireColor());
    mObjColor.x = color.r;
    mObjColor.y = color.g;
    mObjColor.z = color.b;
    gw->setTransform(node->GetNodeTM(t));
    //-------------------------------------------------------------------------
    const DWORD limits = gw->getRndLimits();
    gw->setRndLimits(GW_WIREFRAME | GW_EDGES_ONLY | GW_BACKCULL | (limits & GW_Z_BUFFER));

    if (node->Selected()) {
        gw->setColor(LINE_COLOR, GetSelColor());
    }
    else if (!node->IsFrozen() && !node->Dependent()) {
        gw->setColor(LINE_COLOR, mObjColor);
    }
    mIconMesh.render(gw, mtl, nullptr, COMP_ALL);
    gw->setRndLimits(limits);
    //-------------------------------------------------------------------------
    return 0;
}

//-------------------------------------------------------------------------

void LodObject::makeIcon() {
    float size = 1.0f;
    Interval interval = FOREVER;
    if (!mDisplayPb->GetValue(PLodObjIconScale, mIp ? mIp->GetTime() : 0, size, interval)) {
        LError << "Can't retrieve scale value from param block";
    }

    if (stsff::math::isEqual(mLastIconScale, size, 0.001f) && mIconMesh.getNumVerts() != 0) {
        return;
    }

    auto masterScale = static_cast<float>(GetMasterScale(UNITS_METERS));
    if (masterScale != -1.0f) {
        masterScale = 1.0f / masterScale;
        size = size * masterScale;
        if (size < 0.00001f) {
            size = 0.00001f;
            LError << "The icon scale is too small";
        }
    }
    // todo make icon with the Mesh2Cpp
    mLastIconScale = size;
    LodIcon::fillMesh(mIconMesh, size);
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
