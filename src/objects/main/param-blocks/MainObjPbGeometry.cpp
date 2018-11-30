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

#include "MainObjPbGeometry.h"

#include "resource/resource.h"
#include "objects/ScaleDim.h"
#include "classes-desc/ClassesDescriptions.h"
#include "objects/main/param-blocks/MainObjParams.h"

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

ParamBlockDesc2 MainObjPbGeometry::mPb(static_cast<BlockID>(eMainObjParamsBlocks::MainObjGeometryParams), _T("X-Obj-Geometry"), 0, ClassesDescriptions::mainObj(),
                                       P_AUTO_CONSTRUCT + P_AUTO_UI + P_VERSION,
                                       PbVersionGeometry, eMainObjPbOrder::PbOrderGeometry,
                                       //-------------------------------------------------------------------------
                                       // Rollouts
                                       ROLL_MAINOBJ_GEOMETRY, IDS_ROLL_MAINOBJ_GEOMETRY, 0, APPENDROLL_CLOSED, NULL,
                                       //-------------------------------------------------------------------------
                                       // Export Params
                                       MainObjAttr_ExportEnable, _T("Enable-export"), TYPE_BOOL, 0, NO_IDS,
                                       p_default, TRUE,
                                       p_end,

                                       MainObjGeom_Meshes, _T("Enable-mesh"), TYPE_BOOL, 0, NO_IDS,
                                       p_default, TRUE,
                                       p_ui, TYPE_SINGLECHECKBOX, CHK_ENABLE_MESH,
                                       p_end,

                                       MainObjGeom_Lines, _T("Enable-lines"), TYPE_BOOL, 0, NO_IDS,
                                       p_default, FALSE,
                                       p_ui, TYPE_SINGLECHECKBOX, CHK_ENABLE_LINE,
                                       p_end,

                                       MainObjGeom_Lights, _T("Enable-lights"), TYPE_BOOL, 0, NO_IDS,
                                       p_default, TRUE,
                                       p_ui, TYPE_SINGLECHECKBOX, CHK_ENABLE_LIGHT,
                                       p_end,

                                       MainObjGeom_Animation, _T("Enable-animation"), TYPE_BOOL, 0, NO_IDS,
                                       p_default, TRUE,
                                       p_ui, TYPE_SINGLECHECKBOX, CHK_ENABLE_ANIM,
                                       p_end,

                                       MainObjGeom_Optimization, _T("Enable-optimization"), TYPE_BOOL, 0, NO_IDS,
                                       p_default, TRUE,
                                       p_ui, TYPE_SINGLECHECKBOX, CHK_ENABLE_OPTIMISATION,
                                       p_end,

                                       MainObjGeom_Instancing, _T("Enable-instancing"), TYPE_BOOL, 0, NO_IDS,
                                       p_default, FALSE,
                                       p_ui, TYPE_SINGLECHECKBOX, CHK_ENABLE_INSTANCING,
                                       p_end,

                                       MainObjGeom_Debug, _T("Enable-debug"), TYPE_BOOL, 0, NO_IDS,
                                       p_default, FALSE,
                                       p_ui, TYPE_SINGLECHECKBOX, CHK_ENABLE_DEBUG,
                                       p_end,
                                       //-------------------------------------------------------------------------
                                       MainObjGeom_ScaleValue, _T("Scale-val"), TYPE_FLOAT, 0, NO_IDS,
                                       p_default, 1.0f,
                                       p_range, 0.0f, 999999999.9f,
                                       p_dim, &gScaleDim,
                                       p_ui, TYPE_SPINNER, EDITTYPE_POS_FLOAT, SPN_SCALE_EDIT, SPN_SCALE, SPIN_AUTOSCALE,
                                       p_end,

                                       MainObjGeom_ScaleEnabled, _T("Scale"), TYPE_BOOL, 0, NO_IDS,
                                       p_default, FALSE,
                                       p_enable_ctrls, 1, MainObjGeom_ScaleValue,
                                       p_ui, TYPE_SINGLECHECKBOX, CHK_SCALE_ENABLE,
                                       p_end,
                                       //-------------------------------------------------------------------------
                                       MainObjGeom_NameMeshes, _T("Name-mesh"), TYPE_BOOL, 0, NO_IDS,
                                       p_default, FALSE,
                                       p_ui, TYPE_SINGLECHECKBOX, CHK_NAME_MESH,
                                       p_end,

                                       MainObjGeom_NameLines, _T("Name-lines"), TYPE_BOOL, 0, NO_IDS,
                                       p_default, FALSE,
                                       p_ui, TYPE_SINGLECHECKBOX, CHK_NAME_LINE,
                                       p_end,

                                       MainObjGeom_NameLights, _T("Name-lights"), TYPE_BOOL, 0, NO_IDS,
                                       p_default, FALSE,
                                       p_ui, TYPE_SINGLECHECKBOX, CHK_NAME_LIGHT,
                                       p_end,

                                       MainObjGeom_NameDummies, _T("Name-dummies"), TYPE_BOOL, 0, NO_IDS,
                                       p_default, FALSE,
                                       p_ui, TYPE_SINGLECHECKBOX, CHK_NAME_DUMMY,
                                       p_end,

                                       MainObjGeom_TreeHierarchy, _T("Name-animation"), TYPE_BOOL, 0, NO_IDS,
                                       p_default, FALSE,
                                       p_ui, TYPE_SINGLECHECKBOX, CHK_NAME_TREE, p_end,
                                       //-------------------------------------------------------------------------
                                       p_end);

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

void MainObjPbGeometry::postLoad(IParamBlock2 * /*paramBlock*/) {}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
