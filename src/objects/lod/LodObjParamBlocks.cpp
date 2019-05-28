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

#include "LodObjParamBlocks.h"

#include "resource/resource.h"
#include "objects/ScaleDim.h"
#include "classes-desc/ClassesDescriptions.h"
#include "LodObj.h"
#include "LodObjParamsWrapper.h"

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

#if MAX_VERSION_MAJOR < 15
#	define p_end end
#endif

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

class LodObjIconSizeCallback : public PBAccessor {
public:

    virtual ~LodObjIconSizeCallback() = default;

    void Set(PB2Value & v, ReferenceMaker * owner, const ParamID id, const int tabIndex, const TimeValue t) override {
        auto u = dynamic_cast<LodObject*>(owner);
        DbgAssert(u);
        switch (id) {
            case PLodObjIconScale:
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

static LodObjIconSizeCallback gLodIconSizeCallback;

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

ParamBlockDesc2 LodObjParamBlocks::mParams(LodObjParams, _T("X-Lod"), 0, ClassesDescriptions::lodObj(),
                                           P_AUTO_CONSTRUCT + P_AUTO_UI + P_VERSION,
                                           PbVersionParams, PbOrderParams,
                                           //-------------------------------------------------------------------------
                                           // Rollouts
                                           ROLL_LODOBJ, IDS_ROLL_LOD, 0, 0, NULL,
                                           //-------------------------------------------------------------------------
                                           // Params									
                                           PLodObjNear, _T("Near"), TYPE_FLOAT, 0, IDS_NEAR,
                                           p_default, 0.0f,
                                           p_range, 0.0f, 999999999.9f,
                                           p_ui, TYPE_SPINNER, EDITTYPE_POS_FLOAT, SPN_NEAR_EDIT, SPN_NEAR, 1.0f,
                                           p_end,
                                           //-------------------------------------------------------------------------
                                           PLodObjFar, _T("Far"), TYPE_FLOAT, 0, IDS_FAR,
                                           p_default, 0.0f,
                                           p_range, 0.0f, 999999999.9f,
                                           p_ui, TYPE_SPINNER, EDITTYPE_POS_FLOAT, SPN_FAR_EDIT, SPN_FAR, 1.0f,
                                           p_end,
                                           //-------------------------------------------------------------------------
                                           p_end);

/**************************************************************************************************/
//////////////////////////////////////////* Static area *///////////////////////////////////////////
/**************************************************************************************************/

ParamBlockDesc2 LodObjParamBlocks::mDisplay(LodObjDisplay, _T("X-Lod-Display"), 0, ClassesDescriptions::lodObj(),
                                            P_AUTO_CONSTRUCT + P_AUTO_UI + P_VERSION,
                                            PbVersionDisplay, PbOrderDisplay,
                                            //-------------------------------------------------------------------------
                                            // Rollouts
                                            ROLL_LODOBJ_DISPLAY, IDS_ROLL_LOD_DISPLAY, 0, APPENDROLL_CLOSED, NULL,
                                            //-------------------------------------------------------------------------
                                            // Display									
                                            PLodObjIconScale, _T("IconScale"), TYPE_FLOAT, 0, IDS_SCALE,
                                            p_default, 1.0f,
                                            p_range, 0.01f, 1000.0f,
                                            p_accessor, &gLodIconSizeCallback,
                                            p_dim, &gScaleDim,
                                            p_ui, TYPE_SPINNER, EDITTYPE_POS_FLOAT, IDC_SCALE_EDIT, IDC_SCALE_SPIN, SPIN_AUTOSCALE,
                                            p_end,
                                            //-------------------------------------------------------------------------
                                            p_end);

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void LodObjParamBlocks::postLoadParams(IParamBlock2 * paramBlock) {
    DbgAssert(paramBlock);
    DbgAssert(paramBlock->ID() == LodObjParams);

    const auto baseInterface = paramBlock->GetInterface(IPARAMBLOCK2POSTLOADINFO_ID);
    if (!baseInterface) {
        // block doesn't need updating.
        return;
    }

    auto postLoadInfo = dynamic_cast<IParamBlock2PostLoadInfo*>(baseInterface);
    if (!postLoadInfo) {
        XLError << "Invalid cast";
        return;
    }

    if (postLoadInfo->GetVersion() == 1) {
        XLInfo << "Updating LOD's parameters param block 1" << "->" << PbVersionParams;
        //------------------------------------------------------
        // updating from system units to abstract float units.

        LodObjParamsWrapper wrapper(paramBlock, GetCOREInterface()->GetTime(), FOREVER);
        const auto masterScale = GetMasterScale(UNITS_METERS);
        wrapper.setNearValue(float(wrapper.nearValue() * masterScale));
        wrapper.setFarValue(float(wrapper.farValue() * masterScale));
        //------------------------------------------------------
    }
}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void LodObjParamBlocks::postLoadDisplay(IParamBlock2 * /*paramBlock*/) {}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
