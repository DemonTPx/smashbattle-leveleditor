#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "PlayerStartDialog.h"

PlayerStartDialog * PlayerStartDialog::instance = NULL;

PlayerStartDialog::PlayerStartDialog(wxWindow* parent, wxWindowID id, const wxPoint& pos) : wxDialog(parent, id, _("Set player start"), pos) {
	instance = this;

	InitializeComponents();
}

PlayerStartDialog::~PlayerStartDialog() {
	instance = NULL;
}

void PlayerStartDialog::InitializeComponents() {
	wxBoxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	wxGridSizer * gridSizer = new wxGridSizer(2);

	// Static text
	gridSizer->Add(new wxStaticText(this, wxID_ANY, _("Player:")));
	cmbPlayer = new wxChoice(this, wxID_ANY);
	cmbPlayer->Append(_("1"));
	cmbPlayer->Append(_("2"));
	cmbPlayer->Append(_("3"));
	cmbPlayer->Append(_("4"));
	gridSizer->Add(cmbPlayer, 0, wxEXPAND);

	gridSizer->Add(new wxStaticText(this, wxID_ANY, _("Direction:")));
	cmbDirection = new wxChoice(this, wxID_ANY);
	cmbDirection->Append(_("Left"));
	cmbDirection->Append(_("Right"));
	gridSizer->Add(cmbDirection, 0, wxEXPAND);

	sizer->Add(gridSizer, 0, wxEXPAND | wxALL, 5);
	sizer->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, 5);
	sizer->SetMinSize(220, 0);

	SetSizerAndFit(sizer);
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
