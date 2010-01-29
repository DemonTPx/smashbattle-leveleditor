#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "MainFrame.h"
#include "NPCDialog.h"

NPCDialog * NPCDialog::instance = NULL;

NPCDialog::NPCDialog(wxWindow* parent, wxWindowID id, const wxPoint& pos) : wxDialog(parent, id, _("NPC"), pos, wxSize(200, 185)){
	instance = this;

	InitializeComponents();
}

NPCDialog::~NPCDialog() {
	instance = NULL;
}

void NPCDialog::InitializeComponents() {
	SetClientSize(150, 125);

	// Static text
	lblType = new wxStaticText(this, wxID_ANY, _("Type:"), wxPoint(5, 5), wxSize(50, 20));
	lblPosition = new wxStaticText(this, wxID_ANY, _("Position:"), wxPoint(5, 30), wxSize(50, 20));
	lblDirection = new wxStaticText(this, wxID_ANY, _("Direction:"), wxPoint(5, 55), wxSize(50, 20));

	// Input fields
	cmbType = new wxChoice(this, wxID_ANY, wxPoint(60, 5), wxSize(85, 20));
	for(int i = 0; i < MainFrame::NPCCount; i++) {
		cmbType->Append(MainFrame::NPC[i].name);
	}
	cmbType->Select(0);

	txtX = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(60, 30), wxSize(30, 20));
	txtX->SetMaxLength(3);
	txtY = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(95, 30), wxSize(30, 20));
	txtY->SetMaxLength(3);

	cmbDirection = new wxChoice(this, wxID_ANY, wxPoint(60, 55), wxSize(85, 20));
	cmbDirection->Append(_("Left"));
	cmbDirection->Append(_("Right"));
	cmbDirection->Select(0);

	// Buttons
	btnOK = new wxButton(this, wxID_OK, _("&Save"), wxPoint(40, 90), wxSize(50, 30));
	btnOK->SetDefault();
	btnCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxPoint(95, 90), wxSize(50, 30));
}

void NPCDialog::GetNPC(LEVEL_NPC &npc) {
	long l;
	
	memset(&npc, 0, sizeof(LEVEL_NPC));

	npc.type = cmbType->GetSelection();

	txtX->GetValue().ToLong(&l);
	npc.position.x = (int)l;
	txtY->GetValue().ToLong(&l);
	npc.position.y = (int)l;
	
	npc.move_direction = cmbDirection->GetSelection() == 0 ? -1 : 1;
}

void NPCDialog::SetNPC(LEVEL_NPC &npc) {
	cmbType->SetSelection(npc.type);
	txtX->SetValue(wxString::Format(_("%d"), npc.position.x));
	txtY->SetValue(wxString::Format(_("%d"), npc.position.y));
	cmbDirection->SetSelection(npc.move_direction == -1 ? 0 : 1);
}
