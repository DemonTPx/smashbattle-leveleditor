#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "MainFrame.h"
#include "NPCDialog.h"

NPCDialog * NPCDialog::instance = NULL;

NPCDialog::NPCDialog(wxWindow* parent, wxWindowID id, const wxPoint& pos) :
		wxDialog(parent, id, _("NPC"), pos) {
	instance = this;

	InitializeComponents();
}

NPCDialog::~NPCDialog() {
	instance = NULL;
}

void NPCDialog::InitializeComponents() {
	wxGridSizer * gridSizer = new wxGridSizer(2);

	gridSizer->Add(new wxStaticText(this, wxID_ANY, _("Type:")), 0, wxALIGN_CENTER_VERTICAL);
	cmbType = new wxChoice(this, wxID_ANY);
	for(int i = 0; i < MainFrame::NPCCount; i++) {
		cmbType->Append(MainFrame::NPC[i].name);
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

	gridSizer->Add(new wxStaticText(this, wxID_ANY, _("Direction:")), 0, wxALIGN_CENTER_VERTICAL);
	cmbDirection = new wxChoice(this, wxID_ANY);
	cmbDirection->Append(_("Left"));
	cmbDirection->Append(_("Right"));
	cmbDirection->Select(0);
	gridSizer->Add(cmbDirection, 1, wxEXPAND);

	wxBoxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(gridSizer, 1, wxEXPAND | wxALL, 5);
	sizer->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, 5);
	sizer->SetMinSize(200, 0);

	SetSizerAndFit(sizer);
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
