#pragma once

#include "wx/bmpcbox.h"
#include "Level.h"
#include "TileSelector.h"

class TilePanel : public wxPanel {
public:
	TilePanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = _("panel"));

	static TilePanel * instance;

	void InitializeComponents();

	void setLevel(Level * l, wxBitmap * tiles);
	void setTile(int t);
	void saveTile();
protected:
	Level * level;
	int tile;

	LEVEL_TILE copy;

	DECLARE_EVENT_TABLE()

	TileSelector * tileSelector;

	wxStaticText * lblHP;
	wxSlider * sliderHP;

	wxCheckBox * chkBouncable;
	wxCheckBox * chkShowInPreview;

	wxButton * btnCopy;
	wxButton * btnPaste;

	wxButton * btnPlayerStart;

	void OnSliderHP(wxScrollEvent &event);

	void UpdateLabels();

	void OnChkBouncable(wxCommandEvent &event);
	void OnChkShowInPreview(wxCommandEvent &event);

	void OnBtnCopy(wxCommandEvent &event);
	void OnBtnPaste(wxCommandEvent &event);

	void OnBtnPlayerStart(wxCommandEvent &event);
};

enum {
	ID_sliderTile = 1,
	ID_sliderHP,
	ID_chkBouncable,
	ID_chkShowInPreview,
	ID_btnCopy,
	ID_btnPaste,
	ID_btnPlayerStart
};
