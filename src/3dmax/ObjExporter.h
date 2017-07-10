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

#include "ui/DlgExport.h"
#include "Converters/Converterer.h"

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/

class ObjExporter : public SceneExport {
public:

	ObjExporter();
	~ObjExporter();

	int ExtCount() override; // Number of extensions supported
	const TCHAR * Ext(int n) override; // Extension #n (i.e. "3DS")
	const TCHAR * LongDesc() override; // Long ASCII description (i.e. "Autodesk 3D Studio File")
	const TCHAR * ShortDesc() override; // Short ASCII description (i.e. "3D Studio")
	const TCHAR * AuthorName() override; // ASCII Author name
	const TCHAR * CopyrightMessage() override; // ASCII Copyright message
	const TCHAR * OtherMessage1() override; // Other message #1
	const TCHAR * OtherMessage2() override; // Other message #2
	unsigned int Version() override; // Version number * 100 (i.e. v3.01 = 301)
	void ShowAbout(HWND hWnd) override; // Show DLL's "About..." box

	BOOL SupportsOptions(int ext, DWORD options) override;
	int DoExport(const TCHAR * name, ExpInterface * ei, Interface * i, BOOL suppressPrompts = FALSE, DWORD options = 0) override;

private:

	ui::DlgExport * mDlgMain = nullptr;

};

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
