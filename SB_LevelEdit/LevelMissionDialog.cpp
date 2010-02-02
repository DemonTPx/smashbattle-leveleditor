#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "MainFrame.h"

#include "LevelMissionDialog.h"

LevelMissionDialog * LevelMissionDialog::instance = NULL;

LevelMissionDialog::LevelMissionDialog(wxWindow* parent, wxWindowID id, const wxPoint& pos) : wxDialog(parent, id, _("Edit mission"), pos, wxSize(400, 325)){
	instance = this;

	InitializeComponents();
}

LevelMissionDialog::~LevelMissionDialog() {
	instance = NULL;
}

void LevelMissionDialog::InitializeComponents() {
	SetClientSize(400, 265);

	// Static text
	lblCharacter = new wxStaticText(this, wxID_ANY, _("Character:"), wxPoint(10, 5), wxSize(100, 20));
	lblType = new wxStaticText(this, wxID_ANY, _("Type:"), wxPoint(10, 30), wxSize(100, 20));

	lblBullets = new wxStaticText(this, wxID_ANY, _("Bullets:"), wxPoint(10, 65), wxSize(100, 20));
	lblDoubleDamageBullets = new wxStaticText(this, wxID_ANY, _("DoubleDamage:"), wxPoint(10, 90), wxSize(100, 20));
	lblInstantKillBullets = new wxStaticText(this, wxID_ANY, _("Instant Kill:"), wxPoint(10, 115), wxSize(100, 20));
	lblBombs = new wxStaticText(this, wxID_ANY, _("Bombs:"), wxPoint(10, 140), wxSize(100, 20));

	lblKillAllTimeGold = new wxStaticText(this, wxID_ANY, _("Gold time:"), wxPoint(10, 170), wxSize(100, 20));
	lblKillAllTimeSilver = new wxStaticText(this, wxID_ANY, _("Silver time:"), wxPoint(10, 195), wxSize(100, 20));

	// Input fields
	cmbCharacter = new wxChoice(this, wxID_ANY, wxPoint(120, 5), wxSize(270, 20));
	for(int i = 0; i < MainFrame::CharacterCount; i++) {
		cmbCharacter->Append(MainFrame::CHARACTER[i].name);
	}
	cmbType = new wxChoice(this, wxID_ANY, wxPoint(120, 30), wxSize(270, 20));
	cmbType->Append(_("No mission"));
	cmbType->Append(_("Kill all NPCs"));
	cmbType->Select(0);

	txtBullets = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(120, 65), wxSize(100, 20));
	txtDoubleDamageBullets = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(120, 90), wxSize(100, 20));
	txtInstantKillBullets = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(120, 115), wxSize(100, 20));
	txtBombs = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(120, 140), wxSize(100, 20));

	txtKillAllTimeGold = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(120, 170), wxSize(100, 20));
	txtKillAllTimeSilver = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(120, 195), wxSize(100, 20));

	// Buttons
	btnSave = new wxButton(this, wxID_OK, _("&OK"), wxPoint(220, 230), wxSize(80, 30));
	btnSave->SetDefault();
	btnCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxPoint(310, 230), wxSize(80, 30));
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
