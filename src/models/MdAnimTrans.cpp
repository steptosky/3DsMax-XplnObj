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

#include "MdAnimTrans.h"
#include "common/Logger.h"
#include "models/io/NodeIO.h"
#include "common/String.h"
#include "models/io/AnimIO.h"

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

MdAnimTrans::MdAnimTrans() {
	reset();
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void MdAnimTrans::reset() {
	mDataref = "none";
	mLoopValue = 0.0;
	mEnable = false;
	mReverse = false;
	mLoopEnable = false;
	mKeyList.clear();
}

void MdAnimTrans::cloneData(INode * from, INode * to) {
	AnimIO::cloneTransData(from, to);
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

MdAnimTrans::KeyTimeList MdAnimTrans::getKeyTimeList(INode * inNode) {
	if (!inNode) {
		LError << "Node is nullptr";
		return KeyTimeList();
	}

	KeyTimeList keyTimeList;
	Control * posControl = inNode->GetTMController()->GetPositionController();
	keyTimeList.resize(static_cast<size_t>(posControl->NumKeys()));
	int i = 0;
	for (auto & curr : keyTimeList) {
		curr = posControl->GetKeyTime(i);
		++i;
	}
	return keyTimeList;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void MdAnimTrans::clearLink() {
	mNode = nullptr;
	reset();
}

bool MdAnimTrans::linkNode(INode * node, bool loadData) {
	clearLink();
	if (node == nullptr) {
		return false;
	}

	if (!AnimIO::canApply(node)) {
		return false;
	}

	mNode = node;
	if (loadData) {
		if (!loadFromNode()) {
			clearLink();
			return false;
		}
	}
	return true;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void MdAnimTrans::saveToNode(INode * node) const {
	if (node) {
		try {
			AnimIO::saveTransToNode(node, *this);
		}
		catch (std::exception & e) {
			LCritical << "Can't save data to <" << sts::toMbString(node->GetName())
					<< "> object. Reason: <" << e.what() << ">";
		}
	}
}

bool MdAnimTrans::loadFromNode(INode * node) {
	if (node) {
		try {
			return AnimIO::loadTransFromNode(node, *this);
		}
		catch (std::exception & e) {
			LCritical << "Can't load datafrom <" << sts::toMbString(node->GetName())
					<< "> object. Reason: <" << e.what() << ">";
		}
	}
	return false;
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
