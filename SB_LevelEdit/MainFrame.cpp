#include "wx/wxprec.h"

#include "wx/filename.h"
#include "wx/dcbuffer.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "LevelSettingsDialog.h"
#include "LevelMissionDialog.h"

#include "MainFrame.h"

#define TILE_W 32
#define TILE_H 32
#define TILE_COLS 20
#define TILE_ROWS 15
#define TILE_COUNT (TILE_COLS * TILE_ROWS)

#define PLAYER_W 22
#define PLAYER_H 44

MainFrame * MainFrame::instance = 0;

const int MainFrame::CharacterCount = 20;
const Character MainFrame::CHARACTER[MainFrame::CharacterCount] = {
	{_("Bert")}, {_("Jeroen")}, {_("Steven")}, {_("Tedje")},
	{_("Okke")}, {_("Jeremy")}, {_("Marcel")}, {_("Jay")},
	{_("Donja")}, {_("Rob")}, {_("Eva")}, {_("Tobias")},
	{_("Arjan")}, {_("Ruud")}, {_("Peter")}, {_("Berry")},
	{_("Ton")}, {_("Gijs")}, {_("Rutger")}, {_("Kim")},
};
	
const int MainFrame::PowerupCount = 7;
const Powerup MainFrame::POWERUP[MainFrame::PowerupCount] = {
	{L_PU_HEALTH, _("Healthpack"), 16, 16, 0, 0},
	{L_PU_AMMO,	_("Ammo"), 16, 16, 32, 0},
	{L_PU_DOUBLEDAMAGE, _("Double damage"), 16, 16, 48, 0},
	{L_PU_INSTANTKILL, _("Instant kill"), 16, 16, 64, 0},
	{L_PU_BOMB, _("Bomb"), 16, 16, 16, 0},
	{L_PU_AIRSTRIKE, _("Airstrike"), 16, 16, 80, 0},
	{L_PU_LASERBEAM, _("Laserbeam"), 16, 16, 112, 0},
};

const int MainFrame::NPCCount = 3;
const Npc MainFrame::NPC[MainFrame::NPCCount] = {
	{L_NPC_CHICK, _("Small chick"), 24, 26, 0, 0, 24, 0},
	{L_NPC_CANNON, _("Cannon"), 52, 36, 0, 26, 52, 26},
	{L_NPC_GATLING, _("Gatling gun"), 64, 28, 0, 62, 64, 62},
};

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size) :
		wxFrame((wxFrame *) NULL, -1, title, pos, size, wxDEFAULT_FRAME_STYLE ^ wxRESIZE_BORDER)
{
	instance = this;
	InitializeComponents();

	level = 0;
	level_filename = wxEmptyString;
	level_modified = false;

	bg_color = wxColour(0, 0, 0);

	background = 0;
	tiles = 0;
	props = 0;

	show_grid = false;
	show_background = true;
	show_tiles = true;
	show_props = true;
	show_pstarts = false;
	show_powerups = false;
	show_npcs = false;

	tile_selected = -1;
	prop_selected = -1;
	powerup_selected = -1;
	npc_selected = -1;
	
	memset(&tile_copy, 0, sizeof(LEVEL_TILE));
	tile_copied = false;
}

MainFrame::~MainFrame() {
	if(level != 0) delete level;

	if(background != 0) delete background;
	if(tiles != 0) delete tiles;
	if(props != 0) delete props;
	
	if(pstarts != 0) delete pstarts;
	if(powerups != 0) delete powerups;
	if(npcs != 0) delete npcs;
}

