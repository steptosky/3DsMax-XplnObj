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
#include <vector>
#include "common/Logger.h"
#include "common/String.h"
#include "classes-desc/ClassesDescriptions.h"
#include "additional/math/Compare.h"
#include "LodObjParamBlocks.h"
#include "LodIcon-gen.h"

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

#if MAX_VERSION_MAJOR < 15
#	define p_end end
#endif

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

#define PARAMS_PB_VERSION 1
#define DISPLAY_PB_VERSION 1

enum {
    LodObjParamsOrder = LodObjParams,
    LodObjDisplayOrder = LodObjDisplay,
};

enum eLodObjDisplay : ParamID {
    PLodObjIconScale,
};

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

class LodObjectPostLoadCallback : public PostLoadCallback {
public:

    virtual ~LodObjectPostLoadCallback() {}

    LodObject * pobj;

    explicit LodObjectPostLoadCallback(LodObject * p) {
        pobj = p;
    }

    void proc(ILoad * /*iload*/) override {
        pobj->makeIcon();
    }
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

LodObject::~LodObject() {}

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

    float mVals[2];
    bool mIgnoreTransform;

    stream >> mVals[0];
    stream >> mVals[1];
    stream >> mIgnoreTransform;

    if (!mParamsPb->SetValue(PLodObjNear, mIp ? mIp->GetTime() : 0, mVals[0])) {
        DLError << "Can't save " << TOTEXT(PLodObjNear) << " value to the param block";
        return;
    }

    if (!mParamsPb->SetValue(PLodObjFar, mIp ? mIp->GetTime() : 0, mVals[1])) {
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

IOResult LodObject::Load(ILoad * iload) {
    ULONG temp;
    uint32_t dataSize = 0;
    IOResult res;
    std::vector<char> ba;
    while ((res = iload->OpenChunk()) == IO_OK) {
        if (res == IO_ERROR)
            return res;
        switch (iload->CurChunkID()) {
            case 0:
                iload->Read(&dataSize, sizeof(dataSize), &temp);
                break;
            case 1:
                if (dataSize != 0) {
                    ba.resize(dataSize);
                    iload->Read(ba.data(), dataSize, &temp);
                }
                break;
            default: break;
        }
        iload->CloseChunk();
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
    iload->RegisterPostLoadCallback(new LodObjectPostLoadCallback(this));
    return IO_OK;
}

IOResult LodObject::Save(ISave * /*isave*/) {
    return IO_OK;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void LodObject::BeginEditParams(IObjParam * ip, ULONG flags, Animatable * prev) {
    mIp = ip;
    mEditOb = this;
    mDesc->BeginEditParams(ip, this, flags, prev);
}

void LodObject::EndEditParams(IObjParam * ip, ULONG flags, Animatable * next) {
    mEditOb = nullptr;
    mIp = nullptr;
    mDesc->EndEditParams(ip, this, flags, next);
    ClearAFlag(A_OBJ_CREATING);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

ObjectState LodObject::Eval(TimeValue) { return ObjectState(this); }
Object * LodObject::ConvertToType(TimeValue, Class_ID) { return nullptr; }
int LodObject::CanConvertToType(Class_ID) { return FALSE; }

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void LodObject::GetWorldBoundBox(TimeValue, INode * inode, ViewExp *, Box3 & box) {
    Matrix3 tm = inode->GetObjectTM(GetCOREInterface()->GetTime());
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
    LodObject * newob = new LodObject();
    newob->ReplaceReference(LodObjParamsOrder, mParamsPb->Clone(remap));
    newob->ReplaceReference(LodObjDisplayOrder, mDisplayPb->Clone(remap));
    BaseClone(this, newob, remap);
    return (newob);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

Animatable * LodObject::SubAnim(int i) { return GetParamBlock(i); }

TSTR LodObject::SubAnimName(int i) {
    switch (i) {
        case LodObjParamsOrder: return _T("Parameters");
        case LodObjDisplayOrder: return _T("Display");
        default: return _T("");
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

int LodObject::GetParamBlockIndex(int id) { return id; }
int LodObject::NumParamBlocks() { return 2; }

IParamBlock2 * LodObject::GetParamBlock(int i) {
    switch (i) {
        case LodObjParamsOrder: return mParamsPb;
        case LodObjDisplayOrder: return mDisplayPb;
        default: return nullptr;
    }
}

IParamBlock2 * LodObject::GetParamBlockByID(BlockID id) {
    switch (id) {
        case LodObjParams: return mParamsPb;
        case LodObjDisplay: return mDisplayPb;
        default: return nullptr;
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

int LodObject::NumRefs() { return 2; }

RefTargetHandle LodObject::GetReference(int i) {
    switch (i) {
        case LodObjParamsOrder: return mParamsPb;
        case LodObjDisplayOrder: return mDisplayPb;
        default: return nullptr;
    }
}

void LodObject::SetReference(int i, RefTargetHandle rtarg) {
    switch (i) {
        case LodObjParamsOrder: {
            mParamsPb = static_cast<IParamBlock2*>(rtarg);
            break;
        }
        case LodObjDisplayOrder: {
            mDisplayPb = static_cast<IParamBlock2*>(rtarg);
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
                                      PartID & /*partID*/, RefMessage message) {
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

void LodObject::GetMat(TimeValue t, INode * inode, ViewExp *, Matrix3 & tm) {
    tm = inode->GetObjectTM(t);
    tm.NoScale();
}

int LodObject::HitTest(TimeValue t, INode * inode, int type, int crossing, int flags, IPoint2 * p, ViewExp * vpt) {
    HitRegion hitRegion;
    DWORD savedLimits;
    int res = 0;
    Matrix3 m;
    GraphicsWindow * gw = vpt->getGW();
    Material * mtl = gw->getMaterial();
    MakeHitRegion(hitRegion, type, crossing, 4, p);
    gw->setRndLimits(((savedLimits = gw->getRndLimits()) | GW_PICK) & ~GW_ILLUM);
    GetMat(t, inode, vpt, m);
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

int LodObject::UsesWireColor() { return TRUE; }

//-------------------------------------------------------------------------

int LodObject::Display(TimeValue t, INode * inode, ViewExp * vpt, int /*flags*/) {
    GraphicsWindow * gw = vpt->getGW();
    Material * mtl = gw->getMaterial();
    Color color(inode->GetWireColor());
    mObjColor.x = color.r;
    mObjColor.y = color.g;
    mObjColor.z = color.b;
    gw->setTransform(inode->GetNodeTM(t));
    //-------------------------------------------------------------------------
    DWORD rlim = gw->getRndLimits();
    gw->setRndLimits(GW_WIREFRAME | GW_EDGES_ONLY | GW_BACKCULL | (rlim & GW_Z_BUFFER));

    if (inode->Selected()) {
        gw->setColor(LINE_COLOR, GetSelColor());
    }
    else if (!inode->IsFrozen() && !inode->Dependent()) {
        gw->setColor(LINE_COLOR, mObjColor);
    }
    mIconMesh.render(gw, mtl, nullptr, COMP_ALL);
    gw->setRndLimits(rlim);
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

    float masterScale = static_cast<float>(GetMasterScale(UNITS_METERS));
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

