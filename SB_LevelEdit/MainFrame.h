#pragma once

#include "Level.h"
#include "TilePanel.h"
#include "PropsPanel.h"
#include "PowerupsPanel.h"
#include "NPCsPanel.h"
#include "CharactersPanel.h"
#include "wx/tglbtn.h"
#include "wx/notebook.h"

struct Character {
	wxString name;
};

struct Powerup {
	int id;
	wxString name;
	int w;
	int h;
	int x;
	int y;
};

struct Npc {
	int id;
	wxString name;
	int w;
	int h;
	int left_x;
	int left_y;
	int right_x;
	int right_y;
};

class MainFrame : public wxFrame {
public:
	MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
	~MainFrame();
	
	static MainFrame * instance;

	static const int CharacterCount;
	static const Character CHARACTER[];

	static const int PowerupCount;
	static const Powerup POWERUP[];
	
	static const int NPCCount;
	static const Npc NPC[];
	
	bool level_modified;

	int prop_selected;
	int powerup_selected;
	int npc_selected;
	
	void InitializeComponents();

	bool LoadFiles();

	void TileCopy();
	void TilePaste();
protected:
	void OnExit(wxCloseEvent &event);

	void OnNew(wxCommandEvent &event);
	void OnOpen(wxCommandEvent &event);
	void OnSave(wxCommandEvent &event);

	void OnOptions(wxCommandEvent &event);
	void OnMission(wxCommandEvent &event);

    void OnDisplayPaint(wxPaintEvent &event);
	void OnDisplayErase(wxEraseEvent &event);
	void OnDisplayMouseDown(wxMouseEvent &event);
	void OnDisplayKeyDown(wxKeyEvent &event);
	
	void OnToggleBackground(wxCommandEvent &event);
	void OnToggleTiles(wxCommandEvent &event);
	void OnToggleProps(wxCommandEvent &event);
	void OnTogglePStarts(wxCommandEvent &event);
	void OnTogglePowerups(wxCommandEvent &event);
	void OnToggleNPCs(wxCommandEvent &event);

	void LevelNew();
	void LevelOpen();
	void LevelSave();
	void LevelClose();

	void LevelOptions();
	void LevelMission();

	void LevelLoadBitmaps();
	bool LevelPromptClose();

	DECLARE_EVENT_TABLE()

	wxButton * btnNew;
	wxButton * btnOpen;
	wxButton * btnSave;

	wxButton * btnOptions;
	wxButton * btnMission;

	wxToggleButton * btnBackground;
	wxToggleButton * btnTiles;
	wxToggleButton * btnProps;
	wxToggleButton * btnPStarts;
	wxToggleButton * btnPowerups;
	wxToggleButton * btnNPCs;

	wxPanel * toolbar;
	wxPanel * display;

	wxNotebook * more;
	TilePanel * tilepanel;
	PropsPanel * propspanel;
	PowerupsPanel * powerupspanel;
	NPCsPanel * npcspanel;
	CharactersPanel * characterspanel;

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
	wxBitmap * powerups;
	wxBitmap * npcs;
	wxMask * mask_npcs;

	int tile_selected;
	
	LEVEL_TILE tile_copy;
	bool tile_copied;

	bool show_grid;
	bool show_background;
	bool show_tiles;
	bool show_props;
	bool show_pstarts;
	bool show_powerups;
	bool show_npcs;
};

enum {
	ID_New = 1,
	ID_Open,
	ID_Save,
	ID_Options,
	ID_Mission,
	ID_ToggleBackground,
	ID_ToggleTiles,
	ID_ToggleProps,
	ID_TogglePStarts,
	ID_TogglePowerups,
	ID_ToggleNPCs
};
