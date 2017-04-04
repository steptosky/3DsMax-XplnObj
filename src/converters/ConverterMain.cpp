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

#include "ConverterMain.h"

#pragma warning(push, 0)
#include <decomp.h>
#include <iparamb2.h>
#pragma warning(pop)

#include "Common/String.h"
#include "common/Logger.h"

#include "objects/LodObjParamsWrapper.h"
#include "objects/MainObjParamsWrapper.h"
#include "objects/MainObj.h"
#include "objects/MainObjectParams.h"
#include "objects/LodObjParams.h"

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

INode * ConverterMain::toMax(const xobj::ObjMain & inXObj) {
	Interface * ip = GetCOREInterface();
	HelperObject * pobj = reinterpret_cast<MainObject*>(ip->CreateInstance(HELPER_CLASS_ID, MAINOBJ_CLASS_ID));
	if (pobj == nullptr) {
		LCritical << "Main object <" << inXObj.objectName() << "> couldn't be created.";
		return nullptr;
	}

	INode * pnode = ip->CreateObjectNode(pobj);
	if (pnode == nullptr) {
		LCritical << "Max node for the object <" << inXObj.objectName() << "> couldn't be created.";
		return nullptr;
	}

	MainObjParamsWrapper attr(pnode, GetCOREInterface()->GetTime(), FOREVER);
	auto & xop = inXObj.pExportOptions;

	attr.setOptimisation(xop.isEnabled(xobj::XOBJ_EXP_OPTIMIZATION));
	attr.setInstancing(xop.isEnabled(xobj::XOBJ_EXP_CHECK_INSTANCE));
	attr.setDebug(xop.isEnabled(xobj::XOBJ_EXP_DEBUG) || inXObj.pAttr.isDebug());
	attr.setNameMesh(xop.isEnabled(xobj::XOBJ_EXP_MARK_MESH));
	attr.setNameLines(xop.isEnabled(xobj::XOBJ_EXP_MARK_LINE));
	attr.setNameLights(xop.isEnabled(xobj::XOBJ_EXP_MARK_LIGHT));
	attr.setNameDummies(xop.isEnabled(xobj::XOBJ_EXP_MARK_DUMMY));
	attr.setTreeHierarchy(xop.isEnabled(xobj::XOBJ_EXP_MARK_TREE_HIERARCHY));

	std::string resTexture;
	std::string resPrefix1;
	std::string resPrefix2;
	std::string resPrefix3;

	makeTexturePath(inXObj.pAttr.texture(), resTexture, resPrefix1);
	attr.setPathPrefix(resPrefix1);
	attr.setTexture(resTexture);

	makeTexturePath(inXObj.pAttr.textureLit(), resTexture, resPrefix1);
	attr.setTextureLit(resTexture);

	makeTexturePath(inXObj.pAttr.textureNormal(), resTexture, resPrefix1);
	attr.setTextureNormal(resTexture);

	if (resPrefix1 != resPrefix2 && resPrefix2 != resPrefix3) {
		CLWarning << "Textures have different prefix, only one <"
				<< resPrefix1 << "> prefix will be used.";
	}

	attr.setNoShadow(inXObj.pAttr.isNoShadow());
	attr.setTilted(inXObj.pAttr.isTilted());
	attr.setCockpitLit(inXObj.pAttr.isCockpitLit());

	attr.setBlendGlass(inXObj.pAttr.isBlendGlass());
	attr.setNormalMetalness(inXObj.pAttr.isNormalMetalness());
	attr.setWetDry(inXObj.pAttr.wetDry());
	attr.setBlend(inXObj.pAttr.blend());
	attr.setLayerGroup(inXObj.pAttr.layerGroup());
	attr.setDrapedLayerGroup(inXObj.pAttr.layerGroupDraped());
	attr.setLodDrap(inXObj.pAttr.lodDrap());
	attr.setSlungWeight(inXObj.pAttr.slungLoadWeight());
	attr.setSpecular(inXObj.pAttr.specular());
	attr.setTint(inXObj.pAttr.tint());
	attr.setSlopeLimit(inXObj.pAttr.slopeLimit());
	attr.setCockpitRegion(inXObj.pAttr.cockpitRegion(xobj::AttrCockpitRegion::r1), xobj::AttrCockpitRegion::r1);
	attr.setCockpitRegion(inXObj.pAttr.cockpitRegion(xobj::AttrCockpitRegion::r2), xobj::AttrCockpitRegion::r2);
	attr.setCockpitRegion(inXObj.pAttr.cockpitRegion(xobj::AttrCockpitRegion::r3), xobj::AttrCockpitRegion::r3);
	attr.setCockpitRegion(inXObj.pAttr.cockpitRegion(xobj::AttrCockpitRegion::r4), xobj::AttrCockpitRegion::r4);

	pnode->SetName(toTSTR(inXObj.objectName().c_str()));
	return pnode;
}

