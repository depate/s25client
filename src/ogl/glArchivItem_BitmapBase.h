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
#ifndef GLARCHIVITEM_BITMAPBASE_H_INCLUDED
#define GLARCHIVITEM_BITMAPBASE_H_INCLUDED

#pragma once

#include "ArchivItem_Bitmap.h"
#include "DrawPoint.h"

class glArchivItem_BitmapBase: public virtual libsiedler2::ArchivItem_BitmapBase
{
public:
    glArchivItem_BitmapBase();
    glArchivItem_BitmapBase(const glArchivItem_BitmapBase& other);
    ~glArchivItem_BitmapBase() override;

    glArchivItem_BitmapBase& operator=(const glArchivItem_BitmapBase& item);

    /// liefert das GL-Textur-Handle.
    unsigned int GetTexture();
    /// Löscht die GL-Textur (z.B fürs Neuerstellen)
    virtual void DeleteTexture();
    /// Setzt den Texturfilter auf einen bestimmten Wert.
    virtual void setFilter(unsigned int filter);

    unsigned short GetTexWidth() const { return tex_width_; }
    unsigned short GetTexHeight() const { return tex_height_; }
    DrawPoint GetOrigin() const { return DrawPoint(nx_, ny_); }
    DrawPoint GetSize() const { return DrawPoint(getWidth(), getHeight()); }

private:
    /// Erzeugt die Textur.
    void GenerateTexture();

    unsigned int texture; /// Das GL-Textur-Handle
    unsigned int filter;  /// Der aktuell gewählte Texturfilter

protected:
    /// Returns the internal texure format
    int GetInternalFormat() const;
    /// Fill a just generated texture (glTexImage2D calls)
    virtual void FillTexture() = 0;
    /// Returns the currently set texture or 0 if none created
    unsigned GetTexNoCreate() { return texture; }
};

#endif // GLARCHIVITEM_BITMAPBASE_H_INCLUDED
