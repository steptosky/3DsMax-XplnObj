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

#include "MainObj.h "

#include <Windows.h>
#include <xpln/enums/ELayer.h>
#include <xpln/obj/attributes/AttrBlend.h>
#include <xpln/obj/attributes/AttrWetDry.h>

#include "MainObjectParams.h"
#include "common/Logger.h"
#include "common/String.h"
#include "models/bwc/SerializationId.h"
#include "ui-win/DlgMessageBox.h"
#include "MainObjParamsWrapper.h"
#include "classes-desc/ClassesDescriptions.h"
#include "additional/math/Compare.h"
#include "MainObjIcon-gen.h"
#include "MainObjParamBlocks.h"

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

class MainObjectPostLoadCallback : public PostLoadCallback {
public:

    virtual ~MainObjectPostLoadCallback() {}

    MainObject * pobj;

    explicit MainObjectPostLoadCallback(MainObject * p) {
        pobj = p;
    }

    void proc(ILoad * /*iload*/) override {
        pobj->makeIcon();
    }
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

MainObject::~MainObject() {}

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
    MainObjParamsWrapper wrapper(mAttrParamsPb, mExpPb, GetCOREInterface()->GetTime(), FOREVER);
    updateButtonText(mAttrParamsPb, MainObjAttr_Texture, sts::toString(wrapper.texture()).c_str());
    updateButtonText(mAttrParamsPb, MainObjAttr_TextureLit, sts::toString(wrapper.textureLit()).c_str());
    updateButtonText(mAttrParamsPb, MainObjAttr_TextureNormal, sts::toString(wrapper.textureNormal()).c_str());
}

void MainObject::updateButtonText(IParamBlock2 * pblock, ParamID param, const MCHAR * value) {
    IParamMap2 * map = pblock->GetMap(MainObjParamBlocks::RollGlobShading);
    //DbgAssert(map);
    if (map) {
        TSTR p, f, e, name;
        ParamDef & mapdef = pblock->GetParamDef(param);
        static TCHAR cubemapFile[MAX_PATH];
        _tcscpy(cubemapFile, value ? value : _M("none"));
        TSTR tooltip = cubemapFile;
        mapdef.init_file = cubemapFile;
        TSTR tstr(cubemapFile);
        SplitFilename(tstr, &p, &f, &e);
        name = f + e;
        map->SetText(param, name);
        map->SetTooltip(param, true, tooltip);
    }
}

//-------------------------------------------------------------------------

void MainObject::updateBlendSpinEnabling() const {
    IParamMap2 * map = mAttrParamsPb->GetMap(MainObjParamBlocks::RollGlobShading);
    //DbgAssert(map);
    if (map) {
        MainObjParamsWrapper wrapper(mAttrParamsPb, nullptr, GetCOREInterface()->GetTime(), FOREVER);
        map->Enable(MainObjAttr_BlendingRatio, wrapper.blend() ? 1 : 0);
    }
}

//-------------------------------------------------------------------------

