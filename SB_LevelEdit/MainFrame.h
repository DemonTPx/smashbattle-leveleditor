#pragma once

#include "Level.h"

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
	
	void InitializeComponents();

protected:
	void OnExit(wxCommandEvent &event);
	void OnOpen(wxCommandEvent &event);
    void OnDisplayPaint(wxPaintEvent& event);

	DECLARE_EVENT_TABLE()

	wxButton * btnOpen;
	wxPanel * toolbar;
	wxPanel * display;

	Level * level;
	static wxBitmap * background;
};

enum {
	ID_New = 1,
	ID_Open,
	ID_Save,
};
