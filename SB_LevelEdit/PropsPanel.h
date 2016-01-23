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
	wxButton * btnChg;
	wxButton * btnRem;
	wxButton * btnDuplicate;
	wxButton * btnMoveTop;
	wxButton * btnMoveUp;
	wxButton * btnMoveDown;
	wxButton * btnMoveBottom;

	void OnLstPropSelected(wxCommandEvent &event);

	void OnBtnAdd(wxCommandEvent &event);
	void OnBtnChg(wxCommandEvent &event);
	void OnBtnRem(wxCommandEvent &event);
	void OnBtnDuplicate(wxCommandEvent &event);
	void OnBtnMoveTop(wxCommandEvent &event);
	void OnBtnMoveUp(wxCommandEvent &event);
	void OnBtnMoveDown(wxCommandEvent &event);
	void OnBtnMoveBottom(wxCommandEvent &event);

	void MoveTile(int source, int destination);

	void RefreshProps();
};

enum {
	ID_btnAdd = 1,
	ID_btnChg,
	ID_btnRem,
	ID_btnDuplicate,
	ID_btnMoveTop,
	ID_btnMoveUp,
	ID_btnMoveDown,
	ID_btnMoveBottom
};