//-------------------------------------------------------------------------

bool ConverterMain::toXpln(INode * inNode, xobj::ObjMain & outMain) {
	if (!MainObjParamsWrapper::isMainObj(inNode)) {
		return false;
	}

	MainObjParamsWrapper attr(inNode, GetCOREInterface()->GetTime(), FOREVER);
	auto & xop = outMain.pExportOptions;

	xop.enable(xobj::XOBJ_EXP_APPLY_LOD_TM);

	attr.isOptimisation() ? xop.enable(xobj::XOBJ_EXP_OPTIMIZATION) : xop.disable(xobj::XOBJ_EXP_OPTIMIZATION);
	attr.isInstancing() ? xop.enable(xobj::XOBJ_EXP_CHECK_INSTANCE) : xop.disable(xobj::XOBJ_EXP_CHECK_INSTANCE);
	attr.isDebug() ? xop.enable(xobj::XOBJ_EXP_DEBUG) : xop.disable(xobj::XOBJ_EXP_DEBUG);
	attr.isNameMesh() ? xop.enable(xobj::XOBJ_EXP_MARK_MESH) : xop.disable(xobj::XOBJ_EXP_MARK_MESH);
	attr.isNameLines() ? xop.enable(xobj::XOBJ_EXP_MARK_LINE) : xop.disable(xobj::XOBJ_EXP_MARK_LINE);
	attr.isNameLights() ? xop.enable(xobj::XOBJ_EXP_MARK_LIGHT) : xop.disable(xobj::XOBJ_EXP_MARK_LIGHT);
	attr.isNameDummies() ? xop.enable(xobj::XOBJ_EXP_MARK_DUMMY) : xop.disable(xobj::XOBJ_EXP_MARK_DUMMY);
	attr.isTreeHierarchy() ? xop.enable(xobj::XOBJ_EXP_MARK_TREE_HIERARCHY) : xop.disable(xobj::XOBJ_EXP_MARK_TREE_HIERARCHY);

	outMain.pAttr.setDebug(xop.isEnabled(xobj::XOBJ_EXP_DEBUG));

	std::string prefix = attr.pathPrefix();
	outMain.pAttr.setTexture(makeTexturePath(attr.texture(), prefix));
	outMain.pAttr.setTextureLit(makeTexturePath(attr.textureLit(), prefix));
	outMain.pAttr.setTextureNormal(makeTexturePath(attr.textureNormal(), prefix));

	outMain.pAttr.setNoShadow(attr.isNoShadow());
	outMain.pAttr.setTilted(attr.isTilted());
	outMain.pAttr.setCockpitLit(attr.isCockpitLit());

	outMain.pAttr.setBlendGlass(attr.isBlendGlass());
	outMain.pAttr.setNormalMetalness(attr.isNormalMetalness());
	outMain.pAttr.setWetDry(attr.wetDry());
	outMain.pAttr.setBlend(attr.blend());
	outMain.pAttr.setLayerGroup(attr.layerGroup());
	outMain.pAttr.setLayerGroupDraped(attr.drapedLayerGroup());
	outMain.pAttr.setLodDrap(attr.lodDrap());
	outMain.pAttr.setSlungLoadWeight(attr.slungWeight());
	outMain.pAttr.setSpecular(attr.specular());
	outMain.pAttr.setTint(attr.tint());
	outMain.pAttr.setSlopeLimit(attr.slopeLimit());
	outMain.pAttr.setCockpitRegion(attr.cockpitRegion(xobj::AttrCockpitRegion::r1), xobj::AttrCockpitRegion::r1);
	outMain.pAttr.setCockpitRegion(attr.cockpitRegion(xobj::AttrCockpitRegion::r2), xobj::AttrCockpitRegion::r2);
	outMain.pAttr.setCockpitRegion(attr.cockpitRegion(xobj::AttrCockpitRegion::r3), xobj::AttrCockpitRegion::r3);
	outMain.pAttr.setCockpitRegion(attr.cockpitRegion(xobj::AttrCockpitRegion::r4), xobj::AttrCockpitRegion::r4);

	outMain.setObjectName(sts::toMbString(inNode->GetName()));
	return true;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

INode * ConverterMain::toMax(const xobj::ObjLodGroup & inXObj) {
	Interface * ip = GetCOREInterface();
	HelperObject * pobj = reinterpret_cast<MainObject*>(ip->CreateInstance(HELPER_CLASS_ID, LODOBJ_CLASS_ID));
	if (pobj == nullptr) {
		LCritical << "Lod object <" << inXObj.objectName() << "> couldn't be created.";
		return nullptr;
	}

	INode * pnode = ip->CreateObjectNode(pobj);
	if (pnode == nullptr) {
		LCritical << "Max node for the object <" << inXObj.objectName() << "> couldn't be created.";
		return nullptr;
	}

	LodObjParamsWrapper values(pnode, GetCOREInterface()->GetTime(), FOREVER);
	values.setNearValue(inXObj.nearVal());
	values.setFarValue(inXObj.nearVal());
	pnode->SetName(toTSTR(inXObj.objectName().c_str()));
	return pnode;
}

//-------------------------------------------------------------------------

bool ConverterMain::toXpln(INode * inNode, xobj::ObjLodGroup & outLod) {
	if (!LodObjParamsWrapper::isLodObj(inNode)) {
		return false;
	}

	LodObjParamsWrapper values(inNode, GetCOREInterface()->GetTime(), FOREVER);
	outLod.setNearVal(values.nearValue());
	outLod.setFarVal(values.farValue());
	outLod.setObjectName(sts::toMbString(inNode->GetName()));
	return true;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

std::string ConverterMain::makeTexturePath(const std::string & texture, const std::string & prefix) {
	typedef std::list<std::string> Container;
	if (texture == "none" || texture.empty()) {
		return "";
	}

	Container res = sts::StringUtils<char>::split<Container>(texture, "\\/");
	DbgAssert(!res.empty());
	std::string out(prefix);
	if (!prefix.empty()) {
		out.append("/");
	}
	out.append(res.back());
	return out;
}

void ConverterMain::makeTexturePath(const std::string & texture, std::string & outTextureName, std::string & outPrefix) {
	outTextureName.clear();
	outPrefix.clear();
	if (texture == "none" || texture.empty()) {
		return;
	}

	size_t pos = texture.find_last_of("\\/");
	if (pos != std::string::npos) {
		outPrefix = texture.substr(0, pos);
		outTextureName = texture.substr(pos, texture.size() - pos);
	}
	else {
		outTextureName = texture;
	}
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

INode * ConverterMain::createBone(const xobj::Transform * xTransform) {
	Interface * ip = GetCOREInterface();
	GeomObject * bobj = reinterpret_cast<GeomObject*>(ip->CreateInstance(BONE_CLASS_ID, BONE_OBJ_CLASSID));
	if (bobj == nullptr) {
		LCritical << "Bone object <" << xTransform->name() << "> couldn't be created.";
		return nullptr;
	}

	INode * pnode = ip->CreateObjectNode(bobj);
	if (pnode == nullptr) {
		LCritical << "Max node for the object <" << xTransform->name() << "> couldn't be created.";
		return nullptr;
	}
	pnode->SetBoneNodeOnOff(TRUE, GetCOREInterface()->GetTime());
	return pnode;
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
