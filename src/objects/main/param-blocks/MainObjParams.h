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

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

/*!
 * \details Parameter blocks of the X-Obj object
 */
enum class eMainObjParamsBlocks : BlockID {
    //! Attribute params
    MainObjAttrParams,
    //! Export params
    MainObjGeometryParams,
    //! Display params
    MainObjDisplay,
};

/*!
 * \warning 3ds max 9 does not support the value more than 512
 */
enum eMainObjAttrParams : ParamID {
    //-------------------------------------
    // string

    MainObjAttr_Texture = 0,
    MainObjAttr_TextureLit,
    MainObjAttr_TextureNormal,
    MainObjAttr_PathPrefix,

    //-------------------------------------
    // bool

    MainObjAttr_NoShadow = 10,
    MainObjAttr_Tilted,
    MainObjAttr_CockpitLit,
    MainObjAttr_NormalMetalness,
    MainObjAttr_BlendGlass,

    //-------------------------------------
    // multi-states

    //! int 
    MainObjAttr_DryWet = 20,

    //-------------------------------------
    // multi-values

    //! int 
    MainObjAttr_BlendingType = 25,
    //! float 
    MainObjAttr_BlendingRatio,

    //! string 
    MainObjAttr_LayerGroupLayer = 30,
    //! int 
    MainObjAttr_LayerGroupOffset,

    //! string 
    MainObjAttr_LayerGroupDrapedLayer = 35,
    //! int 
    MainObjAttr_LayerGroupDrapedOffset,

    //! bool 
    MainObjAttr_LodDrapEnable = 40,
    //! float 
    MainObjAttr_LodDrapDistance,

    //! bool 
    MainObjAttr_SlungLoadWeightEnable = 45,
    //! float 
    MainObjAttr_SlungLoadWeight,

    //! bool 
    MainObjAttr_SpecularEnable = 50,
    //! float 
    MainObjAttr_SpecularRatio,

    //! bool 
    MainObjAttr_TintEnable = 55,
    //! float 
    MainObjAttr_TintAlbedo,
    //! float 
    MainObjAttr_TintEmissive,

    //! bool 
    MainObjAttr_SlopeLimitEnable = 60,
    //! float 
    MainObjAttr_SlopeLimitMinPitch,
    //! float 
    MainObjAttr_SlopeLimitMaxPitch,
    //! float 
    MainObjAttr_SlopeLimitMinRoll,
    //! float 
    MainObjAttr_SlopeLimitMaxRoll,

    //-------------------------------------

    //! bool 
    MainObjAttr_CockpitRegion_1 = 70,
    //! int 
    MainObjAttr_CockpitRegion_1_L,
    //! int 
    MainObjAttr_CockpitRegion_1_B,
    //! int 
    MainObjAttr_CockpitRegion_1_R,
    //! int 
    MainObjAttr_CockpitRegion_1_T,

    //! bool 
    MainObjAttr_CockpitRegion_2 = 80,
    //! int 
    MainObjAttr_CockpitRegion_2_L,
    //! int 
    MainObjAttr_CockpitRegion_2_B,
    //! int 
    MainObjAttr_CockpitRegion_2_R,
    //! int 
    MainObjAttr_CockpitRegion_2_T,

    //! bool 
    MainObjAttr_CockpitRegion_3 = 90,
    //! int 
    MainObjAttr_CockpitRegion_3_L,
    //! int 
    MainObjAttr_CockpitRegion_3_B,
    //! int 
    MainObjAttr_CockpitRegion_3_R,
    //! int 
    MainObjAttr_CockpitRegion_3_T,

    //! bool 
    MainObjAttr_CockpitRegion_4 = 100,
    //! int 
    MainObjAttr_CockpitRegion_4_L,
    //! int 
    MainObjAttr_CockpitRegion_4_B,
    //! int 
    MainObjAttr_CockpitRegion_4_R,
    //! int 
    MainObjAttr_CockpitRegion_4_T,

    //-------------------------------------
};

enum eMainObjGeometryParams : ParamID {
    //-------------------------------------
    // bool

    MainObjGeom_Meshes = 0,
    MainObjGeom_Lines,
    MainObjGeom_Lights,
    MainObjGeom_Animation,
    MainObjGeom_Instancing,
    MainObjGeom_Debug,
    MainObjGeom_Optimization,

    //-------------------------------------

    //! bool
    MainObjGeom_ScaleEnabled = 15,
    MainObjGeom_ScaleValue,

    //-------------------------------------
    // bool
    MainObjGeom_NameMeshes = 30,
    MainObjGeom_NameLines,
    MainObjGeom_NameLights,
    MainObjGeom_NameDummies,
    MainObjGeom_TreeHierarchy,

    MainObjAttr_ExportEnable = 50,

    //-------------------------------------

    MainObjAttr_LinkingType = 60,

    //-------------------------------------
};

enum eMainObjDisplayParams : ParamID {
    MainObjDisp_IconScale,
};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

enum class eMainObjPbOrder : int {
    PbOrderAttr = eMainObjPbOrder(eMainObjParamsBlocks::MainObjAttrParams),
    PbOrderGeometry = eMainObjPbOrder(eMainObjParamsBlocks::MainObjGeometryParams),
    PbOrderDisplay = eMainObjPbOrder(eMainObjParamsBlocks::MainObjDisplay),
};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
