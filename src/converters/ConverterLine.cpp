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

#include "ConverterLine.h"

#pragma warning(push, 0)
#include <max.h>
#include <splshape.h>
#pragma warning(pop)

#include <xpln/obj/ObjLine.h>
#include <cassert>
#include "common/String.h"
#include "ConverterUtils.h"
#include "ExportParams.h"
#include "ImportParams.h"

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

ConverterLine::ObjLineList ConverterLine::toXpln(INode * inNode, const Matrix3 & inTargetTm, const ExportParams & params) {
    assert(inNode);

    ObjLineList linelist;

    SplineShape * shape = getShape(inNode, params);
    if (shape == nullptr)
        return linelist;

    const auto currTime = params.mCurrTime;
    Matrix3 tm = inNode->GetNodeTM(currTime);
    Matrix3 offsetTm = ConverterUtils::offsetMatrix(inNode);

    // collect lines to line container... 
#if MAX_VERSION_MAJOR < 20 // 2018
    const int numCurves = shape->NumberOfCurves();
#else
    const int numCurves = shape->NumberOfCurves(currTime);
#endif
    for (int idx = 0; idx < numCurves; idx++) {
        Spline3D * spline = shape->shape.GetSpline(idx);
        int numKnots = spline->KnotCount();
        xobj::ObjLine * xLine = nullptr;
        Point3 point;

        if (numKnots) {
            xLine = new xobj::ObjLine;
            xLine->setObjectName(sts::toMbString(inNode->GetName()));
            linelist.push_back(xLine);
        }
        for (int kid = 0; kid < numKnots; kid++) {
            point = spline->GetKnotPoint(kid);
            point = (point * tm) * Inverse(inTargetTm);
            point = point * offsetTm;
            xLine->verticesList().push_back(xobj::ObjLine::Vertex(xobj::Point3(point.x, point.y, point.z),
                                                                  xobj::Color(0.5, 0.5, 0.5)));
        }
        // TODO: Doesn't work after refactoring
        // xLine->setClosed(spline->Closed() ? true : false);
        xLine->setObjectName(inNode->GetName());
    }

    return linelist;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

SplineShape * ConverterLine::getShape(INode * inNode, const ExportParams & params) {
    SplineShape * shape = nullptr;
    ObjectState os = inNode->EvalWorldState(params.mCurrTime);
    if (os.obj->IsSubClassOf(splineShapeClassID)) {
        shape = static_cast<SplineShape *>(os.obj);
    }
    else if (os.obj->CanConvertToType(splineShapeClassID)) {
        shape = static_cast<SplineShape *>(os.obj->ConvertToType(params.mCurrTime, splineShapeClassID));
    }
    if (shape == nullptr)
        return nullptr;

    return shape;
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
