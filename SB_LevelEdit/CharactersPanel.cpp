#include <wx/dcbuffer.h>
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "wx/event.h"

#include "CharactersPanel.h"

CharactersPanel * CharactersPanel::instance = 0;

CharactersPanel::CharactersPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) {
    instance = this;
    Create(parent, id, pos, size, style, name);

    frame = 0;
    bg_color = wxColour(0x44, 0x44, 0x44);
    character_loaded = false;

    InitializeComponents();
    SetCharacterClips();
}

void CharactersPanel::InitializeComponents() {
    wxBoxSizer * sizer = new wxBoxSizer(wxVERTICAL);

    display = new wxPanel(this);
    display->Connect(wxEVT_PAINT, wxPaintEventHandler(CharactersPanel::OnDisplayPaint));
    sizer->Add(display, 1, wxEXPAND | wxALL, 5);

    wxBoxSizer * buttonsSizer = new wxBoxSizer(wxHORIZONTAL);

    btnOpen = new wxButton(this, ID_CharactersPanel_Open, _("Open"));
    buttonsSizer->Add(btnOpen, 1, wxEXPAND);

    clrBackground = new wxColourPickerCtrl(this, ID_CharactersPanel_Color, bg_color);
    buttonsSizer->Add(clrBackground, 1, wxEXPAND);

    sizer->Add(buttonsSizer, 0, wxEXPAND | wxALL, 5);

    SetSizerAndFit(sizer);

    timer = new wxTimer(this, ID_CharactersPanel_Timer);
    timer->Start(33);
}

BEGIN_EVENT_TABLE(CharactersPanel, wxPanel)
    EVT_BUTTON(ID_CharactersPanel_Open, CharactersPanel::OnOpen)
    EVT_TIMER(ID_CharactersPanel_Timer, CharactersPanel::OnTimer)
    EVT_COLOURPICKER_CHANGED(ID_CharactersPanel_Color, CharactersPanel::OnColorChanged)
END_EVENT_TABLE()

void CharactersPanel::OnDisplayPaint(wxPaintEvent &event) {
    CharactersPanel * panel;
    panel = CharactersPanel::instance;

    wxBufferedPaintDC dc(this);

    wxBrush brush(panel->bg_color);
    dc.SetBackground(brush);
    dc.Clear();

    if ( ! panel->character_loaded) {
        return;
    }

    frame = (frame + 1) % 3600;

    static int clips1[] = {
            SPR_R, SPR_R_WALK1, SPR_R_WALK2, SPR_R_WALK3, SPR_R_RUN1, SPR_R_RUN2, SPR_R_RUN3,
            SPR_L_BRAKE, SPR_R_JUMP, SPR_R_DUCK, SPR_AVATAR
    };
    static int clips2[] = {
            SPR_L, SPR_L_WALK1, SPR_L_WALK2, SPR_L_WALK3, SPR_L_RUN1, SPR_L_RUN2, SPR_L_RUN3,
            SPR_R_BRAKE, SPR_L_JUMP, SPR_L_DUCK, SPR_AVATAR_SELECTED
    };

    int x, y;

    x = 10;
    y = 10;
    for (int i = 0; i < 11; i++) {
        dc.DrawBitmap(panel->character->GetSubBitmap(panel->clip[clips1[i]]), x, y, true);
        x += 30;
    }

    x = 10;
    y = 70;
    for (int i = 0; i < 11; i++) {
        dc.DrawBitmap(panel->character->GetSubBitmap(panel->clip[clips2[i]]), x, y, true);
        x += 30;
    }

    dc.DrawBitmap(panel->character->GetSubBitmap(panel->clip[SPR_AVATAR + ((frame / 15) % 2)]), 380, 10, true);

    static int clips3[] = {SPR_R, SPR_R_DUCK, SPR_R, SPR_R_JUMP, SPR_R, SPR_R_BRAKE};
    static int clips4[] = {SPR_L, SPR_L_DUCK, SPR_L, SPR_L_JUMP, SPR_L, SPR_L_BRAKE};

    dc.DrawBitmap(panel->character->GetSubBitmap(panel->clip[clips3[(frame / 6) % 6]]), 370, 70, true);
    dc.DrawBitmap(panel->character->GetSubBitmap(panel->clip[clips4[(frame / 6) % 6]]), 400, 70, true);

    x = 10 + ((frame * 6) % 340);
    y = 130;
    dc.DrawBitmap(panel->character->GetSubBitmap(panel->clip[SPR_R_WALK1 + ((frame / 3) % 3)]), x, y, true);
    dc.DrawBitmap(panel->character->GetSubBitmap(panel->clip[SPR_R_WALK1 + ((frame / 3) % 3)]), 380, y, true);

    x = 350 - ((frame * 6) % 340);
    y = 190;
    dc.DrawBitmap(panel->character->GetSubBitmap(panel->clip[SPR_L_WALK1 + ((frame / 3) % 3)]), x, y, true);
    dc.DrawBitmap(panel->character->GetSubBitmap(panel->clip[SPR_L_WALK1 + ((frame / 3) % 3)]), 380, y, true);

    x = 10 + ((frame * 10) % 340);
    y = 250;
    dc.DrawBitmap(panel->character->GetSubBitmap(panel->clip[SPR_R_RUN1 + ((frame / 3) % 3)]), x, y, true);
    dc.DrawBitmap(panel->character->GetSubBitmap(panel->clip[SPR_R_RUN1 + ((frame / 3) % 3)]), 380, y, true);

    x = 350 - ((frame * 10) % 340);
    y = 310;
    dc.DrawBitmap(panel->character->GetSubBitmap(panel->clip[SPR_L_RUN1 + ((frame / 3) % 3)]), x, y, true);
    dc.DrawBitmap(panel->character->GetSubBitmap(panel->clip[SPR_L_RUN1 + ((frame / 3) % 3)]), 380, y, true);
}

