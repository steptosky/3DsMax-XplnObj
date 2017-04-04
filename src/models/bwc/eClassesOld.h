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
namespace bwc { // backward compatibility

	/********************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/********************************************************************************************************/

	enum eClassesOld {
		BASE_ATTR_DATA_ID = 50,
		GLOBAL_ATTR_DATA_ID,
		LOD_OBJ_DATA_ID,
		MAIN_OBJ_DATA_ID,
		EXPORT_DATA_ID,

		MANIP_DATA_ID = 100,
		MANIP_DATA_DRAG_XY_ID,
		MANIP_DATA_DRAG_XYZ_ID,
		MANIP_DATA_DRAG_XYZ_PIX_ID,
		MANIP_DATA_CMD_ID,
		MANIP_DATA_CMD_AXIS_ID,
		MANIP_DATA_PUSH_ID,
		MANIP_DATA_RADIO_ID,
		MANIP_DATA_TOGGLE_ID,
		MANIP_DATA_DELTA_ID,
		MANIP_DATA_WRAP_ID,

		LIGHT_DATA_ID = 150,
		LIGHT_DATA_NAMED_ID,
		LIGHT_DATA_PARAM_ID,
		LIGHT_DATA_CUSTOM_ID,
		LIGHT_DATA_SPILL_ID,

		ANIM_DATA_ID = 200,
		ANIM_DATA_TRANS_ID,
		ANIM_DATA_ROTATE_ID,
		ANIM_DATA_VIS_ID,

	};

	/********************************************************************************************************/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/********************************************************************************************************/
}