void MainFrame::InitializeComponents()
{
	toolbar = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(1140, 40));

	int btnW = 100;
	int btnH = 40;
	int spaceW = 20;

	btnNew = new wxButton(toolbar, ID_New, _("&New"), wxPoint(0, 0), wxSize(btnW, btnH));
	btnOpen = new wxButton(toolbar, ID_Open, _("&Open"), wxPoint(btnW * 1, 0), wxSize(btnW, btnH));
	btnSave = new wxButton(toolbar, ID_Save, _("&Save"), wxPoint(btnW * 2, 0), wxSize(btnW, btnH));

	btnOptions = new wxButton(toolbar, ID_Options, _("&Options"), wxPoint(spaceW * 1 + btnW * 3, 0), wxSize(btnW, btnH));
	btnMission = new wxButton(toolbar, ID_Mission, _("&Mission"), wxPoint(spaceW * 1 + btnW * 4, 0), wxSize(btnW, btnH));

	btnBackground = new wxToggleButton(toolbar, ID_ToggleBackground, _("&Background"), wxPoint(spaceW * 2 + btnW * 5, 0), wxSize(btnW, btnH));
	btnBackground->SetValue(true);
	btnTiles = new wxToggleButton(toolbar, ID_ToggleTiles, _("&Tiles"), wxPoint(spaceW * 2 + btnW * 6, 0), wxSize(btnW, btnH));
	btnTiles->SetValue(true);
	btnProps = new wxToggleButton(toolbar, ID_ToggleProps, _("&Props"), wxPoint(spaceW * 2 + btnW * 7, 0), wxSize(btnW, btnH));
	btnProps->SetValue(true);
	btnPStarts = new wxToggleButton(toolbar, ID_TogglePStarts, _("Player &starts"), wxPoint(spaceW * 2 + btnW * 8, 0), wxSize(btnW, btnH));
	btnPowerups = new wxToggleButton(toolbar, ID_TogglePowerups, _("Power&ups"), wxPoint(spaceW * 2 + btnW * 9, 0), wxSize(btnW, btnH));
	btnNPCs = new wxToggleButton(toolbar, ID_ToggleNPCs, _("NP&Cs"), wxPoint(spaceW * 2 + btnW * 10, 0), wxSize(btnW, btnH));

	display = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(640, 480));
	display->Connect(wxEVT_PAINT, wxPaintEventHandler(MainFrame::OnDisplayPaint));
	display->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(MainFrame::OnDisplayMouseDown));
	display->Connect(wxEVT_KEY_DOWN, wxCharEventHandler(MainFrame::OnDisplayKeyDown));
	display->SetFocus();

	more = new wxNotebook(this, wxID_ANY);
	tilepanel = new TilePanel(more);
	propspanel = new PropsPanel(more);
	powerupspanel = new PowerupsPanel(more);
	npcspanel = new NPCsPanel(more);
	characterspanel = new CharactersPanel(more);
	more->AddPage(tilepanel, _("Tiles"));
	more->AddPage(propspanel, _("Props"));
	more->AddPage(powerupspanel, _("Powerups"));
	more->AddPage(npcspanel, _("NPCs"));
	more->AddPage(characterspanel, _("Characters"));

	wxBoxSizer * main = new wxBoxSizer(wxHORIZONTAL);

	main->Add(display, 0, wxEXPAND);
	main->Add(more, 1, wxEXPAND);

	sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(toolbar);
	sizer->Add(main, 1, wxEXPAND);

	SetSizer(sizer);

	pstarts = NULL;
	powerups = NULL;
	npcs = NULL;
}

