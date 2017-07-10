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

#include "Converterer.h"
#include "common/Logger.h"

#include "ConverterDummy.h"
#include "ConverterLine.h"
#include "ConverterMesh.h"
#include "ConverterMain.h"
#include "ConverterLight.h"
#include "ConverterAttr.h"
#include "ConverterAnim.h"
#include "ConverterUtils.h"

#include <cassert>
#include "objects/lod/LodObjParamsWrapper.h"
#include "common/String.h"
#include <xpln/obj/ObjSmoke.h>
#include "ConverterSmoke.h"

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

Converterer::Converterer() {
	mIp = GetCOREInterface();

	mXObjMain = nullptr;
	mMainObj = nullptr;
}

//-------------------------------------------------------------------------

Converterer::~Converterer() {}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool Converterer::toMax(xobj::ObjMain & xObjMain) {
	INode * main = ConverterMain::toMax(xObjMain);
	if (!main) {
		return false;
	}

	for (size_t i = 0; i < xObjMain.lodCount(); ++i) {
		xobj::ObjLodGroup & lod = xObjMain.lod(i);
		INode * maxLod = ConverterMain::toMax(lod);
		if (maxLod == nullptr) {
			return false;
		}
		main->AttachChild(maxLod);
		if (!processXTransformHierarchy(maxLod, &lod.transform())) {
			return false;
		}
	}
	return false;
}

bool Converterer::processXTransformHierarchy(INode * parent, xobj::Transform * xTransform) {
	ConverterUtils::toMaxTransform(*xTransform, parent);
	if (xTransform->hasAnimRotate() || xTransform->hasAnimTrans()) {
		INode * animNode = ConverterMain::createBone(xTransform);
		if (!animNode) {
			return false;
		}
		if (!ConverterAnim::toMax(animNode, xTransform)) {
			return false;
		}
		ConverterUtils::toMaxTransform(*xTransform, animNode);
		parent->AttachChild(animNode);
		parent = animNode;
	}
	//---------------------------
	processXTransformObjects(parent, xTransform);
	//---------------------------
	for (size_t i = 0; i < xTransform->childrenCount(); ++i) {
		if (!processXTransformHierarchy(parent, xTransform->childAt(i))) {
			return false;
		}
	}
	return true;
}

void Converterer::processXTransformObjects(INode * parent, xobj::Transform * xTransform) {
	for (auto & obj: xTransform->objList()) {
		INode * node = processXObjects(*obj);
		if (node) {
			ConverterUtils::toMaxTransform(*xTransform, node);
			parent->AttachChild(node);
		}
	}
}

