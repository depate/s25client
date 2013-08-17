// $Id: MapConsts.h 8846 2013-08-17 11:54:47Z marcus $
//
// Copyright (c) 2005 - 2011 Settlers Freaks (sf-team at siedler25.org)
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
#ifndef MAPCONSTS_H_INCLUDED
#define MAPCONSTS_H_INCLUDED

#pragma once

/// Datentyp f�r Map-Koordinaten
typedef unsigned short MapCoord;

/// Wie hoch und breit ist ein Dreieck?
static const int TR_W = 53;
static const int TR_H = 29;

/// Faktor fr die H�hen
const int HEIGHT_FACTOR = 5;

/// Terrainarten
enum TerrainType
{
	TT_SNOW = 0,
	TT_DESERT,
	TT_SWAMPLAND,
	TT_MEADOW_FLOWERS,
	TT_MOUNTAIN1,
	TT_MOUNTAIN2,
	TT_MOUNTAIN3,
	TT_MOUNTAIN4,
	TT_SAVANNAH,
	TT_MEADOW1,
	TT_MEADOW2,
	TT_MEADOW3,
	TT_STEPPE,
	TT_MOUNTAINMEADOW,
	TT_WATER,
	TT_LAVA
};


/// Terrainzuordnung
const unsigned char TERRAIN_INDIZES [20] =
{
	8,4,0,2,1,14,0,1,9,10,11,5,6,7,12,3,15,0,13,14
};

