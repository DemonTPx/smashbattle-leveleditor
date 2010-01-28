#pragma once

#include "Level.h"

class PowerupsPanel : public wxPanel {
public:
	PowerupsPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = _("panel"));

	static PowerupsPanel * instance;
	
	void InitializeComponents();
	
	void setLevel(Level * l);
protected:
	Level * level;
	
	DECLARE_EVENT_TABLE()

	wxListBox * lstPowerups;
	wxButton * btnAdd;
	wxButton * btnChg;
	wxButton * btnRem;

	void OnLstPowerupSelected(wxCommandEvent &event);

	void OnBtnAdd(wxCommandEvent &event);
	void OnBtnChg(wxCommandEvent &event);
	void OnBtnRem(wxCommandEvent &event);

	void RefreshPowerups();
};
/*
enum {
	ID_btnAdd = 1,
	ID_btnChg,
	ID_btnRem
};*/
