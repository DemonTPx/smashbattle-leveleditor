#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "App.h"
#include "MainFrame.h"

IMPLEMENT_APP(App)

bool App::OnInit() {
	MainFrame * mf;
	wxSize size(1140, 520);

	mf = new MainFrame(_("Smash Battle - level editor"), wxDefaultPosition, wxDefaultSize);
	if(!mf->LoadFiles())
		return false;
	mf->SetClientSize(size);
	mf->SetMinClientSize(size);
	mf->SetMaxClientSize(size);
	mf->Show();
	SetTopWindow(mf);
	return true;
}
