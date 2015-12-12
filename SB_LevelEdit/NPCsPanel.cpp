#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "MainFrame.h"
#include "NPCDialog.h"
#include "NPCsPanel.h"

NPCsPanel * NPCsPanel::instance = 0;

NPCsPanel::NPCsPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) {
	instance = this;
	Create(parent, id, pos, size, style, name);
	InitializeComponents();

	level = 0;
}

void NPCsPanel::InitializeComponents() {
	lstNPCs = new wxListBox(this, wxID_ANY);
	lstNPCs->Connect(wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(NPCsPanel::OnLstNPCSelected));

	btnAdd = new wxButton(this, ID_btnAdd, _("Add"));
	btnChg = new wxButton(this, ID_btnChg, _("Edit"));
	btnRem = new wxButton(this, ID_btnRem, _("Delete"));

	wxBoxSizer * toolbar = new wxBoxSizer(wxHORIZONTAL);
	toolbar->Add(btnAdd, 1, wxEXPAND);
	toolbar->Add(btnChg, 1, wxEXPAND);
	toolbar->Add(btnRem, 1, wxEXPAND);

	wxBoxSizer * panel = new wxBoxSizer(wxVERTICAL);
	panel->Add(lstNPCs, 1, wxALL | wxEXPAND, 5);
	panel->Add(toolbar, 0, wxALL | wxEXPAND, 5);

	SetSizerAndFit(panel);
}

BEGIN_EVENT_TABLE(NPCsPanel, wxPanel)
	EVT_BUTTON(ID_btnAdd, NPCsPanel::OnBtnAdd)
	EVT_BUTTON(ID_btnChg, NPCsPanel::OnBtnChg)
	EVT_BUTTON(ID_btnRem, NPCsPanel::OnBtnRem)
END_EVENT_TABLE()

void NPCsPanel::setLevel(Level * l) {
	level = l;

	RefreshNPCs();
}

void NPCsPanel::RefreshNPCs() {
	wxString str;
	LEVEL_NPC npc;
	int selected;

	lstNPCs->Clear();
	selected = lstNPCs->GetSelection();
	
	if(level == 0) {
		return;
	}

	for(unsigned int i = 0; i < level->npcs->size(); i++) {
		npc = *level->npcs->at(i);
		str = wxString::Format(_("%dx%d %s %s"),
			npc.position.x, npc.position.y, npc.move_direction == -1 ? _("L") : _("R"), MainFrame::NPC[npc.type].name.c_str());
		lstNPCs->Append(str);
	}

	if(selected >= 0 && selected < (int)level->npcs->size()) {
		lstNPCs->Select(selected);
	}
}

void NPCsPanel::OnLstNPCSelected(wxCommandEvent &event) {
	MainFrame::instance->npc_selected = instance->lstNPCs->GetSelection();
	MainFrame::instance->Refresh();
}

void NPCsPanel::OnBtnAdd(wxCommandEvent &event) {
	int ret;
	LEVEL_NPC * npc;

	if(level == 0)
		return;

	NPCDialog dialog(this, wxID_ANY, wxDefaultPosition);
	ret = dialog.ShowModal();
	if(ret == wxID_OK) {
		npc = new LEVEL_NPC;

		dialog.GetNPC(*npc);

		level->npcs->push_back(npc);
		
		lstNPCs->Append(wxString::Format(_("%dx%d %s %s"),
			npc->position.x, npc->position.y, npc->move_direction == -1 ? _("L") : _("R"), MainFrame::NPC[npc->type].name.c_str()));
		lstNPCs->Select(lstNPCs->GetCount() - 1);
		
		MainFrame::instance->npc_selected = lstNPCs->GetSelection();
		MainFrame::instance->level_modified = true;
		MainFrame::instance->Refresh();
	}
}

void NPCsPanel::OnBtnChg(wxCommandEvent &event) {
	int ret;
	int idx;
	LEVEL_NPC * npc;

	idx = lstNPCs->GetSelection();

	if(idx == -1 || idx >= (int)level->npcs->size())
		return;

	NPCDialog dialog(this, wxID_ANY, wxDefaultPosition);
	npc = level->npcs->at(idx);
	dialog.SetNPC(*npc);
	ret = dialog.ShowModal();
	if(ret == wxID_OK) {
		dialog.GetNPC(*npc);
		
		lstNPCs->SetString(idx, wxString::Format(_("%dx%d %s %s"),
			npc->position.x, npc->position.y, npc->move_direction == -1 ? _("L") : _("R"), MainFrame::NPC[npc->type].name.c_str()));
	
		MainFrame::instance->level_modified = true;
		MainFrame::instance->Refresh();
	}
}

void NPCsPanel::OnBtnRem(wxCommandEvent &event) {
	int ret;
	int idx;
	LEVEL_NPC * npc;
	
	idx = lstNPCs->GetSelection();

	if(idx == -1 || idx >= (int)level->npcs->size())
		return;

	ret = wxMessageBox(_("Are you sure you want to delete this NPC?"), _("Delete NPC"), wxYES_NO | wxICON_QUESTION, this);
	if(ret == wxYES) {
		npc = level->npcs->at(idx);
		level->npcs->erase(level->npcs->begin() + idx);
		delete npc;

		RefreshNPCs();
		MainFrame::instance->npc_selected = -1;
		MainFrame::instance->level_modified = true;
		MainFrame::instance->Refresh();
	}
}
