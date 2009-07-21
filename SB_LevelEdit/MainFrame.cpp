#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "MainFrame.h"

wxBitmap * MainFrame::background = 0;

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame((wxFrame *) NULL, -1, title, pos, size) 
{
	InitializeComponents();

	level = 0;
}

void MainFrame::InitializeComponents()
{
	SetWindowStyle(wxCAPTION | wxCLOSE_BOX | wxSYSTEM_MENU);
	SetBackgroundColour(wxColour(0xeeeeee));
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);

	toolbar = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(640, 30));

	btnOpen = new wxButton(toolbar, ID_Open, _("&Open"), wxPoint(0, 0), wxSize(80, 30));

	display = new wxPanel(this, wxID_ANY, wxPoint(0, 30), wxSize(640, 480));
	display->Connect(wxEVT_PAINT, wxPaintEventHandler(MainFrame::OnDisplayPaint));
}

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
//	EVT_BUTTON(BUTTON_btn, MainFrame::OnExit)
//	EVT_BUTTON(BUTTON_btn, MainFrame::OnExit)
	EVT_BUTTON(ID_Open, MainFrame::OnOpen)
END_EVENT_TABLE()

void MainFrame::OnExit(wxCommandEvent &event)
{
	Close(TRUE);
}

void MainFrame::OnDisplayPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);
	
	if(background != 0) {
		dc.DrawBitmap(*background, 0, 0, TRUE);
	}
}

void MainFrame::OnOpen(wxCommandEvent &event)
{
	wxFileDialog * dialog;
	wxString wildcard = _("Level files (*.lvl)|*.lvl|All files (*.*)|*.*");
	
	dialog = new wxFileDialog(this, _("Select level"), wxEmptyString,
		wxEmptyString, wildcard, wxFD_OPEN);
	if(dialog->ShowModal() == wxID_OK) {
		if(level != NULL) {
			delete level;
		}

		level = new Level();
		level->load(dialog->GetPath().ToAscii());
		
		wxString title;
		char * level_name;

		title = _("Smash Battle level editor [");
		title.append(wxString::FromAscii(level->header.name));
		title.append(_("]"));

		this->SetTitle(title);

		wxString bg_file_full;
		bg_file_full = dialog->GetPath().BeforeLast('\\').BeforeLast('\\');
		bg_file_full.append(_("\\gfx\\"));
		bg_file_full.append(wxString::FromAscii(level->header.filename_background));

		background = new wxBitmap();
		if(!background->LoadFile(bg_file_full, wxBITMAP_TYPE_BMP)) {
			// Error: bmp could not be loaded
		}

		display->Refresh();
	}
}
