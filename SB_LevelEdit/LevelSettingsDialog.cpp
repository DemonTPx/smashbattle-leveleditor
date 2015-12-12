#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "wx/filesys.h"

#include "LevelSettingsDialog.h"

LevelSettingsDialog * LevelSettingsDialog::instance = NULL;

LevelSettingsDialog::LevelSettingsDialog(wxWindow* parent, wxWindowID id, const wxPoint& pos) : wxDialog(parent, id, _("New level"), pos){
	instance = this;

	InitializeComponents();
}

LevelSettingsDialog::~LevelSettingsDialog() {
	instance = NULL;
}

void LevelSettingsDialog::InitializeComponents() {
	wxFlexGridSizer *gridSizer = new wxFlexGridSizer(2);

	gridSizer->Add(new wxStaticText(this, wxID_ANY, _("Name:")));
	txtName = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	txtName->SetMaxLength(20);
	gridSizer->Add(txtName, 1, wxEXPAND);

	gridSizer->Add(new wxStaticText(this, wxID_ANY, _("Author:")));
	txtAuthor = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	txtAuthor->SetMaxLength(20);
	gridSizer->Add(txtAuthor, 1, wxEXPAND);

	gridSizer->Add(new wxStaticText(this, wxID_ANY, _("Type:")));
	cmbType = new wxChoice(this, wxID_ANY);
	cmbType->Append(_("Single player"));
	cmbType->Append(_("Multiplayer"));
	cmbType->Select(1);
	gridSizer->Add(cmbType, 1, wxEXPAND);

	gridSizer->Add(new wxStaticText(this, wxID_ANY, _("Max Players:")));
	cmbMaxPlayers = new wxChoice(this, wxID_ANY);
	cmbMaxPlayers->Append(_("2"));
	cmbMaxPlayers->Append(_("3"));
	cmbMaxPlayers->Append(_("4"));
	cmbMaxPlayers->Select(2);
	gridSizer->Add(cmbMaxPlayers, 1, wxEXPAND);

	gridSizer->Add(new wxStaticText(this, wxID_ANY, _("Background color:")));
	clrBackground = new wxColourPickerCtrl(this, wxID_ANY, *wxBLACK);
	gridSizer->Add(clrBackground);

	gridSizer->Add(new wxStaticText(this, wxID_ANY, _("Background file:")));
	cmbFileBackground = new wxChoice(this, wxID_ANY);
	gridSizer->Add(cmbFileBackground, 1, wxEXPAND);

	gridSizer->Add(new wxStaticText(this, wxID_ANY, _("Tiles file:")));
	cmbFileTiles = new wxChoice(this, wxID_ANY);
	gridSizer->Add(cmbFileTiles, 1, wxEXPAND);

	gridSizer->Add(new wxStaticText(this, wxID_ANY, _("Props file:")));
	cmbFileProps = new wxChoice(this, wxID_ANY);
	gridSizer->Add(cmbFileProps, 1, wxEXPAND);

	gridSizer->Add(new wxStaticText(this, wxID_ANY, _("Music file:")));
	cmbFileMusic = new wxChoice(this, wxID_ANY);
	gridSizer->Add(cmbFileMusic, 1, wxEXPAND);

	gridSizer->AddGrowableCol(1, 1);

	wxStdDialogButtonSizer * buttonSizer = CreateStdDialogButtonSizer(wxOK | wxCANCEL);

	wxBoxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(gridSizer, 1, wxEXPAND | wxALL, 5);
	sizer->Add(buttonSizer, 0, wxALL | wxALIGN_RIGHT, 5);
	sizer->SetMinSize(400, 0);

	SetSizerAndFit(sizer);
}

