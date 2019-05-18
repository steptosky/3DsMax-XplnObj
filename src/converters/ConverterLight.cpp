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

#include "ConverterLight.h"

#pragma warning(push, 0)
#include <max.h>
#pragma warning(pop)

#include <memory>
#include <regex>

#include <xpln/obj/ObjLightNamed.h>
#include <xpln/obj/ObjLightParam.h>
#include <xpln/obj/ObjLightCustom.h>
#include <xpln/obj/ObjLightSpillCust.h>
#include <xpln/obj/ObjLightPoint.h>
#include <xpln/obj/Transform.h>

#include "common/String.h"
#include "common/Logger.h"
#include "models/MdLight.h"
#include "ConverterUtils.h"
#include "additional/math/Rad.h"
#include "additional/math/Deg.h"
#include "ExportParams.h"
#include "ImportParams.h"

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

class LightGetter : public LightIO::ILightIO {
public:

    explicit LightGetter(INode * node, const ExportParams * params)
        : mExportParams(params),
          mNode(node) {

        DbgAssert(node);
        DbgAssert(params);
        mOffsetTm = ConverterUtils::offsetMatrix(node);
    }

    //-------------------------------------------------------------------------

    virtual ~LightGetter() = default;

    //-------------------------------------------------------------------------

    void gotLight(const xobj::ObjLightPoint & light) override {
        pLight.reset(static_cast<xobj::ObjAbstractLight *>(light.clone()));
        prepare(pLight.get());
    }

    void gotLight(const xobj::ObjLightNamed & light) override {
        pLight.reset(static_cast<xobj::ObjAbstractLight *>(light.clone()));
        prepare(pLight.get());
    }

    void gotLight(const xobj::ObjLightCustom & light) override {
        pLight.reset(static_cast<xobj::ObjAbstractLight *>(light.clone()));
        prepare(pLight.get());
    }

    void gotLight(const xobj::ObjLightParam & light) override {
        using namespace std::string_literals;
        pLight.reset(static_cast<xobj::ObjAbstractLight *>(light.clone()));
        //-----------------------------------
        auto xLight = static_cast<xobj::ObjLightParam *>(pLight.get());
        auto params = xLight->rawParams();
        xobj::Point3 directionVector(0.0f);
        float falloffAngle = 1.0f;

        const bool isSpotLight = ConverterLight::isSpotLight(mNode);
        if (isSpotLight) {
            falloffAngle = std::get<0>(ConverterLight::coneAngle(mNode, mExportParams->mCurrTime));
            directionVector = ConverterLight::direction(mNode, mExportParams->mCurrTime);
        }

        try {
            const bool isSpill = sts::MbStrUtils::endsWith(xLight->name(), "_sp");
            if (isSpill) {
                params = std::regex_replace(params, std::regex(R"(\$direction)"), "$direction_sp");
            }
            else {
                params = std::regex_replace(params, std::regex(R"(\$direction_sp)"), "$direction");
                auto vars = sts::MbStrUtils::splitToVector(params, " ");
                const auto directionVar = std::find_if(vars.begin(), vars.end(), [](const auto & val) {
                    return sts::MbStrUtils::startsWith(val, "$direction");
                });
                if (directionVar != vars.end()) {
                    auto correctedResult = xobj::LightUtils::billboardCorrectConeAngle(*directionVar, falloffAngle);
                    *directionVar = std::get<0>(correctedResult);
                    falloffAngle = std::get<1>(correctedResult);
                }
                params = sts::MbStrUtils::joinContainer(vars, " ");
            }

            auto directionFn = [&]() {
                const auto dirScaleVal = xobj::LightUtils::billboardDirectionScaleFromAngle(stsff::math::degToRad(falloffAngle));
                return directionVector.toString().append(" ").append(std::to_string(dirScaleVal));
            };
            auto directionSpillFn = [&]() {
                return directionVector.toString();
            };
            auto falloffFn = [&]() {
                if (isSpotLight) {
                    return std::to_string(xobj::LightUtils::spillConeWidthFromAngle(stsff::math::degToRad(falloffAngle)));
                }
                return std::to_string(falloffAngle);
            };
            auto rgbFn = [&]() {
                auto color = ConverterLight::lightColor(mNode, mExportParams->mCurrTime);
                std::string rgb;
                rgb.append(std::to_string(color.red())).append(" ");
                rgb.append(std::to_string(color.green())).append(" ");
                rgb.append(std::to_string(color.blue()));
                return rgb;
            };

            xLight->setParams(params, {
                    {"direction", directionFn},
                    {"direction_sp", directionSpillFn},
                    {"width", falloffFn},
                    {"rgb", rgbFn},
            });

            prepare(pLight.get());
        }
        catch (const std::exception & e) {
            CLError << LogNode(mNode) << " has incorrect light name parameters: " << e.what();
            pLight.reset();
        }
    }

