// Copyright (c) 2005 - 2015 Settlers Freaks (sf-team at siedler25.org)
//
// This file is part of Return To The Roots.
//
// Return To The Roots is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Return To The Roots is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Return To The Roots. If not, see <http://www.gnu.org/licenses/>.
#ifndef CTRLCOMBOBOX_H_INCLUDED
#define CTRLCOMBOBOX_H_INCLUDED

#pragma once

#include "Window.h"
#include "ctrlList.h"
class MouseCoords;
class glArchivItem_Font;

class ctrlComboBox : public Window
{
    public:
        ctrlComboBox(Window* parent, unsigned int id, unsigned short x, unsigned short y, unsigned short width, unsigned short height, TextureColor tc, glArchivItem_Font* font, unsigned short max_list_height, bool readonly);

        void Resize(unsigned short width, unsigned short height) override;

        void AddString(const std::string& text);
        void DeleteAllItems();

        void SetSelection(unsigned short selection);
        unsigned short GetSelection() const { return GetCtrl<ctrlList>(0)->GetSelection(); };
        unsigned short GetCount() const { return GetCtrl<ctrlList>(0)->GetLineCount(); }
        const std::string& GetText(unsigned short item) const { return GetCtrl<ctrlList>(0)->GetItemText(item); }

        void Msg_PaintAfter() override;
        bool Msg_MouseMove(const MouseCoords& mc) override;
        bool Msg_LeftDown(const MouseCoords& mc) override;
        bool Msg_LeftUp(const MouseCoords& mc) override;
        bool Msg_RightDown(const MouseCoords& mc) override;
        bool Msg_WheelUp(const MouseCoords& mc) override;
        bool Msg_WheelDown(const MouseCoords& mc) override;
        void Msg_ListSelectItem(const unsigned int ctrl_id, const int selection) override;

    protected:
        bool Draw_() override;

        void ShowList(bool show);

    private:
        TextureColor tc;
        glArchivItem_Font* font;
        unsigned short max_list_height;
        unsigned short selectionOnListOpen; // What was selected when the combo was opened?
        bool readonly;
        bool last_show;
};

#endif // CTRLCOMBOBOX_H_INCLUDED
