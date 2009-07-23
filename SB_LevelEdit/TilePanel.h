#pragma once

#include "wx/bmpcbox.h"
#include "Level.h"
#include "TileSelector.h"

class TilePanel : public wxPanel {
public:
	TilePanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = _("panel"));

	void InitializeComponents();

	void setLevel(Level * l, wxBitmap * tiles);
	void setTile(int t);
	void saveTile();
protected:
	static TilePanel * instance;

	Level * level;
	int tile;

	DECLARE_EVENT_TABLE()

	TileSelector * tileSelector;

	wxStaticText * lblHP;
	wxSlider * sliderHP;

	wxCheckBox * chkBouncable;
	wxCheckBox * chkShowInPreview;

	wxButton * btnSave;
	wxButton * btnCancel;

	void OnSliderHP(wxScrollEvent &event);

	void UpdateLabels();

	void OnChkBouncable(wxCommandEvent &event);
	void OnChkShowInPreview(wxCommandEvent &event);

	void OnSave(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
};

enum {
	ID_sliderTile = 1,
	ID_sliderHP,
	ID_chkBouncable,
	ID_chkShowInPreview,
	ID_TileSave,
	ID_TileCancel
};
