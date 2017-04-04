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

#include <xpln/obj/Transform.h>

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

class ConverterUtils {
public:

	static const Matrix3 TOOGL_MTX;
	static const Matrix3 FROMOGL_MTX;

	static Point3 toOgl(const Point3 & inPoint) {
		return inPoint * TOOGL_MTX;
	}

	static Matrix3 toOgl(const Matrix3 & inMatrix3) {
		return inMatrix3 * TOOGL_MTX;
	}

	static Point3 toMax(const Point3 & inPoint) {
		return inPoint * FROMOGL_MTX;
	}

	static Matrix3 toMax(const Matrix3 & inMatrix3) {
		return inMatrix3 * FROMOGL_MTX;
	}

	static void toXTransform(INode * inXNode, xobj::Transform & outTransform) {
		assert(inXNode);
		toXTransform(inXNode->GetNodeTM(GetCOREInterface()->GetTime()), outTransform);
	}

	static void toXTransform(const Matrix3 & inTm, xobj::Transform & outTransform) {
		toXTMatrix(inTm, outTransform.pMatrix);

	}

	static void toXTMatrix(const Matrix3 & inTm, xobj::TMatrix & outMtx);

	static xobj::TMatrix toXTMatrix(const Matrix3 & inTm) {
		xobj::TMatrix outTm;
		toXTMatrix(inTm, outTm);
		return outTm;
	}

	static Matrix3 offsetMatrix(INode * node) {
		TimeValue t = GetCOREInterface()->GetTime();
		return node->GetObjectTM(t) * Inverse(node->GetNodeTM(t));
	}

	static void toMaxTransform(const xobj::Transform & inTransform, INode * outNode);

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
