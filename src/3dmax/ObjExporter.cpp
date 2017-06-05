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

#include "ObjExporter.h "
#include "Info.h"
#include "common/String.h"
#include "resource/ResHelper.h"

#pragma warning(push, 0)
#include <iparamb2.h>
#pragma warning(pop)

/**************************************************************************************************/
/////////////////////////////////////////* Static area *////////////////////////////////////////////
/**************************************************************************************************/

class OBJEClassDesc : public ClassDesc2 {
public:

	int IsPublic() override { return TRUE; }
	void * Create(BOOL /*loading = FALSE*/) override { return new ObjExporter(); }
	HINSTANCE HInstance() override { return ResHelper::hInstance; }

	SClass_ID SuperClassID() override { return SCENE_EXPORT_CLASS_ID; }
	Class_ID ClassID() override { return OBJE_CLASS_ID; }

	const TCHAR * ClassName() override { return _T("X-Obj-Export"); }
	const TCHAR * Category() override { return _T("Obj export"); }
	const TCHAR * InternalName() override { return _T("ExportObj"); }

};

static OBJEClassDesc OBJEDesc;

ClassDesc2 * GetObjEDesc() {
	return &OBJEDesc;
}

/**************************************************************************************************/
////////////////////////////////////* Constructors/Destructor */////////////////////////////////////
/**************************************************************************************************/

ObjExporter::ObjExporter() {}

ObjExporter::~ObjExporter() {}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

int ObjExporter::DoExport(const TCHAR * name, ExpInterface * /*ei*/, Interface * ip, BOOL suppressPrompts, DWORD options) {
	mDlgMain = new ui::DlgExport();
	//ip->SetTime(TimeValue(0), TRUE);
	mDlgMain->show(name, ip, suppressPrompts == TRUE, options == SCENE_EXPORT_SELECTED ? true : false);
	delete mDlgMain;
	return TRUE;
}

/**************************************************************************************************/
///////////////////////////////////////////* Functions *////////////////////////////////////////////
/**************************************************************************************************/

int ObjExporter::ExtCount() {
	// Returns the number of file name extensions supported by the plug-in.
	return 1;
}

const TCHAR * ObjExporter::Ext(int /*n*/) {
	// Return the 'i-th' file name extension (i.e. \"3DS\").
	return _T("obj");
}

const TCHAR * ObjExporter::LongDesc() {
	// Return long ASCII description (i.e. \"Targa 2.0 Image File\")
	return _T("X-Obj-Exporter");
}

const TCHAR * ObjExporter::ShortDesc() {
	// Return short ASCII description (i.e. \"Targa\")
	return _T("X-Plane obj");
}

const TCHAR * ObjExporter::AuthorName() {
	return _T(XIO_ORGANIZATION_WEBLINK);
}

const TCHAR * ObjExporter::CopyrightMessage() {
	return _T(XIO_COPYRIGHT);
}

const TCHAR * ObjExporter::OtherMessage1() {
	// Return Other message #1 if any
	return _T(XIO_PROJECT_DESCRIPTION);
}

const TCHAR * ObjExporter::OtherMessage2() {
	// Return other message #2 in any
	return _T("");
}

unsigned int ObjExporter::Version() {
	// Return Version number * 100 (i.e. v3.01 = 301)
	return XIO_VERSION;
}

void ObjExporter::ShowAbout(HWND /*hWnd*/) {
	// Optional
}

BOOL ObjExporter::SupportsOptions(int /*ext*/, DWORD /*options*/) {
	// Decide which options to support.  Simply return true for each option supported by each Extension the exporter supports.
	return TRUE;
}

/**************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************/