bool MainFrame::LoadFiles() {
	pstarts = new wxBitmap();
	if(!pstarts->LoadFile(_("pstarts.bmp"), wxBITMAP_TYPE_BMP)) {
		wxMessageBox(_("Could not load pstarts.bmp"), _("Error loading bitmap"), wxICON_ERROR | wxCENTRE);
		return false;
	}
	powerups = new wxBitmap();
	if(!powerups->LoadFile(_("powerups.bmp"), wxBITMAP_TYPE_BMP)) {
		wxMessageBox(_("Could not load powerups.bmp"), _("Error loading bitmap"), wxICON_ERROR | wxCENTRE);
		return false;
	}
	npcs = new wxBitmap();
	if(!npcs->LoadFile(_("npcs.bmp"), wxBITMAP_TYPE_BMP)) {
		wxMessageBox(_("Could not load npcs.bmp"), _("Error loading bitmap"), wxICON_ERROR | wxCENTRE);
		return false;
	}

	wxColour mask_colour;

	mask_colour = wxColour(0, 0xff, 0xff);
	mask_npcs = new wxMask(*npcs, mask_colour);
	npcs->SetMask(mask_npcs);

	return true;
}

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_BUTTON(ID_New, MainFrame::OnNew)
	EVT_BUTTON(ID_Open, MainFrame::OnOpen)
	EVT_BUTTON(ID_Save, MainFrame::OnSave)
	EVT_BUTTON(ID_Options, MainFrame::OnOptions)
	EVT_BUTTON(ID_Mission, MainFrame::OnMission)
	EVT_TOGGLEBUTTON(ID_ToggleBackground, MainFrame::OnToggleBackground)
	EVT_TOGGLEBUTTON(ID_ToggleTiles, MainFrame::OnToggleTiles)
	EVT_TOGGLEBUTTON(ID_ToggleProps, MainFrame::OnToggleProps)
	EVT_TOGGLEBUTTON(ID_TogglePStarts, MainFrame::OnTogglePStarts)
	EVT_TOGGLEBUTTON(ID_TogglePowerups, MainFrame::OnTogglePowerups)
	EVT_TOGGLEBUTTON(ID_ToggleNPCs, MainFrame::OnToggleNPCs)
	EVT_SHOW(MainFrame::OnShow)
	EVT_SIZE(MainFrame::OnSize)
	EVT_CLOSE(MainFrame::OnExit)
	EVT_CHAR_HOOK(MainFrame::OnDisplayKeyDown)
END_EVENT_TABLE()


void MainFrame::OnShow(wxShowEvent &event) {
	MainFrame::instance->SetSizerAndFit(MainFrame::instance->GetSizer());
}

void MainFrame::OnExit(wxCloseEvent &event)
{
	if(instance->LevelPromptClose()) {
		Destroy();
	} else {
		event.Skip(false);
	}
}

void MainFrame::OnDisplayMouseDown(wxMouseEvent &event) {
	int x, y, sel;
	MainFrame * f;
	f = MainFrame::instance;

	x = (event.GetX() / TILE_W);
	y = (event.GetY() / TILE_H);
	
	sel = (y * TILE_COLS) + x;

	if(f->tile_selected == sel) {
		f->tile_selected = -1;
	} else {
		f->tile_selected = sel;
	}

	f->tilepanel->setTile(sel);

	f->display->SetFocus();
	f->display->Refresh();
}

void MainFrame::OnDisplayKeyDown(wxKeyEvent &event) {
	int sel;
	MainFrame * f;
	f = MainFrame::instance;

	sel = f->tile_selected;

	if(sel != -1) {
		switch(event.GetKeyCode()) {
			case WXK_LEFT:
				if (sel % TILE_COLS == 0) {
					sel += TILE_COLS;
				}
				sel--;
				break;
			case WXK_RIGHT:
				sel++;
				if (sel % TILE_COLS == 0) {
					sel -= TILE_COLS;
				}
				break;
			case WXK_UP:
				sel -= TILE_COLS;
				break;
			case WXK_DOWN:
				sel += TILE_COLS;
				break;
			case 'C':
				if(event.GetModifiers() == wxMOD_CONTROL)
					TileCopy();
				break;
			case 'V':
				if(event.GetModifiers() == wxMOD_CONTROL)
					TilePaste();
				break;
		}
		
		if(sel != f->tile_selected) {
			if(sel < 0) sel += 300;
			if(sel >= 300) sel -= 300;

			f->tile_selected = sel;
			f->tilepanel->setTile(sel);

			f->display->SetFocus();
			f->Refresh();
		}
	}
}

