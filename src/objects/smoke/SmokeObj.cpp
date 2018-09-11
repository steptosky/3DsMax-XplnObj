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

#include "SmokeObj.h"

#include "SmokeObjParams.h"
#include "common/Logger.h"
#include "classes-desc/ClassesDescriptions.h"
#include "SmokeIcon-gen.h"
#include "additional/math/Compare.h"
#include "SmokeObjParamBlocks.h"

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

class SmokeObjectPostLoadCallback : public PostLoadCallback {
public:

    virtual ~SmokeObjectPostLoadCallback() {}

    SmokeObject * pobj;

    explicit SmokeObjectPostLoadCallback(SmokeObject * p) {
        pobj = p;
    }

    void proc(ILoad * /*iload*/) override {
        pobj->makeIcon();
    }
};

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

MouseCallback SmokeObject::mMouseCallback;
SmokeObject * SmokeObject::mEditOb = nullptr;

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

SmokeObject::SmokeObject() {
    mDesc = ClassesDescriptions::smokeObj();
    DbgAssert(mDesc);
    mDesc->MakeAutoParamBlocks(this);
    mObjColor = Point3(1.0, 0.7, 0.4);
    makeIcon();
}

SmokeObject::~SmokeObject() {}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

CreateMouseCallBack * SmokeObject::GetCreateMouseCallBack() { return &mMouseCallback; }

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

IOResult SmokeObject::Load(ILoad * iload) {
    iload->RegisterPostLoadCallback(new SmokeObjectPostLoadCallback(this));
    return IO_OK;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void SmokeObject::BeginEditParams(IObjParam * ip, ULONG flags, Animatable * prev) {
    mIp = ip;
    mEditOb = this;
    mDesc->BeginEditParams(ip, this, flags, prev);
}

void SmokeObject::EndEditParams(IObjParam * ip, ULONG flags, Animatable * next) {
    mEditOb = nullptr;
    mIp = nullptr;
    mDesc->EndEditParams(ip, this, flags, next);
    ClearAFlag(A_OBJ_CREATING);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

ObjectState SmokeObject::Eval(TimeValue) { return ObjectState(this); }
Object * SmokeObject::ConvertToType(TimeValue, Class_ID) { return nullptr; }
int SmokeObject::CanConvertToType(Class_ID) { return FALSE; }

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void SmokeObject::GetWorldBoundBox(TimeValue, INode * inode, ViewExp *, Box3 & box) {
    Matrix3 tm = inode->GetObjectTM(GetCOREInterface()->GetTime());
    box = mIconMesh.getBoundingBox() * tm;
}

void SmokeObject::GetLocalBoundBox(TimeValue, INode *, ViewExp *, Box3 & box) {
    box = mIconMesh.getBoundingBox();
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

#if MAX_VERSION_MAJOR > 14
const MCHAR * SmokeObject::GetObjectName() { return _T("X-Smoke"); }
#else
TCHAR * SmokeObject::GetObjectName() { return _T("X-Smoke"); }
#endif

void SmokeObject::InitNodeName(TSTR & s) { s = _T("X-Smoke-"); }

//-------------------------------------------------------------------------

Class_ID SmokeObject::ClassID() { return mDesc->ClassID(); }
SClass_ID SmokeObject::SuperClassID() { return mDesc->SuperClassID(); }
void SmokeObject::GetClassName(TSTR & s) { s = mDesc->ClassName(); }

RefTargetHandle SmokeObject::Clone(RemapDir & remap) {
    SmokeObject * newob = new SmokeObject();
    newob->ReplaceReference(SmokeObjParamBlocks::PbOrderParams, mParamsPb->Clone(remap));
    newob->ReplaceReference(SmokeObjParamBlocks::PbOrderDisplay, mDisplayPb->Clone(remap));
    BaseClone(this, newob, remap);
    return newob;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

Animatable * SmokeObject::SubAnim(int i) { return GetParamBlock(i); }

TSTR SmokeObject::SubAnimName(int i) {
    switch (i) {
        case SmokeObjParamBlocks::PbOrderParams: return _T("Parameters");
        case SmokeObjParamBlocks::PbOrderDisplay: return _T("Display");
        default: return _T("");
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

int SmokeObject::GetParamBlockIndex(int id) { return id; }
int SmokeObject::NumParamBlocks() { return 2; }

IParamBlock2 * SmokeObject::GetParamBlock(int i) {
    switch (i) {
        case SmokeObjParamBlocks::PbOrderParams: return mParamsPb;
        case SmokeObjParamBlocks::PbOrderDisplay: return mDisplayPb;
        default: return nullptr;
    }
}

IParamBlock2 * SmokeObject::GetParamBlockByID(BlockID id) {
    switch (id) {
        case SmokeObjParams: return mParamsPb;
        case SmokeObjDisplay: return mDisplayPb;
        default: return nullptr;
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

int SmokeObject::NumRefs() { return 2; }

RefTargetHandle SmokeObject::GetReference(int i) {
    switch (i) {
        case SmokeObjParamBlocks::PbOrderParams: return mParamsPb;
        case SmokeObjParamBlocks::PbOrderDisplay: return mDisplayPb;
        default: return nullptr;
    }
}

void SmokeObject::SetReference(int i, RefTargetHandle rtarg) {
    switch (i) {
        case SmokeObjParamBlocks::PbOrderParams: {
            mParamsPb = static_cast<IParamBlock2*>(rtarg);
            break;
        }
        case SmokeObjParamBlocks::PbOrderDisplay: {
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
RefResult SmokeObject::NotifyRefChanged(const Interval & /*changeInt*/, RefTargetHandle /*hTarget*/,
                                        PartID & /*partID*/, RefMessage message, BOOL /*propagate*/) {
#else
RefResult SmokeObject::NotifyRefChanged(Interval /*changeInt*/, RefTargetHandle /*hTarget*/,
                                        PartID & /*partID*/, RefMessage message) {
#endif
    switch (message) {
        case REFMSG_CHANGE:
            if (mEditOb == this) {
                SmokeObjParamBlocks::mParams.InvalidateUI(mParamsPb->LastNotifyParamID());
                SmokeObjParamBlocks::mDisplay.InvalidateUI(mDisplayPb->LastNotifyParamID());
            }
            break;
        default: break;
    }
    return REF_SUCCEED;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void SmokeObject::GetMat(TimeValue t, INode * inode, ViewExp *, Matrix3 & tm) {
    tm = inode->GetObjectTM(t);
    tm.NoScale();
}

int SmokeObject::HitTest(TimeValue t, INode * inode, int type, int crossing, int flags, IPoint2 * p, ViewExp * vpt) {
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

int SmokeObject::UsesWireColor() {
    return TRUE;
}

//-------------------------------------------------------------------------

int SmokeObject::Display(TimeValue t, INode * inode, ViewExp * vpt, int /*flags*/) {
    GraphicsWindow * gw = vpt->getGW();
    Material * mtl = gw->getMaterial();
    Color color(inode->GetWireColor());
    mObjColor.x = color.r;
    mObjColor.y = color.g;
    mObjColor.z = color.b;
    gw->setTransform(inode->GetObjectTM(t));
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

void SmokeObject::makeIcon() {
    float size = 1.0f;
    Interval interval = FOREVER;
    if (!mDisplayPb->GetValue(PSmokeObjIconScale, mIp ? mIp->GetTime() : 0, size, interval)) {
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

    mLastIconScale = size;
    SmokeIcon::fillMesh(mIconMesh, size);
}

/*************************************************************************************************
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
