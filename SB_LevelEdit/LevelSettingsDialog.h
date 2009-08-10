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
	int EditLevel(wxString filename, LEVEL_HEADER &hdr);

	void GetHeader(LEVEL_HEADER &hdr);

	wxString filename;
protected:
	wxStaticText * lblName;
	wxStaticText * lblAuthor;
	wxStaticText * lblType;
	wxStaticText * lblMaxPlayers;
	wxStaticText * lblBackgroundColor;
	wxStaticText * lblFileTiles;
	wxStaticText * lblFileBackground;
	wxStaticText * lblFileProps;

	wxTextCtrl * txtName;
	wxTextCtrl * txtAuthor;
	wxChoice * cmbType;
	wxChoice * cmbMaxPlayers;
	wxColourPickerCtrl * clrBackground;
	wxChoice * cmbFileTiles;
	wxChoice * cmbFileBackground;
	wxChoice * cmbFileProps;

	wxButton * btnSave;
	wxButton * btnCancel;
};