void MainFrame::OnDisplayPaint(wxPaintEvent &event) {
	MainFrame * f;
	f = MainFrame::instance;

	wxBufferedPaintDC dc(this);

	wxBrush br(f->bg_color);
	dc.SetBackground(br);
	dc.Clear();

	if(f->level == 0)
		return;
	if(!f->level->loaded)
		return;
	
	if(f->show_background) {
		if(f->background != 0) {
			// Tile the background
			for(int y = 0; y < 480; y += f->background->GetHeight()) {
				for(int x = 0; x < 640; x += f->background->GetWidth()) {
					dc.DrawBitmap(*f->background, x, y, true);
				}
			}
		}
	}

	// Draw props
	if(f->show_props) {
		wxPoint p;
		wxRect rect;
		wxBitmap prop;
		LEVEL_PROP * pr;
		for(unsigned int i = 0; i < f->level->props->size(); i++) {
			pr = f->level->props->at(i);
			p.x = pr->dst.x; p.y = pr->dst.y;
			rect.x = pr->src.x; rect.y = pr->src.y;
			rect.width = pr->src.w; rect.height = pr->src.h;

			prop = f->props->GetSubBitmap(rect);

			dc.DrawBitmap(prop, p, true);
		}
		

		// Draw selected prop
		if(f->prop_selected != -1) {
			wxRect r;
			LEVEL_PROP * pr;

			pr = f->level->props->at(f->prop_selected);
			r.x = pr->dst.x;
			r.y = pr->dst.y;
			r.width = pr->src.w;
			r.height = pr->src.h;

			dc.SetPen(wxPen(wxColor(0x88, 0x88, 0xff), 2));
			dc.SetBrush(*wxTRANSPARENT_BRUSH);
			dc.DrawRectangle(r);
		}
	}

	// Draw tiles
	if(f->show_tiles) {
		wxPoint p;
		wxRect rect;
		wxBitmap tile;
		LEVEL_TILE * ti;
		for(int i = 0; i < TILE_COUNT; i++) {
			ti = &f->level->tile[i];

			if(ti->tile == 0xffff)
				continue;

			p.x = (i % TILE_COLS) * TILE_W;
			p.y = (i / TILE_COLS) * TILE_H;

			rect.x = f->level->tile[i].tile * TILE_W;
			rect.y = 0;
			rect.width = TILE_W;
			rect.height = TILE_H;

			if(ti->hp < 40)
				rect.y += TILE_H;
			if(ti->hp < 20)
				rect.y += TILE_H;

			tile = f->tiles->GetSubBitmap(rect);

			dc.DrawBitmap(tile, p, true);

		}
	}

	// Grid
	if(f->show_grid) {
		dc.SetPen(* wxGREY_PEN);
		for(int y = 0; y < 480; y += TILE_H) {
			dc.DrawLine(0, y, 640, y);
		}
		for(int x = 0; x < 640; x += TILE_W) {
			dc.DrawLine(x, 0, x, 480);
		}
	}

	// Draw selected tile
	if(f->tile_selected != -1) {
		wxRect r;
		r.x = 1 + (f->tile_selected % TILE_COLS) * TILE_W;
		r.y = 1 + (f->tile_selected / TILE_COLS) * TILE_H;
		r.width = TILE_W;
		r.height = TILE_H;

		dc.SetPen(wxPen(wxColor(0x88, 0x88, 0xff), 2));
		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		dc.DrawRectangle(r);
	}

	// Draw player starts
	if(f->show_pstarts) {
		for(int i = 0; i < 4; i++) {
			if(f->level->playerstart[i].player != 0xffff) {
				wxRect r;
				wxBitmap sub;
				wxPoint p;

				r.x = PLAYER_W * f->level->playerstart[i].player;
				r.y = f->level->playerstart[i].facing_right ? 0 : PLAYER_H;
				r.width = PLAYER_W;
				r.height = PLAYER_H;

				sub = f->pstarts->GetSubBitmap(r);

				p.x = (f->level->playerstart[i].x * TILE_W) + ((TILE_W - PLAYER_W) / 2);
				p.y = (f->level->playerstart[i].y * TILE_H) - PLAYER_H;

				dc.DrawBitmap(sub, p);
			}
		}
	}

	// Draw powerups
	if(f->show_powerups) {
		wxPoint p;
		wxRect rect;
		wxBitmap powerup;
		LEVEL_POWERUP * pu;
		for(unsigned int i = 0; i < f->level->powerups->size(); i++) {
			pu = f->level->powerups->at(i);
			p.x = pu->position.x; p.y = pu->position.y;

			rect.x = POWERUP[pu->type].x; rect.y = POWERUP[pu->type].y;
			rect.width = POWERUP[pu->type].w; rect.height = POWERUP[pu->type].h;

			powerup = f->powerups->GetSubBitmap(rect);

			dc.DrawBitmap(powerup, p, true);
		}
		

		// Draw selected prop
		if(f->powerup_selected != -1) {
			wxRect r;

			pu = f->level->powerups->at(f->powerup_selected);
			r.x = pu->position.x;
			r.y = pu->position.y;
			r.width = POWERUP[pu->type].w;
			r.height = POWERUP[pu->type].h;

			dc.SetPen(wxPen(wxColor(0x88, 0x88, 0xff), 2));
			dc.SetBrush(*wxTRANSPARENT_BRUSH);
			dc.DrawRectangle(r);
		}
	}

	// Draw npcs
	if(f->show_npcs) {
		wxPoint p;
		wxRect rect;
		wxBitmap enemy;
		LEVEL_NPC * npc;
		for(unsigned int i = 0; i < f->level->npcs->size(); i++) {
			npc = f->level->npcs->at(i);
			p.x = npc->position.x; p.y = npc->position.y;

			if(npc->move_direction == -1) {
				rect.x = NPC[npc->type].left_x; rect.y = NPC[npc->type].left_y;
			} else {
				rect.x = NPC[npc->type].right_x; rect.y = NPC[npc->type].right_y;
			}
			rect.width = NPC[npc->type].w; rect.height = NPC[npc->type].h;

			enemy = f->npcs->GetSubBitmap(rect);

			dc.DrawBitmap(enemy, p, true);
		}
		

		// Draw selected prop
		if(f->npc_selected != -1) {
			wxRect r;

			npc = f->level->npcs->at(f->npc_selected);
			r.x = npc->position.x;
			r.y = npc->position.y;
			r.width = NPC[npc->type].w;
			r.height = NPC[npc->type].h;

			dc.SetPen(wxPen(wxColor(0x88, 0x88, 0xff), 2));
			dc.SetBrush(*wxTRANSPARENT_BRUSH);
			dc.DrawRectangle(r);
		}
	}
}

