#pragma once

#include "wx/clrpicker.h"

#define PLAYER_W 22
#define PLAYER_H 44

#define SPR_R       0
#define SPR_R_WALK1 1
#define SPR_R_WALK2 2
#define SPR_R_WALK3 3
#define SPR_R_RUN1  4
#define SPR_R_RUN2  5
#define SPR_R_RUN3  6
#define SPR_L_BRAKE 7
#define SPR_R_JUMP  8
#define SPR_R_DUCK  9
#define SPR_L       10
#define SPR_L_WALK1 11
#define SPR_L_WALK2 12
#define SPR_L_WALK3 13
#define SPR_L_RUN1  14
#define SPR_L_RUN2  15
#define SPR_L_RUN3  16
#define SPR_R_BRAKE 17
#define SPR_L_JUMP  18
#define SPR_L_DUCK  19

#define SPR_COUNT   22

#define SPR_AVATAR  20
#define SPR_AVATAR_SELECTED 21

#define PLAYER_SURF_COLS 10
#define PLAYER_SURF_COUNT 20

class CharactersPanel : public wxPanel {
public:
    CharactersPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = _("panel"));

    static CharactersPanel * instance;

    void InitializeComponents();
protected:
    DECLARE_EVENT_TABLE()

    wxPanel * display;
    wxButton * btnOpen;
    wxColourPickerCtrl * clrBackground;

    wxTimer * timer;

    void OnDisplayPaint(wxPaintEvent &event);

    void OnTimer(wxTimerEvent &event);
    void OnColorChanged(wxColourPickerEvent &event);
    void OnOpen(wxCommandEvent &event);

    void CharacterOpen();
    void CleanupBitmaps();

    void SetCharacterClips();

    wxColour bg_color;

    bool character_loaded;

    wxBitmap * character;

    wxRect clip[SPR_COUNT];

    int frame;
};

enum {
    ID_CharactersPanel_Open,
    ID_CharactersPanel_Color,
    ID_CharactersPanel_Timer
};
