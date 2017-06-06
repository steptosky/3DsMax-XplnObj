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

#include "AnimIO.h"
#include "common/Logger.h"
#include "gup/ObjCommon.h"
#include "models/eDataId.h"
#include "NodeIO.h"
#include "objects/main/MainObj.h"

#include "common/String.h"
#include "models/bwc/MdAnimIoOld.h"
#include "classes-desc/ClassesDescriptions.h"

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool AnimIO::canApply(INode * node) {
	assert(node != nullptr);
	Class_ID id = node->GetObjectRef()->ClassID();
	return id != ClassesDescriptions::mainObj()->ClassID() &&
			id != ClassesDescriptions::lodObj()->ClassID();
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void AnimIO::saveRotateToNode(INode * outNode, const MdAnimRot & inAnimRot, eAnimRotateIOID axis) {
	if (!outNode) {
		LError << "Node is nullptr";
		return;
	}
	assert(canApply(outNode));
	std::stringstream buf;
	sts::DataStreamO stream(&buf);
	stream.writeFormatVersion();
	stream.setValue<uint8_t>(inAnimRot.mVersion);
	stream.setValue<bool>(inAnimRot.mEnable);
	stream.setValue<bool>(inAnimRot.mReverse);
	stream.setValue<bool>(inAnimRot.mLoopEnable);
	stream.setValue<float>(inAnimRot.mLoopValue);
	stream.setValue<std::string>(inAnimRot.mDataref);

	uint32_t size = static_cast<uint32_t>(inAnimRot.mKeyList.size());
	stream.setValue<uint32_t>(size);
	if (size) {
		stream.writeRawData(reinterpret_cast<const char*>(inAnimRot.mKeyList.data()),
							size * sizeof(MdAnimTrans::KeyValueList::value_type));
	}

	NodeIO::saveData(outNode, static_cast<DWORD>(axis), buf.str());
}

//-------------------------------------------------------------------------

bool AnimIO::loadRotateFromNode(INode * inNode, MdAnimRot & outAnimRot, eAnimRotateIOID axis) {
	// TODO implement for the choosing the correct axis for applying from previous version
	outAnimRot.reset();
	if (!inNode) {
		LError << "Node is nullptr";
		return false;
	}

	AppDataChunk * chunk = NodeIO::dataChunk(inNode, static_cast<DWORD>(axis));
	if (!chunk) {
		chunk = bcw::MdAnimIoOld::dataRotate(inNode);
		if (!chunk) {
			return true;
		}
		if (bcw::MdAnimIoOld::loadRotateFromNode(inNode, chunk, outAnimRot)) {
			saveRotateToNode(inNode, outAnimRot, axis);
			bcw::MdAnimIoOld::removeRotateFromNode(inNode);
			return true;
		}
		return false;
	}

	if (chunk->length == 0) {
		log_unexpected_data_length(inNode, chunk->length);
		return false;
	}

	std::stringstream buf(std::string(reinterpret_cast<const char *>(chunk->data), chunk->length));
	sts::DataStreamI stream(&buf);
	stream.readAndSetFormatVersion();
	uint8_t version = stream.value<uint8_t>();
	if (version != outAnimRot.mVersion) {
		log_unexpected_version(inNode, version);
		return false;
	}

	stream.value<bool>(outAnimRot.mEnable);
	stream.value<bool>(outAnimRot.mReverse);
	stream.value<bool>(outAnimRot.mLoopEnable);
	stream.value<float>(outAnimRot.mLoopValue);
	stream.value<std::string>(outAnimRot.mDataref);

	outAnimRot.mKeyList.resize(static_cast<size_t>(stream.value<uint32_t>()), 0.0f);
	if (!outAnimRot.mKeyList.empty()) {
		stream.readRawData(reinterpret_cast<char*>(outAnimRot.mKeyList.data()),
							outAnimRot.mKeyList.size() * sizeof(MdAnimTrans::KeyValueList::value_type));
	}
	return true;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void AnimIO::saveTransToNode(INode * outNode, const MdAnimTrans & inAnimTrans) {
	if (!outNode) {
		LError << "Node is nullptr";
		return;
	}
	assert(canApply(outNode));
	std::stringstream buf;
	sts::DataStreamO stream(&buf);
	stream.writeFormatVersion();
	stream.setValue<uint8_t>(inAnimTrans.mVersion);
	stream.setValue<bool>(inAnimTrans.mEnable);
	stream.setValue<bool>(inAnimTrans.mReverse);
	stream.setValue<bool>(inAnimTrans.mLoopEnable);
	stream.setValue<float>(inAnimTrans.mLoopValue);
	stream.setValue<std::string>(inAnimTrans.mDataref);

	uint32_t size = static_cast<uint32_t>(inAnimTrans.mKeyList.size());
	stream.setValue<uint32_t>(size);
	if (size) {
		stream.writeRawData(reinterpret_cast<const char*>(inAnimTrans.mKeyList.data()),
							size * sizeof(MdAnimTrans::KeyValueList::value_type));
	}

	NodeIO::saveData(outNode, static_cast<DWORD>(eAnimTransIOID::ANIM_TRANS), buf.str());
}

//-------------------------------------------------------------------------

bool AnimIO::loadTransFromNode(INode * inNode, MdAnimTrans & outAnimTrans) {
	outAnimTrans.reset();
	if (!inNode) {
		LError << "Node is nullptr";
		return false;
	}

	AppDataChunk * chunk = NodeIO::dataChunk(inNode, static_cast<DWORD>(eAnimTransIOID::ANIM_TRANS));
	if (!chunk) {
		chunk = bcw::MdAnimIoOld::dataTrans(inNode);
		if (!chunk) {
			return true;
		}
		if (bcw::MdAnimIoOld::loadTransFromNode(inNode, chunk, outAnimTrans)) {
			saveTransToNode(inNode, outAnimTrans);
			bcw::MdAnimIoOld::removeAnimTransFromNode(inNode);
			return true;
		}
		return false;
	}

	if (chunk->length == 0) {
		log_unexpected_data_length(inNode, chunk->length);
		return false;
	}

	std::stringstream buf(std::string(reinterpret_cast<const char *>(chunk->data), chunk->length));
	sts::DataStreamI stream(&buf);
	stream.readAndSetFormatVersion();
	uint8_t version = stream.value<uint8_t>();
	if (version != outAnimTrans.mVersion) {
		log_unexpected_version(inNode, version);
		return false;
	}

	stream.value<bool>(outAnimTrans.mEnable);
	stream.value<bool>(outAnimTrans.mReverse);
	stream.value<bool>(outAnimTrans.mLoopEnable);
	stream.value<float>(outAnimTrans.mLoopValue);
	stream.value<std::string>(outAnimTrans.mDataref);

	outAnimTrans.mKeyList.resize(static_cast<size_t>(stream.value<uint32_t>()), 0.0f);
	if (!outAnimTrans.mKeyList.empty()) {
		stream.readRawData(reinterpret_cast<char*>(outAnimTrans.mKeyList.data()),
							outAnimTrans.mKeyList.size() * sizeof(MdAnimTrans::KeyValueList::value_type));
	}
	return true;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void AnimIO::saveVisibilityToNode(INode * outNode, const MdAnimVis & inAnimVis) {
	if (!outNode) {
		LError << "Node is nullptr";
		return;
	}
	assert(canApply(outNode));
	std::stringstream buf;
	sts::DataStreamO stream(&buf);
	stream.writeFormatVersion();
	stream.setValue<uint8_t>(inAnimVis.mVersion);
	stream.setValue<bool>(inAnimVis.mEnable);

	stream.setValue<uint32_t>(static_cast<uint32_t>(inAnimVis.mKeyList.size()));
	for (auto & curr : inAnimVis.mKeyList) {
		stream.setValue<uint8_t>(static_cast<uint8_t>(curr.pType));
		stream.setValue<float>(curr.pValue1);
		stream.setValue<float>(curr.pValue2);
		stream.setValue<std::string>(curr.pDrf);
	}
	NodeIO::saveData(outNode, static_cast<DWORD>(eAnimVisIOID::ANIM_VISIBILITY), buf.str());
}

//-------------------------------------------------------------------------

bool AnimIO::loadVisibilityFromNode(INode * inNode, MdAnimVis & outAnimVis) {
	outAnimVis.reset();
	if (!inNode) {
		LError << "Node is nullptr";
		return false;
	}

	AppDataChunk * chunk = NodeIO::dataChunk(inNode, static_cast<DWORD>(eAnimVisIOID::ANIM_VISIBILITY));
	if (!chunk) {
		chunk = bcw::MdAnimIoOld::dataVisibility(inNode);
		if (!chunk) {
			return true;
		}
		if (bcw::MdAnimIoOld::loadVisibilityFromNode(inNode, chunk, outAnimVis)) {
			saveVisibilityToNode(inNode, outAnimVis);
			bcw::MdAnimIoOld::removeVisibilityFromNode(inNode);
			return true;
		}
		return false;
	}

	if (chunk->length == 0) {
		log_unexpected_data_length(inNode, chunk->length);
		return false;
	}

	std::stringstream buf(std::string(reinterpret_cast<const char *>(chunk->data), chunk->length));
	sts::DataStreamI stream(&buf);
	stream.readAndSetFormatVersion();
	uint8_t version = stream.value<uint8_t>();
	if (version != outAnimVis.mVersion) {
		log_unexpected_version(inNode, version);
		return false;
	}

	stream.value<bool>(outAnimVis.mEnable);
	outAnimVis.mKeyList.resize(static_cast<size_t>(stream.value<uint32_t>()));
	for (auto & curr : outAnimVis.mKeyList) {
		curr.pType = static_cast<xobj::AnimVisibility::Key::eType>(stream.value<uint8_t>());
		stream.value<float>(curr.pValue1);
		stream.value<float>(curr.pValue2);
		stream.value<std::string>(curr.pDrf);
	}
	return true;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void AnimIO::removeRotateFromNode(INode * node, eAnimRotateIOID axis) {
	if (!node) {
		node->RemoveAppDataChunk(COMMON_CLASS_ID, GUP_CLASS_ID, static_cast<DWORD>(axis));
	}
}

void AnimIO::removeAnimTransFromNode(INode * node) {
	if (!node) {
		node->RemoveAppDataChunk(COMMON_CLASS_ID, GUP_CLASS_ID, static_cast<DWORD>(eAnimTransIOID::ANIM_TRANS));
	}
}

void AnimIO::removeVisibilityFromNode(INode * node) {
	if (!node) {
		node->RemoveAppDataChunk(COMMON_CLASS_ID, GUP_CLASS_ID, static_cast<DWORD>(eAnimVisIOID::ANIM_VISIBILITY));
	}
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool AnimIO::cloneData(INode * NodeFrom, INode * NodeTo, DWORD index) {
	if (!canApply(NodeTo)) {
		return false;
	}
	NodeIO::cloneData(NodeFrom, NodeTo, index);
	return true;
}

bool AnimIO::cloneTransData(INode * NodeFrom, INode * NodeTo) {
	return cloneData(NodeFrom, NodeTo, static_cast<DWORD>(eAnimTransIOID::ANIM_TRANS));
}

bool AnimIO::cloneRotateData(INode * NodeFrom, INode * NodeTo) {
	if (!cloneData(NodeFrom, NodeTo, static_cast<DWORD>(eAnimRotateIOID::ANIM_X_ROTATE))) {
		return false;
	}
	if (!cloneData(NodeFrom, NodeTo, static_cast<DWORD>(eAnimRotateIOID::ANIM_Y_ROTATE))) {
		return false;
	}
	if (!cloneData(NodeFrom, NodeTo, static_cast<DWORD>(eAnimRotateIOID::ANIM_Z_ROTATE))) {
		return false;
	}
	return true;
}

bool AnimIO::cloneVisibilityData(INode * NodeFrom, INode * NodeTo) {
	return cloneData(NodeFrom, NodeTo, static_cast<DWORD>(eAnimVisIOID::ANIM_VISIBILITY));
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