void MainFrame::OnNew(wxCommandEvent &event)
{
	if(LevelPromptClose()) {
		LevelNew();
	}
}

void MainFrame::OnOpen(wxCommandEvent &event)
{
	if(LevelPromptClose()) {
		LevelOpen();
	}
}

void MainFrame::OnSave(wxCommandEvent &event)
{
	LevelSave();
}

void MainFrame::OnOptions(wxCommandEvent &event)
{
	LevelOptions();
}

void MainFrame::OnMission(wxCommandEvent &event)
{
	LevelMission();
}

void MainFrame::LevelNew() {
	LevelSettingsDialog dialog(this, wxID_ANY);
	LEVEL_META meta;

	if(dialog.NewLevel() == wxID_OK) {
		LevelClose();

		level = new Level();
		
		level_filename = dialog.filename;
		dialog.GetMeta(meta);
		level->create(level_filename.ToAscii(), meta);
		wxString title;

		title = _("Smash Battle - level editor [");
		title.Append(wxString::FromAscii(level->meta.name)).Append(_("]"));

		this->SetTitle(title);

		LevelLoadBitmaps();

		tile_selected = -1;
		prop_selected = -1;
		powerup_selected = -1;
		npc_selected = -1;
		
		level_modified = false;

		display->Refresh();
	}
}

