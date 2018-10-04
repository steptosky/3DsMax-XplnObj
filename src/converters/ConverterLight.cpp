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
        pLight.reset(static_cast<xobj::ObjAbstractLight *>(light.clone()));
        prepare(pLight.get());
    }

    void gotLight(const xobj::ObjLightSpillCust & light) override {
        pLight.reset(static_cast<xobj::ObjAbstractLight *>(light.clone()));
        //-----------------------------------
        auto xLight = static_cast<xobj::ObjLightSpillCust *>(pLight.get());
        if (ConverterLight::isSpotLight(mNode)) {
            xLight->setSemiAngle(stsff::math::degToRad(ConverterLight::coneAngle(mNode, mExportParams->mCurrTime)));
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

bool ConverterLight::isSpotLight(INode * mode) {
    auto object = mode->GetObjectRef();
    DbgAssert(object);
    return object->ClassID() == Class_ID(SPOT_LIGHT_CLASS_ID, 0);
}

xobj::Point3 ConverterLight::direction(INode * mode, const TimeValue time) {
    // Spot
    if (isSpotLight(mode)) {
        auto targetNode = mode->GetTarget();
        if (targetNode) {
            const Matrix3 targetNodeTm = targetNode->GetNodeTM(time);
            const Matrix3 objectTm = mode->GetObjectTM(time);
            const Point3 targetPosition = Inverse(objectTm * Inverse(targetNodeTm)).GetRow(3);
            return xobj::Point3(targetPosition.x, targetPosition.y, targetPosition.z);
        }
    }
    // Omni
    return xobj::Point3(0.0f, 0.0f, 0.0f);
}

float ConverterLight::coneAngle(INode * mode, const TimeValue time) {
    // Spot
    if (isSpotLight(mode)) {
        const auto object = mode->GetObjectRef();
        auto genLight = dynamic_cast<GenLight*>(object);
        if (genLight) {
            return genLight->GetFallsize(time);
        }
    }
    // Omni
    return 1.0f;
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
        setPosition(params.mCurrTime, node, xLight->transform()->pMatrix, xLight->position());
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
        setPosition(params.mCurrTime, node, xLight->transform()->pMatrix, xLight->position());
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
        setPosition(params.mCurrTime, node, xLight->transform()->pMatrix, xLight->position());
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
        setPosition(params.mCurrTime, node, xLight->transform()->pMatrix, xLight->position());

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
        setPosition(params.mCurrTime, node, xLight->transform()->pMatrix, xLight->position());
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
