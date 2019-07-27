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

#include "ConverterAnimTranslate.h"
#include "common/Logger.h"
#include "models/MdAnimTrans.h"
#include "models/io/AnimIO.h"
#include "additional/math/Compare.h"
#include "ExportParams.h"
#include "ImportParams.h"

/********************************************************************************************************/
//////////////////////////////////////////////* Functions *///////////////////////////////////////////////
/********************************************************************************************************/

bool ConverterAnimTranslate::toXpln(INode & node, xobj::Transform & transform, const ExportParams & params) {
    if (!AnimIO::canApply(&node)) {
        return false;
    }
    objAnimTrans(&node, transform, params);
    return true;
}

bool ConverterAnimTranslate::toMax(INode &, xobj::Transform &, const ImportParams &) {
    // TODO Implementation
    return true;
}

/********************************************************************************************************/
//////////////////////////////////////////////* Functions *///////////////////////////////////////////////
/********************************************************************************************************/

Point3 ConverterAnimTranslate::translateValue(Control * xCtrl, Control * yCtrl, Control * zCtrl, const TimeValue time) {
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

void ConverterAnimTranslate::objAnimTrans(INode * node, xobj::Transform & transform, const ExportParams & params) {
    DbgAssert(node);
    //----------------------------------------------
    Control * tmControl = node->GetTMController();
    if (!tmControl) {
        return;
    }
    Control * posControl = tmControl->GetPositionController();
    if (!posControl || !posControl->IsAnimated()) {
        return;
    }
    //----------------------------------------------
    MdAnimTrans mdAnimTrans;
    if (!mdAnimTrans.linkNode(node, true)) {
        return;
    }
    //----------------------------------------------
    if (posControl->IsAnimated()) {
        if (!mdAnimTrans.mEnable) {
            CLError << LogNode(node) << "has enabled translate animation export but the object does not have any animation keys.";
            return;
        }
    }
    if (!mdAnimTrans.mEnable) {
        CLError << LogNode(node) << "has animation translate but the animation export isn't enabled, this may lead to wrong animation result.";
        return;
    }
    if (mdAnimTrans.mReverse) {
        CLError << LogNode(node) << "uses removed 'reverse' checkbox in translation, you have to fix your animation.";
    }
    //----------------------------------------------
    const int posControlKeyNum = posControl->NumKeys();
    if (posControlKeyNum < 2) {
        CLError << LogNode(node) << "has translation keys less than 2.";
        return;
    }
    MdAnimTrans::KeyValueList & keyValueList = mdAnimTrans.mKeyList;
    if (posControlKeyNum != int(keyValueList.size())) {
        CLError << LogNode(node) << "mismatch animation keys and dataref values number on translation: "
                << posControlKeyNum << "/" << keyValueList.size() << ".";
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
    xobj::Translate & anim = transform.mPosition.mAnimation.emplace_back();

    /*
     *    p - parent position
     *    k - anim key
     *    o - object position that is set by time
     *    
     *              [offsetRelativeParent]
     *    |---------------------------------------|
     *    P-------------------K-------------------O-------------------K
     *    0                   1                   2                   3
     *    |-------------------|-------------------|
     *   [firstRelativePosition]      [diff]
     */

    const Point3 offsetRelativeParent = translateValue(xCtrl, yCtrl, zCtrl, params.mCurrTime);

    anim.mKeys.reserve(static_cast<std::size_t>(posControlKeyNum));
    for (int keyNum = 0; keyNum < posControlKeyNum; ++keyNum) {
        const Point3 pos = translateValue(xCtrl, yCtrl, zCtrl, posControl->GetKeyTime(keyNum)) - offsetRelativeParent;
        xobj::Translate::Key & key = anim.mKeys.emplace_back();
        key.mPos.set(pos.x, pos.y, pos.z);
        key.mDrfValue = keyValueList.at(keyNum);
    }

    anim.mDataRef = xobj::String::from(mdAnimTrans.mDataref);
    anim.mLoop = mdAnimTrans.mLoopEnable ? std::optional(mdAnimTrans.mLoopValue) : std::nullopt;
    checkTransKeysValue(node, anim.mKeys, "position");
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

bool ConverterAnimTranslate::transAnimValidation(INode * node, Control * control, const char * ctrlName, const char axis) {
    if (!control) {
        CLError << LogNode(node) << "does not have valid \"" << axis << " " << ctrlName << "\" controller";
        return false;
    }

    IKeyControl * iKeys = GetKeyControlInterface(control);
    if (!iKeys) {
        CLError << LogNode(node) << "does not have valid \""
                << axis << " " << ctrlName << "\" controller."
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
                            << " of " << "\"" << axis << " " << ctrlName << "\" controller."
                            << " Please, use linear 'in' and linear 'out' tangents.";
                }
            }
        }
        else if (control->ClassID() != Class_ID(LININTERP_FLOAT_CLASS_ID, 0)) {
            CLError << LogNode(node)
                    << "has unsupported \"" << ctrlName << "\" controller on \"" << axis << "\" axis."
                    << " Please, use either Bezier float or linear float controller.";
            return false;
        }
    }
    else if (numKeys == 1) {
        CLError << LogNode(node) << "\"" << ctrlName << "\" controller on \"" << axis << "\" axis has only one key.";
        return false;
    }
    return true;
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

// todo can be moved into the library
bool ConverterAnimTranslate::checkTransKeysValue(INode * node, const xobj::Translate::KeyList & keyList, const char * ctrlName) {
    DbgAssert(node);
    DbgAssert(ctrlName);
    const float threshold = 0.0001f;
    const size_t size = keyList.size();
    //-------------------------------------------------------------------------
    if (size == 0 || size == 1) {
        return false;
    }
    //-------------------------------------------------------------------------
    if (size == 2) {
        if (keyList[0].mPos == keyList[1].mPos) {
            CLWarning << LogNode(node) << "has the same position value [0:1] on \"" << ctrlName << "\" controller.";
            return false;
        }

        if (keyList[0].mDrfValue == keyList[1].mDrfValue) {
            CLWarning << LogNode(node) << "has the same dataref value [0:1] on \"" << ctrlName << "\" controller.";
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

        if (keyList[k1].mPos == keyList[k2].mPos &&
            keyList[k2].mPos == keyList[k3].mPos) {
            CLWarning << LogNode(node) << "has the same position value [" << k1 << ":" << k2 << ":" << k3 << "] on \""
                    << ctrlName << "\" controller.";
            return false;
        }

        if (stsff::math::isEqual(keyList[k1].mDrfValue, keyList[k2].mDrfValue, threshold) &&
            stsff::math::isEqual(keyList[k2].mDrfValue, keyList[k3].mDrfValue, threshold)) {
            CLWarning << LogNode(node) << "has the same dataref value [" << k1 << ":" << k2 << ":" << k3 << "] on \""
                    << ctrlName << "\" controller.";
            return false;
        }
    }
    //-------------------------------------------------------------------------
    return true;
}

/*********************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*********************************************************************************************************/
