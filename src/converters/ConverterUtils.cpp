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

#include "ConverterUtils.h"
#include <cassert>

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

const Matrix3 ConverterUtils::TOOGL_MTX =
        Matrix3(Point3(1, 0, 0), Point3(0, 0, -1), Point3(0, 1, 0), Point3(0, 0, 0));

const Matrix3 ConverterUtils::FROMOGL_MTX =
        Matrix3(Point3(1, 0, 0), Point3(0, 0, 1), Point3(0, -1, 0), Point3(0, 0, 0));

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

void ConverterUtils::toXTMatrix(const Matrix3 & inTm, xobj::TMatrix & outMtx) {
    outMtx.set(inTm[0].x, inTm[0].y, inTm[0].z,
               inTm[1].x, inTm[1].y, inTm[1].z,
               inTm[2].x, inTm[2].y, inTm[2].z,
               inTm[3].x, inTm[3].y, inTm[3].z);

}

void ConverterUtils::toMaxTransform(const xobj::Transform & inTransform, INode * outNode) {
    xobj::Point3 row1 = inTransform.pMatrix.row(0);
    xobj::Point3 row2 = inTransform.pMatrix.row(1);
    xobj::Point3 row3 = inTransform.pMatrix.row(2);
    xobj::Point3 row4 = inTransform.pMatrix.row(3);
    Matrix3 mtx(Point3(row1.x, row1.y, row1.z),
                Point3(row2.x, row2.y, row2.z),
                Point3(row3.x, row3.y, row3.z),
                Point3(row4.x, row4.y, row4.z));
    outNode->SetNodeTM(GetCOREInterface()->GetTime(), mtx);
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
