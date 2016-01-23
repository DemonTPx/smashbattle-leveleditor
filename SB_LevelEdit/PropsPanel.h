#pragma once

#include "Level.h"

class PropsPanel : public wxPanel {
public:
	PropsPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = _("panel"));

	static PropsPanel * instance;
	
	void InitializeComponents();
	
	void setLevel(Level * l);
protected:
	Level * level;
	
	DECLARE_EVENT_TABLE()

	wxListBox * lstProps;
	wxButton * btnAdd;
	wxButton * btnDuplicate;
	wxButton * btnChg;
	wxButton * btnRem;

	void OnLstPropSelected(wxCommandEvent &event);

	void OnBtnAdd(wxCommandEvent &event);
	void OnBtnDuplicate(wxCommandEvent &event);
	void OnBtnChg(wxCommandEvent &event);
	void OnBtnRem(wxCommandEvent &event);

	void RefreshProps();
};

enum {
	ID_btnAdd = 1,
	ID_btnDuplicate,
	ID_btnChg,
	ID_btnRem
};
