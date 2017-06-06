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

#include "ConverterSmoke.h"
#include <memory>
#include "common/String.h"
#include "ConverterUtils.h"
#include "objects/smoke/SmokeObjParamsWrapper.h"
#include "objects/smoke/SmokeObj.h"
#include "common/BaseLogger.h"
#include "classes-desc/ClassesDescriptions.h"

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

xobj::ObjSmoke * ConverterSmoke::toXpln(INode * inNode) {
	if (!SmokeObjParamsWrapper::isSmokeObj(inNode)) {
		return nullptr;
	}

	SmokeObjParamsWrapper params(inNode, GetCOREInterface()->GetTime(), FOREVER);

	std::unique_ptr<xobj::ObjSmoke> smoke = std::make_unique<xobj::ObjSmoke>();
	smoke->setSmokeType(params.type());
	smoke->setSize(params.size());
	smoke->setObjectName(sts::toMbString(inNode->GetName()));

	Matrix3 mOffsetTm = ConverterUtils::offsetMatrix(inNode);
	smoke->applyTransform(ConverterUtils::toXTMatrix(mOffsetTm));
	return smoke.release();
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

INode * ConverterSmoke::toMax(const xobj::ObjAbstract * object) {
	if (object->objType() != xobj::OBJ_SMOKE) {
		return nullptr;
	}

	const xobj::ObjSmoke * smoke = static_cast<const xobj::ObjSmoke*>(object);

	Interface * ip = GetCOREInterface();
	HelperObject * pobj = reinterpret_cast<SmokeObject*>(ip->CreateInstance(HELPER_CLASS_ID,
																			ClassesDescriptions::smokeObj()->ClassID()));
	if (pobj == nullptr) {
		LCritical << "Lod object <" << object->objectName() << "> couldn't be created.";
		return nullptr;
	}

	INode * pnode = ip->CreateObjectNode(pobj);
	if (pnode == nullptr) {
		LCritical << "Max node for the object <" << object->objectName() << "> couldn't be created.";
		return nullptr;
	}

	SmokeObjParamsWrapper values(pnode, GetCOREInterface()->GetTime(), FOREVER);
	values.setType(smoke->smokeType());
	values.setSize(smoke->size());
	if (!smoke->objectName().empty()) {
		pnode->SetName(toTSTR(smoke->objectName().c_str()));
	}
	// todo set the position XYZ
	return pnode;
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
