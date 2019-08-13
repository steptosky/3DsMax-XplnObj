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

#include "MainObj.h"

#include <xpln/enums/ELayer.h>

#include "objects/main/param-blocks/MainObjParams.h"
#include "common/Logger.h"
#include "common/String.h"
#include "MainObjParamsWrapper.h"
#include "classes-desc/ClassesDescriptions.h"
#include "additional/math/Compare.h"
#include "MainObjIcon-gen.h"
#include "param-blocks/MainObjPbAttr.h"
#include "param-blocks/MainObjPbDisplay.h"
#include "param-blocks/MainObjPbGeometry.h"

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

class MainObjectPostLoadCallback : public PostLoadCallback {
public:

    explicit MainObjectPostLoadCallback(MainObject * obj)
        : mObj(obj) { }

    virtual ~MainObjectPostLoadCallback() = default;

    //-------------------------------------------------------------------------

    void proc(ILoad *) override {
        mObj->makeIcon();
        MainObjPbAttr::postLoad(mObj->mAttrParamsPb);
        MainObjPbGeometry::postLoad(mObj->mGeomPb);
        MainObjPbDisplay::postLoad(mObj->mDisplayPb);
        delete this;
    }

    //-------------------------------------------------------------------------

    MainObject * mObj;
};

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

MouseCallback MainObject::mMouseCallback;
MainObject * MainObject::mEditOb = nullptr;

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