void MainObject::updateLyerGroupSpinEnabling() const {
    IParamMap2 * map = mAttrParamsPb->GetMap(MainObjParamBlocks::RollGlobAttr);
    //DbgAssert(map);
    if (map) {
        MainObjParamsWrapper wrapper(mAttrParamsPb, nullptr, GetCOREInterface()->GetTime(), FOREVER);
        map->Enable(MainObjAttr_LayerGroupOffset, wrapper.layerGroup().layer().isValid() ? 1 : 0);
        map->Enable(MainObjAttr_LayerGroupDrapedOffset, wrapper.drapedLayerGroup().layer().isValid() ? 1 : 0);
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void MainObject::loadMdDisplayObj(sts_bwc::DataStream & stream) const {
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
    if (!mDisplayPb->SetValue(MainObjDisp_IconScale, mIp ? mIp->GetTime() : 0, iconSize)) {
        DLError << "Can't save " << TOTEXT(MainObjDisp_IconScale) << " value to the param block";
    }
}

void MainObject::loadRawGlobAttr(sts_bwc::DataStream & stream) const {
    bcw::SerializationId id;
    id.load(stream.getStdStream());
    if (id != bcw::SerializationId(0x37af3f56, 0x7eae24ae)) {
        LError << "Unexpected data input: " << id.toString();
        return;
    }
    unsigned char version;
    stream >> version;
    if (version != 1) {
        LError << "Unexpected data version: " << version;
        return;
    }

    struct {
        double noBlendRatio;
        double shadowBlendRatio;
        double specularRatio;

        double tintAlbedo;
        double tintEmissive;

        double slopePitchMax;
        double slopePitchMin;
        double slopeRollMax;
        double slopeRollhMin;

        double slungLoad;
        int layerGroup;
        double lodDrapedDist;
        int drapedLayerGroup;

        std::wstring strLayerGrpup;
        std::wstring strDrapedLayerGrpup;

        bool chkNoShadow;
        bool chkTilted;
        bool chkCockpitLit;
        bool chkDry;
        bool chkWet;

        bool chkNoBlend;
        bool chkShadowBlend;
        bool chkSpecular;
        bool chkTint;
        bool chkSlopeLimit;
        bool chkSlungLoad;
        bool chkLodDraped;

        bool chkLayerGroup;
        bool chkDrapedLayerGroup;

        std::wstring strDiffuse;
        std::wstring strLit;
        std::wstring strBump;
    } data;

    stream >> data.noBlendRatio;
    stream >> data.shadowBlendRatio;
    stream >> data.specularRatio;

    stream >> data.tintAlbedo;
    stream >> data.tintEmissive;

    stream >> data.slopePitchMax;
    stream >> data.slopePitchMin;
    stream >> data.slopeRollMax;
    stream >> data.slopeRollhMin;

    stream >> data.slungLoad;
    stream >> data.layerGroup;
    stream >> data.lodDrapedDist;
    stream >> data.drapedLayerGroup;

    stream >> data.strLayerGrpup;
    stream >> data.strDrapedLayerGrpup;

    stream >> data.chkNoShadow;
    stream >> data.chkTilted;
    stream >> data.chkCockpitLit;
    stream >> data.chkDry;
    stream >> data.chkWet;

    stream >> data.chkNoBlend;
    stream >> data.chkShadowBlend;
    stream >> data.chkSpecular;
    stream >> data.chkTint;
    stream >> data.chkSlopeLimit;
    stream >> data.chkSlungLoad;
    stream >> data.chkLodDraped;

    stream >> data.chkLayerGroup;
    stream >> data.chkDrapedLayerGroup;

    stream >> data.strDiffuse;
    stream >> data.strLit;
    stream >> data.strBump;

    //-------------------------------------------------------------------------
    if (data.chkNoBlend && data.chkShadowBlend) {
        std::string msg("\"shadow blend\" and \"no blend\" can not be enabled at same time. Only data about \"no blend\" will be loaded.");
        ui::win::DlgMessageBox::warning(nullptr, "[Backward Compatibility]", msg);
        LWarning << msg;
    }
    MainObjParamsWrapper wrapper(mAttrParamsPb, mExpPb, TimeValue(0), FOREVER);
    if (data.chkNoBlend) {
        wrapper.setBlend(xobj::AttrBlend(xobj::AttrBlend::no_blend, float(data.noBlendRatio)));
    }
    else if (data.chkShadowBlend) {
        wrapper.setBlend(xobj::AttrBlend(xobj::AttrBlend::shadow_blend, float(data.noBlendRatio)));
    }
    else {
        wrapper.setBlend(xobj::AttrBlend());
    }
    //-------------------------------------------------------------------------
    if (data.chkSpecular) {
        wrapper.setSpecular(xobj::AttrSpecular(float(data.specularRatio)));
    }
    else {
        wrapper.setSpecular(xobj::AttrSpecular());
    }
    //-------------------------------------------------------------------------
    if (data.chkTint) {
        wrapper.setTint(xobj::AttrTint(float(data.tintAlbedo), float(data.tintEmissive)));
    }
    else {
        wrapper.setTint(xobj::AttrTint());
    }
    //-------------------------------------------------------------------------
    if (data.chkSlopeLimit) {
        wrapper.setSlopeLimit(xobj::AttrSlopeLimit(float(data.slopePitchMin), float(data.slopePitchMax),
                                                   float(data.slopeRollhMin), float(data.slopeRollMax)));
    }
    else {
        wrapper.setSlopeLimit(xobj::AttrSlopeLimit());
    }
    //-------------------------------------------------------------------------
    if (data.chkSlungLoad) {
        wrapper.setSlungWeight(xobj::AttrSlungLoadWeight(float(data.slungLoad)));
    }
    else {
        wrapper.setSlungWeight(xobj::AttrSlungLoadWeight());
    }
    //-------------------------------------------------------------------------
    if (data.chkLodDraped) {
        wrapper.setLodDrap(xobj::AttrLodDrap(float(data.lodDrapedDist)));
    }
    else {
        wrapper.setLodDrap(xobj::AttrLodDrap());
    }
    //-------------------------------------------------------------------------
    if (data.chkLayerGroup) {
        wrapper.setLayerGroup(xobj::AttrLayerGroup(xobj::ELayer::fromString(sts::toMbString(data.strLayerGrpup).c_str()),
                                                   int(data.layerGroup)));
    }
    else {
        wrapper.setLayerGroup(xobj::AttrLayerGroup());
    }
    //-------------------------------------------------------------------------
    if (data.chkDrapedLayerGroup) {
        wrapper.setDrapedLayerGroup(xobj::AttrDrapedLayerGroup(xobj::ELayer::fromString(sts::toMbString(data.strDrapedLayerGrpup).c_str()),
                                                               int(data.drapedLayerGroup)));
    }
    else {
        wrapper.setDrapedLayerGroup(xobj::AttrDrapedLayerGroup());
    }
    //-------------------------------------------------------------------------
    wrapper.setNoShadow(data.chkNoShadow);
    wrapper.setTilted(data.chkTilted);
    wrapper.setCockpitLit(data.chkCockpitLit);
    //-------------------------------------------------------------------------
    if (data.chkDry) {
        wrapper.setWetDry(xobj::AttrWetDry(xobj::AttrWetDry::dry));
    }
    else if (data.chkWet) {
        wrapper.setWetDry(xobj::AttrWetDry(xobj::AttrWetDry::wet));
    }
    else {
        wrapper.setWetDry(xobj::AttrWetDry());
    }
    //-------------------------------------------------------------------------
    wrapper.setTexture(sts::toMbString(data.strDiffuse));
    wrapper.setTextureLit(sts::toMbString(data.strLit));
    wrapper.setTextureNormal(sts::toMbString(data.strBump));
    //-------------------------------------------------------------------------
}

void MainObject::loadRawExpOption(sts_bwc::DataStream & stream) const {
    bcw::SerializationId id;
    id.load(stream.getStdStream());
    if (id != bcw::SerializationId(0xf34588f, 0x1863592b)) {
        LError << "Unexpected data input: " << id.toString();
        return;
    }
    unsigned char version;
    stream >> version;
    if (version != 1) {
        LError << "Unexpected data version: " << version;
        return;
    }

    //--------------------------
    uint64_t flags = 0;
    double scale = 0.0;
    stream >> flags;
    stream >> scale;
    //--------------------------
    {
        bcw::SerializationId idx;
        idx.load(stream.getStdStream());
        if (idx != bcw::SerializationId(0x3b0318d2, 0x7b7e3761)) {
            LError << "Unexpected data input: " << idx.toString();
            return;
        }
        unsigned char verx;
        stream >> verx;
        if (version != 1) {
            LError << "Unexpected data version: " << version;
            return;
        }

        int32_t xVersion = 0;
        stream >> xVersion;

        std::string signature;
        uint64_t oFlags = 0;
        stream >> oFlags;
        stream >> signature;

        enum {
            EXP_NO_OPT = 0,
            EXP_MARK_MESH = 1 << 0,
            EXP_MARK_LINE = 1 << 1,
            EXP_MARK_DUMMY = 1 << 2,
            EXP_MARK_LIGHT = 1 << 3,
            EXP_MARK_TREE = 1 << 4,
            EXP_MARK_COMMENTS = 1 << 5,
            EXP_MARK_TRANSFORM = 1 << 6,
            EXP_MARK_VERTEX = 1 << 7,
            EXP_OBJ_MESH = 1 << 11,
            EXP_OBJ_LINE = 1 << 12,
            EXP_OBJ_DUMMY = 1 << 13,
            EXP_OBJ_LIGHT = 1 << 14,
            EXP_ANIMATION = 1 << 21,
            EXP_OPTIMIZATION = 1 << 22,
            EXP_CHECK_INSTANCE = 1 << 23,
            EXP_DEBUG = 1 << 24,
        };

        if (mExpPb) {
            MainObjParamsWrapper wrapper(mAttrParamsPb, mExpPb, TimeValue(0), FOREVER);
            //-------------------------------------------------------------------------
            wrapper.setMeshExport((oFlags & EXP_OBJ_MESH) == EXP_OBJ_MESH);
            wrapper.setLinesExport(false);
            wrapper.setLightsExport((oFlags & EXP_OBJ_LIGHT) == EXP_OBJ_LIGHT);
            wrapper.setAnimationExport((oFlags & EXP_ANIMATION) == EXP_ANIMATION);
            //-------------------------------------------------------------------------
            wrapper.setNameMesh((oFlags & EXP_MARK_MESH) == EXP_MARK_MESH);
            wrapper.setNameLines((oFlags & EXP_MARK_LINE) == EXP_MARK_LINE);
            wrapper.setNameLights((oFlags & EXP_MARK_LIGHT) == EXP_MARK_LIGHT);
            wrapper.setNameDummies((oFlags & EXP_MARK_DUMMY) == EXP_MARK_DUMMY);
            wrapper.setTreeHierarchy((oFlags & EXP_MARK_TREE) == EXP_MARK_TREE);
            //-------------------------------------------------------------------------
            if (!stsff::math::isEqual(scale, 1.0)) {
                wrapper.setScale(true, float(scale));
            }
            //-------------------------------------------------------------------------
        }
    }
}

void MainObject::load186(std::vector<char> & inByteArray) {
    LMessage << "Object: <" << sts::toMbString(GetObjectName()) << "> has got data from previous version.";
    std::stringbuf buf(std::string(reinterpret_cast<char*>(inByteArray.data()), inByteArray.size()));
    sts_bwc::DataStream stream(buf);

    std::streampos pos = stream.getStdStream().tellg();
    uint8_t version;
    stream >> version; // 64 >= when incorrect version
    stream.getStdStream().seekg(pos);
    if (version != 0) {
        loadRawExpOption(stream);
        loadRawGlobAttr(stream);
    }
    else {
        uint8_t verx;
        stream >> verx;
        loadRawExpOption(stream);
        loadRawGlobAttr(stream);
        loadMdDisplayObj(stream);
    }
}

IOResult MainObject::Load(ILoad * iload) {
    ULONG temp;
    uint32_t dataSize = 0;
    IOResult res;
    std::vector<char> ba;
    while ((res = iload->OpenChunk()) == IO_OK) {
        switch (iload->CurChunkID()) {
            case 0:
                iload->Read(&dataSize, sizeof(dataSize), &temp);
                break;
            case 1: {
                if (dataSize != 0) {
                    ba.resize(dataSize);
                    iload->Read(ba.data(), dataSize, &temp);
                }
                break;
            }
            default: break;
        }
        iload->CloseChunk();
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

    iload->RegisterPostLoadCallback(new MainObjectPostLoadCallback(this));
    return IO_OK;
}

IOResult MainObject::Save(ISave * /*isave*/) {
    return IO_OK;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void MainObject::BeginEditParams(IObjParam * ip, ULONG flags, Animatable * prev) {
    mIp = ip;
    mEditOb = this;
    mDesc->BeginEditParams(ip, this, flags, prev);
    updateTexturesButtons();
    updateBlendSpinEnabling();
    updateLyerGroupSpinEnabling();
}

void MainObject::EndEditParams(IObjParam * ip, ULONG flags, Animatable * next) {
    mEditOb = nullptr;
    mIp = nullptr;
    mDesc->EndEditParams(ip, this, flags, next);
    ClearAFlag(A_OBJ_CREATING);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

ObjectState MainObject::Eval(TimeValue) { return ObjectState(this); }
Object * MainObject::ConvertToType(TimeValue, Class_ID) { return nullptr; }
int MainObject::CanConvertToType(Class_ID) { return FALSE; }

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void MainObject::GetWorldBoundBox(TimeValue, INode * inode, ViewExp *, Box3 & box) {
    Matrix3 tm = inode->GetObjectTM(GetCOREInterface()->GetTime());
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

Class_ID MainObject::ClassID() { return mDesc->ClassID(); }
SClass_ID MainObject::SuperClassID() { return mDesc->SuperClassID(); }
void MainObject::GetClassName(TSTR & s) { s = mDesc->ClassName(); }

RefTargetHandle MainObject::Clone(RemapDir & remap) {
    MainObject * newob = new MainObject();
    newob->ReplaceReference(MainObjParamBlocks::PbOrderAttr, mAttrParamsPb->Clone(remap));
    newob->ReplaceReference(MainObjParamBlocks::PbOrderExport, mExpPb->Clone(remap));
    newob->ReplaceReference(MainObjParamBlocks::PbOrderDisplay, mDisplayPb->Clone(remap));
    BaseClone(this, newob, remap);
    return (newob);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

Animatable * MainObject::SubAnim(int i) { return GetParamBlock(i); }

TSTR MainObject::SubAnimName(int i) {
    switch (i) {
        case MainObjParamBlocks::PbOrderAttr: return _T("Attributes");
        case MainObjParamBlocks::PbOrderExport: return _T("Options");
        case MainObjParamBlocks::PbOrderDisplay: return _T("Display");
        default: return _T("");
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

int MainObject::GetParamBlockIndex(int id) { return id; }
int MainObject::NumParamBlocks() { return 3; }

IParamBlock2 * MainObject::GetParamBlock(int i) {
    switch (i) {
        case MainObjParamBlocks::PbOrderAttr: return mAttrParamsPb;
        case MainObjParamBlocks::PbOrderExport: return mExpPb;
        case MainObjParamBlocks::PbOrderDisplay: return mDisplayPb;
        default: return nullptr;
    }
}

IParamBlock2 * MainObject::GetParamBlockByID(BlockID id) {
    switch (id) {
        case MainObjAttrParams: return mAttrParamsPb;
        case MainObjExpParams: return mExpPb;
        case MainObjDisplay: return mDisplayPb;
        default: return nullptr;
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

int MainObject::NumRefs() { return 3; }

RefTargetHandle MainObject::GetReference(int i) {
    switch (i) {
        case MainObjParamBlocks::PbOrderAttr: return mAttrParamsPb;
        case MainObjParamBlocks::PbOrderExport: return mExpPb;
        case MainObjParamBlocks::PbOrderDisplay: return mDisplayPb;
        default: return nullptr;
    }
}

void MainObject::SetReference(int i, RefTargetHandle rtarg) {
    switch (i) {
        case MainObjParamBlocks::PbOrderAttr: {
            mAttrParamsPb = static_cast<IParamBlock2*>(rtarg);
            break;
        }
        case MainObjParamBlocks::PbOrderExport: {
            mExpPb = static_cast<IParamBlock2*>(rtarg);
            break;
        }
        case MainObjParamBlocks::PbOrderDisplay: {
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
RefResult MainObject::NotifyRefChanged(const Interval & /*changeInt*/, RefTargetHandle /*hTarget*/,
                                       PartID & /*partID*/, RefMessage message, BOOL /*propagate*/) {
#else
RefResult MainObject::NotifyRefChanged(Interval /*changeInt*/, RefTargetHandle /*hTarget*/,
                                       PartID & /*partID*/, RefMessage message) {
#endif
    switch (message) {
        case REFMSG_CHANGE:
            if (mEditOb == this) {
                MainObjParamBlocks::mAttr.InvalidateUI(mAttrParamsPb->LastNotifyParamID());
                MainObjParamBlocks::mExport.InvalidateUI(mExpPb->LastNotifyParamID());
                MainObjParamBlocks::mDisplay.InvalidateUI(mDisplayPb->LastNotifyParamID());
            }
            break;
        default: break;
    }
    return REF_SUCCEED;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

int MainObject::HitTest(TimeValue t, INode * inode, int type, int crossing, int flags, IPoint2 * p, ViewExp * vpt) {
    HitRegion hitRegion;
    DWORD savedLimits;
    int res = 0;
    GraphicsWindow * gw = vpt->getGW();
    Material * mtl = gw->getMaterial();
    MakeHitRegion(hitRegion, type, crossing, 4, p);
    gw->setRndLimits(((savedLimits = gw->getRndLimits()) | GW_PICK) & ~GW_ILLUM);
    Matrix3 m = inode->GetObjectTM(t);
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

int MainObject::UsesWireColor() { return TRUE; }

//-------------------------------------------------------------------------

int MainObject::Display(TimeValue t, INode * inode, ViewExp * vpt, int /*flags*/) {
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

void MainObject::makeIcon() {
    float size = 1.0f;
    Interval interval = FOREVER;
    if (!mDisplayPb->GetValue(MainObjDisp_IconScale, mIp ? mIp->GetTime() : 0, size, interval)) {
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
    MainObjIcon::fillMesh(mIconMesh, size);
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
