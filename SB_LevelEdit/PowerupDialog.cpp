#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "MainFrame.h"
#include "PowerupDialog.h"

PowerupDialog * PowerupDialog::instance = NULL;

PowerupDialog::PowerupDialog(wxWindow* parent, wxWindowID id, const wxPoint& pos) : wxDialog(parent, id, _("Powerup"), pos, wxSize(200, 185)){
	instance = this;

	InitializeComponents();
}

PowerupDialog::~PowerupDialog() {
	instance = NULL;
}

void PowerupDialog::InitializeComponents() {
	SetClientSize(150, 95);

	// Static text
	lblType = new wxStaticText(this, wxID_ANY, _("Type:"), wxPoint(5, 5), wxSize(50, 20));
	lblPosition = new wxStaticText(this, wxID_ANY, _("Position:"), wxPoint(5, 30), wxSize(50, 20));

	// Input fields
	cmbType = new wxChoice(this, wxID_ANY, wxPoint(60, 5), wxSize(85, 20));
	for(int i = 0; i < MainFrame::PowerupCount; i++) {
		cmbType->Append(MainFrame::POWERUP[i].name);
	}
	cmbType->Select(0);

	txtX = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(60, 30), wxSize(30, 20));
	txtX->SetMaxLength(3);
	txtY = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(95, 30), wxSize(30, 20));
	txtY->SetMaxLength(3);

	// Buttons
	btnOK = new wxButton(this, wxID_OK, _("&Save"), wxPoint(40, 60), wxSize(50, 30));
	btnOK->SetDefault();
	btnCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxPoint(95, 60), wxSize(50, 30));
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
