#include "wx/wxprec.h"

#include "wx/filename.h"
#include "wx/dcbuffer.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "MainFrame.h"

#define TILE_W 32
#define TILE_H 32
#define TILE_COLS 20
#define TILE_ROWS 15
#define TILE_COUNT (TILE_COLS * TILE_ROWS)

MainFrame * MainFrame::instance = 0;

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame((wxFrame *) NULL, -1, title, pos, size) 
{
	instance = this;
	InitializeComponents();

	level = 0;

	bg_color = wxColour(0, 0, 0);

	background = 0;
	tiles = 0;
	props = 0;

	show_background = true;
	show_tiles = true;
	show_props = true;

	tile_selected = -1;
}

MainFrame::~MainFrame() {
	if(level != 0) delete level;

	if(background != 0) delete background;
	if(tiles != 0) delete tiles;
	if(props != 0) delete props;
}

void MainFrame::InitializeComponents()
{
	SetWindowStyle(wxCAPTION | wxCLOSE_BOX | wxSYSTEM_MENU);
	SetBackgroundColour(wxColour(0xeeeeee));
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);

	toolbar = new wxPanel(this, wxID_ANY, wxPoint(0, 0), wxSize(840, 30));

	btnOpen = new wxButton(toolbar, ID_Open, _("&Open"), wxPoint(0, 0), wxSize(80, 30));
	btnClose = new wxButton(toolbar, ID_Close, _("&Close"), wxPoint(80, 0), wxSize(80, 30));

	btnBackground = new wxToggleButton(toolbar, ID_ToggleBackground, _("&Background"), wxPoint(200, 0), wxSize(80, 30));
	btnBackground->SetValue(true);
	btnTiles = new wxToggleButton(toolbar, ID_ToggleTiles, _("&Tiles"), wxPoint(280, 0), wxSize(80, 30));
	btnTiles->SetValue(true);
	btnProps = new wxToggleButton(toolbar, ID_ToggleProps, _("&Props"), wxPoint(360, 0), wxSize(80, 30));
	btnProps->SetValue(true);

	display = new wxPanel(this, wxID_ANY, wxPoint(0, 30), wxSize(640, 480));
	display->Connect(wxEVT_PAINT, wxPaintEventHandler(MainFrame::OnDisplayPaint));
	display->Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(MainFrame::OnDisplayErase));
	display->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(MainFrame::OnDisplayMouseDown));

	tilepanel = new TilePanel(this, wxID_ANY, wxPoint(640, 30), wxSize(200, 480));
}

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
//	EVT_BUTTON(BUTTON_btn, MainFrame::OnExit)
//	EVT_BUTTON(BUTTON_btn, MainFrame::OnExit)
	EVT_BUTTON(ID_Open, MainFrame::OnOpen)
	EVT_BUTTON(ID_Close, MainFrame::OnClose)
	EVT_TOGGLEBUTTON(ID_ToggleBackground, MainFrame::OnToggleBackground)
	EVT_TOGGLEBUTTON(ID_ToggleTiles, MainFrame::OnToggleTiles)
	EVT_TOGGLEBUTTON(ID_ToggleProps, MainFrame::OnToggleProps)
END_EVENT_TABLE()

void MainFrame::OnExit(wxCommandEvent &event)
{
	Close(TRUE);
}

void MainFrame::OnDisplayMouseDown(wxMouseEvent &event) {
	int x, y, sel;
	MainFrame * f;
	f = MainFrame::instance;

	f->tilepanel->saveTile();

	x = (event.GetX() / TILE_W);
	y = (event.GetY() / TILE_H);
	
	sel = (y * TILE_COLS) + x;

	if(f->tile_selected == sel) {
		f->tile_selected = -1;
	} else {
		f->tile_selected = sel;
	}

	f->tilepanel->setTile(sel);

	f->display->Refresh();
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

			if(ti->hp < 40)
				rect.y += TILE_H;
			if(ti->hp < 20)
				rect.y += TILE_H;

			tile = f->tiles->GetSubBitmap(rect);

			dc.DrawBitmap(tile, p, true);

		}
	}

	// Grid
	dc.SetPen(* wxGREY_PEN);
	for(int y = 0; y < 480; y += TILE_H) {
		dc.DrawLine(0, y, 640, y);
	}
	for(int x = 0; x < 640; x += TILE_W) {
		dc.DrawLine(x, 0, x, 480);
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
}

void MainFrame::OnOpen(wxCommandEvent &event)
{
	wxFileDialog * dialog;
	wxString wildcard = _("Level files (*.lvl)|*.lvl|All files (*.*)|*.*");
	
	dialog = new wxFileDialog(this, _("Select level"), wxEmptyString,
		wxEmptyString, wildcard, wxFD_OPEN);
	if(dialog->ShowModal() == wxID_OK) {
		OnClose(event);

		level = new Level();
		level->load(dialog->GetPath().ToAscii());
		
		wxString title;

		title = _("Smash Battle level editor [");
		title.Append(wxString::FromAscii(level->header.name)).Append(_("]"));

		this->SetTitle(title);
		
		bg_color = wxColour((unsigned long)level->header.background_color);

		wxString path;
		wxString bg_file_full;
		wxString tiles_file_full;
		wxString props_file_full;
		wxColour mask_colour;
		char sep;

		mask_colour = wxColour(0, 0xff, 0xff);

		sep = wxFileName::GetPathSeparator();

		path = dialog->GetPath().BeforeLast(sep).BeforeLast(sep);
		bg_file_full = _("");
		bg_file_full.Append(path).Append(sep).Append(_("gfx")).Append(sep);
		bg_file_full.Append(wxString::FromAscii(level->header.filename_background));

		background = new wxBitmap();
		if(!background->LoadFile(bg_file_full, wxBITMAP_TYPE_BMP)) {
			// Error: bmp could not be loaded
		}

		tiles_file_full = _("");
		tiles_file_full.Append(path).Append(sep).Append(_("gfx")).Append(sep);
		tiles_file_full.Append(wxString::FromAscii(level->header.filename_tiles));
		
		tiles = new wxBitmap();
		if(!tiles->LoadFile(tiles_file_full, wxBITMAP_TYPE_BMP)) {
			// Error: bmp could not be loaded
		}
		mask_tiles = new wxMask(*tiles, mask_colour);
		tiles->SetMask(mask_tiles);

		props_file_full = _("");
		props_file_full.Append(path).Append(sep).Append(_("gfx")).Append(sep);
		props_file_full.Append(wxString::FromAscii(level->header.filename_props));
		
		props = new wxBitmap();
		if(!props->LoadFile(props_file_full, wxBITMAP_TYPE_BMP)) {
			// Error: bmp could not be loaded
		}
		mask_props = new wxMask(*props, mask_colour);
		props->SetMask(mask_props);

		tilepanel->setLevel(level, tiles);
		tilepanel->setTile(-1);

		display->Refresh();
	}

	delete dialog;
}

void MainFrame::OnClose(wxCommandEvent &event)
{
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
