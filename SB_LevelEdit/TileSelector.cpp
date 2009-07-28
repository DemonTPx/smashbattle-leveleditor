#include "wx/wxprec.h"

#include "wx/dcbuffer.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "TilePanel.h"
#include "MainFrame.h"
#include "TileSelector.h"

#define TILES_PER_LINE 5
#define TILE_SPACING 2
#define BORDER 5

TileSelector::TileSelector(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
{
	Create(parent, id, pos, size, style, name);
	SetBackgroundColour(*wxBLACK);

	tiles_max = 0;
	tiles = 0;
}

void TileSelector::setTiles(wxBitmap * t) {
	tiles = t;
	tiles_max = t->GetWidth() / TILE_W;

	Refresh();
}

void TileSelector::SetSelected(int s) {
	if(s > tiles_max)
		selected = -1;
	selected = s;

	Refresh();
}

int TileSelector::GetSelected() {
	return selected;
}

BEGIN_EVENT_TABLE(TileSelector, wxScrolledWindow)
	EVT_PAINT(TileSelector::OnPaint)
	EVT_LEFT_DOWN(TileSelector::OnClick)
END_EVENT_TABLE()

void TileSelector::OnPaint(wxPaintEvent &event) {
	wxPaintDC dc(this);

	if(tiles == 0)
		return;

	wxBitmap tile;
	wxRect rect;

	wxPoint p;

	for(int i = 0; i < tiles_max; i++) {
		rect.x = i * TILE_W;
		rect.y = 0;
		rect.width = TILE_W;
		rect.height = TILE_H;

		p.x = BORDER + (i % TILES_PER_LINE) * (TILE_W + (TILE_SPACING * 2));
		p.y = BORDER + (i / TILES_PER_LINE) * (TILE_H + (TILE_SPACING * 2));

		if(selected == i) {
			wxSize size;
			size.x = TILE_W + (TILE_SPACING * 2);
			size.y = TILE_H + (TILE_SPACING * 2);
			dc.SetPen(wxPen(wxColor(0x88, 0x88, 0xff), 2));
			dc.SetBrush(*wxTRANSPARENT_BRUSH);
			dc.DrawRectangle(p, size);
		}

		p.x += TILE_SPACING;
		p.y += TILE_SPACING;

		tile = tiles->GetSubBitmap(rect);
		dc.DrawBitmap(tile, p, true);
	}
}

void TileSelector::OnClick(wxMouseEvent &event) {
	int x, y;

	x = event.GetX();
	y = event.GetY();

	if(x < BORDER || x > (BORDER + ((TILE_W + (TILE_SPACING * 2)) * TILES_PER_LINE))) {
		selected = -1;
	} else if(y < BORDER) {
		selected = -1;
	} else {
		x = (x - BORDER) / (TILE_W + (TILE_SPACING * 2));
		y = (y - BORDER) / (TILE_H + (TILE_SPACING * 2));

		selected = (y * TILES_PER_LINE) + x;

		if(selected >= tiles_max) {
			selected = -1;
		}
	}

	Refresh();
}
