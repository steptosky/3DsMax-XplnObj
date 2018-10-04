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

#include <xpln/common/Point3.h>

namespace xobj {
class ObjAbstract;
class ObjAbstractLight;
class ObjLightNamed;
class ObjLightParam;
class ObjLightCustom;
class ObjLightSpillCust;
class ObjLightPoint;
class Point3;
class TMatrix;
}

class ExportParams;
class ImportParams;
class LightGetter;

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

class ConverterLight {
    friend LightGetter;
    ConverterLight() = default;
    ~ConverterLight() = default;
public:

    static xobj::ObjAbstractLight * toXpln(INode * node, const ExportParams & params);
    static INode * toMax(const xobj::ObjAbstract * object, const ImportParams & params);

private:

    // to MAX
    static INode * toMaxLightNamed(const xobj::ObjLightNamed * xLight, const ImportParams & params);
    static INode * toMaxLightParam(const xobj::ObjLightParam * xLight, const ImportParams & params);
    static INode * toMaxLightCustom(const xobj::ObjLightCustom * xLight, const ImportParams & params);
    static INode * toMaxLightSpillCust(const xobj::ObjLightSpillCust * xLight, const ImportParams & params);
    static INode * toMaxLightPoint(const xobj::ObjLightPoint * xLight, const ImportParams & params);

    static void setPosition(TimeValue t, INode * mode,
                            const xobj::TMatrix & targetTm, const xobj::Point3 & pos);

    static xobj::Point3 direction(INode * mode, TimeValue time);
    static bool isSpotLight(INode * mode);
    static float coneAngle(INode * mode, TimeValue time);

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