/// R�nder-Zuordnung
const unsigned char BORDER_TABLES[3][16][16][2] =
{
	// Gr�nland
	{
		{ {0,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0} },
		{ {0,1}, {0,0}, {3,0}, {3,0}, {3,0}, {3,0}, {3,0}, {3,0}, {3,0}, {3,0}, {3,0}, {3,0}, {3,3}, {0,2}, {3,0}, {3,0} },
		{ {0,1}, {0,3}, {0,0}, {0,4}, {0,2}, {0,2}, {0,2}, {0,2}, {0,4}, {0,4}, {0,4}, {0,4}, {0,3}, {0,2}, {4,0}, {4,0} },
		{ {0,1}, {0,3}, {4,0}, {0,0}, {0,2}, {0,2}, {0,2}, {0,2}, {4,4}, {4,4}, {4,4}, {4,4}, {0,3}, {0,2}, {4,0}, {4,0} },
		{ {0,1}, {0,3}, {2,0}, {2,0}, {0,0}, {2,2}, {2,2}, {2,2}, {2,0}, {2,0}, {2,0}, {2,0}, {0,3}, {0,2}, {2,0}, {2,0} },
		{ {0,1}, {0,3}, {2,0}, {2,0}, {2,2}, {0,0}, {2,2}, {2,2}, {2,0}, {2,0}, {2,0}, {2,0}, {0,3}, {0,2}, {2,0}, {2,0} },
		{ {0,1}, {0,3}, {2,0}, {2,0}, {2,2}, {2,2}, {0,0}, {2,2}, {2,0}, {2,0}, {2,0}, {2,0}, {0,3}, {0,2}, {2,0}, {2,0} },
		{ {0,1}, {0,3}, {2,0}, {2,0}, {2,2}, {2,2}, {2,2}, {0,0}, {2,0}, {2,0}, {2,0}, {2,0}, {0,3}, {0,2}, {2,0}, {2,0} },
		{ {0,1}, {0,3}, {4,0}, {4,4}, {0,2}, {0,2}, {0,2}, {0,2}, {0,0}, {4,4}, {4,4}, {4,4}, {0,3}, {0,2}, {4,0}, {4,0} },
		{ {0,1}, {0,3}, {4,0}, {4,4}, {0,2}, {0,2}, {0,2}, {0,2}, {4,4}, {0,0}, {4,4}, {4,4}, {0,3}, {0,2}, {4,0}, {4,0} },
		{ {0,1}, {0,3}, {4,0}, {4,4}, {0,2}, {0,2}, {0,2}, {0,2}, {4,4}, {4,4}, {0,0}, {4,4}, {0,3}, {0,2}, {4,0}, {4,0} },
		{ {0,1}, {0,3}, {4,0}, {4,4}, {0,2}, {0,2}, {0,2}, {0,2}, {4,4}, {4,4}, {4,4}, {0,0}, {0,3}, {0,2}, {4,0}, {4,0} },
		{ {0,1}, {3,3}, {3,0}, {3,0}, {3,0}, {3,0}, {3,0}, {3,0}, {3,0}, {3,0}, {3,0}, {3,0}, {0,0}, {0,2}, {3,0}, {3,0} },
		{ {0,1}, {2,0}, {2,0}, {2,0}, {2,0}, {2,0}, {2,0}, {2,0}, {2,0}, {2,0}, {2,0}, {2,0}, {2,0}, {0,0}, {2,0}, {2,0} },
		{ {0,1}, {0,3}, {0,4}, {0,4}, {0,2}, {0,2}, {0,2}, {0,2}, {0,4}, {0,4}, {0,4}, {0,4}, {0,3}, {0,2}, {0,0}, {5,0} },
		{ {0,1}, {0,3}, {0,4}, {0,4}, {0,2}, {0,2}, {0,2}, {0,2}, {0,4}, {0,4}, {0,4}, {0,4}, {0,3}, {0,2}, {0,5}, {0,0} }
	},

	// �dland
	{
		{ {0,0}, {0,3}, {0,1}, {0,4}, {0,4}, {0,4}, {0,4}, {0,4}, {0,4}, {0,4}, {0,4}, {0,4}, {0,3}, {0,1}, {0,2}, {0,0} },
		{ {3,0}, {0,0}, {0,1}, {3,0}, {0,4}, {0,4}, {0,4}, {0,4}, {3,0}, {3,0}, {3,0}, {3,0}, {3,3}, {0,1}, {0,2}, {3,0} },
		{ {1,0}, {1,0}, {0,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {0,1}, {1,0}, {1,0} },
		{ {4,0}, {0,3}, {0,1}, {0,0}, {4,0}, {4,0}, {4,0}, {4,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,3}, {0,1}, {0,2}, {4,0} },
		{ {4,0}, {4,0}, {0,1}, {0,4}, {0,0}, {0,0}, {0,0}, {0,0}, {0,4}, {0,4}, {0,4}, {0,4}, {4,0}, {0,1}, {4,0}, {4,0} },
		{ {4,0}, {4,0}, {0,1}, {0,4}, {0,0}, {0,0}, {0,0}, {0,0}, {0,4}, {0,4}, {0,4}, {0,4}, {4,0}, {0,1}, {4,0}, {4,0} },
		{ {4,0}, {4,0}, {0,1}, {0,4}, {0,0}, {0,0}, {0,0}, {0,0}, {0,4}, {0,4}, {0,4}, {0,4}, {4,0}, {0,1}, {4,0}, {4,0} },
		{ {4,0}, {4,0}, {0,1}, {0,4}, {0,0}, {0,0}, {0,0}, {0,0}, {0,4}, {0,4}, {0,4}, {0,4}, {4,0}, {0,1}, {4,0}, {4,0} },
		{ {4,0}, {0,3}, {0,1}, {0,0}, {4,0}, {4,0}, {4,0}, {4,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,3}, {0,1}, {0,2}, {4,0} },
		{ {4,0}, {0,3}, {0,1}, {0,0}, {4,0}, {4,0}, {4,0}, {4,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,3}, {0,1}, {0,2}, {4,0} },
		{ {4,0}, {0,3}, {0,1}, {0,0}, {4,0}, {4,0}, {4,0}, {4,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,3}, {0,1}, {0,2}, {4,0} },
		{ {4,0}, {0,3}, {0,1}, {0,0}, {4,0}, {4,0}, {4,0}, {4,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,3}, {0,1}, {0,2}, {4,0} },
		{ {3,0}, {3,3}, {0,1}, {3,0}, {0,4}, {0,4}, {0,4}, {0,4}, {3,0}, {3,0}, {3,0}, {3,0}, {0,0}, {0,1}, {0,2}, {3,0} },
		{ {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {1,0} },
		{ {2,0}, {2,0}, {0,1}, {2,0}, {0,4}, {0,4}, {0,4}, {0,4}, {2,0}, {2,0}, {2,0}, {2,0}, {2,0}, {0,1}, {0,0}, {2,0} },
		{ {0,0}, {0,3}, {0,1}, {0,4}, {0,4}, {0,4}, {0,4}, {0,4}, {0,4}, {0,4}, {0,4}, {0,4}, {0,3}, {0,1}, {0,2}, {0,0} }
	},

	// Winterwelt
	{
		{ {0,0}, {5,0}, {0,0}, {5,0}, {0,2}, {0,2}, {0,2}, {0,2}, {5,0}, {5,0}, {5,0}, {5,0}, {5,0}, {5,0}, {0,0}, {5,0} },
		{ {0,5}, {0,0}, {0,5}, {3,0}, {0,2}, {0,2}, {0,2}, {0,2}, {3,0}, {3,0}, {3,0}, {3,0}, {3,0}, {3,0}, {0,5}, {3,0} },
		{ {0,0}, {5,0}, {0,0}, {5,0}, {0,2}, {0,2}, {0,2}, {0,2}, {5,0}, {5,0}, {5,0}, {5,0}, {5,0}, {5,0}, {0,0}, {5,0} },
		{ {0,5}, {0,3}, {0,5}, {0,0}, {0,2}, {0,2}, {0,2}, {0,2}, {4,4}, {4,4}, {4,4}, {4,4}, {4,0}, {0,1}, {0,5}, {4,0} },
		{ {2,0}, {2,0}, {2,0}, {2,0}, {0,0}, {2,2}, {2,2}, {2,2}, {2,0}, {2,0}, {2,0}, {2,0}, {2,0}, {0,1}, {2,0}, {2,0} },
		{ {2,0}, {2,0}, {2,0}, {2,0}, {2,2}, {0,0}, {2,2}, {2,2}, {2,0}, {2,0}, {2,0}, {2,0}, {2,0}, {0,1}, {2,0}, {2,0} },
		{ {2,0}, {2,0}, {2,0}, {2,0}, {2,2}, {2,2}, {0,0}, {2,2}, {2,0}, {2,0}, {2,0}, {2,0}, {2,0}, {0,1}, {2,0}, {2,0} },
		{ {2,0}, {2,0}, {2,0}, {2,0}, {2,2}, {2,2}, {2,2}, {0,0}, {2,0}, {2,0}, {2,0}, {2,0}, {2,0}, {0,1}, {2,0}, {2,0} },
		{ {0,5}, {0,3}, {0,5}, {4,4}, {0,2}, {0,2}, {0,2}, {0,2}, {0,0}, {4,4}, {4,4}, {4,4}, {4,0}, {0,1}, {0,5}, {4,0} },
		{ {0,5}, {0,3}, {0,5}, {4,4}, {0,2}, {0,2}, {0,2}, {0,2}, {4,4}, {0,0}, {4,4}, {4,4}, {4,0}, {0,1}, {0,5}, {4,0} },
		{ {0,5}, {0,3}, {0,5}, {4,4}, {0,2}, {0,2}, {0,2}, {0,2}, {4,4}, {4,4}, {0,0}, {4,4}, {4,0}, {0,1}, {0,5}, {4,0} },
		{ {0,5}, {0,3}, {0,5}, {4,4}, {0,2}, {0,2}, {0,2}, {0,2}, {4,4}, {4,4}, {4,4}, {0,0}, {4,0}, {0,1}, {0,5}, {4,0} },
		{ {0,5}, {0,3}, {0,5}, {0,4}, {0,2}, {0,2}, {0,2}, {0,2}, {0,4}, {0,4}, {0,4}, {0,4}, {0,0}, {0,1}, {0,5}, {4,0} },
		{ {0,5}, {0,3}, {0,5}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {0,0}, {0,5}, {1,0} },
		{ {0,0}, {5,0}, {0,0}, {5,0}, {0,2}, {0,2}, {0,2}, {0,2}, {5,0}, {5,0}, {5,0}, {5,0}, {5,0}, {5,0}, {0,0}, {5,0} },
		{ {0,5}, {0,3}, {0,5}, {0,4}, {0,2}, {0,2}, {0,2}, {0,2}, {0,4}, {0,4}, {0,4}, {0,4}, {0,4}, {0,1}, {0,0}, {0,0} }
	}
};

/// Sichtbarkeit f�r ALLE Spieler
enum Visibility
{
	VIS_INVISIBLE = 0, /// Darkness
	VIS_FOW, /// Fog of war
	VIS_VISIBLE /// Visible
};

/// Granittyp
enum GraniteType
{
	GT_1 = 0,
	GT_2
};

/// Flaggentyp
enum FlagType
{
	FT_NORMAL,
	FT_LARGE,
	FT_WATER
};


#endif // !MAPCONSTS_H_INCLUDED
