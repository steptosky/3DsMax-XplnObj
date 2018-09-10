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

#include <cassert>
#include <memory>

#include "ConverterAnim.h"
#include "ConverterUtils.h"
#include "common/String.h"
#include "common/Logger.h"
#include "models/MdAnimTrans.h"
#include "models/MdAnimVis.h"
#include "models/io/AnimIO.h"
#include "additional/math/Compare.h"
#include "additional/math/Rad.h"

/********************************************************************************************************/
//////////////////////////////////////////////* Functions *///////////////////////////////////////////////
/********************************************************************************************************/

bool ConverterAnim::toXPLN(INode * node, xobj::Transform * transform, const ExportParams & params) {
    assert(node && transform);

    if (!AnimIO::canApply(node))
        return false;

    objAnimTrans(node, *transform, params);
    objAnimRotate(node, *transform, params);
    visibilityToXPLN(node, *transform, params);
    return true;
}

/********************************************************************************************************/
//////////////////////////////////////////////* Functions *///////////////////////////////////////////////
/********************************************************************************************************/

void ConverterAnim::visibilityToXPLN(INode * node, xobj::Transform & transform, const ExportParams &) {
    MdAnimVis mdAnimVis;
    if (!mdAnimVis.linkNode(node, true)) {
        return;
    }

    if (!mdAnimVis.mEnable) {
        return;
    }
    transform.pAnimVis.pKeys.swap(mdAnimVis.mKeyList);
}

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

float ConverterAnim::rotateValue(Control * inAxis, TimeValue t) {
    Quat q;
    Interval interval(FOREVER);
    inAxis->GetValue(t, q, interval);
    return q.x;
}

xobj::AnimRotate::KeyList * ConverterAnim::getRotateAxisAnimation(Control * inAxis, const MdAnimRot::KeyValueList & inVals,
                                                                  int inIsReversed, const ExportParams & params) {
    int rotateKeyCount = inAxis->NumKeys();
    xobj::AnimRotate::KeyList * xAnim = new xobj::AnimRotate::KeyList(static_cast<size_t>(rotateKeyCount));
    //------------------------------------------------------------

    float shift = rotateValue(inAxis, params.mCurrTime);
    for (int currentKey = 0; currentKey < rotateKeyCount; ++currentKey) {
        float value = rotateValue(inAxis, inAxis->GetKeyTime(currentKey));
        value -= shift;
        if (inIsReversed)
            value = -value;
        xobj::AnimRotate::Key & key = (*xAnim)[static_cast<size_t>(currentKey)];
        key.pAngleDegrees = stsff::math::radToDeg(value);
        key.pDrfValue = inVals[currentKey];
    }
    //------------------------------------------------------------

    return xAnim;
}

//-------------------------------------------------------------------------

void ConverterAnim::objAnimRotateAxis(INode * node, Control * control, char axis,
                                      xobj::AnimRotate & outXAnim, const ExportParams & params) {
    Control * rotateConrol = nullptr;
    std::unique_ptr<MdAnimRot> mdAnimRot = nullptr;
    switch (axis) {
        case 'x': {
            rotateConrol = control->GetXController();
            mdAnimRot = std::make_unique<MdAnimRot>(MdAnimRot::X);
            break;
        }
        case 'y': {
            rotateConrol = control->GetYController();
            mdAnimRot = std::make_unique<MdAnimRot>(MdAnimRot::Y);
            break;
        }
        case 'z': {
            rotateConrol = control->GetZController();
            mdAnimRot = std::make_unique<MdAnimRot>(MdAnimRot::Z);
            break;
        }
        default: return;
    }
    //------------------------------------------------------------

    if (!rotAnimValidation(node, rotateConrol, "rotate", axis)) {
        return;
    }
    //------------------------------------------------------------

    if (!mdAnimRot->linkNode(node, true)) {
        return;
    }

    if (rotateConrol->IsAnimated()) {
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
        float value = rotateValue(rotateConrol, params.mCurrTime);
        outXAnim.pKeys.emplace_back(xobj::AnimRotate::Key(stsff::math::radToDeg(value), 0.0f));
        return;
    }
    //------------------------------------------------------------

    int rotateKeyCount = rotateConrol->NumKeys();
    if (rotateKeyCount != static_cast<int>(mdAnimRot->mKeyList.size())) {
        CLError << LogNode(node) << "animation keys and animation key values in the object are not equaled!";
        return;
    }

    xobj::AnimRotate::KeyList * keyList = getRotateAxisAnimation(rotateConrol, mdAnimRot->mKeyList, mdAnimRot->mReverse, params);
    if (keyList) {
        outXAnim.pDrf = sts::toMbString(mdAnimRot->mDataref);
        outXAnim.pHasLoop = mdAnimRot->mLoopEnable;
        outXAnim.pLoopValue = mdAnimRot->mLoopValue;
        outXAnim.pKeys.swap(*keyList);
        delete keyList;
        checkRotateKeysValue(node, outXAnim.pKeys, "rotate", axis);
    }
}

