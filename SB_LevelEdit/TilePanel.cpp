#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "MainFrame.h"
#include "PlayerStartDialog.h"
#include "TilePanel.h"

TilePanel * TilePanel::instance = 0;

TilePanel::TilePanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) {
	instance = this;
	Create(parent, id, pos, size, style, name);
	InitializeComponents();

	level = 0;
	setTile(-1);

	memset(&copy, 0, sizeof(LEVEL_TILE));
}

void TilePanel::InitializeComponents() {
	tileSelector = new TileSelector(this, wxID_ANY);

	lblHP = new wxStaticText(this, wxID_ANY, _("HP: (infinite)"));
	sliderHP = new wxSlider(this, ID_sliderHP, 50, 0, 250);
	sliderHP->Connect(wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(TilePanel::OnSliderHP));

	chkIndestructible = new wxCheckBox(this, ID_chkIndestructible, _("Indestructible"));
	chkBouncable = new wxCheckBox(this, ID_chkBouncable, _("Bouncable"));
	chkShowInPreview = new wxCheckBox(this, ID_chkShowInPreview, _("Show in preview"));

	btnCopy = new wxButton(this, ID_btnCopy, _("&Copy"));
	btnCopy->Disable();

	btnPaste = new wxButton(this, ID_btnPaste, _("&Paste"));
	btnPaste->Disable();

	btnPlayerStart = new wxButton(this, ID_btnPlayerStart, _("Player start"));
	btnPlayerStart->Disable();

	wxBoxSizer * copyPasteSizer = new wxBoxSizer(wxHORIZONTAL);
	copyPasteSizer->Add(btnCopy, 1, wxEXPAND);
	copyPasteSizer->Add(btnPaste, 1, wxEXPAND);

	wxBoxSizer * sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(tileSelector, 1, wxEXPAND | wxALL, 5);
	sizer->Add(lblHP, 0, wxLEFT | wxRIGHT, 5);
	sizer->Add(sliderHP, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);
	sizer->Add(chkIndestructible, 0, wxLEFT | wxRIGHT | wxTOP, 5);
	sizer->Add(chkBouncable, 0, wxLEFT | wxRIGHT, 5);
	sizer->Add(chkShowInPreview, 0, wxLEFT | wxRIGHT, 5);
	sizer->Add(copyPasteSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 5);
	sizer->Add(btnPlayerStart, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

	SetSizerAndFit(sizer);
}

BEGIN_EVENT_TABLE(TilePanel, wxPanel)
	EVT_CHECKBOX(ID_chkIndestructible, TilePanel::OnChkIndestructible)
	EVT_CHECKBOX(ID_chkBouncable, TilePanel::OnChkBouncable)
	EVT_CHECKBOX(ID_chkShowInPreview, TilePanel::OnChkShowInPreview)
	EVT_BUTTON(ID_btnCopy, TilePanel::OnBtnCopy)
	EVT_BUTTON(ID_btnPaste, TilePanel::OnBtnPaste)
	EVT_BUTTON(ID_btnPlayerStart, TilePanel::OnBtnPlayerStart)
END_EVENT_TABLE()

void TilePanel::setLevel(Level * l, wxBitmap * tiles) {
	level = l;

	tileSelector->setTiles(tiles);
}

void TilePanel::setTile(int t) {
	tile = t;
	if(t == -1 || level == 0) {
		sliderHP->SetValue(50);
		sliderHP->Disable();

		chkIndestructible->Disable();
		chkBouncable->Disable();
		chkShowInPreview->Disable();

		btnCopy->Disable();
		btnPlayerStart->Disable();
	} else {
		sliderHP->Enable();

		chkIndestructible->Enable();
		chkBouncable->Enable();
		chkShowInPreview->Enable();
		
		tileSelector->SetSelected(level->tile[t].tile);
		sliderHP->SetValue(level->tile[t].hp);

		chkIndestructible->SetValue(level->tile[t].indestructible);
		chkBouncable->SetValue(level->tile[t].bouncing);
		chkShowInPreview->SetValue(level->tile[t].show_in_preview);
		UpdateLabels();
		
		btnCopy->Enable();
		btnPlayerStart->Enable();
	}
}

void TilePanel::saveTile() {
	if(level == 0)
		return;

	if(tileSelector->GetSelected() == -1) {
		level->tile[tile].tile = 0xffff;
	} else {
		level->tile[tile].tile = tileSelector->GetSelected();
	}
	level->tile[tile].hp = sliderHP->GetValue();
	level->tile[tile].indestructible = chkIndestructible->GetValue();
	level->tile[tile].bouncing = chkBouncable->GetValue();
	level->tile[tile].show_in_preview = chkShowInPreview->GetValue();

	MainFrame::instance->level_modified = true;
}

void TilePanel::OnSliderHP(wxScrollEvent &event) {
	UpdateLabels();
	instance->saveTile();
	MainFrame::instance->Refresh();
}

void TilePanel::OnChkIndestructible(wxCommandEvent &event) {
	instance->saveTile();
}

void TilePanel::OnChkBouncable(wxCommandEvent &event) {
	instance->saveTile();
}

void TilePanel::OnChkShowInPreview(wxCommandEvent &event) {
	instance->saveTile();
}

void TilePanel::OnBtnCopy(wxCommandEvent &event) {
	MainFrame::instance->TileCopy();
	btnPaste->Enable();
}

void TilePanel::OnBtnPaste(wxCommandEvent &event) {
	MainFrame::instance->TilePaste();
}

void TilePanel::OnBtnPlayerStart(wxCommandEvent &event) {
	int ret;
	int pstile;
	PlayerStartDialog dialog(this, wxID_ANY);

	LEVEL_PLAYERSTART * ps;

	for(unsigned int i = 0; i < 4; i++) {
		pstile = (level->playerstart[i].y * TILE_COLS) + level->playerstart[i].x;
		if(tile == pstile) {
			dialog.SetPlayerStart(level->playerstart[i]);
		}
	}
	
	ret = dialog.ShowModal();
	if(ret == wxID_OK) {
		for(unsigned int i = 0; i < 4; i++) {
			pstile = (level->playerstart[i].y * TILE_COLS) + level->playerstart[i].x;
			if(tile == pstile) {
				level->playerstart[i].player = 0xffff;
			}
		}
		if(dialog.GetPlayer() >= 0 && dialog.GetPlayer() < 4) {
			ps = &level->playerstart[dialog.GetPlayer()];
			ps->x = tile % TILE_COLS;
			ps->y = tile / TILE_COLS;
			dialog.GetPlayerStart(*ps);
		}

		MainFrame::instance->level_modified = true;
		MainFrame::instance->Refresh();
	}
}

void TilePanel::UpdateLabels() {
	TilePanel * t = TilePanel::instance;
	wxString label;
	int p;
	
	p = t->sliderHP->GetValue();
	label = _("HP: (");
	if(p == -1)
		label.Append(_("infinite"));
	else
		label << p;
	label << _(")");

	t->lblHP->SetLabel(label);
}
