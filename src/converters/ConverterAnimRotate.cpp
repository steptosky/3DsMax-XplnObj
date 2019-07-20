/*
**  Copyright(C) 2019, StepToSky
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

#include <memory>
#include <decomp.h>

#include <xpln/utils/LinearRotateHelper.h>
#include <xpln/utils/EulerXyzRotateHelper.h>
#include "ConverterAnimRotate.h"
#include "ConverterUtils.h"
#include "common/Logger.h"
#include "models/io/AnimIO.h"
#include "additional/math/Compare.h"
#include "additional/math/Rad.h"
#include "ExportParams.h"
#include "ImportParams.h"

/********************************************************************************************************/
//////////////////////////////////////////////* Functions *///////////////////////////////////////////////
/********************************************************************************************************/

bool ConverterAnimRotate::toXpln(INode & node, xobj::Transform & transform, const ExportParams & params) {
    if (!AnimIO::canApply(&node)) {
        return false;
    }
    //-------------------------------
    Control * controller = node.GetTMController();
    Control * rotation = controller->GetRotationController();
    if (!rotation) {
        return false;
    }
    if (!rotation->IsAnimated()) {
        return false;
    }

    const Class_ID controlId = rotation->ClassID();

    if (controlId == Class_ID(LININTERP_ROTATION_CLASS_ID, 0)) {
        processLinearRotate(node, transform, *rotation, params);
    }
    else {
        objAnimRotate(&node, transform, *rotation, params);
    }
    return true;
}

bool ConverterAnimRotate::toMax(INode &, xobj::Transform &, const ImportParams &) {
    // TODO Implementation
    return true;
}

/********************************************************************************************************/
//////////////////////////////////////////////* Functions *///////////////////////////////////////////////
/********************************************************************************************************/

