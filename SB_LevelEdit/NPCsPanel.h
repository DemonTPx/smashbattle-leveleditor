#pragma once

#include "Level.h"

class NPCsPanel : public wxPanel {
public:
	NPCsPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = _("panel"));

	static NPCsPanel * instance;
	
	void InitializeComponents();
	
	void setLevel(Level * l);
protected:
	Level * level;
	
	DECLARE_EVENT_TABLE()

	wxListBox * lstNPCs;
	wxButton * btnAdd;
	wxButton * btnChg;
	wxButton * btnRem;

	void OnLstNPCSelected(wxCommandEvent &event);

	void OnBtnAdd(wxCommandEvent &event);
	void OnBtnChg(wxCommandEvent &event);
	void OnBtnRem(wxCommandEvent &event);

	void RefreshNPCs();
};
/*
enum {
	ID_btnAdd = 1,
	ID_btnChg,
	ID_btnRem
};*/