    void gotLight(const xobj::ObjLightSpillCust & light) override {
        pLight.reset(static_cast<xobj::ObjAbstractLight *>(light.clone()));
        //-----------------------------------
        auto xLight = static_cast<xobj::ObjLightSpillCust *>(pLight.get());
        if (ConverterLight::isSpotLight(mNode)) {
            auto cone = ConverterLight::coneAngle(mNode, mExportParams->mCurrTime);
            xLight->setSemiAngle(stsff::math::degToRad(std::get<0>(cone)));
            xLight->setDirection(ConverterLight::direction(mNode, mExportParams->mCurrTime));
        }
        else {
            xLight->setSemiRaw(1.0f);
            xLight->setDirection(xobj::Point3(0.0f));
        }
        prepare(pLight.get());
    }

    void gotNoLight() override {
        pLight.reset();
        pIsValid = false;
        pLight = nullptr;
    }

    //-------------------------------------------------------------------------

    bool pIsValid = false;
    std::unique_ptr<xobj::ObjAbstractLight> pLight;

    //-------------------------------------------------------------------------

private:

    void prepare(xobj::ObjAbstract * inOutLight) {
        inOutLight->setObjectName(sts::toMbString(mNode->GetName()));
        inOutLight->applyTransform(ConverterUtils::toXTMatrix(mOffsetTm));
        pIsValid = true;
    }

    const ExportParams * mExportParams = nullptr;
    INode * mNode = nullptr;
    Matrix3 mOffsetTm;

};

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

