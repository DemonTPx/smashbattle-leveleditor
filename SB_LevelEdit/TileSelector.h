#pragma once

#include "wx/scrolwin.h"

#include <vector>

class TileSelector : public wxScrolledWindow {
public:
	TileSelector(wxWindow* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxVSCROLL, const wxString& name = _("scrolledWindow"));
	
	void setTiles(wxBitmap * t);
	
	void SetSelected(int selected);
	int GetSelected();
protected:
	DECLARE_EVENT_TABLE()

	void OnPaint(wxPaintEvent &event);
	void OnClick(wxMouseEvent &event);
	void OnSize(wxSizeEvent &event);

	wxBitmap * tiles;
	int tiles_max;
	int tiles_per_line;

	int selected;
};
