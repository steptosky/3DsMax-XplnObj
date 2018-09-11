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

struct LightGetter : LightIO::ILightIO {

    explicit LightGetter(INode * node, const ExportParams * params)
        : mExportParams(params),
          mNode(node) {

        assert(node);
        assert(params);
        mOffsetTm = ConverterUtils::offsetMatrix(node);
    }

    //-------------------------------------------------------------------------

    virtual ~LightGetter() { delete pLight; }

    //-------------------------------------------------------------------------

    void gotLight(const xobj::ObjLightCustom & inLight) override {
        pLight = static_cast<xobj::ObjAbstractLight *>(inLight.clone());
        pLight->setObjectName(sts::toMbString(mNode->GetName()));
        pLight->applyTransform(ConverterUtils::toXTMatrix(mOffsetTm));
        pIsValid = true;
    }

    void gotLight(const xobj::ObjLightNamed & inLight) override {
        pLight = static_cast<xobj::ObjAbstractLight *>(inLight.clone());
        pLight->setObjectName(sts::toMbString(mNode->GetName()));
        pLight->applyTransform(ConverterUtils::toXTMatrix(mOffsetTm));
        pIsValid = true;
    }

    void gotLight(const xobj::ObjLightParam & inLight) override {
        pLight = static_cast<xobj::ObjAbstractLight *>(inLight.clone());
        pLight->setObjectName(sts::toMbString(mNode->GetName()));
        pLight->applyTransform(ConverterUtils::toXTMatrix(mOffsetTm));
        pIsValid = true;
    }

    void gotLight(const xobj::ObjLightPoint & inLight) override {
        pLight = static_cast<xobj::ObjAbstractLight *>(inLight.clone());
        pLight->setObjectName(sts::toMbString(mNode->GetName()));
        pLight->applyTransform(ConverterUtils::toXTMatrix(mOffsetTm));
        pIsValid = true;
    }

    void gotLight(const xobj::ObjLightSpillCust & inLight) override {
        pLight = static_cast<xobj::ObjAbstractLight *>(inLight.clone());
        //-----------------------------------
        auto * lobj = static_cast<xobj::ObjLightSpillCust *>(pLight);
        Object * obj = mNode->GetObjectRef();

        // Omni
        if (obj->ClassID() != Class_ID(SPOT_LIGHT_CLASS_ID, 0)) {
            lobj->setSemiRaw(1.0f);
            lobj->setDirection(xobj::Point3(0.0f, 0.0f, 0.0f));
        }
        // Spot
        if (obj->ClassID() == Class_ID(SPOT_LIGHT_CLASS_ID, 0)) {
            // get/check target
            INode * targetNode = mNode->GetTarget();
            DbgAssert(targetNode);
            if (targetNode) {
                // Get vector
                const TimeValue currTime = mExportParams->mCurrTime;
                const Matrix3 targetNodeTm = targetNode->GetNodeTM(currTime);
                const Matrix3 nodeTm = mNode->GetObjectTM(currTime);
                const Point3 targetPosition = Inverse(nodeTm * Inverse(targetNodeTm)).GetRow(3);
                lobj->setDirection(xobj::Point3(targetPosition.x, targetPosition.y, targetPosition.z));
                //Get cone
                GenLight * light = dynamic_cast<GenLight*>(obj);
                lobj->setSemiAngle(stsff::math::degToRad(light->GetFallsize(currTime)));
            }
        }

        pLight->setObjectName(sts::toMbString(mNode->GetName()));
        pLight->applyTransform(ConverterUtils::toXTMatrix(mOffsetTm));
        pIsValid = true;
    }

    void gotNoLight() override {
        delete pLight;
        pIsValid = false;
        pLight = nullptr;
    }

    //-------------------------------------------------------------------------

    bool pIsValid = false;
    xobj::ObjAbstractLight * pLight = nullptr;

    //-------------------------------------------------------------------------

private:
    const ExportParams * mExportParams = nullptr;
    INode * mNode = nullptr;
    Matrix3 mOffsetTm;

};

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

