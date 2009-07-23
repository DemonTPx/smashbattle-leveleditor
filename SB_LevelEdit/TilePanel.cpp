#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "TilePanel.h"

TilePanel * TilePanel::instance = 0;

TilePanel::TilePanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) {
	instance = this;
	Create(parent, id, pos, size, style, name);
	InitializeComponents();

	level = 0;
	setTile(-1);
}

void TilePanel::InitializeComponents() {
	tileSelector = new TileSelector(this, wxID_ANY, wxPoint(5, 0), wxSize(190, 190));

	lblHP = new wxStaticText(this, wxID_ANY, _("HP: (infinite)"), wxPoint(5, 200), wxSize(190, 20));
	sliderHP = new wxSlider(this, ID_sliderHP, -1, -1, 250, wxPoint(5, 220), wxSize(190, 20));
	sliderHP->Connect(wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(TilePanel::OnSliderHP));

	chkBouncable = new wxCheckBox(this, ID_chkBouncable, _("Bouncable"), wxPoint(5, 245), wxSize(190, 20));
	chkShowInPreview = new wxCheckBox(this, ID_chkShowInPreview, _("Show in preview"), wxPoint(5, 265), wxSize(190, 20));
	
}

BEGIN_EVENT_TABLE(TilePanel, wxPanel)
END_EVENT_TABLE()

void TilePanel::setLevel(Level * l, wxBitmap * tiles) {
	level = l;

	tileSelector->setTiles(tiles);
}

void TilePanel::setTile(int t) {
	tile = t;
	if(t == -1 || level == 0) {
		sliderHP->Enable(false);

		chkBouncable->Enable(false);
		chkShowInPreview->Enable(false);
	} else {
		sliderHP->Enable(true);

		chkBouncable->Enable(true);
		chkShowInPreview->Enable(true);
		
		sliderHP->SetValue(level->tile[t].hp);
		if(level->tile[t].indestructible)
			sliderHP->SetValue(-1);

		chkBouncable->SetValue(level->tile[t].bouncing);
		chkShowInPreview->SetValue(level->tile[t].show_in_preview);
		UpdateLabels();
	}
}

void TilePanel::saveTile() {
	return;
	if(sliderHP->GetValue() == -1) {
		level->tile[tile].hp = 0xffff;
		level->tile[tile].indestructible = true;
	} else {
		level->tile[tile].hp = sliderHP->GetValue();
		level->tile[tile].indestructible = false;
	}
	level->tile[tile].bouncing = chkBouncable->GetValue();
	level->tile[tile].show_in_preview = chkShowInPreview->GetValue();
}

void TilePanel::OnSliderHP(wxScrollEvent &event) {
	UpdateLabels();
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

void TilePanel::OnSave(wxCommandEvent &event) {
}

void TilePanel::OnCancel(wxCommandEvent &event) {
}
