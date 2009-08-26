#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "MainFrame.h"
#include "PropDialog.h"
#include "PropsPanel.h"

PropsPanel * PropsPanel::instance = 0;

PropsPanel::PropsPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) {
	instance = this;
	Create(parent, id, pos, size, style, name);
	InitializeComponents();

	level = 0;
}

void PropsPanel::InitializeComponents() {
	lstProps = new wxListBox(this, wxID_ANY, wxPoint(5, 5), wxSize(190, 70));
	lstProps->Connect(wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler(PropsPanel::OnLstPropSelected));

	btnAdd = new wxButton(this, ID_btnAdd, _("add"), wxPoint(5, 80), wxSize(60, 30));
	btnChg = new wxButton(this, ID_btnChg, _("edit"), wxPoint(70, 80), wxSize(60, 30));
	btnRem = new wxButton(this, ID_btnRem, _("delete"), wxPoint(135, 80), wxSize(60, 30));
}

BEGIN_EVENT_TABLE(PropsPanel, wxPanel)
	EVT_BUTTON(ID_btnAdd, PropsPanel::OnBtnAdd)
	EVT_BUTTON(ID_btnChg, PropsPanel::OnBtnChg)
	EVT_BUTTON(ID_btnRem, PropsPanel::OnBtnRem)
END_EVENT_TABLE()

void PropsPanel::setLevel(Level * l) {
	level = l;

	RefreshProps();
}

void PropsPanel::RefreshProps() {
	wxString str;
	LEVEL_PROP prop;
	int selected;

	lstProps->Clear();
	selected = lstProps->GetSelection();
	
	if(level == 0) {
		return;
	}

	for(unsigned int i = 0; i < level->props->size(); i++) {
		prop = *level->props->at(i);
		str = wxString::Format(_("size:%dx%d src:%dx%d dst:%dx%d"),
			prop.src.w, prop.src.h,
			prop.src.x, prop.src.y,
			prop.dst.x, prop.dst.y);
		lstProps->Append(str);
	}

	if(selected >= 0 && selected < (int)level->props->size()) {
		lstProps->Select(selected);
	}
}

void PropsPanel::OnLstPropSelected(wxCommandEvent &event) {
	MainFrame::instance->prop_selected = instance->lstProps->GetSelection();
	MainFrame::instance->Refresh();
}

void PropsPanel::OnBtnAdd(wxCommandEvent &event) {
	int ret;
	LEVEL_PROP * p;

	PropDialog dialog(this, wxID_ANY, wxDefaultPosition);
	ret = dialog.ShowModal();
	if(ret == wxID_OK) {
		p = new LEVEL_PROP;

		dialog.GetProp(*p);
		level->props->push_back(p);
		
		lstProps->Append(wxString::Format(_("size:%dx%d src:%dx%d dst:%dx%d"),
			p->src.w, p->src.h,
			p->src.x, p->src.y,
			p->dst.x, p->dst.y));
		lstProps->Select(lstProps->GetCount() - 1);
		
		MainFrame::instance->prop_selected = lstProps->GetSelection();
		MainFrame::instance->level_modified = true;
		MainFrame::instance->Refresh();
	}
}

void PropsPanel::OnBtnChg(wxCommandEvent &event) {
	int ret;
	int idx;
	LEVEL_PROP * p;

	idx = lstProps->GetSelection();

	if(idx == -1 || idx >= (int)level->props->size())
		return;

	PropDialog dialog(this, wxID_ANY, wxDefaultPosition);
	p = level->props->at(idx);
	dialog.SetProp(*p);
	ret = dialog.ShowModal();
	if(ret == wxID_OK) {
		dialog.GetProp(*p);
		
		lstProps->SetString(idx, wxString::Format(_("size:%dx%d src:%dx%d dst:%dx%d"),
			p->src.w, p->src.h,
			p->src.x, p->src.y,
			p->dst.x, p->dst.y));
	
		MainFrame::instance->level_modified = true;
		MainFrame::instance->Refresh();
	}
}

void PropsPanel::OnBtnRem(wxCommandEvent &event) {
	int ret;
	int idx;
	LEVEL_PROP * p;
	
	idx = lstProps->GetSelection();

	if(idx == -1 || idx >= (int)level->props->size())
		return;

	ret = wxMessageBox(_("Are you sure you want to delete this prop?"), _("Delete prop"), wxYES_NO | wxICON_QUESTION, this);
	if(ret == wxYES) {
		p = level->props->at(idx);
		level->props->erase(level->props->begin() + idx);
		delete p;

		RefreshProps();
		MainFrame::instance->prop_selected = -1;
		MainFrame::instance->level_modified = true;
		MainFrame::instance->Refresh();
	}
}
