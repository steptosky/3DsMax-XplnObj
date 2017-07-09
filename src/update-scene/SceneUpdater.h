#pragma once

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

#include "additional/utils/SemVersion.h"

/********************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************************/

class SceneUpdater {
public:

	static void update(const sts::SemVersion & versionFrom, const sts::SemVersion & versionTo);

private:

	/*!
	 * \details In the 0.4.0 XplnObj library was fixed manipulator logic 
	 *          and added new manipulator 'panel'. This method checks 
	 *          each INode in the scene and if it has cockpit attribute 
	 *          the method applies the panel manipulator to the INode.
	 *          This behavior is needed for the backward compatibility.
	 *          
	 *          It should not break the scene but it is good if the user
	 *          manually checks each cockpit object to make sure all 
	 *          things are correct with the manipulators after this method
	 *          and disables manipulators if they are not needed.
	 *          
	 *          Although it should not break the scene the behavior
	 *          can be changed it is not possible to avoid this situation
	 *          because the previous behavior was based on the bug 
	 *          and had undefined logic behavior in some cases, 
	 *          it is too difficult to recognize and fix it programmatically.
	 */
	static void panelManipulators(const sts::SemVersion & versionFrom);

};

/********************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************************/
