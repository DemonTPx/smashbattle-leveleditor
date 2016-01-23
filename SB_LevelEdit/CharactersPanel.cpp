#include <wx/dcbuffer.h>
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#	include "wx/wx.h"
#endif

#include "wx/event.h"
#include "wx/rawbmp.h"

#include "CharactersPanel.h"

CharactersPanel * CharactersPanel::instance = 0;

const int CharactersPanel::SUIT_ORIGINAL[5] = {
        0x486d9f, // shirt
        0x395479, // shirt dark
        0x6d9360, // pants
        0x5e7855, // pants dark
        0x4c5f45 // pants darker
};

const int CharactersPanel::SUIT_REPLACE[4][5] = {
        //  shirt      dark     pants      dark    darker
        {0xb45454, 0x993b3b, 0x6d9360, 0x5e7855, 0x4c5f45},
        {0x486d9f, 0x395479, 0x6d9360, 0x5e7855, 0x4c5f45},
        {0x84c584, 0x71af71, 0x6d9360, 0x5e7855, 0x4c5f45},
        {0xf0dd60, 0xd5c660, 0x6d9360, 0x5e7855, 0x4c5f45}
};

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
    
    wxBoxSizer * playerButtonsSizer = new wxBoxSizer(wxHORIZONTAL);
    
    btnPlayerDefault = new wxButton(this, ID_CharactersPanel_PlayerDefault, _("Default"));
    btnPlayer1 = new wxButton(this, ID_CharactersPanel_Player1, _("Player 1"));
    btnPlayer2 = new wxButton(this, ID_CharactersPanel_Player2, _("Player 2"));
    btnPlayer3 = new wxButton(this, ID_CharactersPanel_Player3, _("Player 3"));
    btnPlayer4 = new wxButton(this, ID_CharactersPanel_Player4, _("Player 4"));

    playerButtonsSizer->Add(btnPlayerDefault, 1, wxEXPAND);
    playerButtonsSizer->Add(btnPlayer1, 1, wxEXPAND);
    playerButtonsSizer->Add(btnPlayer2, 1, wxEXPAND);
    playerButtonsSizer->Add(btnPlayer3, 1, wxEXPAND);
    playerButtonsSizer->Add(btnPlayer4, 1, wxEXPAND);

    sizer->Add(buttonsSizer, 0, wxEXPAND | wxALL, 5);
    sizer->Add(playerButtonsSizer, 0, wxEXPAND | wxALL, 5);

    SetSizerAndFit(sizer);

    timer = new wxTimer(this, ID_CharactersPanel_Timer);
    timer->Start(33);
}

