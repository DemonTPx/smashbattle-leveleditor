#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "MainFrame.h"
#include "PowerupDialog.h"

PowerupDialog * PowerupDialog::instance = NULL;

PowerupDialog::PowerupDialog(wxWindow* parent, wxWindowID id, const wxPoint& pos) :
		wxDialog(parent, id, _("Powerup"), pos){
	instance = this;

	InitializeComponents();
}

PowerupDialog::~PowerupDialog() {
	instance = NULL;
}

void PowerupDialog::InitializeComponents() {
	wxGridSizer * gridSizer = new wxGridSizer(2);

	gridSizer->Add(new wxStaticText(this, wxID_ANY, _("Type:")), 0, wxALIGN_CENTER_VERTICAL);
	cmbType = new wxChoice(this, wxID_ANY);
	for(int i = 0; i < MainFrame::PowerupCount; i++) {
		cmbType->Append(MainFrame::POWERUP[i].name);
	}
	cmbType->Select(0);
	gridSizer->Add(cmbType, 1, wxEXPAND);

	gridSizer->Add(new wxStaticText(this, wxID_ANY, _("Position:")), 0, wxALIGN_CENTER_VERTICAL);

	wxBoxSizer * positionSizer = new wxBoxSizer(wxHORIZONTAL);
	txtX = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	txtX->SetMaxLength(3);
	positionSizer->Add(txtX, 1, wxEXPAND);
	txtY = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	txtY->SetMaxLength(3);
	positionSizer->Add(txtY, 1, wxEXPAND);

	gridSizer->Add(positionSizer, 1, wxEXPAND);

	wxBoxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(gridSizer, 1, wxEXPAND | wxALL, 5);
	sizer->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_RIGHT | wxALL, 5);
	sizer->SetMinSize(200, 0);

	SetSizerAndFit(sizer);
}

void PowerupDialog::GetPowerup(LEVEL_POWERUP &powerup) {
	long l;
	
	memset(&powerup, 0, sizeof(LEVEL_POWERUP));

	powerup.type = cmbType->GetSelection();

	txtX->GetValue().ToLong(&l);
	powerup.position.x = (int)l;
	txtY->GetValue().ToLong(&l);
	powerup.position.y = (int)l;
}

void PowerupDialog::SetPowerup(LEVEL_POWERUP &powerup) {
	cmbType->SetSelection(powerup.type);
	txtX->SetValue(wxString::Format(_("%d"), powerup.position.x));
	txtY->SetValue(wxString::Format(_("%d"), powerup.position.y));
}
