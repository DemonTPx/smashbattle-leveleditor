#pragma once

#include "Level.h"

class PropDialog : public wxDialog {
public:
	PropDialog(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition);
	~PropDialog();

	static PropDialog * instance;
	
	void InitializeComponents();

	void GetProp(LEVEL_PROP &prop);
	void SetProp(LEVEL_PROP &prop);
protected:
	wxTextCtrl * txtSrcX;
	wxTextCtrl * txtSrcY;
	wxTextCtrl * txtSrcW;
	wxTextCtrl * txtSrcH;
	
	wxTextCtrl * txtDstX;
	wxTextCtrl * txtDstY;
};