BEGIN_EVENT_TABLE(CharactersPanel, wxPanel)
    EVT_BUTTON(ID_CharactersPanel_Open, CharactersPanel::OnOpen)
    EVT_TIMER(ID_CharactersPanel_Timer, CharactersPanel::OnTimer)
    EVT_COLOURPICKER_CHANGED(ID_CharactersPanel_Color, CharactersPanel::OnColorChanged)
    EVT_BUTTON(ID_CharactersPanel_PlayerDefault, CharactersPanel::OnPlayerDefault)
    EVT_BUTTON(ID_CharactersPanel_Player1, CharactersPanel::OnPlayer1)
    EVT_BUTTON(ID_CharactersPanel_Player2, CharactersPanel::OnPlayer2)
    EVT_BUTTON(ID_CharactersPanel_Player3, CharactersPanel::OnPlayer3)
    EVT_BUTTON(ID_CharactersPanel_Player4, CharactersPanel::OnPlayer4)
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

    wxBitmap * character = panel->character_display;

    panel->frame = (panel->frame + 1) % 3600;

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
        dc.DrawBitmap(character->GetSubBitmap(panel->clip[clips1[i]]), x, y, true);
        x += 30;
    }

    x = 10;
    y = 70;
    for (int i = 0; i < 11; i++) {
        dc.DrawBitmap(character->GetSubBitmap(panel->clip[clips2[i]]), x, y, true);
        x += 30;
    }

    dc.DrawBitmap(character->GetSubBitmap(panel->clip[SPR_AVATAR + ((panel->frame / 15) % 2)]), 380, 10, true);

    static int clips3[] = {SPR_R, SPR_R_DUCK, SPR_R, SPR_R_JUMP, SPR_R, SPR_R_BRAKE};
    static int clips4[] = {SPR_L, SPR_L_DUCK, SPR_L, SPR_L_JUMP, SPR_L, SPR_L_BRAKE};

    dc.DrawBitmap(character->GetSubBitmap(panel->clip[clips3[(panel->frame / 6) % 6]]), 370, 70, true);
    dc.DrawBitmap(character->GetSubBitmap(panel->clip[clips4[(panel->frame / 6) % 6]]), 400, 70, true);

    x = 10 + ((panel->frame * 6) % 340);
    y = 130;
    dc.DrawBitmap(character->GetSubBitmap(panel->clip[SPR_R_WALK1 + ((panel->frame / 3) % 3)]), x, y, true);
    dc.DrawBitmap(character->GetSubBitmap(panel->clip[SPR_R_WALK1 + ((panel->frame / 3) % 3)]), 380, y, true);

    x = 350 - ((panel->frame * 6) % 340);
    y = 190;
    dc.DrawBitmap(character->GetSubBitmap(panel->clip[SPR_L_WALK1 + ((panel->frame / 3) % 3)]), x, y, true);
    dc.DrawBitmap(character->GetSubBitmap(panel->clip[SPR_L_WALK1 + ((panel->frame / 3) % 3)]), 380, y, true);

    x = 10 + ((panel->frame * 10) % 340);
    y = 250;
    dc.DrawBitmap(character->GetSubBitmap(panel->clip[SPR_R_RUN1 + ((panel->frame / 3) % 3)]), x, y, true);
    dc.DrawBitmap(character->GetSubBitmap(panel->clip[SPR_R_RUN1 + ((panel->frame / 3) % 3)]), 380, y, true);

    x = 350 - ((panel->frame * 10) % 340);
    y = 310;
    dc.DrawBitmap(character->GetSubBitmap(panel->clip[SPR_L_RUN1 + ((panel->frame / 3) % 3)]), x, y, true);
    dc.DrawBitmap(character->GetSubBitmap(panel->clip[SPR_L_RUN1 + ((panel->frame / 3) % 3)]), 380, y, true);
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

    if ( ! character.LoadFile(dialog.GetPath(), wxBITMAP_TYPE_BMP)) {
        wxMessageBox(_("Character file could not be loaded"), _("Error"), wxICON_EXCLAMATION);
        return;
    }

    character.SetMask(new wxMask(character, mask_color));

    CreatePlayerCharacters();

    character_display = &character;

    character_loaded = true;
    display->Refresh();
}

void CharactersPanel::CreatePlayerCharacters() {
    int r, g, b;

    for (int player = 0; player < 4; ++player) {
        character_player[player] = character.GetSubBitmap(wxRect(0, 0, character.GetWidth(), character.GetHeight()));

        wxNativePixelData data(character_player[player]);
        wxNativePixelData::Iterator p(data);

        for (int i = 0; i < data.GetWidth() * data.GetHeight(); ++i) {
            for (int c = 0; c < 5; ++c) {
                r = (SUIT_ORIGINAL[c] >> 16) & 0xff;
                g = (SUIT_ORIGINAL[c] >> 8) & 0xff;
                b = SUIT_ORIGINAL[c] & 0xff;

                if ((int) p.Red() == r && (int) p.Green() == g && (int) p.Blue() == b) {
                    p.Red() = (SUIT_REPLACE[player][c] >> 16) & 0xff;
                    p.Green() = (SUIT_REPLACE[player][c] >> 8) & 0xff;
                    p.Blue() = SUIT_REPLACE[player][c] & 0xff;
                }
            }
            p++;
        }
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

void CharactersPanel::OnPlayerDefault(wxCommandEvent &event) {
    character_display = &character;
}

void CharactersPanel::OnPlayer1(wxCommandEvent &event) {
    character_display = &character_player[0];
}

void CharactersPanel::OnPlayer2(wxCommandEvent &event) {
    character_display = &character_player[1];
}

void CharactersPanel::OnPlayer3(wxCommandEvent &event) {
    character_display = &character_player[2];
}

void CharactersPanel::OnPlayer4(wxCommandEvent &event) {
    character_display = &character_player[3];
}
