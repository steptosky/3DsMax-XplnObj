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

#pragma warning(push, 0)
#include <iparamb2.h>
#pragma warning(pop)

#include "resource/resource.h"
#include "models/bwc/SerializationId.h"
#include "LodObjParams.h"
#include <vector>
#include "ScaleDim.h"
#include "common/Logger.h"
#include "common/String.h"
#include "sts/utilities/Compare.h"
#include "resource/ResHelper.h"

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

#if MAX_VERSION_MAJOR < 15
#	define p_end end
#endif

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

class ObjLodClassDesc : public ClassDesc2 {
public:

	int IsPublic() override { return TRUE; }
	void * Create(BOOL /*loading = FALSE*/) override { return new LodObject(); }
	HINSTANCE HInstance() override { return ResHelper::hInstance; }

	SClass_ID SuperClassID() override { return HELPER_CLASS_ID; }
	Class_ID ClassID() override { return LODOBJ_CLASS_ID; }

	const TCHAR * ClassName() override { return _T("X-Lod"); }
	const TCHAR * Category() override { return _T("X-Plane"); }
	const TCHAR * InternalName() override { return _T("xObjectLod"); }

};

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

class LodObjIconeSizeCallback : public PBAccessor {
public:

	virtual ~LodObjIconeSizeCallback() {}

	void Set(PB2Value & v, ReferenceMaker * owner, ParamID id, int tabIndex, TimeValue t) override {
		LodObject * u = dynamic_cast<LodObject*>(owner);
		DbgAssert(u);
		switch (id) {
			case PLodObjIconScale:
				u->makeIcon();
				break;
			default: break;
		}
		PBAccessor::Set(v, owner, id, tabIndex, t);
	}
};

static LodObjIconeSizeCallback gLodIconeSizeCallback;

//-------------------------------------------------------------------------

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

static ObjLodClassDesc gObjLodDesc;
MouseCallback LodObject::mMouseCallback;
LodObject * LodObject::mEditOb = nullptr;

ClassDesc2 * GetObjLodDesc() {
	return &gObjLodDesc;
}

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

static ParamBlockDesc2 gLodParamsPb(LodObjParams, _T("X-Lod"), 0, &gObjLodDesc, P_AUTO_CONSTRUCT + P_AUTO_UI + P_VERSION,
									PARAMS_PB_VERSION, LodObjParamsOrder,
									//-------------------------------------------------------------------------
									// Rollouts
									ROLL_LODOBJ, IDS_ROLL_LOD, 0, 0, NULL,
									//-------------------------------------------------------------------------
									// Params									
									PLodObjNear, _T("Near"), TYPE_FLOAT, 0, IDS_NEAR,
									p_default, 0.0f,
									p_range, 0.0f, 999999999.9f,
									p_ui, TYPE_SPINNER, EDITTYPE_POS_UNIVERSE, SPN_NEAR_EDIT, SPN_NEAR, 1.0f,
									p_end,
									//-------------------------------------------------------------------------
									PLodObjFar, _T("Far"), TYPE_FLOAT, 0, IDS_FAR,
									p_default, 0.0f,
									p_range, 0.0f, 999999999.9f,
									p_ui, TYPE_SPINNER, EDITTYPE_POS_UNIVERSE, SPN_FAR_EDIT, SPN_FAR, 1.0f,
									p_end,
									//-------------------------------------------------------------------------
									p_end);

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

static ParamBlockDesc2 gLodDisplayPb(LodObjDisplay, _T("X-Lod-Display"), 0, &gObjLodDesc, P_AUTO_CONSTRUCT + P_AUTO_UI + P_VERSION,
									DISPLAY_PB_VERSION, LodObjDisplayOrder,
									//-------------------------------------------------------------------------
									// Rollouts
									ROLL_LODOBJ_DISPLAY, IDS_ROLL_LOD_DISPLAY, 0, APPENDROLL_CLOSED, NULL,
									//-------------------------------------------------------------------------
									// Display									
									PLodObjIconScale, _T("IconScale"), TYPE_FLOAT, 0, IDS_SCALE,
									p_default, 1.0f,
									p_range, 0.01f, 1000.0f,
									p_accessor, &gLodIconeSizeCallback,
									p_dim, &gScaleDim,
									p_ui, TYPE_SPINNER, EDITTYPE_POS_FLOAT, IDC_SCALE_EDIT, IDC_SCALE_SPIN, SPIN_AUTOSCALE,
									p_end,
									//-------------------------------------------------------------------------
									p_end);

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

