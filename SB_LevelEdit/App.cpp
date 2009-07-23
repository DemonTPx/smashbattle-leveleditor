#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "App.h"
#include "MainFrame.h"

IMPLEMENT_APP(App)

bool App::OnInit() {
	MainFrame * mf;
	wxSize size;

	mf = new MainFrame(_("Smash Battle - level editor"), wxDefaultPosition, wxDefaultSize);
	mf->SetClientSize(840, 510);
	mf->CenterOnScreen();
	mf->Show();
	SetTopWindow(mf);
	return true;
}
