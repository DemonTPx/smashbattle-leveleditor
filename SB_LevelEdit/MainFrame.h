#pragma once

#include "Level.h"
#include "wx/tglbtn.h"

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
	
	void InitializeComponents();

protected:
	void OnExit(wxCommandEvent &event);
	void OnOpen(wxCommandEvent &event);
	void OnClose(wxCommandEvent &event);
    void OnDisplayPaint(wxPaintEvent& event);
	
	void OnToggleBackground(wxCommandEvent &event);
	void OnToggleTiles(wxCommandEvent &event);
	void OnToggleProps(wxCommandEvent &event);

	DECLARE_EVENT_TABLE()

	wxButton * btnOpen;
	wxButton * btnClose;

	wxToggleButton * btnBackground;
	wxToggleButton * btnTiles;
	wxToggleButton * btnProps;

	wxPanel * toolbar;
	wxPanel * display;

	static MainFrame * instance;

	Level * level;

	wxColour bg_color;

	wxBitmap * background;
	wxMask * mask_bg;

	wxBitmap * tiles;
	wxMask * mask_tiles;

	wxBitmap * props;
	wxMask * mask_props;

	bool show_background;
	bool show_tiles;
	bool show_props;
};

enum {
	ID_New = 1,
	ID_Open,
	ID_Save,
	ID_Close,
	ID_ToggleBackground,
	ID_ToggleTiles,
	ID_ToggleProps,
};
