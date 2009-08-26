#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "PlayerStartDialog.h"

PlayerStartDialog * PlayerStartDialog::instance = NULL;

PlayerStartDialog::PlayerStartDialog(wxWindow* parent, wxWindowID id, const wxPoint& pos) : wxDialog(parent, id, _("Set player start"), pos, wxSize(200, 185)){
	instance = this;

	InitializeComponents();
}

PlayerStartDialog::~PlayerStartDialog() {
	instance = NULL;
}

void PlayerStartDialog::InitializeComponents() {
	SetClientSize(195, 100);
	
	// Static text
	lblPlayer = new wxStaticText(this, wxID_ANY, _("Player:"), wxPoint(5, 5), wxSize(90, 20));
	lblDirection = new wxStaticText(this, wxID_ANY, _("Direction:"), wxPoint(5, 30), wxSize(90, 20));

	// Input fields
	cmbPlayer = new wxChoice(this, wxID_ANY, wxPoint(100, 5), wxSize(90, 20));
	cmbPlayer->Append(_("1"));
	cmbPlayer->Append(_("2"));
	cmbPlayer->Append(_("3"));
	cmbPlayer->Append(_("4"));
	cmbDirection = new wxChoice(this, wxID_ANY, wxPoint(100, 30), wxSize(90, 20));
	cmbDirection->Append(_("Left"));
	cmbDirection->Append(_("Right"));

	// Buttons
	btnOK = new wxButton(this, wxID_OK, _("&Save"), wxPoint(85, 60), wxSize(50, 30));
	btnOK->SetDefault();
	btnCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxPoint(140, 60), wxSize(50, 30));
}

void PlayerStartDialog::SetPlayerStart(LEVEL_PLAYERSTART &pstart) {
	cmbPlayer->SetSelection(pstart.player);
	cmbDirection->SetSelection(pstart.facing_right ? 1 : 0);
}

void PlayerStartDialog::GetPlayerStart(LEVEL_PLAYERSTART &pstart) {
	pstart.player = cmbPlayer->GetSelection();
	pstart.facing_right = (cmbDirection->GetSelection() == 1);
}

int PlayerStartDialog::GetPlayer() {
	return cmbPlayer->GetSelection();
}