void ConverterAnimRotate::processLinearRotate(INode & node, xobj::Transform & transform, Control & control, const ExportParams & /*params*/) {
    const auto numKeys = control.NumKeys();
    if (numKeys == 1) {
        CLError << LogNodeRef(node) << "has animation rotate with one key, supported number 2 and more.";
        return;
    }

    const auto getQuat = [](Control & control, const TimeValue time) ->Quat {
        Quat q;
        Interval interval(FOREVER);
        control.GetValue(time, q, interval);
        return q;
    };
    Interval interval(FOREVER);

    if (numKeys != 0) {
        xobj::LinearRotateHelper::Input keys;
        keys.reserve(std::size_t(numKeys));

        const float valAdd = 1.0f / float(numKeys - 1);

        for (int i = 0; i < numKeys; ++i) {
            auto quat = getQuat(control, control.GetKeyTime(i));
            const AngAxis a1(quat);
            keys.emplace_back(xobj::LinearRotateHelper::Key{xobj::Quat(quat.w, quat.x, quat.y, quat.z), valAdd * float(i)});
            // CLInfo << "[" << mzbt(a1.axis.x) << " : " << mzbt(a1.axis.y) << " : " << mzbt(a1.axis.z) << " : " << mzbt(a1.angle) << "]  "
            //         << "[" << quat.x << " : " << quat.y << " : " << quat.z << " : " << quat.w << "]  ";
            CLInfo << "[" << quat.w << " : " << quat.x << " : " << quat.y << " : " << quat.z << "]  ";

        }

        const auto mtx = ConverterUtils::toXTMatrix(node.GetNodeTM(GetCOREInterface()->GetTime(), &interval)).toRotation();
        auto animList = xobj::LinearRotateHelper::makeAnimations(keys, mtx);
        for (auto & a : animList) {
            a.mDrf = "sim/cockpit2/controls/yoke_pitch_ratio";
            LVar(msg, xobj::Logger::mInstance).info() << "[" << a.mVector.x << " : " << a.mVector.y << " : " << a.mVector.z;
            for (auto & k : a.mKeys) {
                msg << " : (" << k.mAngleDegrees << "/" << k.mDrfValue << ")";
            }
            msg << "]";
            transform.mAnimRotate.emplace_back(std::move(a));
        }
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

float ConverterAnimRotate::rotateValue(Control * inAxis, const TimeValue t) {
    Quat q;
    Interval interval(FOREVER);
    inAxis->GetValue(t, q, interval);
    return q.x;
}

xobj::AnimRotate::KeyList * ConverterAnimRotate::getRotateAxisAnimation(Control * ctrlAxis, const MdAnimRot::KeyValueList & keys,
                                                                        const int isReversed, const ExportParams & params) {
    const int rotateKeyCount = ctrlAxis->NumKeys();
    xobj::AnimRotate::KeyList * xAnim = new xobj::AnimRotate::KeyList(static_cast<size_t>(rotateKeyCount));
    //------------------------------------------------------------

    const float shift = rotateValue(ctrlAxis, params.mCurrTime);
    for (int currentKey = 0; currentKey < rotateKeyCount; ++currentKey) {
        float value = rotateValue(ctrlAxis, ctrlAxis->GetKeyTime(currentKey));
        value -= shift;
        if (isReversed) {
            value = -value;
        }
        xobj::AnimRotate::Key & key = (*xAnim)[static_cast<size_t>(currentKey)];
        key.mAngleDegrees = stsff::math::radToDeg(value);
        key.mDrfValue = keys[currentKey];
    }
    //------------------------------------------------------------

    return xAnim;
}

//-------------------------------------------------------------------------

void ConverterAnimRotate::objAnimRotateAxis(INode * node, Control * control, char axis,
                                            xobj::AnimRotate & outXAnim, const ExportParams & params) {
    Control * rotateControl = nullptr;
    std::unique_ptr<MdAnimRot> mdAnimRot = nullptr;
    switch (axis) {
        case 'x': {
            rotateControl = control->GetXController();
            mdAnimRot = std::make_unique<MdAnimRot>(MdAnimRot::X);
            break;
        }
        case 'y': {
            rotateControl = control->GetYController();
            mdAnimRot = std::make_unique<MdAnimRot>(MdAnimRot::Y);
            break;
        }
        case 'z': {
            rotateControl = control->GetZController();
            mdAnimRot = std::make_unique<MdAnimRot>(MdAnimRot::Z);
            break;
        }
        default: return;
    }
    //------------------------------------------------------------

    if (!rotAnimValidation(node, rotateControl, "rotate", axis)) {
        return;
    }
    //------------------------------------------------------------

    if (!mdAnimRot->linkNode(node, true)) {
        return;
    }

    if (rotateControl->IsAnimated()) {
        if (!mdAnimRot->mEnable) {
            CLError << LogNode(node) << "has the animated \"" << axis << "\" rotate controller but the animation export is not enabled,"
                    << " this may lead to wrong animation result.";
            return;
        }
    }
    else {
        if (mdAnimRot->mEnable) {
            CLError << LogNode(node) << "has enabled the \"" << axis
                    << "\" rotate animation export but the object does not have any animation keys.";
        }
        const float value = rotateValue(rotateControl, params.mCurrTime);
        outXAnim.mKeys.emplace_back(xobj::AnimRotate::Key(stsff::math::radToDeg(value), 0.0f));
        return;
    }
    //------------------------------------------------------------

    const int rotateKeyCount = rotateControl->NumKeys();
    if (rotateKeyCount != static_cast<int>(mdAnimRot->mKeyList.size())) {
        CLError << LogNode(node) << "animation keys and animation key values in the object are not equaled!";
        return;
    }

    xobj::AnimRotate::KeyList * keyList = getRotateAxisAnimation(rotateControl, mdAnimRot->mKeyList, mdAnimRot->mReverse, params);
    if (keyList) {
        outXAnim.mDrf = sts::toMbString(mdAnimRot->mDataref);
        outXAnim.mLoop = mdAnimRot->mLoopEnable ? std::optional(mdAnimRot->mLoopValue) : std::nullopt;
        outXAnim.mKeys.swap(*keyList);
        delete keyList;
        checkRotateKeysValue(node, outXAnim.mKeys, "rotate", axis);
    }
}

//-------------------------------------------------------------------------

void ConverterAnimRotate::objAnimRotate(INode * node, xobj::Transform & transform, Control & control, const ExportParams & params) {
    xobj::EulerXyzRotateHelper xEuler;

    objAnimRotateAxis(node, &control, 'x', xEuler.pX, params);
    objAnimRotateAxis(node, &control, 'y', xEuler.pY, params);
    objAnimRotateAxis(node, &control, 'z', xEuler.pZ, params);

    std::size_t animCount = 0;
    if (xEuler.pX.mKeys.size() > 1) {
        ++animCount;
    }
    if (xEuler.pY.mKeys.size() > 1) {
        ++animCount;
    }
    if (xEuler.pZ.mKeys.size() > 1) {
        ++animCount;
    }
    if (animCount > 1) {
        CLError << LogNode(node) << "the plugin does not support more than one axis of rotate animation at the same time";
    }
    xEuler.addToTransform(transform);
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool ConverterAnimRotate::rotAnimValidation(INode * node, Control * control, const char * inCtrlName, const char axis) {
    if (!control) {
        CLError << LogNode(node) << "does not have valid \"" << axis << " " << inCtrlName << "\" controller";
        return false;
    }

    IKeyControl * iKeys = GetKeyControlInterface(control);
    if (!iKeys) {
        CLError << LogNode(node) << "does not have valid \""
                << axis << " " << inCtrlName << "\" controller."
                << " Please, use either Bezier float or linear float controller.";
        return false;
    }

    const int numKeys = iKeys->GetNumKeys();
    if (numKeys != 0) {
        if (control->ClassID() == Class_ID(HYBRIDINTERP_FLOAT_CLASS_ID, 0)) {
            IBezFloatKey key;
            for (int i = 0; i < numKeys; i++) {
                iKeys->GetKey(i, &key);
                if (GetInTanType(key.flags) != BEZKEY_LINEAR || GetOutTanType(key.flags) != BEZKEY_LINEAR) {
                    CLWarning << LogNode(node)
                            << "has unsupported Bezier tangents in key number \"" << i << "\""
                            << " of " << "\"" << axis << " " << inCtrlName << "\" controller."
                            << " Please, use linear \"in\" and linear \"out\" tangents.";
                }
            }
        }
        else if (control->ClassID() != Class_ID(LININTERP_FLOAT_CLASS_ID, 0)) {
            CLError << LogNode(node)
                    << "has unsupported \"" << inCtrlName << "\" controller on \"" << axis << "\" axis."
                    << " Please, use either Bezier float or linear float controller.";
            return false;
        }
    }
    else if (numKeys == 1) {
        CLError << LogNode(node) << "\"" << inCtrlName << "\" controller on \"" << axis << "\" axis has only one key.";
        return false;
    }
    return true;
}

bool ConverterAnimRotate::checkRotateKeysValue(INode * node, const xobj::AnimRotate::KeyList & keys, const char * ctrlName, const char axis) {
    DbgAssert(node);
    DbgAssert(ctrlName);
    const float threshold = 0.0001f;
    const auto size = keys.size();
    //-------------------------------------------------------------------------
    if (size == 0 || size == 1) {
        return false;
    }
    //-------------------------------------------------------------------------
    if (size == 2) {
        if (stsff::math::isEqual(keys[0].mAngleDegrees, keys[1].mAngleDegrees, threshold)) {
            CLWarning << LogNode(node) << "has the same key value [0:1] on \"" << axis << "-" << ctrlName << "\" controller.";
            return false;
        }

        if (stsff::math::isEqual(keys[0].mDrfValue, keys[1].mDrfValue, threshold)) {
            CLWarning << LogNode(node) << "has the same dataref value [0:1] on \"" << axis << "-" << ctrlName << "\" controller.";
            return false;
        }
        return true;
    }
    //-------------------------------------------------------------------------
    for (size_t i = 0; i < size; ++i) {
        const size_t k1 = i + 0;
        const size_t k2 = i + 1;
        const size_t k3 = i + 2;
        if (k3 >= size) {
            return true;
        }

        if (stsff::math::isEqual(keys[k1].mAngleDegrees, keys[k2].mAngleDegrees, threshold) &&
            stsff::math::isEqual(keys[k2].mAngleDegrees, keys[k3].mAngleDegrees, threshold)) {
            CLWarning << LogNode(node) << "has the same key value [" << k1 << ":" << k2 << ":" << k3 << "] on \""
                    << axis << "-" << ctrlName << "\" controller.";
            return false;
        }

        if (stsff::math::isEqual(keys[k1].mDrfValue, keys[k2].mDrfValue, threshold) &&
            stsff::math::isEqual(keys[k2].mDrfValue, keys[k3].mDrfValue, threshold)) {
            CLWarning << LogNode(node) << "has the same dataref value [" << k1 << ":" << k2 << ":" << k3 << "] on \""
                    << axis << "-" << ctrlName << "\" controller.";
            return false;
        }
    }
    //-------------------------------------------------------------------------
    return true;
}

/*********************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*********************************************************************************************************/
