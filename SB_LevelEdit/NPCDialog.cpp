#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

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
	SetClientSize(130, 125);

	// Static text
	lblType = new wxStaticText(this, wxID_ANY, _("Type:"), wxPoint(5, 5), wxSize(50, 20));
	lblPosition = new wxStaticText(this, wxID_ANY, _("Position:"), wxPoint(5, 30), wxSize(50, 20));
	lblDirection = new wxStaticText(this, wxID_ANY, _("Direction:"), wxPoint(5, 55), wxSize(50, 20));

	// Input fields
	txtType = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(60, 5), wxSize(30, 20));
	txtType->SetMaxLength(3);

	txtX = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(60, 30), wxSize(30, 20));
	txtX->SetMaxLength(3);
	txtY = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(95, 30), wxSize(30, 20));
	txtY->SetMaxLength(3);
	
	txtDirection = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(60, 55), wxSize(30, 20));
	txtDirection->SetMaxLength(3);

	// Buttons
	btnOK = new wxButton(this, wxID_OK, _("&Save"), wxPoint(20, 90), wxSize(50, 30));
	btnOK->SetDefault();
	btnCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxPoint(75, 90), wxSize(50, 30));
}

void NPCDialog::GetNPC(LEVEL_NPC &npc) {
	long l;
	
	memset(&npc, 0, sizeof(LEVEL_NPC));

	txtType->GetValue().ToLong(&l);
	npc.type = (int)l;

	txtX->GetValue().ToLong(&l);
	npc.position.x = (int)l;
	txtY->GetValue().ToLong(&l);
	npc.position.y = (int)l;
	
	txtDirection->GetValue().ToLong(&l);
	npc.move_direction = (int)l;
}

void NPCDialog::SetNPC(LEVEL_NPC &npc) {
	txtType->SetValue(wxString::Format(_("%d"), npc.type));
	txtX->SetValue(wxString::Format(_("%d"), npc.position.x));
	txtY->SetValue(wxString::Format(_("%d"), npc.position.y));
	txtDirection->SetValue(wxString::Format(_("%d"), npc.move_direction));
}
