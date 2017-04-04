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

#include "MdAnimRot.h"
#include "common/Logger.h"
#include "common/String.h"
#include "models/io/AnimIO.h"

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

MdAnimRot::MdAnimRot(eAxis inAxis)
	: mAxis(inAxis) {
	reset();
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void MdAnimRot::reset() {
	mDataref = "none";
	mLoopValue = 0.0;
	mEnable = false;
	mReverse = false;
	mLoopEnable = false;
	mKeyList.clear();
}

void MdAnimRot::cloneData(INode * from, INode * to) {
	AnimIO::cloneRotateData(from, to);
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

MdAnimRot::KeyTimeList MdAnimRot::getKeyTimeList(INode * inNode, eAxis inAxis) {
	if (!inNode) {
		LError << "Node is nullptr";
		return KeyTimeList();
	}

	KeyTimeList keyTimeList;
	Control * rotateConrol = nullptr;
	switch (inAxis) {
		case X: {
			Control * tmctrl = inNode->GetTMController();
			if (tmctrl) {
				Control * rotctrl = tmctrl->GetRotationController();
				if (rotctrl) {
					rotateConrol = rotctrl->GetXController();
				}
			}
			break;
		}
		case Y: {
			Control * tmctrl = inNode->GetTMController();
			if (tmctrl) {
				Control * rotctrl = tmctrl->GetRotationController();
				if (rotctrl) {
					rotateConrol = rotctrl->GetYController();
				}
			}
			break;
		}
		case Z: {
			Control * tmctrl = inNode->GetTMController();
			if (tmctrl) {
				Control * rotctrl = tmctrl->GetRotationController();
				if (rotctrl) {
					rotateConrol = rotctrl->GetZController();
				}
			}
			break;
		}
		default: break;
	}
	if (rotateConrol == nullptr) {
		keyTimeList.clear();
		return keyTimeList;
	}
	keyTimeList.resize(static_cast<size_t>(rotateConrol->NumKeys()));
	int i = 0;
	for (auto & curr : keyTimeList) {
		curr = rotateConrol->GetKeyTime(i);
		++i;
	}
	return keyTimeList;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void MdAnimRot::clearLink() {
	mNode = nullptr;
	reset();
}

bool MdAnimRot::linkNode(INode * node, bool loadData) {
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

void MdAnimRot::saveToNode(INode * node) const {
	if (node) {
		try {
			AnimIO::saveRotateToNode(node, *this, static_cast<eAnimRotateIOID>(mAxis));
		}
		catch (std::exception & e) {
			LCritical << "Can't save data to <" << sts::toMbString(node->GetName())
					<< "> object. Reason: <" << e.what() << ">";
		}
	}
}

bool MdAnimRot::loadFromNode(INode * node) {
	if (node) {
		try {
			return AnimIO::loadRotateFromNode(node, *this, static_cast<eAnimRotateIOID>(mAxis));
		}
		catch (std::exception & e) {
			LCritical << "Can't load data from <" << sts::toMbString(node->GetName())
					<< "> object. Reason: <" << e.what() << ">";
		}
	}
	return false;
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
