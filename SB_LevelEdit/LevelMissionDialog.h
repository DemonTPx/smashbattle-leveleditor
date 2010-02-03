#pragma once

#include "Level.h"

class LevelMissionDialog : public wxDialog {
public:
	LevelMissionDialog(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition);
	~LevelMissionDialog();

	static LevelMissionDialog * instance;

	void OnKillAllTimeGoldUpdated(wxCommandEvent &event);
	void OnKillAllTimeSilverUpdated(wxCommandEvent &event);
	
	void InitializeComponents();

	int EditLevel(LEVEL_MISSION &mission);

	void GetMission(LEVEL_MISSION &mission);

	wxString filename;
protected:
	wxStaticText * lblCharacter;
	wxStaticText * lblType;
	wxStaticText * lblBullets;
	wxStaticText * lblDoubleDamageBullets;
	wxStaticText * lblInstantKillBullets;
	wxStaticText * lblBombs;
	wxStaticText * lblKillAllTimeGold;
	wxStaticText * lblKillAllTimeSilver;

	wxChoice * cmbCharacter;
	wxChoice * cmbType;
	wxTextCtrl * txtBullets;
	wxTextCtrl * txtDoubleDamageBullets;
	wxTextCtrl * txtInstantKillBullets;
	wxTextCtrl * txtBombs;
	wxTextCtrl * txtKillAllTimeGold;
	wxTextCtrl * txtKillAllTimeSilver;

	wxStaticText * lblKillAllTimeGoldTime;
	wxStaticText * lblKillAllTimeSilverTime;

	wxButton * btnSave;
	wxButton * btnCancel;
};
