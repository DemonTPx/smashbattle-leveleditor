#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "MainFrame.h"

#include "LevelMissionDialog.h"

LevelMissionDialog * LevelMissionDialog::instance = NULL;

LevelMissionDialog::LevelMissionDialog(wxWindow* parent, wxWindowID id, const wxPoint& pos) :
		wxDialog(parent, id, _("Edit mission"), pos){
	instance = this;

	InitializeComponents();
}

LevelMissionDialog::~LevelMissionDialog() {
	instance = NULL;
}

void LevelMissionDialog::InitializeComponents() {
	wxFlexGridSizer * gridSizer = new wxFlexGridSizer(2);

	// Static text
	gridSizer->Add(new wxStaticText(this, wxID_ANY, _("Character:")), 0, wxALIGN_CENTER_VERTICAL);
	cmbCharacter = new wxChoice(this, wxID_ANY);
	for(int i = 0; i < MainFrame::CharacterCount; i++) {
		cmbCharacter->Append(MainFrame::CHARACTER[i].name);
	}
	gridSizer->Add(cmbCharacter, 1, wxEXPAND);

	gridSizer->Add(new wxStaticText(this, wxID_ANY, _("Type:")), 0, wxALIGN_CENTER_VERTICAL);
	cmbType = new wxChoice(this, wxID_ANY);
	cmbType->Append(_("No mission"));
	cmbType->Append(_("Kill all NPCs"));
	cmbType->Select(0);
	gridSizer->Add(cmbType, 1, wxEXPAND);

	gridSizer->Add(new wxStaticText(this, wxID_ANY, _("Bullets:")), 0, wxALIGN_CENTER_VERTICAL);
	txtBullets = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	gridSizer->Add(txtBullets, 1, wxEXPAND);

	gridSizer->Add(new wxStaticText(this, wxID_ANY, _("DoubleDamage:")), 0, wxALIGN_CENTER_VERTICAL);
	txtDoubleDamageBullets = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	gridSizer->Add(txtDoubleDamageBullets, 1, wxEXPAND);

	gridSizer->Add(new wxStaticText(this, wxID_ANY, _("Instant Kill:")), 0, wxALIGN_CENTER_VERTICAL);
	txtInstantKillBullets = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	gridSizer->Add(txtInstantKillBullets, 1, wxEXPAND);

	gridSizer->Add(new wxStaticText(this, wxID_ANY, _("Bombs:")), 0, wxALIGN_CENTER_VERTICAL);
	txtBombs = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	gridSizer->Add(txtBombs, 1, wxEXPAND);

	gridSizer->Add(new wxStaticText(this, wxID_ANY, _("Gold time:")), 0, wxALIGN_CENTER_VERTICAL);

	txtKillAllTimeGold = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	txtKillAllTimeGold->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(LevelMissionDialog::OnKillAllTimeGoldUpdated));

	lblKillAllTimeGoldTime = new wxStaticText(this, wxID_ANY, wxEmptyString);

	wxBoxSizer * goldTimeSizer = new wxBoxSizer(wxHORIZONTAL);
	goldTimeSizer->Add(txtKillAllTimeGold, 1, wxEXPAND);
	goldTimeSizer->Add(lblKillAllTimeGoldTime, 1, wxLEFT | wxRIGHT, 5);

	gridSizer->Add(goldTimeSizer, 1, wxEXPAND);

	gridSizer->Add(new wxStaticText(this, wxID_ANY, _("Silver time:")), 0, wxALIGN_CENTER_VERTICAL);
	txtKillAllTimeSilver = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
	txtKillAllTimeSilver->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(LevelMissionDialog::OnKillAllTimeSilverUpdated));

	lblKillAllTimeSilverTime = new wxStaticText(this, wxID_ANY, wxEmptyString);

	wxBoxSizer * silverTimeSizer = new wxBoxSizer(wxHORIZONTAL);
	silverTimeSizer->Add(txtKillAllTimeSilver, 1, wxEXPAND);
	silverTimeSizer->Add(lblKillAllTimeSilverTime, 1, wxLEFT | wxRIGHT, 5);

	gridSizer->Add(silverTimeSizer, 1, wxEXPAND);

	gridSizer->AddGrowableCol(1, 1);

	wxBoxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(gridSizer, 1, wxEXPAND | wxALL, 5);
	sizer->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, 5);
	sizer->SetMinSize(400, 0);

	SetSizerAndFit(sizer);
}

int LevelMissionDialog::EditLevel(LEVEL_MISSION &mission) {
	cmbCharacter->Select(mission.character);
	cmbType->Select(mission.type);

	txtBullets->SetValue(wxString::Format(_("%d"), mission.bullets));
	txtDoubleDamageBullets->SetValue(wxString::Format(_("%d"), mission.doubledamagebullets));
	txtInstantKillBullets->SetValue(wxString::Format(_("%d"), mission.instantkillbullets));
	txtBombs->SetValue(wxString::Format(_("%d"), mission.bombs));
	
	txtKillAllTimeGold->SetValue(wxString::Format(_("%d"), mission.kill_all_time_gold));
	txtKillAllTimeSilver->SetValue(wxString::Format(_("%d"), mission.kill_all_time_silver));

	return this->ShowModal();
}

void LevelMissionDialog::GetMission(LEVEL_MISSION &mission) {
	long l;

	memset(&mission, 0, sizeof(LEVEL_MISSION));

	mission.character = cmbCharacter->GetSelection();
	mission.type = cmbType->GetSelection();

	txtBullets->GetValue().ToLong(&l);
	mission.bullets = (int)l;
	txtDoubleDamageBullets->GetValue().ToLong(&l);
	mission.doubledamagebullets = (int)l;
	txtInstantKillBullets->GetValue().ToLong(&l);
	mission.instantkillbullets = (int)l;
	txtBombs->GetValue().ToLong(&l);
	mission.bombs = (int)l;

	txtKillAllTimeGold->GetValue().ToLong(&l);
	mission.kill_all_time_gold = (int)l;
	txtKillAllTimeSilver->GetValue().ToLong(&l);
	mission.kill_all_time_silver = (int)l;
}

void LevelMissionDialog::OnKillAllTimeGoldUpdated(wxCommandEvent &event) {
	long frames;
	int min, sec, msec;

	instance->txtKillAllTimeGold->GetValue().ToLong(&frames);

	min = frames / 3600;
	sec = (frames / 60) % 60;
	msec = frames % 60;

	instance->lblKillAllTimeGoldTime->SetLabel(wxString::Format(_("%d:%02d.%02d"), min, sec, msec));
}

void LevelMissionDialog::OnKillAllTimeSilverUpdated(wxCommandEvent &event) {
	long frames;
	int min, sec, msec;

	instance->txtKillAllTimeSilver->GetValue().ToLong(&frames);

	min = frames / 3600;
	sec = (frames / 60) % 60;
	msec = frames % 60;

	instance->lblKillAllTimeSilverTime->SetLabel(wxString::Format(_("%d:%02d.%02d"), min, sec, msec));
}
