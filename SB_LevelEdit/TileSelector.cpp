#include "wx/wxprec.h"

#include "wx/dcbuffer.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "TilePanel.h"
#include "MainFrame.h"
#include "TileSelector.h"

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

		p.x = (i % 5) * TILE_W;
		p.y = (i / 5) * TILE_H;

		tile = tiles->GetSubBitmap(rect);
		dc.DrawBitmap(tile, p, true);
	}
}

void TileSelector::OnClick(wxMouseEvent &event) {
	selected = ((event.GetY() / TILE_H) * 5) + (event.GetX() / TILE_W);
}
