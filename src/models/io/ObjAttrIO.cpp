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

#include "ObjAttrIO.h "
#include "sts/io/DataStream.h"
#include "objects/MainObjectParams.h"
#include "objects/LodObjParams.h"
#include "NodeIO.h"
#include "models/eDataId.h"
#include "common/String.h"
#include "models/bwc/MdObjAttrIOOld.h"
#include "common/Logger.h"
#include "classes-desc/ClassesDescriptions.h"

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool ObjAttrIO::canApply(INode * node) {
	assert(node);
	Class_ID id = node->GetObjectRef()->ClassID();
	if (id == ClassesDescriptions::mainObj()->ClassID() || 
		id == ClassesDescriptions::lodObj()->ClassID())
		return false;
	SClass_ID sid = node->GetObjectRef()->SuperClassID();
	if (sid == GEOMOBJECT_CLASS_ID)
		return true;
	sid = node->EvalWorldState(GetCOREInterface()->GetTime()).obj->SuperClassID();
	if (sid == GEOMOBJECT_CLASS_ID)
		return true;
	return false;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void ObjAttrIO::save(sts::DataStreamO & stream, const xobj::AttrSet & inAttr) {
	stream.setValue<uint8_t>(uint8_t(1)); // manip io version
	stream.setValue<bool>(inAttr.isSunLight());
	stream.setValue<bool>(inAttr.isTwoSided());
	stream.setValue<bool>(inAttr.isDraw());
	stream.setValue<bool>(inAttr.isDraped());
	stream.setValue<bool>(inAttr.isCastShadow());
	stream.setValue<bool>(inAttr.isSolidForCamera());
}

bool ObjAttrIO::load(INode * node, sts::DataStreamI & stream, xobj::AttrSet & outAttr) {
	uint8_t version = stream.value<uint8_t>();
	if (version != 1) {
		log_unexpected_version(node, version);
		return false;
	}
	outAttr.setSunLight(stream.value<bool>());
	outAttr.setTwoSided(stream.value<bool>());
	outAttr.setDraw(stream.value<bool>());
	outAttr.setDraped(stream.value<bool>());
	outAttr.setCastShadow(stream.value<bool>());
	outAttr.setSolidForCamera(stream.value<bool>());
	return true;
}

/***************************************************************************************/

void ObjAttrIO::save(sts::DataStreamO & stream, const xobj::AttrPolyOffset & inAttr) {
	stream.setValue<uint8_t>(uint8_t(1)); // io version
	stream.setValue<float>(inAttr.offset());
	stream.setValue<bool>(inAttr);
}

bool ObjAttrIO::loadPolyOffset(INode * node, sts::DataStreamI & stream, xobj::AttrSet & outAttr) {
	uint8_t version = stream.value<uint8_t>();
	if (version != 1) {
		log_unexpected_version(node, version);
		return false;
	}
	xobj::AttrPolyOffset out(stream.value<float>());
	out.setEnabled(stream.value<bool>());
	outAttr.setPolyOffset(out);
	return true;
}

/***************************************************************************************/

void ObjAttrIO::save(sts::DataStreamO & stream, const xobj::AttrHard & inAttr) {
	stream.setValue<uint8_t>(uint8_t(1)); // io version
	stream.setValue<std::string>(inAttr.surface().toString());
	stream.setValue<bool>(inAttr.isDeck());
	stream.setValue<bool>(inAttr);
}

bool ObjAttrIO::loadHard(INode * node, sts::DataStreamI & stream, xobj::AttrSet & outAttr) {
	uint8_t version = stream.value<uint8_t>();
	if (version != 1) {
		log_unexpected_version(node, version);
		return false;
	}
	std::string str;
	stream.value<std::string>(str);

	xobj::AttrHard out;
	out.setESurface(xobj::ESurface::fromString(str.c_str()), stream.value<bool>());
	out.setEnabled(stream.value<bool>());
	outAttr.setHard(out);
	return true;
}

/***************************************************************************************/

void ObjAttrIO::save(sts::DataStreamO & stream, const xobj::AttrShiny & inAttr) {
	stream.setValue<uint8_t>(uint8_t(1));// io version
	stream.setValue<float>(inAttr.ratio());
	stream.setValue<bool>(inAttr);
}

bool ObjAttrIO::loadShiny(INode * node, sts::DataStreamI & stream, xobj::AttrSet & outAttr) {
	uint8_t version = stream.value<uint8_t>();
	if (version != 1) {
		log_unexpected_version(node, version);
		return false;
	}
	xobj::AttrShiny out(stream.value<float>());
	out.setEnabled(stream.value<bool>());
	outAttr.setShiny(out);
	return true;
}

/***************************************************************************************/

void ObjAttrIO::save(sts::DataStreamO & stream, const xobj::AttrBlend & inAttr) {
	stream.setValue<uint8_t>(uint8_t(1)); // io version
	stream.setValue<uint8_t>(inAttr.type());
	stream.setValue<float>(inAttr.ratio());
	stream.setValue<bool>(inAttr);
}

bool ObjAttrIO::loadBlend(INode * node, sts::DataStreamI & stream, xobj::AttrSet & outAttr) {
	uint8_t version = stream.value<uint8_t>();
	if (version != 1) {
		log_unexpected_version(node, version);
		return false;
	}
	xobj::AttrBlend::eType type = static_cast<xobj::AttrBlend::eType>(stream.value<uint8_t>());
	xobj::AttrBlend out(type, stream.value<float>());
	out.setEnabled(stream.value<bool>());
	outAttr.setBlend(out);
	return true;
}

/***************************************************************************************/

void ObjAttrIO::save(sts::DataStreamO & stream, const xobj::AttrLightLevel & inAttr) {
	stream.setValue<uint8_t>(uint8_t(1)); // io version
	stream.setValue<float>(inAttr.val1());
	stream.setValue<float>(inAttr.val2());
	stream.setValue<std::string>(inAttr.dataref());
	stream.setValue<bool>(inAttr);
}

bool ObjAttrIO::loadLightLevel(INode * node, sts::DataStreamI & stream, xobj::AttrSet & outAttr) {
	uint8_t version = stream.value<uint8_t>();
	if (version != 1) {
		log_unexpected_version(node, version);
		return false;
	}
	xobj::AttrLightLevel out;
	out.setVal1(stream.value<float>());
	out.setVal2(stream.value<float>());
	out.setDataref(stream.value<std::string>());
	out.setEnabled(stream.value<bool>());
	outAttr.setLightLevel(out);
	return true;
}

/***************************************************************************************/

void ObjAttrIO::save(sts::DataStreamO & stream, const xobj::AttrCockpit & inAttr) {
	stream.setValue<uint8_t>(uint8_t(1));// io version
	stream.setValue<uint8_t>(inAttr.type());
	stream.setValue<bool>(inAttr);
}

bool ObjAttrIO::loadCockpit(INode * node, sts::DataStreamI & stream, xobj::AttrSet & outAttr) {
	uint8_t version = stream.value<uint8_t>();
	if (version != 1) {
		log_unexpected_version(node, version);
		return false;
	}
	xobj::AttrCockpit out(static_cast<xobj::AttrCockpit::eType>(stream.value<uint8_t>()));
	out.setEnabled(stream.value<bool>());
	outAttr.setCockpit(out);
	return true;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool ObjAttrIO::cloneData(INode * NodeFrom, INode * NodeTo) {
	if (!canApply(NodeTo)) {
		return false;
	}
	NodeIO::cloneData(NodeFrom, NodeTo, static_cast<DWORD>(eAttribuesIOID::OBJ_ATTRIBUTES));
	return true;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool ObjAttrIO::loadFromNode(INode * inNode, xobj::AttrSet & outAttrSet) {
	outAttrSet.reset();
	if (!inNode) {
		LError << "INode is nullptr";
		return false;
	}

	AppDataChunk * chunk = NodeIO::dataChunk(inNode, static_cast<DWORD>(eAttribuesIOID::OBJ_ATTRIBUTES));
	if (!chunk) {
		chunk = bcw::MdObjAttrIOOld::data(inNode);
		if (!chunk) {
			return true;
		}
		if (bcw::MdObjAttrIOOld::loadFromNode(inNode, chunk, outAttrSet)) {
			saveToNode(inNode, outAttrSet);
			bcw::MdObjAttrIOOld::removeFromNode(inNode);
			return true;
		}
		return false;
	}

	if (chunk->length == 0) {
		log_unexpected_data_length(inNode, chunk->length);
		return false;
	}

	//-------------------------------------------------------------------------

	std::stringstream buf(std::string(reinterpret_cast<const char *>(chunk->data), chunk->length));
	sts::DataStream stream(&buf);
	stream.readAndSetFormatVersion();
	uint8_t version = stream.value<uint8_t>();
	if (version != 1) {
		log_unexpected_version(inNode, version);
		return false;
	}
	return load(inNode, stream, outAttrSet) &&
			loadPolyOffset(inNode, stream, outAttrSet) &&
			loadHard(inNode, stream, outAttrSet) &&
			loadShiny(inNode, stream, outAttrSet) &&
			loadBlend(inNode, stream, outAttrSet) &&
			loadLightLevel(inNode, stream, outAttrSet) &&
			loadCockpit(inNode, stream, outAttrSet);
}

void ObjAttrIO::removeFromNode(INode * node) {
	NodeIO::removeData(node, static_cast<DWORD>(eAttribuesIOID::OBJ_ATTRIBUTES));
}

void ObjAttrIO::saveToNode(INode * outNode, const xobj::AttrSet & inAttrSet) {
	if (!outNode) {
		LError << "INode is nullptr";
		return;
	}

	std::stringstream buf;
	sts::DataStream stream(&buf);
	stream.writeFormatVersion();
	stream.setValue<uint8_t>(uint8_t(1)); // node io version
	save(stream, inAttrSet);
	save(stream, inAttrSet.polyOffset());
	save(stream, inAttrSet.hard());
	save(stream, inAttrSet.shiny());
	save(stream, inAttrSet.blend());
	save(stream, inAttrSet.lightLevel());
	save(stream, inAttrSet.cockpit());
	NodeIO::saveData(outNode, static_cast<DWORD>(eAttribuesIOID::OBJ_ATTRIBUTES), buf.str());
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