void MainFrame::LevelOpen() {
	wxString wildcard = _("Level files (*.lvl)|*.lvl|All files (*.*)|*.*");
	wxFileDialog dialog(this, _("Select level"), wxEmptyString, wxEmptyString, wildcard, wxFD_OPEN);
	
	if(dialog.ShowModal() == wxID_OK) {
		LevelClose();

		level = new Level();
		level->load(dialog.GetPath().ToAscii());
		level_filename = dialog.GetPath();
		
		wxString title;

		title = _("Smash Battle - level editor [");
		title.Append(wxString::FromAscii(level->meta.name)).Append(_("]"));

		this->SetTitle(title);

		LevelLoadBitmaps();

		tile_selected = -1;
		prop_selected = -1;
		powerup_selected = -1;
		npc_selected = -1;
		
		level_modified = false;

		display->Refresh();
	}
}

bool MainFrame::LevelPromptClose() {
	int result;

	if(level == NULL)
		return true;
	
	if(!level_modified)
		return true;

	result = wxMessageBox(_("Do you want to save the current level?"), _("Open level"), wxYES_NO | wxCANCEL | wxICON_QUESTION, this);
	if(result == wxCANCEL)
		return false;
	if(result == wxYES)
		LevelSave();

	return true;
}

void MainFrame::LevelClose() {
	this->SetTitle(_("Smash Battle - level editor"));

	level_filename = wxEmptyString;

	tilepanel->setLevel(0, 0);
	tilepanel->setTile(-1);

	propspanel->setLevel(0);
	powerupspanel->setLevel(0);
	npcspanel->setLevel(0);

	delete level;
	level = 0;

	bg_color = wxColour(0, 0, 0);

	delete background;
	background = 0;

	delete tiles;
	tiles = 0;

	delete props;
	props = 0;

	display->Refresh();
}

void MainFrame::LevelSave() {
	if(level == 0)
		return;

 	level->save(level_filename.ToAscii());
	level_modified = false;
}

void MainFrame::LevelOptions() {
	if(level == 0)
		return;

	LevelSettingsDialog dialog(this, wxID_ANY);
	LEVEL_META meta;

	if(dialog.EditLevel(level_filename, level->meta) == wxID_OK) {
		dialog.GetMeta(meta);

		memcpy(&level->meta, &meta, sizeof(LEVEL_META));
		
		wxString title;

		title = _("Smash Battle - level editor [");
		title.Append(wxString::FromAscii(level->meta.name)).Append(_("]"));

		this->SetTitle(title);
				
		LevelLoadBitmaps();

		level_modified = true;
		display->Refresh();
	}
}

void MainFrame::LevelMission() {
	if(level == 0)
		return;

	LevelMissionDialog dialog(this, wxID_ANY);
	LEVEL_MISSION mission;

	if(dialog.EditLevel(level->mission) == wxID_OK) {
		dialog.GetMission(mission);
		
		memcpy(&level->mission, &mission, sizeof(LEVEL_MISSION));

		level_modified = true;
		display->Refresh();
	}
}