xobj::ObjAbstractLight * ConverterLight::toXpln(INode * node, const ExportParams & params) {
    DbgAssert(node);

    MdLight mdLight;
    if (!mdLight.linkNode(node)) {
        return nullptr;
    }

    LightGetter getter(node, &params);
    if (!mdLight.loadFromNode(&getter)) {
        return nullptr;
    }
    if (!getter.pIsValid) {
        return nullptr;
    }
    return static_cast<xobj::ObjAbstractLight *>(getter.pLight->clone());
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

bool ConverterLight::isSpotLight(INode * node) {
    auto object = node->GetObjectRef();
    DbgAssert(object);
    return object->ClassID() == Class_ID(SPOT_LIGHT_CLASS_ID, 0);
}

xobj::Color ConverterLight::lightColor(INode * node, const TimeValue time) {
    const auto object = node->GetObjectRef();
    if (object->SuperClassID() == SClass_ID(LIGHT_CLASS_ID)) {
        auto genLight = dynamic_cast<GenLight*>(object);
        if (genLight) {
            const auto rgb = genLight->GetRGBColor(time);
            return xobj::Color(rgb.x, rgb.y, rgb.z);
        }
        LError << LogNode(node) << "couldn't get GenLight";
    }
    return xobj::Color(1.0f, 1.0f, 1.0f);
}

xobj::Point3 ConverterLight::direction(INode * node, const TimeValue time) {
    // Spot
    if (isSpotLight(node)) {
        auto targetNode = node->GetTarget();
        if (targetNode) {
            const Matrix3 targetNodeTm = targetNode->GetNodeTM(time);
            const Matrix3 objectTm = node->GetObjectTM(time);
            const Point3 targetPosition = Inverse(objectTm * Inverse(targetNodeTm)).GetRow(3);
            return xobj::Point3(targetPosition.x, targetPosition.y, targetPosition.z);
        }
    }
    // Omni
    return xobj::Point3(0.0f, 0.0f, 0.0f);
}

std::tuple<float, float> ConverterLight::coneAngle(INode * node, const TimeValue time) {
    // Spot
    if (isSpotLight(node)) {
        const auto object = node->GetObjectRef();
        auto genLight = dynamic_cast<GenLight*>(object);
        if (genLight) {
            return std::make_tuple(genLight->GetFallsize(time), genLight->GetHotspot(time));
        }
    }
    // Omni
    return std::make_tuple(1.0f, 1.0f);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

INode * ConverterLight::toMax(const xobj::ObjAbstract * object, const ImportParams & params) {
    switch (object->objType()) {
        case xobj::OBJ_LIGHT_NAMED: return toMaxLightNamed(static_cast<const xobj::ObjLightNamed*>(object), params);
        case xobj::OBJ_LIGHT_CUSTOM: return toMaxLightCustom(static_cast<const xobj::ObjLightCustom*>(object), params);
        case xobj::OBJ_LIGHT_PARAM: return toMaxLightParam(static_cast<const xobj::ObjLightParam*>(object), params);
        case xobj::OBJ_LIGHT_SPILL_CUSTOM: return toMaxLightSpillCust(static_cast<const xobj::ObjLightSpillCust*>(object), params);
        case xobj::OBJ_LIGHT_POINT: return toMaxLightPoint(static_cast<const xobj::ObjLightPoint*>(object), params);
        default: break;
    }
    return nullptr;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

INode * ConverterLight::toMaxLightNamed(const xobj::ObjLightNamed * xLight, const ImportParams & params) {
    if (xLight) {
        const auto genLight = params.mCoreInterface->CreateLightObject(OMNI_LIGHT);
        if (genLight == nullptr) {
            return nullptr;
        }
        const auto node = params.mCoreInterface->CreateObjectNode(genLight);
        if (node == nullptr) {
            return nullptr;
        }
        MdLight mdLight;
        if (!mdLight.linkNode(node)) {
            LError << "Internal logic error.";
            params.mCoreInterface->DeleteNode(node);
            return nullptr;
        }
        mdLight.saveToNode(*xLight);
        setPosition(params.mCurrTime, node, xLight->transform()->mMatrix, xLight->position());
        return node;
    }
    return nullptr;
}

//-------------------------------------------------------------------------

INode * ConverterLight::toMaxLightParam(const xobj::ObjLightParam * xLight, const ImportParams & params) {
    if (xLight) {
        const auto genLight = params.mCoreInterface->CreateLightObject(OMNI_LIGHT);
        if (genLight == nullptr) {
            return nullptr;
        }
        const auto node = params.mCoreInterface->CreateObjectNode(genLight);
        if (node == nullptr) {
            return nullptr;
        }

        MdLight mdLight;
        if (!mdLight.linkNode(node)) {
            LError << "Internal logic error.";
            params.mCoreInterface->DeleteNode(node);
            return nullptr;
        }
        mdLight.saveToNode(*xLight);
        setPosition(params.mCurrTime, node, xLight->transform()->mMatrix, xLight->position());
        return node;
    }
    return nullptr;
}

//-------------------------------------------------------------------------

INode * ConverterLight::toMaxLightCustom(const xobj::ObjLightCustom * xLight, const ImportParams & params) {
    if (xLight) {
        const auto genLight = params.mCoreInterface->CreateLightObject(OMNI_LIGHT);
        if (genLight == nullptr) {
            return nullptr;
        }
        const auto node = params.mCoreInterface->CreateObjectNode(genLight);
        if (node == nullptr) {
            return nullptr;
        }

        MdLight mdLight;
        if (!mdLight.linkNode(node)) {
            LError << "Internal logic error.";
            params.mCoreInterface->DeleteNode(node);
            return nullptr;
        }
        mdLight.saveToNode(*xLight);
        setPosition(params.mCurrTime, node, xLight->transform()->mMatrix, xLight->position());
        return node;
    }
    return nullptr;
}

//-------------------------------------------------------------------------

INode * ConverterLight::toMaxLightSpillCust(const xobj::ObjLightSpillCust * xLight, const ImportParams & params) {
    if (xLight) {
        const xobj::Point3 & xPoint = xLight->direction();
        const auto genLight = params.mCoreInterface->CreateLightObject(xPoint.isEmpty() ? OMNI_LIGHT : TSPOT_LIGHT);
        if (genLight == nullptr) {
            return nullptr;
        }
        const auto node = params.mCoreInterface->CreateObjectNode(genLight);
        if (node == nullptr) {
            return nullptr;
        }

        MdLight mdLight;
        if (!mdLight.linkNode(node)) {
            LError << "Internal logic error.";
            params.mCoreInterface->DeleteNode(node);
            return nullptr;
        }
        mdLight.saveToNode(*xLight);
        setPosition(params.mCurrTime, node, xLight->transform()->mMatrix, xLight->position());

        if (!xPoint.isEmpty()) {
            INode * targetNode = node->GetTarget();
            DbgAssert(targetNode);
            if (targetNode) {
                // Get vector
                const TimeValue currTime = params.mCoreInterface->GetTime();
                Matrix3 nodeTm = node->GetObjectTM(currTime);
                nodeTm.Translate(Point3(xPoint.x, xPoint.y, xPoint.z));
                targetNode->SetNodeTM(currTime, nodeTm);
                genLight->SetFallsize(currTime, stsff::math::radToDeg(xLight->semiAngle()));
            }
        }
        return node;
    }
    return nullptr;
}

//-------------------------------------------------------------------------

INode * ConverterLight::toMaxLightPoint(const xobj::ObjLightPoint * xLight, const ImportParams & params) {
    if (xLight) {
        const auto genLight = params.mCoreInterface->CreateLightObject(OMNI_LIGHT);
        if (genLight == nullptr) {
            return nullptr;
        }
        const auto node = params.mCoreInterface->CreateObjectNode(genLight);
        if (node == nullptr) {
            return nullptr;
        }

        MdLight mdLight;
        if (!mdLight.linkNode(node)) {
            LError << "Internal logic error.";
            params.mCoreInterface->DeleteNode(node);
            return nullptr;
        }
        mdLight.saveToNode(*xLight);
        setPosition(params.mCurrTime, node, xLight->transform()->mMatrix, xLight->position());
        return node;
    }
    return nullptr;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void ConverterLight::setPosition(TimeValue /*t*/, INode * /*mode*/,
                                 const xobj::TMatrix & /*targetTm*/, const xobj::Point3 & /*position*/) {
    // TODO Implementation
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
