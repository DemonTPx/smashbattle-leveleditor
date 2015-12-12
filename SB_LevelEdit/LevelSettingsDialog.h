#pragma once

#include <vector>
#include "wx/clrpicker.h"

#include "Level.h"

class LevelSettingsDialog : public wxDialog {
public:
	LevelSettingsDialog(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition);
	~LevelSettingsDialog();

	static LevelSettingsDialog * instance;
	
	void InitializeComponents();

	int NewLevel();
	int EditLevel(wxString filename, LEVEL_META &meta);

	void GetMeta(LEVEL_META &meta);

	wxString filename;
protected:
	wxTextCtrl * txtName;
	wxTextCtrl * txtAuthor;
	wxChoice * cmbType;
	wxChoice * cmbMaxPlayers;
	wxColourPickerCtrl * clrBackground;
	wxChoice * cmbFileTiles;
	wxChoice * cmbFileBackground;
	wxChoice * cmbFileProps;
	wxChoice * cmbFileMusic;
};