//-------------------------------------------------------------------------

void ConverterAnim::objAnimRotate(INode * node, xobj::Transform & transform, const ExportParams & params) {
    assert(node);

    Control * tmConrol = node->GetTMController();
    if (!tmConrol)
        return;

    Control * rotateConrol = tmConrol->GetRotationController();
    if (!rotateConrol || !rotateConrol->IsAnimated())
        return;

    xobj::EulerXyzHelper xEuler;

    objAnimRotateAxis(node, rotateConrol, 'x', xEuler.pX, params);
    objAnimRotateAxis(node, rotateConrol, 'y', xEuler.pY, params);
    objAnimRotateAxis(node, rotateConrol, 'z', xEuler.pZ, params);

    size_t animCount = 0;
    if (xEuler.pX.pKeys.size() > 1) {
        ++animCount;
    }
    if (xEuler.pY.pKeys.size() > 1) {
        ++animCount;
    }
    if (xEuler.pZ.pKeys.size() > 1) {
        ++animCount;
    }
    if (animCount > 1) {
        CLError << LogNode(node) << "the plugin does not support more than one axis of rotate animation at the same time";
    }
    xEuler.addToTransform(transform);
}

/********************************************************************************************************/
//////////////////////////////////////////////* Functions *///////////////////////////////////////////////
/********************************************************************************************************/

Point3 ConverterAnim::translateValue(Control * xCtrl, Control * yCtrl, Control * zCtrl, TimeValue time) {
    Point3 px, py, pz;
    Interval interval(FOREVER);
    if (xCtrl) {
        xCtrl->GetValue(time, px, interval);
    }
    if (yCtrl) {
        yCtrl->GetValue(time, py, interval);
    }
    if (zCtrl) {
        zCtrl->GetValue(time, pz, interval);
    }
    return Point3(px.x, py.x, pz.x);
}

//-------------------------------------------------------------------------