xobj::ObjAbstractLight * ConverterLight::toXpln(INode * node, const ExportParams & params) {
    assert(node);

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

INode * ConverterLight::toMaxLightNamed(const xobj::ObjLightNamed * inObjLight, const ImportParams & params) {
    if (inObjLight) {
        GenLight * mLight = params.mCoreInterface->CreateLightObject(OMNI_LIGHT);
        if (mLight == nullptr)
            return nullptr;
        INode * pnode = params.mCoreInterface->CreateObjectNode(mLight);
        if (pnode == nullptr)
            return nullptr;

        MdLight mdLight;
        if (!mdLight.linkNode(pnode)) {
            LError << "Internal logic error.";
            params.mCoreInterface->DeleteNode(pnode);
            return nullptr;
        }
        mdLight.saveToNode(*inObjLight);
        setPosition(params.mCurrTime, pnode, inObjLight->transform()->pMatrix, inObjLight->position());
        return pnode;
    }
    return nullptr;
}

//-------------------------------------------------------------------------

INode * ConverterLight::toMaxLightParam(const xobj::ObjLightParam * inObjLight, const ImportParams & params) {
    if (inObjLight) {
        GenLight * mLight = params.mCoreInterface->CreateLightObject(OMNI_LIGHT);
        if (mLight == nullptr)
            return nullptr;
        INode * pnode = params.mCoreInterface->CreateObjectNode(mLight);
        if (pnode == nullptr)
            return nullptr;

        MdLight mdLight;
        if (!mdLight.linkNode(pnode)) {
            LError << "Internal logic error.";
            params.mCoreInterface->DeleteNode(pnode);
            return nullptr;
        }
        mdLight.saveToNode(*inObjLight);
        setPosition(params.mCurrTime, pnode, inObjLight->transform()->pMatrix, inObjLight->position());
        return pnode;
    }
    return nullptr;
}

//-------------------------------------------------------------------------

INode * ConverterLight::toMaxLightCustom(const xobj::ObjLightCustom * inObjLight, const ImportParams & params) {
    if (inObjLight) {
        GenLight * light = params.mCoreInterface->CreateLightObject(OMNI_LIGHT);
        if (light == nullptr)
            return nullptr;
        INode * pnode = params.mCoreInterface->CreateObjectNode(light);
        if (pnode == nullptr)
            return nullptr;

        MdLight mdLight;
        if (!mdLight.linkNode(pnode)) {
            LError << "Internal logic error.";
            params.mCoreInterface->DeleteNode(pnode);
            return nullptr;
        }
        mdLight.saveToNode(*inObjLight);
        setPosition(params.mCurrTime, pnode, inObjLight->transform()->pMatrix, inObjLight->position());
        return pnode;
    }
    return nullptr;
}

//-------------------------------------------------------------------------

INode * ConverterLight::toMaxLightSpillCust(const xobj::ObjLightSpillCust * inObjLight, const ImportParams & params) {
    if (inObjLight) {
        const xobj::Point3 & xpoint = inObjLight->direction();
        GenLight * light = params.mCoreInterface->CreateLightObject(xpoint.isEmpty() ? OMNI_LIGHT : TSPOT_LIGHT);
        if (light == nullptr)
            return nullptr;
        INode * pnode = params.mCoreInterface->CreateObjectNode(light);
        if (pnode == nullptr)
            return nullptr;

        MdLight mdLight;
        if (!mdLight.linkNode(pnode)) {
            LError << "Internal logic error.";
            params.mCoreInterface->DeleteNode(pnode);
            return nullptr;
        }
        mdLight.saveToNode(*inObjLight);
        setPosition(params.mCurrTime, pnode, inObjLight->transform()->pMatrix, inObjLight->position());

        if (!xpoint.isEmpty()) {
            INode * targetNode = pnode->GetTarget();
            DbgAssert(targetNode);
            if (targetNode) {
                // Get vector
                const TimeValue currTime = params.mCoreInterface->GetTime();
                Matrix3 nodeTm = pnode->GetObjectTM(currTime);
                nodeTm.Translate(Point3(xpoint.x, xpoint.y, xpoint.z));
                targetNode->SetNodeTM(currTime, nodeTm);
                light->SetFallsize(currTime, stsff::math::radToDeg(inObjLight->semiAngle()));
            }
        }
        return pnode;
    }
    return nullptr;
}

//-------------------------------------------------------------------------

INode * ConverterLight::toMaxLightPoint(const xobj::ObjLightPoint * inObjLight, const ImportParams & params) {
    if (inObjLight) {
        GenLight * light = params.mCoreInterface->CreateLightObject(OMNI_LIGHT);
        if (light == nullptr)
            return nullptr;
        INode * pnode = params.mCoreInterface->CreateObjectNode(light);
        if (pnode == nullptr)
            return nullptr;

        MdLight mdLight;
        if (!mdLight.linkNode(pnode)) {
            LError << "Internal logic error.";
            params.mCoreInterface->DeleteNode(pnode);
            return nullptr;
        }
        mdLight.saveToNode(*inObjLight);
        setPosition(params.mCurrTime, pnode, inObjLight->transform()->pMatrix, inObjLight->position());
        return pnode;
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
