#pragma once

#include "Level.h"
#include "TilePanel.h"
#include "wx/tglbtn.h"

class MainFrame : public wxFrame {
public:
	MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
	~MainFrame();
	
	static MainFrame * instance;
	
	bool level_modified;
	
	void InitializeComponents();

	void TileCopy();
	void TilePaste();
protected:
	void OnExit(wxCloseEvent &event);

	void OnNew(wxCommandEvent &event);
	void OnOpen(wxCommandEvent &event);
	void OnSave(wxCommandEvent &event);
	void OnOptions(wxCommandEvent &event);

    void OnDisplayPaint(wxPaintEvent &event);
	void OnDisplayErase(wxEraseEvent &event);
	void OnDisplayMouseDown(wxMouseEvent &event);
	void OnDisplayKeyDown(wxKeyEvent &event);
	
	void OnToggleBackground(wxCommandEvent &event);
	void OnToggleTiles(wxCommandEvent &event);
	void OnToggleProps(wxCommandEvent &event);
	void OnTogglePStarts(wxCommandEvent &event);

	void LevelNew();
	void LevelOpen();
	void LevelSave();
	void LevelClose();
	void LevelOptions();
	void LevelLoadBitmaps();
	bool LevelPromptClose();

	DECLARE_EVENT_TABLE()

	wxButton * btnNew;
	wxButton * btnOpen;
	wxButton * btnSave;
	wxButton * btnOptions;

	wxToggleButton * btnBackground;
	wxToggleButton * btnTiles;
	wxToggleButton * btnProps;
	wxToggleButton * btnPStarts;

	wxPanel * toolbar;
	wxPanel * display;
	TilePanel * tilepanel;

	Level * level;
	wxString level_filename;

	wxColour bg_color;

	wxBitmap * background;
	wxMask * mask_bg;

	wxBitmap * tiles;
	wxMask * mask_tiles;

	wxBitmap * props;
	wxMask * mask_props;

	wxBitmap * pstarts;

	int tile_selected;
	
	LEVEL_TILE tile_copy;
	bool tile_copied;

	bool show_grid;
	bool show_background;
	bool show_tiles;
	bool show_props;
	bool show_pstarts;
};

enum {
	ID_New = 1,
	ID_Open,
	ID_Save,
	ID_Options,
	ID_ToggleBackground,
	ID_ToggleTiles,
	ID_ToggleProps,
	ID_TogglePStarts,
};