LodObject::LodObject() {
	gObjLodDesc.MakeAutoParamBlocks(this);
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
	gObjLodDesc.BeginEditParams(ip, this, flags, prev);
}

void LodObject::EndEditParams(IObjParam * ip, ULONG flags, Animatable * next) {
	mEditOb = nullptr;
	mIp = nullptr;
	gObjLodDesc.EndEditParams(ip, this, flags, next);
	ClearAFlag(A_OBJ_CREATING);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

ObjectState LodObject::Eval(TimeValue /*t*/) {
	return ObjectState(this);
}

Object * LodObject::ConvertToType(TimeValue /*t*/, Class_ID /*obtype*/) {
	return nullptr;
}

int LodObject::CanConvertToType(Class_ID /*obtype*/) {
	return FALSE;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void LodObject::GetWorldBoundBox(TimeValue /*t*/, INode * inode, ViewExp * /*vpt*/, Box3 & box) {
	Matrix3 tm = inode->GetObjectTM(GetCOREInterface()->GetTime());
	box = mIconMesh.getBoundingBox() * tm;
}

void LodObject::GetLocalBoundBox(TimeValue /*t*/, INode * /*inode*/, ViewExp * /*vpt*/, Box3 & box) {
	box = mIconMesh.getBoundingBox();
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

#if MAX_VERSION_MAJOR > 14

const MCHAR * LodObject::GetObjectName() {
	return _T("X-Lod");
}

#else

TCHAR * LodObject::GetObjectName() {
	return _T("X-Lod");
}

#endif

//-------------------------------------------------------------------------

Class_ID LodObject::ClassID() {
	return LODOBJ_CLASS_ID;
}

SClass_ID LodObject::SuperClassID() {
	return HELPER_CLASS_ID;
}

void LodObject::GetClassName(TSTR & s) {
	s = _T("X-Lod");
}

RefTargetHandle LodObject::Clone(RemapDir & remap) {
	LodObject * newob = new LodObject();
	newob->ReplaceReference(LodObjParamsOrder, mParamsPb->Clone(remap));
	newob->ReplaceReference(LodObjDisplayOrder, mDisplayPb->Clone(remap));
	//newob->ivalid.SetEmpty();
	BaseClone(this, newob, remap);
	return (newob);
}

void LodObject::InitNodeName(TSTR & s) {
	s = _T("X-Lod-");
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

Animatable * LodObject::SubAnim(int i) {
	switch (i) {
		case LodObjParamsOrder: return mParamsPb;
		case LodObjDisplayOrder: return mDisplayPb;
		default: return nullptr;
	}
}

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

int LodObject::GetParamBlockIndex(int id) {
	return id;
	//	if (mParamsPb && id >= 0 && id < mParamsPb->NumParams()) {
	//		return id;
	//	}
	//	return -1;
}

int LodObject::NumParamBlocks() {
	return 2;
}

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

int LodObject::NumRefs() {
	return 2;
}

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
				gLodParamsPb.InvalidateUI(mParamsPb->LastNotifyParamID());
				gLodDisplayPb.InvalidateUI(mDisplayPb->LastNotifyParamID());
			}
			break;
		default: break;
	}
	return REF_SUCCEED;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void LodObject::GetMat(TimeValue t, INode * inode, ViewExp * /*vpt*/, Matrix3 & tm) {
	tm = inode->GetObjectTM(t);
	tm.NoScale();
	//float scaleFactor = vpt->NonScalingObjectSize() * vpt->GetVPWorldWidth(tm.GetTrans())/(float)360.0;
	//tm.Scale(Point3(scaleFactor,scaleFactor,scaleFactor));
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

int LodObject::UsesWireColor() {
	return TRUE;
}

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
		LError << "Can't retrive scale value from param block";
	}

	if (sts::isEqual(mLastIconScale, size, 0.001f) && mIconMesh.getNumVerts() != 0) {
		return;
	}

	// Because box size is 2 meters we make it 1 meter, if the icon is changed the scale must be corrected.
	size *= 0.5f;

	float masterScale = static_cast<float>(GetMasterScale(UNITS_METERS));
	if (masterScale != -1.0f) {
		masterScale = 1.0f / masterScale;
		size = size * masterScale;
		if (size < 0.000001f) {
			size = 0.000001f;
			LError << "The icon scale is too small";
		}
	}

	mLastIconScale = size;

	mIconMesh = Mesh();
	mIconMesh.setNumVerts(68);
	mIconMesh.setNumFaces(64);
	mIconMesh.setVert(0, size * Point3(-1.0, -0.5, -0.5));
	mIconMesh.setVert(1, size * Point3(1.0, -0.5, -0.5));
	mIconMesh.setVert(2, size * Point3(-1.0, 0.5, -0.5));
	mIconMesh.setVert(3, size * Point3(1.0, 0.5, -0.5));
	mIconMesh.setVert(4, size * Point3(-1.0, -0.5, 0.5));
	mIconMesh.setVert(5, size * Point3(1.0, -0.5, 0.5));
	mIconMesh.setVert(6, size * Point3(-1.0, 0.5, 0.5));
	mIconMesh.setVert(7, size * Point3(1.0, 0.5, 0.5));
	mIconMesh.setVert(8, size * Point3(0.213615, 0.187986, 0.5));
	mIconMesh.setVert(9, size * Point3(0.107339, 0.303623, 0.5));
	mIconMesh.setVert(10, size * Point3(0.0632985, 0.242289, 0.5));
	mIconMesh.setVert(11, size * Point3(0.139152, 0.153872, 0.5));
	mIconMesh.setVert(12, size * Point3(0.250182, 0.0197559, 0.5));
	mIconMesh.setVert(13, size * Point3(0.213615, -0.148474, 0.5));
	mIconMesh.setVert(14, size * Point3(0.139152, -0.11436, 0.5));
	mIconMesh.setVert(15, size * Point3(0.165081, 0.0197559, 0.5));
	mIconMesh.setVert(16, size * Point3(0.107339, -0.264111, 0.5));
	mIconMesh.setVert(17, size * Point3(0.0632985, -0.202777, 0.5));
	mIconMesh.setVert(18, size * Point3(-0.0491985, -0.305577, 0.5));
	mIconMesh.setVert(19, size * Point3(-0.312012, 0.187986, 0.5));
	mIconMesh.setVert(20, size * Point3(-0.237549, 0.153872, 0.5));
	mIconMesh.setVert(21, size * Point3(-0.205736, 0.303623, 0.5));
	mIconMesh.setVert(22, size * Point3(-0.161695, 0.242289, 0.5));
	mIconMesh.setVert(23, size * Point3(-0.0491985, 0.345088, 0.5));
	mIconMesh.setVert(24, size * Point3(-0.0491985, 0.273682, 0.5));
	mIconMesh.setVert(25, size * Point3(-0.348579, 0.0197559, 0.5));
	mIconMesh.setVert(26, size * Point3(-0.263478, 0.0197559, 0.5));
	mIconMesh.setVert(27, size * Point3(-0.237549, -0.11436, 0.5));
	mIconMesh.setVert(28, size * Point3(-0.312012, -0.148474, 0.5));
	mIconMesh.setVert(29, size * Point3(-0.161695, -0.202777, 0.5));
	mIconMesh.setVert(30, size * Point3(-0.205736, -0.264111, 0.5));
	mIconMesh.setVert(31, size * Point3(-0.0491985, -0.23417, 0.5));
	mIconMesh.setVert(32, size * Point3(-0.422797, -0.284415, 0.5));
	mIconMesh.setVert(33, size * Point3(-0.422797, -0.210074, 0.5));
	mIconMesh.setVert(34, size * Point3(-0.73092, -0.210074, 0.5));
	mIconMesh.setVert(35, size * Point3(-0.73092, 0.34259, 0.5));
	mIconMesh.setVert(36, size * Point3(-0.814065, 0.34259, 0.5));
	mIconMesh.setVert(37, size * Point3(-0.814065, -0.284415, 0.5));
	mIconMesh.setVert(38, size * Point3(0.582652, -0.284415, 0.5));
	mIconMesh.setVert(39, size * Point3(0.683197, -0.273724, 0.5));
	mIconMesh.setVert(40, size * Point3(0.757912, -0.242285, 0.5));
	mIconMesh.setVert(41, size * Point3(0.814906, -0.185399, 0.5));
	mIconMesh.setVert(42, size * Point3(0.857601, -0.0940851, 0.5));
	mIconMesh.setVert(43, size * Point3(0.356802, 0.34259, 0.5));
	mIconMesh.setVert(44, size * Point3(0.356802, -0.284415, 0.5));
	mIconMesh.setVert(45, size * Point3(0.439947, 0.26825, 0.5));
	mIconMesh.setVert(46, size * Point3(0.571434, 0.26825, 0.5));
	mIconMesh.setVert(47, size * Point3(0.781658, -0.070883, 0.5));
	mIconMesh.setVert(48, size * Point3(0.737107, -0.152644, 0.5));
	mIconMesh.setVert(49, size * Point3(0.670687, -0.198543, 0.5));
	mIconMesh.setVert(50, size * Point3(0.573574, -0.210074, 0.5));
	mIconMesh.setVert(51, size * Point3(0.439947, -0.210074, 0.5));
	mIconMesh.setVert(52, size * Point3(0.781658, 0.120177, 0.5));
	mIconMesh.setVert(53, size * Point3(0.857601, 0.143379, 0.5));
	mIconMesh.setVert(54, size * Point3(0.670687, 0.247837, 0.5));
	mIconMesh.setVert(55, size * Point3(0.572412, 0.34259, 0.5));
	mIconMesh.setVert(56, size * Point3(0.683197, 0.323018, 0.5));
	mIconMesh.setVert(57, size * Point3(0.737107, 0.201939, 0.5));
	mIconMesh.setVert(58, size * Point3(0.814906, 0.234693, 0.5));
	mIconMesh.setVert(59, size * Point3(0.757912, 0.29158, 0.5));
	mIconMesh.setVert(60, size * Point3(1.0, -0.5, 0.5));
	mIconMesh.setVert(61, size * Point3(-1.0, -0.5, 0.5));
	mIconMesh.setVert(62, size * Point3(-1.0, -0.853553, 0.146447));
	mIconMesh.setVert(63, size * Point3(1.0, -0.853554, 0.146447));
	mIconMesh.setVert(64, size * Point3(-1.0, 0.5, 0.5));
	mIconMesh.setVert(65, size * Point3(1.0, 0.5, 0.5));
	mIconMesh.setVert(66, size * Point3(1.0, 0.853554, 0.146447));
	mIconMesh.setVert(67, size * Point3(-1.0, 0.853554, 0.146447));
	mIconMesh.faces[0].setVerts(5, 4, 0);
	mIconMesh.faces[0].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[0].setSmGroup(8);
	mIconMesh.faces[1].setVerts(0, 1, 5);
	mIconMesh.faces[1].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[1].setSmGroup(8);
	mIconMesh.faces[2].setVerts(7, 5, 1);
	mIconMesh.faces[2].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[2].setSmGroup(16);
	mIconMesh.faces[3].setVerts(1, 3, 7);
	mIconMesh.faces[3].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[3].setSmGroup(16);
	mIconMesh.faces[4].setVerts(6, 7, 3);
	mIconMesh.faces[4].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[4].setSmGroup(32);
	mIconMesh.faces[5].setVerts(3, 2, 6);
	mIconMesh.faces[5].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[5].setSmGroup(32);
	mIconMesh.faces[6].setVerts(4, 6, 2);
	mIconMesh.faces[6].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[6].setSmGroup(64);
	mIconMesh.faces[7].setVerts(2, 0, 4);
	mIconMesh.faces[7].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[7].setSmGroup(64);
	mIconMesh.faces[8].setVerts(15, 12, 8);
	mIconMesh.faces[8].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[8].setSmGroup(1);
	mIconMesh.faces[9].setVerts(8, 11, 15);
	mIconMesh.faces[9].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[9].setSmGroup(1);
	mIconMesh.faces[10].setVerts(11, 8, 9);
	mIconMesh.faces[10].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[10].setSmGroup(1);
	mIconMesh.faces[11].setVerts(9, 10, 11);
	mIconMesh.faces[11].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[11].setSmGroup(1);
	mIconMesh.faces[12].setVerts(24, 10, 9);
	mIconMesh.faces[12].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[12].setSmGroup(1);
	mIconMesh.faces[13].setVerts(9, 23, 24);
	mIconMesh.faces[13].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[13].setSmGroup(1);
	mIconMesh.faces[14].setVerts(15, 14, 13);
	mIconMesh.faces[14].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[14].setSmGroup(1);
	mIconMesh.faces[15].setVerts(13, 12, 15);
	mIconMesh.faces[15].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[15].setSmGroup(1);
	mIconMesh.faces[16].setVerts(14, 17, 16);
	mIconMesh.faces[16].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[16].setSmGroup(1);
	mIconMesh.faces[17].setVerts(16, 13, 14);
	mIconMesh.faces[17].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[17].setSmGroup(1);
	mIconMesh.faces[18].setVerts(31, 18, 16);
	mIconMesh.faces[18].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[18].setSmGroup(1);
	mIconMesh.faces[19].setVerts(16, 17, 31);
	mIconMesh.faces[19].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[19].setSmGroup(1);
	mIconMesh.faces[20].setVerts(26, 20, 19);
	mIconMesh.faces[20].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[20].setSmGroup(1);
	mIconMesh.faces[21].setVerts(19, 25, 26);
	mIconMesh.faces[21].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[21].setSmGroup(1);
	mIconMesh.faces[22].setVerts(20, 22, 21);
	mIconMesh.faces[22].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[22].setSmGroup(1);
	mIconMesh.faces[23].setVerts(21, 19, 20);
	mIconMesh.faces[23].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[23].setSmGroup(1);
	mIconMesh.faces[24].setVerts(24, 23, 21);
	mIconMesh.faces[24].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[24].setSmGroup(1);
	mIconMesh.faces[25].setVerts(21, 22, 24);
	mIconMesh.faces[25].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[25].setSmGroup(1);
	mIconMesh.faces[26].setVerts(26, 25, 28);
	mIconMesh.faces[26].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[26].setSmGroup(1);
	mIconMesh.faces[27].setVerts(28, 27, 26);
	mIconMesh.faces[27].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[27].setSmGroup(1);
	mIconMesh.faces[28].setVerts(27, 28, 30);
	mIconMesh.faces[28].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[28].setSmGroup(1);
	mIconMesh.faces[29].setVerts(30, 29, 27);
	mIconMesh.faces[29].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[29].setSmGroup(1);
	mIconMesh.faces[30].setVerts(31, 29, 30);
	mIconMesh.faces[30].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[30].setSmGroup(1);
	mIconMesh.faces[31].setVerts(30, 18, 31);
	mIconMesh.faces[31].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[31].setSmGroup(1);
	mIconMesh.faces[32].setVerts(37, 34, 35);
	mIconMesh.faces[32].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[32].setSmGroup(1);
	mIconMesh.faces[33].setVerts(35, 36, 37);
	mIconMesh.faces[33].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[33].setSmGroup(1);
	mIconMesh.faces[34].setVerts(45, 46, 55);
	mIconMesh.faces[34].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[34].setSmGroup(1);
	mIconMesh.faces[35].setVerts(45, 55, 43);
	mIconMesh.faces[35].setEdgeVisFlags(0, 1, 0);
	mIconMesh.faces[35].setSmGroup(1);
	mIconMesh.faces[36].setVerts(44, 38, 50);
	mIconMesh.faces[36].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[36].setSmGroup(1);
	mIconMesh.faces[37].setVerts(44, 50, 51);
	mIconMesh.faces[37].setEdgeVisFlags(0, 1, 0);
	mIconMesh.faces[37].setSmGroup(1);
	mIconMesh.faces[38].setVerts(43, 44, 51);
	mIconMesh.faces[38].setEdgeVisFlags(1, 0, 0);
	mIconMesh.faces[38].setSmGroup(1);
	mIconMesh.faces[39].setVerts(45, 43, 51);
	mIconMesh.faces[39].setEdgeVisFlags(0, 0, 1);
	mIconMesh.faces[39].setSmGroup(1);
	mIconMesh.faces[40].setVerts(34, 37, 32);
	mIconMesh.faces[40].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[40].setSmGroup(1);
	mIconMesh.faces[41].setVerts(32, 33, 34);
	mIconMesh.faces[41].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[41].setSmGroup(1);
	mIconMesh.faces[42].setVerts(38, 39, 49);
	mIconMesh.faces[42].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[42].setSmGroup(1);
	mIconMesh.faces[43].setVerts(49, 50, 38);
	mIconMesh.faces[43].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[43].setSmGroup(1);
	mIconMesh.faces[44].setVerts(41, 42, 47);
	mIconMesh.faces[44].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[44].setSmGroup(1);
	mIconMesh.faces[45].setVerts(47, 48, 41);
	mIconMesh.faces[45].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[45].setSmGroup(1);
	mIconMesh.faces[46].setVerts(48, 40, 41);
	mIconMesh.faces[46].setEdgeVisFlags(1, 1, 1);
	mIconMesh.faces[46].setSmGroup(1);
	mIconMesh.faces[47].setVerts(40, 48, 49);
	mIconMesh.faces[47].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[47].setSmGroup(1);
	mIconMesh.faces[48].setVerts(49, 39, 40);
	mIconMesh.faces[48].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[48].setSmGroup(1);
	mIconMesh.faces[49].setVerts(53, 52, 47);
	mIconMesh.faces[49].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[49].setSmGroup(1);
	mIconMesh.faces[50].setVerts(47, 42, 53);
	mIconMesh.faces[50].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[50].setSmGroup(1);
	mIconMesh.faces[51].setVerts(55, 46, 54);
	mIconMesh.faces[51].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[51].setSmGroup(1);
	mIconMesh.faces[52].setVerts(54, 56, 55);
	mIconMesh.faces[52].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[52].setSmGroup(1);
	mIconMesh.faces[53].setVerts(58, 57, 52);
	mIconMesh.faces[53].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[53].setSmGroup(1);
	mIconMesh.faces[54].setVerts(52, 53, 58);
	mIconMesh.faces[54].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[54].setSmGroup(1);
	mIconMesh.faces[55].setVerts(57, 58, 59);
	mIconMesh.faces[55].setEdgeVisFlags(1, 1, 1);
	mIconMesh.faces[55].setSmGroup(1);
	mIconMesh.faces[56].setVerts(59, 56, 54);
	mIconMesh.faces[56].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[56].setSmGroup(1);
	mIconMesh.faces[57].setVerts(54, 57, 59);
	mIconMesh.faces[57].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[57].setSmGroup(1);
	mIconMesh.faces[58].setVerts(63, 60, 61);
	mIconMesh.faces[58].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[58].setSmGroup(0);
	mIconMesh.faces[59].setVerts(61, 62, 63);
	mIconMesh.faces[59].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[59].setSmGroup(0);
	mIconMesh.faces[60].setVerts(67, 64, 65);
	mIconMesh.faces[60].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[60].setSmGroup(0);
	mIconMesh.faces[61].setVerts(65, 66, 67);
	mIconMesh.faces[61].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[61].setSmGroup(0);
	mIconMesh.faces[62].setVerts(3, 1, 0);
	mIconMesh.faces[62].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[62].setSmGroup(0);
	mIconMesh.faces[63].setVerts(0, 2, 3);
	mIconMesh.faces[63].setEdgeVisFlags(1, 1, 0);
	mIconMesh.faces[63].setSmGroup(0);

	//--------------------------------

	mIconMesh.InvalidateGeomCache();
}

/*************************************************************************************************
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
