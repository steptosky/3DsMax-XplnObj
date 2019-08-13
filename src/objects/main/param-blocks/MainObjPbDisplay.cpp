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

#include "MainObjPbDisplay.h"

#include "resource/resource.h"
#include "objects/ScaleDim.h"
#include "classes-desc/ClassesDescriptions.h"
#include "objects/main/param-blocks/MainObjParams.h"
#include "objects/main/MainObj.h"

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

#if MAX_VERSION_MAJOR < 15
#	define p_end end
#endif

#if MAX_VERSION_MAJOR < 19
#	define TYPE_SINGLECHECKBOX TYPE_SINGLECHEKBOX
#endif

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

class MainObjIconSizeCallback : public PBAccessor {
public:

    virtual ~MainObjIconSizeCallback() = default;

    void Set(PB2Value & v, ReferenceMaker * owner, const ParamID id, const int tabIndex, const TimeValue t) override {
        auto u = dynamic_cast<MainObject*>(owner);
        DbgAssert(u);
        switch (id) {
            case MainObjDisp_IconScale:
                u->makeIcon();
                break;
            default: break;
        }
        PBAccessor::Set(v, owner, id, tabIndex, t);
    }
};

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

static MainObjIconSizeCallback gMainObjIconSizeCallback;

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

ParamBlockDesc2 MainObjPbDisplay::mPb(static_cast<BlockID>(eMainObjParamsBlocks::MainObjDisplay), _T("X-Obj-Display"), 0, ClassesDescriptions::mainObj(),
                                      P_AUTO_CONSTRUCT + P_AUTO_UI + P_VERSION,
                                      PbVersionDisplay, eMainObjPbOrder::PbOrderDisplay,
                                      //-------------------------------------------------------------------------
                                      // Rollouts
                                      ROLL_LODOBJ_DISPLAY, IDS_ROLL_LOD_DISPLAY, 0, APPENDROLL_CLOSED, NULL,
                                      //-------------------------------------------------------------------------
                                      // Display								
                                      MainObjDisp_IconScale, _T("IconScale"), TYPE_FLOAT, 0, NO_IDS,
                                      p_default, 1.0f,
                                      p_range, 0.01f, 1000.0f,
                                      p_accessor, &gMainObjIconSizeCallback,
                                      p_dim, &gScaleDim,
                                      p_ui, TYPE_SPINNER, EDITTYPE_POS_FLOAT, IDC_SCALE_EDIT, IDC_SCALE_SPIN, SPIN_AUTOSCALE,
                                      p_end,
                                      //-------------------------------------------------------------------------
                                      MainObjDisp_DrawName, _T("DrawName"), TYPE_BOOL, 0, NO_IDS,
                                      p_default, TRUE,
                                      p_ui, TYPE_SINGLECHECKBOX, IDC_CHECK_DRAW_NAME,
                                      p_end,
                                      //-------------------------------------------------------------------------
                                      p_end);

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void MainObjPbDisplay::postLoad(IParamBlock2 * /*paramBlock*/) {}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