void CharactersPanel::OnTimer(wxTimerEvent &event) {
    this->Refresh();
}

void CharactersPanel::OnColorChanged(wxColourPickerEvent &event) {
    wxColour c = event.GetColour();
    bg_color.Set(c.Red(), c.Green(), c.Blue());
}

void CharactersPanel::OnOpen(wxCommandEvent &event) {
    CharacterOpen();
}

void CharactersPanel::CharacterOpen() {
    wxString wildcard = _("Character sprite files (*.bmp)|*.bmp|All files (*.*)|*.*");
    wxFileDialog dialog(this, _("Select character sprite file"), wxEmptyString, wxEmptyString, wildcard, wxFD_OPEN);

    if (dialog.ShowModal() != wxID_OK) {
        return;
    }

    character_loaded = false;

    wxColour mask_color;

    mask_color = wxColour(0, 0xff, 0xff);

    CleanupBitmaps();

    character = new wxBitmap();
    if ( ! character->LoadFile(dialog.GetPath(), wxBITMAP_TYPE_BMP)) {
        CleanupBitmaps();
        wxMessageBox(_("Character file could not be loaded"), _("Error"), wxICON_EXCLAMATION);
        return;
    }

    character->SetMask(new wxMask(*character, mask_color));

    character_loaded = true;
    display->Refresh();
}

void CharactersPanel::CleanupBitmaps() {
    if (character != 0) {
        delete character;
        character = 0;
    }
}

void CharactersPanel::SetCharacterClips() {
    int row_width = PLAYER_W * PLAYER_SURF_COLS;
    for (int i = 0; i < PLAYER_SURF_COUNT; i++) {
        clip[i] = wxRect(
                (i * PLAYER_W) % row_width,
                (i / PLAYER_SURF_COLS) * PLAYER_H,
                PLAYER_W,
                PLAYER_H
        );
    }

    clip[SPR_AVATAR] = wxRect(row_width, 0, PLAYER_W * 2, PLAYER_H);
    clip[SPR_AVATAR_SELECTED] = wxRect(row_width, PLAYER_H, PLAYER_W * 2, PLAYER_H);
}
