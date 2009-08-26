#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "MainFrame.h"
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
	tileSelector = new TileSelector(this, wxID_ANY, wxPoint(5, 0), wxSize(190, 190));

	lblHP = new wxStaticText(this, wxID_ANY, _("HP: (infinite)"), wxPoint(5, 200), wxSize(190, 20));
	sliderHP = new wxSlider(this, ID_sliderHP, -1, -1, 250, wxPoint(5, 220), wxSize(190, 20));
	sliderHP->Connect(wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(TilePanel::OnSliderHP));

	chkBouncable = new wxCheckBox(this, ID_chkBouncable, _("Bouncable"), wxPoint(5, 245), wxSize(190, 20));
	chkShowInPreview = new wxCheckBox(this, ID_chkShowInPreview, _("Show in preview"), wxPoint(5, 265), wxSize(190, 20));

	btnCopy = new wxButton(this, ID_btnCopy, _("&Copy"), wxPoint(5, 290), wxSize(90, 30));
	btnCopy->Disable();

	btnPaste = new wxButton(this, ID_btnPaste, _("&Paste"), wxPoint(105, 290), wxSize(90, 30));
	btnPaste->Disable();
}

BEGIN_EVENT_TABLE(TilePanel, wxPanel)
	EVT_CHECKBOX(ID_chkBouncable, TilePanel::OnChkBouncable)
	EVT_CHECKBOX(ID_chkShowInPreview, TilePanel::OnChkShowInPreview)
	EVT_BUTTON(ID_btnCopy, TilePanel::OnBtnCopy)
	EVT_BUTTON(ID_btnPaste, TilePanel::OnBtnPaste)
END_EVENT_TABLE()

void TilePanel::setLevel(Level * l, wxBitmap * tiles) {
	level = l;

	tileSelector->setTiles(tiles);
}

void TilePanel::setTile(int t) {
	tile = t;
	if(t == -1 || level == 0) {
		sliderHP->Disable();

		chkBouncable->Disable();
		chkShowInPreview->Disable();

		btnCopy->Disable();
	} else {
		sliderHP->Enable();

		chkBouncable->Enable();
		chkShowInPreview->Enable();
		
		tileSelector->SetSelected(level->tile[t].tile);
		if(level->tile[t].indestructible)
			sliderHP->SetValue(-1);
		else
			sliderHP->SetValue(level->tile[t].hp);

		chkBouncable->SetValue(level->tile[t].bouncing);
		chkShowInPreview->SetValue(level->tile[t].show_in_preview);
		UpdateLabels();
		
		btnCopy->Enable();
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
	if(sliderHP->GetValue() == -1) {
		level->tile[tile].hp = 0xffff;
		level->tile[tile].indestructible = true;
	} else {
		level->tile[tile].hp = sliderHP->GetValue();
		level->tile[tile].indestructible = false;
	}
	level->tile[tile].bouncing = chkBouncable->GetValue();
	level->tile[tile].show_in_preview = chkShowInPreview->GetValue();

	MainFrame::instance->level_modified = true;
}

void TilePanel::OnSliderHP(wxScrollEvent &event) {
	UpdateLabels();
	instance->saveTile();
	MainFrame::instance->Refresh();
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
