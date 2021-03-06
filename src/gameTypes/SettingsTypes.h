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

#ifndef SettingsTypes_h__
#define SettingsTypes_h__

#include "gameTypes/GoodTypes.h"
#include "gameTypes/BuildingTypes.h"
#include <boost/array.hpp>

//TODO: Make this structs so meanings are obvious

/// Custom mapping of (Ware, Receiver)-tuple to percentage of wares distributed to that building
typedef boost::array<unsigned char, 23> Distributions;
/// Ordering of building types by priority. All buildings in here except unused and HQ
typedef boost::array<BuildingType, BLD_COUNT - NUM_UNUSED_BLD_TYPES - 1> BuildOrders;
/// Mapping transport priority -> standard transport priority of ware(group):
/// E.g. std prio of coins = 0 -> TransportOrders[0] = stdPrio[COINS] = 0
/// New prio of coints = 1 -> TransportOrders[1] = stdPrio[COINS] = 0
typedef boost::array<unsigned char, 14> TransportOrders;
typedef boost::array<unsigned char, WARE_TYPES_COUNT> TransportPriorities;
/// Priority of each tool
typedef boost::array<unsigned char, TOOL_COUNT> ToolSettings;
/// Value of each military slider
/// 0: Recruiting ratio (to max possible recruits)
/// 1: Defender strength (ratio to max available rank)
/// 2: Active defenders (engaging attackers by leaving building): Chance that one is sent
/// 3: Ratio of used attackers to available attackers
/// 4-7: Ratio of soldiers in buildings to full occupation for inland, middle region, harbor spots, border regions
typedef boost::array<unsigned char, 8> MilitarySettings;

#endif // SettingsTypes_h__

