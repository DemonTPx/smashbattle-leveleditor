#pragma once

#include "Level.h"

class PlayerStartDialog : public wxDialog {
public:
	PlayerStartDialog(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition);
	~PlayerStartDialog();

	static PlayerStartDialog * instance;
	
	void InitializeComponents();

	void SetPlayerStart(LEVEL_PLAYERSTART &pstart);
	void GetPlayerStart(LEVEL_PLAYERSTART &pstart);

	int GetPlayer();
protected:
	wxStaticText * lblPlayer;
	wxStaticText * lblDirection;
	
	wxChoice * cmbPlayer;
	wxChoice * cmbDirection;
	
	wxButton * btnOK;
	wxButton * btnCancel;
};
