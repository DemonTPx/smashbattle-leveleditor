#include "wx/wxprec.h"

#include "wx/filename.h"
#include "wx/dcbuffer.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "LevelSettingsDialog.h"

#include "MainFrame.h"

#define TILE_W 32
#define TILE_H 32
#define TILE_COLS 20
#define TILE_ROWS 15
#define TILE_COUNT (TILE_COLS * TILE_ROWS)

#define PLAYER_W 22
#define PLAYER_H 44

MainFrame * MainFrame::instance = 0;

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame((wxFrame *) NULL, -1, title, pos, size) 
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

	tile_selected = -1;
	
	memset(&tile_copy, 0, sizeof(LEVEL_TILE));
	tile_copied = false;
}

MainFrame::~MainFrame() {
	if(level != 0) delete level;

	if(background != 0) delete background;
	if(tiles != 0) delete tiles;
	if(props != 0) delete props;
	
	if(pstarts != 0) delete pstarts;
}

void MainFrame::InitializeComponents()
{
	SetWindowStyle(wxCAPTION | wxCLOSE_BOX | wxSYSTEM_MENU);
	SetBackgroundColour(wxColour(0xeeeeee));
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);

	toolbar = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(840, 30));

	btnNew = new wxButton(toolbar, ID_New, _("&New"), wxPoint(0, 0), wxSize(80, 30));
	btnOpen = new wxButton(toolbar, ID_Open, _("&Open"), wxPoint(80, 0), wxSize(80, 30));
	btnSave = new wxButton(toolbar, ID_Save, _("&Save"), wxPoint(160, 0), wxSize(80, 30));
	btnOptions = new wxButton(toolbar, ID_Options, _("&Options"), wxPoint(260, 0), wxSize(80, 30));

	btnBackground = new wxToggleButton(toolbar, ID_ToggleBackground, _("&Background"), wxPoint(400, 0), wxSize(80, 30));
	btnBackground->SetValue(true);
	btnTiles = new wxToggleButton(toolbar, ID_ToggleTiles, _("&Tiles"), wxPoint(480, 0), wxSize(80, 30));
	btnTiles->SetValue(true);
	btnProps = new wxToggleButton(toolbar, ID_ToggleProps, _("&Props"), wxPoint(560, 0), wxSize(80, 30));
	btnProps->SetValue(true);
	btnPStarts = new wxToggleButton(toolbar, ID_TogglePStarts, _("Player &starts"), wxPoint(640, 0), wxSize(80, 30));

	display = new wxPanel(this, wxID_ANY, wxPoint(0, 30), wxSize(640, 480));
	display->Connect(wxEVT_PAINT, wxPaintEventHandler(MainFrame::OnDisplayPaint));
	display->Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(MainFrame::OnDisplayErase));
	display->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(MainFrame::OnDisplayMouseDown));
	display->Connect(wxEVT_KEY_DOWN, wxCharEventHandler(MainFrame::OnDisplayKeyDown));
	display->SetFocus();

	tilepanel = new TilePanel(this, wxID_ANY, wxPoint(640, 30), wxSize(200, 480));

	pstarts = new wxBitmap(_("pstarts.bmp"), wxBITMAP_TYPE_BMP);
}

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_BUTTON(ID_New, MainFrame::OnNew)
	EVT_BUTTON(ID_Open, MainFrame::OnOpen)
	EVT_BUTTON(ID_Save, MainFrame::OnSave)
	EVT_BUTTON(ID_Options, MainFrame::OnOptions)
	EVT_TOGGLEBUTTON(ID_ToggleBackground, MainFrame::OnToggleBackground)
	EVT_TOGGLEBUTTON(ID_ToggleTiles, MainFrame::OnToggleTiles)
	EVT_TOGGLEBUTTON(ID_ToggleProps, MainFrame::OnToggleProps)
	EVT_TOGGLEBUTTON(ID_TogglePStarts, MainFrame::OnTogglePStarts)
	EVT_CLOSE(MainFrame::OnExit)
	EVT_CHAR_HOOK(MainFrame::OnDisplayKeyDown)
END_EVENT_TABLE()

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
	int x, y, sel;
	MainFrame * f;
	f = MainFrame::instance;

	sel = f->tile_selected;

	switch(event.GetKeyCode()) {
		case WXK_LEFT:
			sel--;
			break;
		case WXK_RIGHT:
			sel++;
			break;
		case WXK_UP:
			sel -= 20;
			break;
		case WXK_DOWN:
			sel += 20;
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


void MainFrame::OnDisplayErase(wxEraseEvent &event) {
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

			if(!ti->indestructible) {
				if(ti->hp < 40)
					rect.y += TILE_H;
				if(ti->hp < 20)
					rect.y += TILE_H;
			}

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

void MainFrame::LevelNew() {
	LevelSettingsDialog dialog(this, wxID_ANY);
	LEVEL_HEADER hdr;

	if(dialog.NewLevel() == wxID_OK) {
		LevelClose();

		level = new Level();
		
		level_filename = dialog.filename;
		dialog.GetHeader(hdr);
		level->create(level_filename.ToAscii(), hdr);
		wxString title;

		title = _("Smash Battle - level editor [");
		title.Append(wxString::FromAscii(level->header.name)).Append(_("]"));

		this->SetTitle(title);

		LevelLoadBitmaps();

		tile_selected = -1;
		
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
		title.Append(wxString::FromAscii(level->header.name)).Append(_("]"));

		this->SetTitle(title);

		LevelLoadBitmaps();

		tile_selected = -1;
		
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
	LevelSettingsDialog dialog(this, wxID_ANY);
	LEVEL_HEADER hdr;

	if(dialog.EditLevel(level_filename, level->header) == wxID_OK) {
		dialog.GetHeader(hdr);

		memcpy(&level->header, &hdr, sizeof(LEVEL_HEADER));
				
		LevelLoadBitmaps();

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
		
	bg_color = wxColour((level->header.background_color >> 16) & 0xff, (level->header.background_color >> 8) & 0xff, level->header.background_color & 0xff);

	mask_colour = wxColour(0, 0xff, 0xff);

	sep = wxFileName::GetPathSeparator();

	path = level_filename.BeforeLast(sep).BeforeLast(sep);

	if(level->header.filename_background[0] == 0) {
		delete background;
		background = 0;
	} else {
		bg_file_full = _("");
		bg_file_full.Append(path).Append(sep).Append(_("gfx")).Append(sep);
		bg_file_full.Append(wxString::FromAscii(level->header.filename_background));

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
	tiles_file_full.Append(wxString::FromAscii(level->header.filename_tiles));

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

	if(level->header.filename_props[0] == 0) {
		delete props;
		props = 0;
	} else {
		props_file_full = _("");
		props_file_full.Append(path).Append(sep).Append(_("gfx")).Append(sep);
		props_file_full.Append(wxString::FromAscii(level->header.filename_props));

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
}

void MainFrame::TileCopy() {
	memcpy(&instance->tile_copy, &instance->level->tile[instance->tile_selected], sizeof(LEVEL_TILE));
	tile_copied = true;
}

void MainFrame::TilePaste() {
	if(!tile_copied) return;
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

