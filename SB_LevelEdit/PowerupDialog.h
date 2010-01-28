#pragma once

#include "Level.h"

class PowerupDialog : public wxDialog {
public:
	PowerupDialog(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition);
	~PowerupDialog();

	static PowerupDialog * instance;
	
	void InitializeComponents();

	void GetPowerup(LEVEL_POWERUP &powerup);
	void SetPowerup(LEVEL_POWERUP &powerup);
protected:
	wxStaticText * lblType;
	wxStaticText * lblPosition;

	wxTextCtrl * txtType;
	wxTextCtrl * txtX;
	wxTextCtrl * txtY;

	wxButton * btnOK;
	wxButton * btnCancel;
};