void MainFrame::LevelLoadBitmaps() {
	wxString path;
	wxString bg_file_full;
	wxString tiles_file_full;
	wxString props_file_full;
	wxColour mask_colour;
	char sep;
		
	bg_color = wxColour((level->meta.background_color >> 16) & 0xff, (level->meta.background_color >> 8) & 0xff, level->meta.background_color & 0xff);

	mask_colour = wxColour(0, 0xff, 0xff);

	sep = wxFileName::GetPathSeparator();

	path = level_filename.BeforeLast(sep).BeforeLast(sep);

	if(level->meta.filename_background[0] == 0) {
		delete background;
		background = 0;
	} else {
		bg_file_full = _("");
		bg_file_full.Append(path).Append(sep).Append(_("gfx")).Append(sep);
		bg_file_full.Append(wxString::FromAscii(level->meta.filename_background));

		if(background != 0) {
			delete background;
			background = 0;
		}

		background = new wxBitmap();
		if(!background->LoadFile(bg_file_full, wxBITMAP_TYPE_BMP)) {
			// Error: bmp could not be loaded
			delete background;
			background = 0;
			wxMessageBox(_("Background file could not be loaded"), _("Error"), wxICON_EXCLAMATION);
		}
	}

	tiles_file_full = _("");
	tiles_file_full.Append(path).Append(sep).Append(_("gfx")).Append(sep);
	tiles_file_full.Append(wxString::FromAscii(level->meta.filename_tiles));

	if(tiles != 0) {
		delete tiles;
		tiles = 0;
	}

	tiles = new wxBitmap();
	if(!tiles->LoadFile(tiles_file_full, wxBITMAP_TYPE_BMP)) {
		// Error: bmp could not be loaded
		delete tiles;
		tiles = 0;
		wxMessageBox(_("Tiles file could not be loaded"), _("Error"), wxICON_EXCLAMATION);
	} else {
		mask_tiles = new wxMask(*tiles, mask_colour);
		tiles->SetMask(mask_tiles);
	}

	if(level->meta.filename_props[0] == 0) {
		delete props;
		props = 0;
	} else {
		props_file_full = _("");
		props_file_full.Append(path).Append(sep).Append(_("gfx")).Append(sep);
		props_file_full.Append(wxString::FromAscii(level->meta.filename_props));

		if(props != 0) {
			delete props;
			props = 0;
		}

		props = new wxBitmap();
		if(!props->LoadFile(props_file_full, wxBITMAP_TYPE_BMP)) {
			// Error: bmp could not be loaded
			delete props;
			props = 0;
			wxMessageBox(_("Props file could not be loaded"), _("Error"), wxICON_EXCLAMATION);
		} else {
			mask_props = new wxMask(*props, mask_colour);
			props->SetMask(mask_props);
		}
	}

	tilepanel->setLevel(level, tiles);
	tilepanel->setTile(-1);

	propspanel->setLevel(level);
	powerupspanel->setLevel(level);
	npcspanel->setLevel(level);
}

void MainFrame::TileCopy() {
	memcpy(&instance->tile_copy, &instance->level->tile[instance->tile_selected], sizeof(LEVEL_TILE));
	instance->tile_copied = true;
	instance->tilepanel->enablePasteButton();
}

void MainFrame::TilePaste() {
	if(!instance->tile_copied) return;
	memcpy(&instance->level->tile[instance->tile_selected], &instance->tile_copy, sizeof(LEVEL_TILE));
	instance->tilepanel->setTile(instance->tile_selected);
	instance->Refresh();
	level_modified = true;
}

void MainFrame::OnToggleBackground(wxCommandEvent &event) {
	MainFrame::instance->show_background = event.IsChecked();
	MainFrame::instance->display->Refresh();
}

void MainFrame::OnToggleTiles(wxCommandEvent &event) {
	MainFrame::instance->show_tiles = event.IsChecked();
	MainFrame::instance->display->Refresh();
}

void MainFrame::OnToggleProps(wxCommandEvent &event) {
	MainFrame::instance->show_props = event.IsChecked();
	MainFrame::instance->display->Refresh();
}

void MainFrame::OnTogglePStarts(wxCommandEvent &event) {
	MainFrame::instance->show_pstarts = event.IsChecked();
	MainFrame::instance->display->Refresh();
}

void MainFrame::OnTogglePowerups(wxCommandEvent &event) {
	MainFrame::instance->show_powerups = event.IsChecked();
	MainFrame::instance->display->Refresh();
}

void MainFrame::OnToggleNPCs(wxCommandEvent &event) {
	MainFrame::instance->show_npcs = event.IsChecked();
	MainFrame::instance->display->Refresh();
}
