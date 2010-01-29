#pragma once

#include "Level.h"

class NPCDialog : public wxDialog {
public:
	NPCDialog(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition);
	~NPCDialog();

	static NPCDialog * instance;
	
	void InitializeComponents();

	void GetNPC(LEVEL_NPC &npc);
	void SetNPC(LEVEL_NPC &npc);
protected:
	wxStaticText * lblType;
	wxStaticText * lblPosition;
	wxStaticText * lblDirection;

	wxChoice * cmbType;

	wxTextCtrl * txtX;
	wxTextCtrl * txtY;

	wxChoice * cmbDirection;

	wxButton * btnOK;
	wxButton * btnCancel;
};