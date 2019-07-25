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

#include "ConverterAnimRotate.h"
#include "ConverterUtils.h"
#include "common/Logger.h"
#include "models/io/AnimIO.h"
#include "additional/math/Compare.h"
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
    if (!controller) {
        return false;
    }

    Control * rotation = controller->GetRotationController();
    if (!rotation) {
        return false;
    }

    if (rotation->ClassID() == Class_ID(LININTERP_ROTATION_CLASS_ID, 0)) {
        processLinearRotate(node, transform, *rotation, params);
        DbgAssert(false);
    }
    else {
        processEuler(&node, transform, *rotation, params);
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

std::size_t ConverterAnimRotate::calculateLinearAxisNum(INode * node) {
    if (!node) {
        return 0;
    }
    Control * controller = node->GetTMController();
    DbgAssert(controller);
    Control * rotation = controller->GetRotationController();
    if (!rotation) {
        return 0;
    }
    if (rotation->ClassID() != Class_ID(LININTERP_ROTATION_CLASS_ID, 0)) {
        return 0;
    }
    const auto numKeys = rotation->NumKeys();
    if (numKeys < 2) {
        return 0;
    }

    Interval interval(FOREVER);
    xobj::LinearRotation anim;
    anim.mKeys.reserve(std::size_t(numKeys));

    for (int i = 0; i < numKeys; ++i) {
        Quat quat;
        rotation->GetValue(rotation->GetKeyTime(i), quat, interval);
        anim.mKeys.emplace_back(xobj::LinearRotation::Key{xobj::Quat(quat.w, quat.x, quat.y, quat.z), 0});
    }

    return anim.retrieveAxes().mAxes.size();
}

/********************************************************************************************************/
//////////////////////////////////////////////* Functions *///////////////////////////////////////////////
/********************************************************************************************************/

void ConverterAnimRotate::processLinearRotate(INode & node, xobj::Transform & transform, Control & control, const ExportParams & params) {
    const auto numKeys = control.NumKeys();
    if (numKeys == 1) {
        CLError << LogNodeRef(node) << "has animation rotate with one key, supported number 2 and more.";
        return;
    }
    //------------------------------------------------------------
    MdAnimRot animRotate(MdAnimRot::LINEAR);
    if (!animRotate.linkNode(&node, true)) {
        return;
    }
    if (!control.IsAnimated()) {
        if (animRotate.mEnable) {
            CLError << LogNodeRef(node) << "has enabled rotate animation export but the object does not have any animation keys.";
        }
        return;
    }
    if (!animRotate.mEnable) {
        CLError << LogNodeRef(node) << "has animation rotate but the animation export isn't enabled, this may lead to wrong animation result.";
        return;
    }
    if (animRotate.mReverse) {
        CLError << LogNodeRef(node) << "uses removed 'reverse' checkbox in rotation, you have to fix your animation.";
    }
    //----------------------------------------------
    if (numKeys < 2) {
        CLError << LogNodeRef(node) << "has rotation keys less than 2.";
        return;
    }
    //----------------------------------------------
    if (numKeys != static_cast<int>(animRotate.mKeyList.size())) {
        CLError << LogNodeRef(node) << "mismatch animation keys and dataref values number on rotation";
        return;
    }
    //------------------------------------------------------------
    Interval interval(FOREVER);
    if (numKeys != 0) {
        xobj::LinearRotation & linearAnim = transform.mRotation.mAnimation.emplace<xobj::LinearRotation>();

        AffineParts offsetRelativeParent;
        decomp_affine(node.GetNodeTM(params.mCurrTime), &offsetRelativeParent);
        const Quat offset = (offsetRelativeParent.q * offsetRelativeParent.q).Inverse();
        linearAnim.mOffset = xobj::Quat(offset.w, offset.x, offset.y, offset.z);
        linearAnim.mLoop = animRotate.mLoopEnable ? std::optional(animRotate.mLoopValue) : std::nullopt;
        linearAnim.mDataRef = xobj::String::from(animRotate.mDataref);

        linearAnim.mKeys.reserve(std::size_t(numKeys));
        for (int i = 0; i < numKeys; ++i) {
            Quat quat;
            control.GetValue(control.GetKeyTime(i), quat, interval);
            linearAnim.mKeys.emplace_back(xobj::LinearRotation::Key{
                xobj::Quat(quat.w, quat.x, quat.y, quat.z),
                animRotate.mKeyList.at(i)
            });
        }

        const auto check = linearAnim.checkDatarefValuesOrder();
        if (check) {
            CLError << LogNodeRef(node) << "has unexpected dataref value for the rotation key:"
                    << check.value() + 1 << " value:" << linearAnim.mKeys.at(check.value()).value;
        }
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

inline float axisValueAt(Control * inAxis, const TimeValue t) {
    Quat q;
    Interval interval(FOREVER);
    inAxis->GetValue(t, q, interval);
    return q.x;
}

void ConverterAnimRotate::processEulerAxis(INode * node, Control * control, const char axis,
                                           xobj::RotationAxis & outAnim, const ExportParams & params) {
    Control * ctrl = nullptr;
    std::unique_ptr<MdAnimRot> mdAnimRot = nullptr;
    switch (axis) {
        case 'x': {
            ctrl = control->GetXController();
            mdAnimRot = std::make_unique<MdAnimRot>(MdAnimRot::X);
            break;
        }
        case 'y': {
            ctrl = control->GetYController();
            mdAnimRot = std::make_unique<MdAnimRot>(MdAnimRot::Y);
            break;
        }
        case 'z': {
            ctrl = control->GetZController();
            mdAnimRot = std::make_unique<MdAnimRot>(MdAnimRot::Z);
            break;
        }
        default: return;
    }
    //------------------------------------------------------------
    if (!validateCtrl(node, ctrl, "rotate", axis)) {
        return;
    }
    //------------------------------------------------------------
    if (!mdAnimRot->linkNode(node, true)) {
        return;
    }
    if (!ctrl->IsAnimated()) {
        if (mdAnimRot->mEnable) {
            CLError << LogNode(node) << "has enabled rotate animation export but the object does not have any animation keys.";
        }
        return;
    }
    if (!mdAnimRot->mEnable) {
        CLError << LogNode(node) << "has animation rotate but the animation export isn't enabled, this may lead to wrong animation result.";
        return;
    }
    if (mdAnimRot->mReverse) {
        CLError << LogNode(node) << "uses removed 'reverse' checkbox in rotation, you have to fix your animation.";
    }
    //------------------------------------------------------------
    const int keysNum = ctrl->NumKeys();
    if (keysNum < 2) {
        CLError << LogNode(node) << "has rotation keys less than 2.";
        return;
    }
    if (keysNum != static_cast<int>(mdAnimRot->mKeyList.size())) {
        CLError << LogNode(node) << "mismatch animation keys and dataref values number on rotation.";
        return;
    }
    //------------------------------------------------------------
    outAnim.mKeys.reserve(static_cast<size_t>(keysNum));
    const float offset = axisValueAt(ctrl, params.mCurrTime);

    for (int currentKey = 0; currentKey < keysNum; ++currentKey) {
        const float value = axisValueAt(ctrl, ctrl->GetKeyTime(currentKey));
        xobj::RotationAxis::Key & key = outAnim.mKeys.emplace_back();
        key.angleDeg.setRad(value - offset);
        key.value = mdAnimRot->mKeyList.at(currentKey);
    }
    //------------------------------------------------------------
    outAnim.mDataRef = xobj::String::from(mdAnimRot->mDataref);
    outAnim.mLoop = mdAnimRot->mLoopEnable ? std::optional(mdAnimRot->mLoopValue) : std::nullopt;
    checkKeys(node, outAnim.mKeys, "rotate", axis);
}

//-------------------------------------------------------------------------

void ConverterAnimRotate::processEuler(INode * node, xobj::Transform & transform, Control & control, const ExportParams & params) {
    xobj::AxisSetRotation & euler = transform.mRotation.mAnimation.emplace<xobj::AxisSetRotation>();

    euler.mAxes.reserve(3);
    auto & x = euler.mAxes.emplace_back(xobj::RotationAxis::makeX());
    auto & y = euler.mAxes.emplace_back(xobj::RotationAxis::makeY());
    auto & z = euler.mAxes.emplace_back(xobj::RotationAxis::makeZ());

    processEulerAxis(node, &control, 'x', x, params);
    processEulerAxis(node, &control, 'y', y, params);
    processEulerAxis(node, &control, 'z', z, params);

    std::size_t animCount = 0;
    if (x.isAnimated()) {
        ++animCount;
    }
    if (y.isAnimated()) {
        ++animCount;
    }
    if (z.isAnimated()) {
        ++animCount;
    }
    if (animCount > 1) {
        CLError << LogNode(node) << "the plugin doesn't support more than one axis of rotate animation at the same time";
        return;
    }

    if (x.isAnimated()) {
        xobj::TMatrix mtx;
        mtx.rotateRadiansY(axisValueAt(control.GetYController(), params.mCurrTime));
        mtx.rotateRadiansZ(axisValueAt(control.GetZController(), params.mCurrTime));
        mtx.transformVector(x.mVector);
    }

    if (y.isAnimated()) {
        xobj::TMatrix mtx;
        mtx.rotateRadiansZ(axisValueAt(control.GetZController(), params.mCurrTime));
        mtx.transformVector(y.mVector);
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool ConverterAnimRotate::validateCtrl(INode * node, Control * control, const char * inCtrlName, const char axis) {
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

// todo this can be moved into the library
bool ConverterAnimRotate::checkKeys(INode * node, const xobj::RotationAxis::KeyList & keys,
                                    const char * ctrlName, const char axis) {

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
        if (stsff::math::isEqual(keys[0].angleDeg.value(), keys[1].angleDeg.value(), threshold)) {
            CLWarning << LogNode(node) << "has the same key value [0:1] on \"" << axis << "-" << ctrlName << "\" controller.";
            return false;
        }

        if (stsff::math::isEqual(keys[0].value, keys[1].value, threshold)) {
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

        if (stsff::math::isEqual(keys[k1].angleDeg.value(), keys[k2].angleDeg.value(), threshold) &&
            stsff::math::isEqual(keys[k2].angleDeg.value(), keys[k3].angleDeg.value(), threshold)) {
            CLWarning << LogNode(node) << "has the same key value [" << k1 << ":" << k2 << ":" << k3 << "] on \""
                    << axis << "-" << ctrlName << "\" controller.";
            return false;
        }

        if (stsff::math::isEqual(keys[k1].value, keys[k2].value, threshold) &&
            stsff::math::isEqual(keys[k2].value, keys[k3].value, threshold)) {
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