int LevelSettingsDialog::NewLevel() {
	wxString wildcard = _("Level files (*.lvl)|*.lvl|All files (*.*)|*.*");
	wxFileDialog dialog(this, _("New level"), wxEmptyString, wxEmptyString, wildcard, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	int ret;
	wxFileSystem fs;
	wxString file;
	wxString gfx_path;
	wxString music_path;
	char sep;

	ret = dialog.ShowModal();
	if(ret == wxID_OK) {
		filename = dialog.GetPath();

		// TODO: check if the file is in the right directory
		sep = wxFileName::GetPathSeparator();

		gfx_path = dialog.GetPath().BeforeLast(sep).BeforeLast(sep);
		gfx_path.Append(sep).Append(_("gfx")).Append(sep);

		fs.ChangePathTo(gfx_path);
		file = fs.FindFirst(_("*.bmp"), wxFILE);
		while(file != wxEmptyString) {
			cmbFileTiles->Append(file.AfterLast(sep));
			cmbFileBackground->Append(file.AfterLast(sep));
			cmbFileProps->Append(file.AfterLast(sep));
			
			file = fs.FindNext();
		}

		music_path = dialog.GetPath().BeforeLast(sep).BeforeLast(sep);
		music_path.Append(sep).Append(_("music")).Append(sep);

		fs.ChangePathTo(music_path);
		file = fs.FindFirst(_("*.ogg"), wxFILE);
		while(file != wxEmptyString) {
			cmbFileMusic->Append(file.AfterLast(sep));
			
			file = fs.FindNext();
		}

		ret = this->ShowModal();
	}

	return ret;
}

int LevelSettingsDialog::EditLevel(wxString filename, LEVEL_META &meta) {
	wxFileSystem fs;
	wxString file;
	wxString basename;
	wxString gfx_path;
	wxString music_path;
	char sep;
	wxColour color;

	this->SetTitle(_("Edit level"));

	this->filename = wxString(filename, wxConvUTF8);

	sep = wxFileName::GetPathSeparator();

	gfx_path = this->filename.BeforeLast(sep).BeforeLast(sep);
	gfx_path.Append(sep).Append(_("gfx")).Append(sep);

	cmbFileTiles->Append(wxEmptyString);
	cmbFileBackground->Append(wxEmptyString);
	cmbFileProps->Append(wxEmptyString);

	fs.ChangePathTo(gfx_path);
	file = fs.FindFirst(_("*.bmp"), wxFILE);
	while(file != wxEmptyString) {
		basename = file.AfterLast(sep);

		cmbFileTiles->Append(basename);
		if(basename == wxString(meta.filename_tiles, wxConvUTF8))
			cmbFileTiles->Select(cmbFileTiles->GetCount() - 1);

		cmbFileBackground->Append(basename);
		if(basename == wxString(meta.filename_background, wxConvUTF8))
			cmbFileBackground->Select(cmbFileBackground->GetCount() - 1);

		cmbFileProps->Append(basename);
		if(basename == wxString(meta.filename_props, wxConvUTF8))
			cmbFileProps->Select(cmbFileProps->GetCount() - 1);
		
		file = fs.FindNext();
	}

	music_path = this->filename.BeforeLast(sep).BeforeLast(sep);
	music_path.Append(sep).Append(_("music")).Append(sep);

	cmbFileMusic->Append(wxEmptyString);

	fs.ChangePathTo(music_path);
	file = fs.FindFirst(_("*.ogg"), wxFILE);
	while(file != wxEmptyString) {
		basename = file.AfterLast(sep);

		cmbFileMusic->Append(basename);
		if(basename == wxString(meta.filename_music, wxConvUTF8))
			cmbFileMusic->Select(cmbFileMusic->GetCount() - 1);
		
		file = fs.FindNext();
	}

	txtName->SetValue(wxString(meta.name, wxConvUTF8));
	txtAuthor->SetValue(wxString(meta.author, wxConvUTF8));

	cmbType->SetSelection(meta.multiplayer ? 1 : 0);
	
	color.Set((meta.background_color >> 16) & 0xff, (meta.background_color >> 8) & 0xff, meta.background_color & 0xff);
	clrBackground->SetColour(color);

	return this->ShowModal();
}

void LevelSettingsDialog::GetMeta(LEVEL_META &meta) {
	unsigned int color;
	wxColour wxc;

	memset(&meta, 0, sizeof(LEVEL_META));

	strncpy(meta.name, txtName->GetValue().ToAscii(), 20);
	strncpy(meta.author, txtAuthor->GetValue().ToAscii(), 20);

	meta.multiplayer = (cmbType->GetSelection() == 1);
	meta.max_players = wxAtoi(cmbMaxPlayers->GetStringSelection());

	wxc = clrBackground->GetColour();
	color = wxc.Red() << 16 | wxc.Green() << 8 | wxc.Blue();

	meta.background_color = color;
	
	strncpy(meta.filename_background, cmbFileBackground->GetStringSelection().ToAscii(), 30);
	strncpy(meta.filename_tiles, cmbFileTiles->GetStringSelection().ToAscii(), 30);
	strncpy(meta.filename_props, cmbFileProps->GetStringSelection().ToAscii(), 30);
	strncpy(meta.filename_music, cmbFileMusic->GetStringSelection().ToAscii(), 30);
}
