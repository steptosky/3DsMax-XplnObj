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

#pragma once

#pragma warning(push, 0)
#include <max.h>
#pragma warning(pop)

#include "additional/utils/Single.h"
#include <xpln/obj/Transform.h>
#include "models/MdAnimRot.h"
#include "xpln/common/EulerXyzHelper.h"

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

class ConverterAnim : public sts::Single<ConverterAnim> {

	ConverterAnim() = default;
	~ConverterAnim() = default;

public:

	//------------------------------------------------------------------

	static bool toXPLN(INode * node, xobj::Transform * transform);

	static bool toMax(INode * /*node*/, xobj::Transform * /*transform*/) {
		// TODO Implementation
		return true;
	}

	//------------------------------------------------------------------

private:

	static void visibilityToXPLN(INode * node, xobj::Transform & transform);

	static void objAnimRotate(INode * node, xobj::Transform & transform);
	static void objAnimRotateAxis(INode * node, Control * control, char axis, xobj::AnimRotate & outXAnim);
	static xobj::AnimRotate::KeyList * getRotateAxisAnimation(Control * inAxis,
															const MdAnimRot::KeyValueList & inVals, int inIsReversed);

	//------------------------------------------------------------------

	static void objAnimTrans(INode * node, xobj::Transform & transform);
	static Point3 translateValue(Control * inXCtrl, Control * inYCtrl, Control * inZCtrl, TimeValue time);

	//------------------------------------------------------------------

	static bool rotAnimValidation(INode * node, Control * inControl, const char * inCtrlName, char axis);
	static bool transAnimValidation(INode * node, Control * inControl, const char * inCtrlName, char axis);

	//------------------------------------------------------------------

	static bool checkRotateKeysValue(INode * node,
									const xobj::AnimRotate::KeyList & keyList, const char * inCtrlName, char axis);
	static bool checkTransKeysValue(INode * node,
									const xobj::AnimTrans::KeyList & keyList, const char * inCtrlName);

	static float rotateValue(Control * inAxis, TimeValue t);

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
