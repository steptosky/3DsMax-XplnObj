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

#include "ObjImporter.h"
#include "Info.h"
#include "resource/ResHelper.h"

#pragma warning(push, 0)
#include <iparamb2.h>
#pragma warning(pop)

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

ObjImporter::ObjImporter() {}

ObjImporter::~ObjImporter() {}

/**************************************************************************************************/
//////////////////////////////////////////* Functions */////////////////////////////////////////////
/**************************************************************************************************/

int ObjImporter::DoImport(const TCHAR * name, ImpInterface * /*ei*/, Interface * ip, BOOL suppressPrompts) {
	mDlgMain = new ui::DlgImport();
	ip->SetTime(TimeValue(0), TRUE);
	mDlgMain->show(name, ip, suppressPrompts == TRUE);
	delete mDlgMain;
	return TRUE;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

int ObjImporter::ExtCount() {
	// Returns the number of file name extensions supported by the plug-in.
	return 1;
}

const TCHAR * ObjImporter::Ext(int /*n*/) {
	// Return the 'i-th' file name extension (i.e. \"3DS\").
	return _T("obj");
}

const TCHAR * ObjImporter::LongDesc() {
	// Return long ASCII description (i.e. \"Targa 2.0 Image File\")
	return _T("X-Obj-Importer");
}

const TCHAR * ObjImporter::ShortDesc() {
	// Return short ASCII description (i.e. \"Targa\")
	return _T("X-Plane obj");
}

const TCHAR * ObjImporter::AuthorName() {
	return _T(XIO_ORGANIZATION_WEBLINK);
}

const TCHAR * ObjImporter::CopyrightMessage() {
	return _T(XIO_COPYRIGHT);
}

const TCHAR * ObjImporter::OtherMessage1() {
	// Return Other message #1 if any
	return _T(XIO_PROJECT_DESCRIPTION);
}

const TCHAR * ObjImporter::OtherMessage2() {
	// Return other message #2 in any
	return _T("");
}

unsigned int ObjImporter::Version() {
	// Return Version number * 100 (i.e. v3.01 = 301)
	return XIO_VERSION;
}

void ObjImporter::ShowAbout(HWND /*hWnd*/) {
	// Optional
}

BOOL ObjImporter::SupportsOptions(int /*ext*/, DWORD /*options*/) {
	// Decide which options to support.  Simply return true for each option supported by each Extension the exporter supports.
	return TRUE;
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