INode * Converterer::processXObjects(const xobj::ObjAbstract & xObj) {
	INode * node = ConverterMesh::toMax(&xObj);
	if (!node) {
		node = ConverterLight::toMax(&xObj);
	}
	else if (!node) {
		node = ConverterSmoke::toMax(&xObj);
	}
	else if (!node) {
		node = ConverterDummy::toMax(&xObj);
	}
	else if (!node) {
		node = ConverterLine::toMax(&xObj);
	}
	return node;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool Converterer::toXpln(MainObjParamsWrapper * mainNode, xobj::ObjMain & xObjMain) {
	assert(mainNode);
	mXObjMain = &xObjMain;
	mMainObj = mainNode;

	ConverterMain::toXpln(mainNode->node(), xObjMain);
	mLods.clear();
	if (!collectLods(mainNode->node(), mainNode->node(), mLods)) {
		return false;
	}

	if (mLods.empty()) {
		mLods.emplace_back(mainNode->node());
	}

	for (auto currLodNode : mLods) {
		if (currLodNode->NumberOfChildren() == 0) {
			CLWarning << "The lod: <" << sts::toMbString(currLodNode->GetName()) << "> does not have any objects.";
			continue;
		}
		//-------------------------------------------------------------------------
		xobj::ObjLodGroup & lod = xObjMain.addLod();
		xobj::Transform & currObjTransform = lod.transform();

		if (mainNode->node() != currLodNode) {
			ConverterMain::toXpln(currLodNode, lod);
		}
		//-------------------------------------------------------------------------
		float scale = mainNode->scale();
		if (!mainNode->isManualScale()) {
			// TODO Needs implementation of auto-scale value relative system units
			scale = 1.0f;
		}
		//-------------------------------------------------------------------------
		Matrix3 ttm = Inverse(currLodNode->GetNodeTM(mainNode->timeValue()));
		if (scale < 0.00001) {
			CLError << "<" << xObjMain.objectName() << "> has too small scale <" << scale << ">";
			return false;
		}
		ttm.Scale(Point3(scale, scale, scale), TRUE);
		//-------------------------------------------------------------------------
		ConverterUtils::toXTransform(ttm, currObjTransform);

		int numChildren = currLodNode->NumberOfChildren();
		for (int idx = 0; idx < numChildren; ++idx) {
			if (!processNode(currLodNode->GetChildNode(idx), &currObjTransform)) {
				return false;
			}
		}
		//-------------------------------------------------------------------------
	}
	return true;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool Converterer::processNode(INode * node, xobj::Transform * xTransform) const {
	xobj::Transform & tr = xTransform->createChild(sts::toMbString(node->GetName()).c_str());
	ConverterUtils::toXTransform(node->GetNodeTM(mMainObj->timeValue()), tr);
	//-------------------------------------------------------------------------
	// animation
	if (mMainObj->isAnimationExport()) {
		ConverterAnim::toXPLN(node, &tr);
	}
	//-------------------------------------------------------------------------
	// translate object
	static ObjAbstractList xObjList;
	toXpln(node, Matrix3(1), xObjList);
	for (auto curr : xObjList) {
		tr.addObject(curr);
	}
	//-------------------------------------------------------------------------
	// translate children
	int numChildren = node->NumberOfChildren();
	for (int idx = 0; idx < numChildren; ++idx) {
		if (!processNode(node->GetChildNode(idx), &tr)) {
			return false;
		}
	}
	//-------------------------------------------------------------------------
	return true;
}

//-------------------------------------------------------------------------

void Converterer::toXpln(INode * inNode, const Matrix3 & baseTm, ObjAbstractList & outList) const {
	outList.clear();

	if (mMainObj->isMeshExport()) {
		xobj::ObjAbstract * xObj = ConverterMesh::toXpln(inNode);
		if (xObj) {
			ConverterAttr::toXpln(*xObj, inNode);
			outList.emplace_back(xObj);
			return;
		}
	}
	if (mMainObj->isLinesExport()) {
		outList = ConverterLine::toXpln(inNode, baseTm);
		if (!outList.empty()) {
			return;
		}
	}
	if (mMainObj->isLightsExport()) {
		xobj::ObjAbstract * xObj = ConverterLight::toXpln(inNode);
		if (xObj) {
			ConverterAttr::toXpln(*xObj, inNode);
			outList.emplace_back(xObj);
			return;
		}
	}

	xobj::ObjSmoke * xSmokeObj = ConverterSmoke::toXpln(inNode);
	if (xSmokeObj) {
		outList.emplace_back(xSmokeObj);
		return;
	}

	xobj::ObjAbstract * xDummyObj = ConverterDummy::toXpln(inNode);
	if (xDummyObj) {
		ConverterAttr::toXpln(*xDummyObj, inNode);
		outList.emplace_back(xDummyObj);
		return;
	}
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool Converterer::collectLods(INode * ownerNode, INode * currNode, std::vector<INode*> & outLods) {
	if (LodObjParamsWrapper::isLodObj(currNode)) {
		if (currNode->GetParentNode() != ownerNode) {
			CLError << "The lod object <"
			<< sts::toMbString(currNode->GetName()) << "> is not linked to <"
			<< sts::toMbString(ownerNode->GetName()) << ">";
			return false;
		}
		outLods.emplace_back(currNode);
	}

	int count = currNode->NumberOfChildren();
	for (int i = 0; i < count; ++i) {
		if (!collectLods(ownerNode, currNode->GetChildNode(i), outLods)) {
			return false;
		}
	}
	return true;
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