void ConverterAnim::objAnimTrans(INode * node, xobj::Transform & transform, const ExportParams & params) {
    assert(node);
    //----------------------------------------------

    Control * tmConrol = node->GetTMController();
    if (!tmConrol)
        return;

    Control * posControl = tmConrol->GetPositionController();
    if (!posControl || !posControl->IsAnimated())
        return;
    //----------------------------------------------

    MdAnimTrans mdAnimTrans;
    if (!mdAnimTrans.linkNode(node, true)) {
        return;
    }

    //----------------------------------------------

    if (posControl->IsAnimated()) {
        if (!mdAnimTrans.mEnable) {
            CLError << LogNode(node) << "has the animated position controller but the animation export is not enabled,"
                    << " this may lead to wrong animation result.";
            return;
        }
    }
    else {
        if (mdAnimTrans.mEnable) {
            CLError << LogNode(node) << "has enabled the position animation export but the object does not have any animation keys.";
        }
        return;
    }

    //----------------------------------------------

    int posControlKeyNum = posControl->NumKeys();
    if (posControlKeyNum < 2) {
        CLError << LogNode(node) << "has animation translate keys count less 2!";
        return;
    }

    //----------------------------------------------

    MdAnimTrans::KeyValueList & keyValueList = mdAnimTrans.mKeyList;
    // If Values count not equaled key count
    if (posControlKeyNum != int(keyValueList.size())) {
        CLError << LogNode(node) << "animation keys and animation key values in the object are not equaled!";
        return;
    }
    //----------------------------------------------

    Control * xCtrl = posControl->GetXController();
    Control * yCtrl = posControl->GetYController();
    Control * zCtrl = posControl->GetZController();

    transAnimValidation(node, xCtrl, "position", 'x');
    transAnimValidation(node, yCtrl, "position", 'y');
    transAnimValidation(node, zCtrl, "position", 'z');
    //----------------------------------------------

    transform.pAnimTrans.emplace_back();
    xobj::AnimTrans & anim = transform.pAnimTrans.back();
    Point3 shift = translateValue(xCtrl, yCtrl, zCtrl, params.mCurrTime);

    anim.pKeys.resize(static_cast<size_t>(posControlKeyNum));
    for (int keyNum = 0; keyNum < posControlKeyNum; ++keyNum) {
        Point3 pos = translateValue(xCtrl, yCtrl, zCtrl, posControl->GetKeyTime(keyNum));
        pos = pos - shift;
        if (mdAnimTrans.mReverse) {
            pos = shift - (pos - shift);
        }
        xobj::AnimTrans::Key & key = anim.pKeys[static_cast<size_t>(keyNum)];
        key.pPosition.set(pos.x, pos.y, pos.z);
        key.pDrfValue = keyValueList[keyNum];
    }

    anim.pDrf = sts::toMbString(mdAnimTrans.mDataref);
    anim.pHasLoop = mdAnimTrans.mLoopEnable;
    anim.pLoopValue = mdAnimTrans.mLoopValue;
    checkTransKeysValue(node, anim.pKeys, "position");
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool ConverterAnim::rotAnimValidation(INode * node, Control * inControl, const char * inCtrlName, char axis) {
    if (!inControl) {
        CLError << LogNode(node) << "does not have valid \"" << axis << " " << inCtrlName << "\" controller";
        return false;
    }

    IKeyControl * ikeys = GetKeyControlInterface(inControl);
    if (!ikeys) {
        CLError << LogNode(node) << "does not have valid \""
                << axis << " " << inCtrlName << "\" controller."
                << " Please, use either Bezier float or linear float controller.";
        return false;
    }

    int numKeys = ikeys->GetNumKeys();
    if (numKeys != 0) {
        if (inControl->ClassID() == Class_ID(HYBRIDINTERP_FLOAT_CLASS_ID, 0)) {
            IBezFloatKey key;
            for (int i = 0; i < numKeys; i++) {
                ikeys->GetKey(i, &key);
                if (GetInTanType(key.flags) != BEZKEY_LINEAR || GetOutTanType(key.flags) != BEZKEY_LINEAR) {
                    CLWarning << LogNode(node)
                            << "has unsupported Bezier tangents in key number \"" << i << "\""
                            << " of " << "\"" << axis << " " << inCtrlName << "\" controller."
                            << " Please, use linear \"in\" and linear \"out\" tangents.";
                }
            }
        }
        else if (inControl->ClassID() != Class_ID(LININTERP_FLOAT_CLASS_ID, 0)) {
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

//-------------------------------------------------------------------------

bool ConverterAnim::transAnimValidation(INode * node, Control * inControl, const char * inCtrlName, char axis) {
    if (!inControl) {
        CLError << LogNode(node) << "does not have valid \"" << axis << " " << inCtrlName << "\" controller";
        return false;
    }

    IKeyControl * ikeys = GetKeyControlInterface(inControl);
    if (!ikeys) {
        CLError << LogNode(node) << "does not have valid \""
                << axis << " " << inCtrlName << "\" controller."
                << " Please, use either Bezier float or linear float controller.";
        return false;
    }

    int numKeys = ikeys->GetNumKeys();
    if (numKeys != 0) {
        if (inControl->ClassID() == Class_ID(HYBRIDINTERP_FLOAT_CLASS_ID, 0)) {
            IBezFloatKey key;
            for (int i = 0; i < numKeys; i++) {
                ikeys->GetKey(i, &key);
                if (GetInTanType(key.flags) != BEZKEY_LINEAR || GetOutTanType(key.flags) != BEZKEY_LINEAR) {
                    CLWarning << LogNode(node)
                            << "has unsupported Bezier tangents in key number \"" << i << "\""
                            << " of " << "\"" << axis << " " << inCtrlName << "\" controller."
                            << " Please, use linear 'in' and linear 'out' tangents.";
                }
            }
        }
        else if (inControl->ClassID() != Class_ID(LININTERP_FLOAT_CLASS_ID, 0)) {
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

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool ConverterAnim::checkTransKeysValue(INode * node, const xobj::AnimTrans::KeyList & keyList, const char * inCtrlName) {
    DbgAssert(node);
    DbgAssert(inCtrlName);
    const float threshold = 0.0001f;
    const size_t size = keyList.size();
    //-------------------------------------------------------------------------
    if (size == 0 || size == 1) {
        return false;
    }
    //-------------------------------------------------------------------------
    if (size == 2) {
        if (keyList[0].pPosition == keyList[1].pPosition) {
            CLWarning << LogNode(node) << "has the same position value [0:1] on \"" << inCtrlName << "\" controller.";
            return false;
        }

        if (keyList[0].pDrfValue == keyList[1].pDrfValue) {
            CLWarning << LogNode(node) << "has the same dataref value [0:1] on \"" << inCtrlName << "\" controller.";
            return false;
        }
        return true;
    }
    //-------------------------------------------------------------------------
    for (size_t i = 0; i < size; ++i) {
        size_t k1 = i + 0;
        size_t k2 = i + 1;
        size_t k3 = i + 2;
        if (k3 >= size) {
            return true;
        }

        if (keyList[k1].pPosition == keyList[k2].pPosition &&
            keyList[k2].pPosition == keyList[k3].pPosition) {
            CLWarning << LogNode(node) << "has the same position value [" << k1 << ":" << k2 << ":" << k3 << "] on \""
                    << inCtrlName << "\" controller.";
            return false;
        }

        if (stsff::math::isEqual(keyList[k1].pDrfValue, keyList[k2].pDrfValue, threshold) &&
            stsff::math::isEqual(keyList[k2].pDrfValue, keyList[k3].pDrfValue, threshold)) {
            CLWarning << LogNode(node) << "has the same dataref value [" << k1 << ":" << k2 << ":" << k3 << "] on \""
                    << inCtrlName << "\" controller.";
            return false;
        }
    }
    //-------------------------------------------------------------------------
    return true;
}

bool ConverterAnim::checkRotateKeysValue(INode * node, const xobj::AnimRotate::KeyList & keyList, const char * inCtrlName, char axis) {
    DbgAssert(node);
    DbgAssert(inCtrlName);
    const float threshold = 0.0001f;
    const size_t size = keyList.size();
    //-------------------------------------------------------------------------
    if (size == 0 || size == 1) {
        return false;
    }
    //-------------------------------------------------------------------------
    if (size == 2) {
        if (stsff::math::isEqual(keyList[0].pAngleDegrees, keyList[1].pAngleDegrees, threshold)) {
            CLWarning << LogNode(node) << "has the same key value [0:1] on \"" << axis << "-" << inCtrlName << "\" controller.";
            return false;
        }

        if (stsff::math::isEqual(keyList[0].pDrfValue, keyList[1].pDrfValue, threshold)) {
            CLWarning << LogNode(node) << "has the same dataref value [0:1] on \"" << axis << "-" << inCtrlName << "\" controller.";
            return false;
        }
        return true;
    }
    //-------------------------------------------------------------------------
    for (size_t i = 0; i < size; ++i) {
        size_t k1 = i + 0;
        size_t k2 = i + 1;
        size_t k3 = i + 2;
        if (k3 >= size) {
            return true;
        }

        if (stsff::math::isEqual(keyList[k1].pAngleDegrees, keyList[k2].pAngleDegrees, threshold) &&
            stsff::math::isEqual(keyList[k2].pAngleDegrees, keyList[k3].pAngleDegrees, threshold)) {
            CLWarning << LogNode(node) << "has the same key value [" << k1 << ":" << k2 << ":" << k3 << "] on \""
                    << axis << "-" << inCtrlName << "\" controller.";
            return false;
        }

        if (stsff::math::isEqual(keyList[k1].pDrfValue, keyList[k2].pDrfValue, threshold) &&
            stsff::math::isEqual(keyList[k2].pDrfValue, keyList[k3].pDrfValue, threshold)) {
            CLWarning << LogNode(node) << "has the same dataref value [" << k1 << ":" << k2 << ":" << k3 << "] on \""
                    << axis << "-" << inCtrlName << "\" controller.";
            return false;
        }
    }
    //-------------------------------------------------------------------------
    return true;
}

/*********************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*********************************************************************************************************/
