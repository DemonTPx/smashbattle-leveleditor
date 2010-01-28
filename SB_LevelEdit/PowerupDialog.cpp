#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

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
	SetClientSize(130, 95);

	// Static text
	lblType = new wxStaticText(this, wxID_ANY, _("Type:"), wxPoint(5, 5), wxSize(50, 20));
	lblPosition = new wxStaticText(this, wxID_ANY, _("Position:"), wxPoint(5, 30), wxSize(50, 20));

	// Input fields
	txtType = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(60, 5), wxSize(30, 20));
	txtType->SetMaxLength(3);

	txtX = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(60, 30), wxSize(30, 20));
	txtX->SetMaxLength(3);
	txtY = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(95, 30), wxSize(30, 20));
	txtY->SetMaxLength(3);

	// Buttons
	btnOK = new wxButton(this, wxID_OK, _("&Save"), wxPoint(20, 60), wxSize(50, 30));
	btnOK->SetDefault();
	btnCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxPoint(75, 60), wxSize(50, 30));
}

void PowerupDialog::GetPowerup(LEVEL_POWERUP &powerup) {
	long l;
	
	memset(&powerup, 0, sizeof(LEVEL_POWERUP));

	txtType->GetValue().ToLong(&l);
	powerup.type = (int)l;

	txtX->GetValue().ToLong(&l);
	powerup.position.x = (int)l;
	txtY->GetValue().ToLong(&l);
	powerup.position.y = (int)l;
}

void PowerupDialog::SetPowerup(LEVEL_POWERUP &powerup) {
	txtType->SetValue(wxString::Format(_("%d"), powerup.type));
	txtX->SetValue(wxString::Format(_("%d"), powerup.position.x));
	txtY->SetValue(wxString::Format(_("%d"), powerup.position.y));
}