MainObject::MainObject() {
    mDesc = ClassesDescriptions::mainObj();
    mDesc->MakeAutoParamBlocks(this);
    mObjColor = Point3(1.0, 0.7, 0.4);
    makeIcon();
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

CreateMouseCallBack * MainObject::GetCreateMouseCallBack() {
    return &mMouseCallback;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void MainObject::updateTexturesButtons() const {
    MainObjParamsWrapper wrapper(mAttrParamsPb, mGeomPb, GetCOREInterface()->GetTime(), FOREVER);
    updateButtonText(mAttrParamsPb, MainObjAttr_Texture, sts::toString(wrapper.texture().value_or(std::string())).c_str());
    updateButtonText(mAttrParamsPb, MainObjAttr_TextureLit, sts::toString(wrapper.textureLit().value_or(std::string())).c_str());
    updateButtonText(mAttrParamsPb, MainObjAttr_TextureNormal, sts::toString(wrapper.textureNormal().value_or(std::string())).c_str());
}

void MainObject::updateButtonText(IParamBlock2 * pBlock, const ParamID param, const MCHAR * value) {
    IParamMap2 * map = pBlock->GetMap(MainObjPbAttr::RollGlobShading);
    //DbgAssert(map);
    if (map) {
        TSTR p, f, e, name;
        ParamDef & mapDef = pBlock->GetParamDef(param);
        static TCHAR cubeMapFile[MAX_PATH];
        _tcscpy(cubeMapFile, value ? value : _M("none"));
        TSTR tooltip = cubeMapFile;
        mapDef.init_file = cubeMapFile;
        TSTR tStr(cubeMapFile);
        SplitFilename(tStr, &p, &f, &e);
        name = f + e;
        map->SetText(param, name);
        map->SetTooltip(param, true, tooltip);
    }
}

//-------------------------------------------------------------------------

void MainObject::updateBlendSpinEnabling() const {
    IParamMap2 * map = mAttrParamsPb->GetMap(MainObjPbAttr::RollGlobShading);
    //DbgAssert(map);
    if (map) {
        MainObjParamsWrapper wrapper(mAttrParamsPb, nullptr, GetCOREInterface()->GetTime(), FOREVER);
        map->Enable(MainObjAttr_BlendingRatio, wrapper.blend() ? 1 : 0);
    }
}

//-------------------------------------------------------------------------

void MainObject::updateLayerGroupSpinEnabling() const {
    IParamMap2 * map = mAttrParamsPb->GetMap(MainObjPbAttr::RollGlobAttr);
    //DbgAssert(map);
    if (map) {
        MainObjParamsWrapper wrapper(mAttrParamsPb, nullptr, GetCOREInterface()->GetTime(), FOREVER);
        map->Enable(MainObjAttr_LayerGroupOffset, wrapper.layerGroup().value_or(xobj::AttrLayerGroup()).mLayer.isValid() ? 1 : 0);
        map->Enable(MainObjAttr_LayerGroupDrapedOffset, wrapper.drapedLayerGroup().value_or(xobj::AttrDrapedLayerGroup()).mLayer.isValid() ? 1 : 0);
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

IOResult MainObject::Load(ILoad * load) {
    ULONG temp;
    uint32_t dataSize = 0;
    IOResult res;
    std::vector<char> ba;
    while ((res = load->OpenChunk()) == IO_OK) {
        switch (load->CurChunkID()) {
            case 0:
                load->Read(&dataSize, sizeof(dataSize), &temp);
                break;
            case 1: {
                if (dataSize != 0) {
                    ba.resize(dataSize);
                    load->Read(ba.data(), dataSize, &temp);
                }
                break;
            }
            default: break;
        }
        load->CloseChunk();
        if (res != IO_OK)
            return res;
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

    load->RegisterPostLoadCallback(new MainObjectPostLoadCallback(this));
    return IO_OK;
}

IOResult MainObject::Save(ISave *) {
    return IO_OK;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void MainObject::BeginEditParams(IObjParam * ip, const ULONG flags, Animatable * prev) {
    mIp = ip;
    mEditOb = this;
    mDesc->BeginEditParams(ip, this, flags, prev);
    updateTexturesButtons();
    updateBlendSpinEnabling();
    updateLayerGroupSpinEnabling();
}

void MainObject::EndEditParams(IObjParam * ip, const ULONG flags, Animatable * next) {
    mEditOb = nullptr;
    mIp = nullptr;
    mDesc->EndEditParams(ip, this, flags, next);
    ClearAFlag(A_OBJ_CREATING);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

ObjectState MainObject::Eval(TimeValue) {
    return ObjectState(this);
}

Object * MainObject::ConvertToType(TimeValue, Class_ID) {
    return nullptr;
}

int MainObject::CanConvertToType(Class_ID) {
    return FALSE;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void MainObject::GetWorldBoundBox(TimeValue, INode * node, ViewExp *, Box3 & box) {
    const Matrix3 tm = node->GetObjectTM(GetCOREInterface()->GetTime());
    box = mIconMesh.getBoundingBox() * tm;
}

void MainObject::GetLocalBoundBox(TimeValue, INode *, ViewExp *, Box3 & box) {
    box = mIconMesh.getBoundingBox();
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

#if MAX_VERSION_MAJOR > 14
const MCHAR * MainObject::GetObjectName() { return _T("X-Obj"); }
#else
TCHAR * MainObject::GetObjectName() { return _T("X-Obj"); }
#endif

void MainObject::InitNodeName(TSTR & s) { s = _T("X-Obj-"); }

//-------------------------------------------------------------------------

Class_ID MainObject::ClassID() {
    return mDesc->ClassID();
}

SClass_ID MainObject::SuperClassID() {
    return mDesc->SuperClassID();
}

void MainObject::GetClassName(TSTR & s) {
    s = mDesc->ClassName();
}

RefTargetHandle MainObject::Clone(RemapDir & remap) {
    auto newObj = new MainObject();
    newObj->ReplaceReference(static_cast<int>(eMainObjPbOrder::PbOrderAttr), mAttrParamsPb->Clone(remap));
    newObj->ReplaceReference(static_cast<int>(eMainObjPbOrder::PbOrderGeometry), mGeomPb->Clone(remap));
    newObj->ReplaceReference(static_cast<int>(eMainObjPbOrder::PbOrderDisplay), mDisplayPb->Clone(remap));
    BaseClone(this, newObj, remap);
    return (newObj);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

Animatable * MainObject::SubAnim(const int i) {
    return GetParamBlock(i);
}

TSTR MainObject::SubAnimName(const int i) {
    switch (static_cast<eMainObjPbOrder>(i)) {
        case eMainObjPbOrder::PbOrderAttr: return _T("Attributes");
        case eMainObjPbOrder::PbOrderGeometry: return _T("Options");
        case eMainObjPbOrder::PbOrderDisplay: return _T("Display");
        default: return _T("");
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

int MainObject::GetParamBlockIndex(const int id) {
    return id;
}

int MainObject::NumParamBlocks() {
    return 3;
}

IParamBlock2 * MainObject::GetParamBlock(const int i) {
    switch (static_cast<eMainObjPbOrder>(i)) {
        case eMainObjPbOrder::PbOrderAttr: return mAttrParamsPb;
        case eMainObjPbOrder::PbOrderGeometry: return mGeomPb;
        case eMainObjPbOrder::PbOrderDisplay: return mDisplayPb;
        default: return nullptr;
    }
}

IParamBlock2 * MainObject::GetParamBlockByID(const BlockID id) {
    switch (static_cast<eMainObjParamsBlocks>(id)) {
        case eMainObjParamsBlocks::MainObjAttrParams: return mAttrParamsPb;
        case eMainObjParamsBlocks::MainObjGeometryParams: return mGeomPb;
        case eMainObjParamsBlocks::MainObjDisplay: return mDisplayPb;
        default: return nullptr;
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

int MainObject::NumRefs() {
    return 3;
}

RefTargetHandle MainObject::GetReference(const int i) {
    switch (static_cast<eMainObjPbOrder>(i)) {
        case eMainObjPbOrder::PbOrderAttr: return mAttrParamsPb;
        case eMainObjPbOrder::PbOrderGeometry: return mGeomPb;
        case eMainObjPbOrder::PbOrderDisplay: return mDisplayPb;
        default: return nullptr;
    }
}

void MainObject::SetReference(const int i, RefTargetHandle target) {
    switch (static_cast<eMainObjPbOrder>(i)) {
        case eMainObjPbOrder::PbOrderAttr: {
            mAttrParamsPb = static_cast<IParamBlock2*>(target);
            break;
        }
        case eMainObjPbOrder::PbOrderGeometry: {
            mGeomPb = static_cast<IParamBlock2*>(target);
            break;
        }
        case eMainObjPbOrder::PbOrderDisplay: {
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
RefResult MainObject::NotifyRefChanged(const Interval & /*changeInt*/, RefTargetHandle /*hTarget*/,
                                       PartID & /*partId*/, RefMessage message, BOOL /*propagate*/) {
#else
RefResult MainObject::NotifyRefChanged(Interval /*changeInt*/, RefTargetHandle /*hTarget*/,
                                       PartID & /*partId*/, const RefMessage message) {
#endif
    switch (message) {
        case REFMSG_CHANGE:
            if (mEditOb == this) {
                MainObjPbAttr::mPb.InvalidateUI(mAttrParamsPb->LastNotifyParamID());
                MainObjPbGeometry::mPb.InvalidateUI(mGeomPb->LastNotifyParamID());
                MainObjPbDisplay::mPb.InvalidateUI(mDisplayPb->LastNotifyParamID());
            }
            break;
        default: break;
    }
    return REF_SUCCEED;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

int MainObject::HitTest(const TimeValue t, INode * node, const int type, const int crossing, const int flags, IPoint2 * p, ViewExp * vpt) {
    HitRegion hitRegion;
    DWORD savedLimits;
    const int res = 0;
    GraphicsWindow * gw = vpt->getGW();
    Material * mtl = gw->getMaterial();
    MakeHitRegion(hitRegion, type, crossing, 4, p);
    gw->setRndLimits(((savedLimits = gw->getRndLimits()) | GW_PICK) & ~GW_ILLUM);
    const Matrix3 m = node->GetObjectTM(t);
    gw->setTransform(m);
    gw->clearHitCode();
    if (mIconMesh.select(gw, mtl, &hitRegion, flags & HIT_ABORTONHIT)) {
        return TRUE;
    }
    gw->clearHitCode();
    gw->setRndLimits(savedLimits);
    return res;
}

//-------------------------------------------------------------------------

int MainObject::UsesWireColor() {
    return TRUE;
}

//-------------------------------------------------------------------------

int MainObject::Display(const TimeValue t, INode * node, ViewExp * vpt, int /*flags*/) {
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

void MainObject::makeIcon() {
    float size = 1.0f;
    Interval interval = FOREVER;
    if (!mDisplayPb->GetValue(MainObjDisp_IconScale, mIp ? mIp->GetTime() : 0, size, interval)) {
        XLError << "Can't retrieve scale value from param block";
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
            XLError << "The icon scale is too small";
        }
    }

    mLastIconScale = size;
    MainObjIcon::fillMesh(mIconMesh, size);
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
