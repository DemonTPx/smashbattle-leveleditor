#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "MainFrame.h"
#include "PowerupDialog.h"
#include "PowerupsPanel.h"

PowerupsPanel * PowerupsPanel::instance = 0;

PowerupsPanel::PowerupsPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) {
	instance = this;
	Create(parent, id, pos, size, style, name);
	InitializeComponents();

	level = 0;
}

void PowerupsPanel::InitializeComponents() {
	lstPowerups = new wxListBox(this, wxID_ANY, wxPoint(5, 5), wxSize(180, 95));
	lstPowerups->Connect(wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(PowerupsPanel::OnLstPowerupSelected));

	btnAdd = new wxButton(this, ID_btnAdd, _("add"), wxPoint(5, 105), wxSize(55, 20));
	btnChg = new wxButton(this, ID_btnChg, _("edit"), wxPoint(65, 105), wxSize(55, 20));
	btnRem = new wxButton(this, ID_btnRem, _("delete"), wxPoint(125, 105), wxSize(55, 20));
}

BEGIN_EVENT_TABLE(PowerupsPanel, wxPanel)
	EVT_BUTTON(ID_btnAdd, PowerupsPanel::OnBtnAdd)
	EVT_BUTTON(ID_btnChg, PowerupsPanel::OnBtnChg)
	EVT_BUTTON(ID_btnRem, PowerupsPanel::OnBtnRem)
END_EVENT_TABLE()

void PowerupsPanel::setLevel(Level * l) {
	level = l;

	RefreshPowerups();
}

void PowerupsPanel::RefreshPowerups() {
	wxString str;
	LEVEL_POWERUP powerup;
	int selected;

	lstPowerups->Clear();
	selected = lstPowerups->GetSelection();
	
	if(level == 0) {
		return;
	}

	for(unsigned int i = 0; i < level->powerups->size(); i++) {
		powerup = *level->powerups->at(i);
		str = wxString::Format(_("type:%d @%dx%d"),
			powerup.type, powerup.position.x, powerup.position.y);
		lstPowerups->Append(str);
	}

	if(selected >= 0 && selected < (int)level->powerups->size()) {
		lstPowerups->Select(selected);
	}
}

void PowerupsPanel::OnLstPowerupSelected(wxCommandEvent &event) {
	MainFrame::instance->powerup_selected = instance->lstPowerups->GetSelection();
	MainFrame::instance->Refresh();
}

void PowerupsPanel::OnBtnAdd(wxCommandEvent &event) {
	int ret;
	LEVEL_POWERUP * p;

	if(level == 0)
		return;

	PowerupDialog dialog(this, wxID_ANY, wxDefaultPosition);
	ret = dialog.ShowModal();
	if(ret == wxID_OK) {
		p = new LEVEL_POWERUP;

		dialog.GetPowerup(*p);

		level->powerups->push_back(p);
		
		lstPowerups->Append(wxString::Format(_("type:%d @%dx%d"),
			p->type, p->position.x, p->position.y));
		lstPowerups->Select(lstPowerups->GetCount() - 1);
		
		MainFrame::instance->powerup_selected = lstPowerups->GetSelection();
		MainFrame::instance->level_modified = true;
		MainFrame::instance->Refresh();
	}
}

void PowerupsPanel::OnBtnChg(wxCommandEvent &event) {
	int ret;
	int idx;
	LEVEL_POWERUP * p;

	idx = lstPowerups->GetSelection();

	if(idx == -1 || idx >= (int)level->powerups->size())
		return;

	PowerupDialog dialog(this, wxID_ANY, wxDefaultPosition);
	p = level->powerups->at(idx);
	dialog.SetPowerup(*p);
	ret = dialog.ShowModal();
	if(ret == wxID_OK) {
		dialog.GetPowerup(*p);
		
		lstPowerups->SetString(idx, wxString::Format(_("type:%d @%dx%d"),
			p->type, p->position.x, p->position.y));
	
		MainFrame::instance->level_modified = true;
		MainFrame::instance->Refresh();
	}
}

void PowerupsPanel::OnBtnRem(wxCommandEvent &event) {
	int ret;
	int idx;
	LEVEL_POWERUP * p;
	
	idx = lstPowerups->GetSelection();

	if(idx == -1 || idx >= (int)level->powerups->size())
		return;

	ret = wxMessageBox(_("Are you sure you want to delete this powerup?"), _("Delete powerup"), wxYES_NO | wxICON_QUESTION, this);
	if(ret == wxYES) {
		p = level->powerups->at(idx);
		level->powerups->erase(level->powerups->begin() + idx);
		delete p;

		RefreshPowerups();
		MainFrame::instance->powerup_selected = -1;
		MainFrame::instance->level_modified = true;
		MainFrame::instance->Refresh();
	}
}
