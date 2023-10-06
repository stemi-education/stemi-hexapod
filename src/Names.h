/*

This file is part of STEMI hexapod library.

Copyright (C) 2017 STEMI d.o.o.
Authors: Luka Fucek, et al.
Contact: info@stemi.education
Web: https://www.stemi.education/

STEMI hexapod library is free software exclusively
for non-commercial activities.

In case of developing any comercial activities involving STEMI
hexapod library please contact stemi.education for providing custom
license at: info@stemi.education.

For non-commercial activities you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License,
or (at your option) any later version.

STEMI hexapod library is
distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with STEMI hexapod library.
If not, see http://www.gnu.org/licenses/.

For additional information please check http://www.stemi.education.

*/


#ifndef NAMES_H
#define NAMES_H

#include <string>
#include "Preferences.h"

#define NUMBER_OF_NAMES 110
#define NUMBER_OF_ADJECTIVES 24

class Names
{
public:
	Names();
	std::string generateName(uint16_t number1, uint16_t number2);
	uint16_t sumStringMemberValues1(uint8_t str[6]);
	uint16_t sumStringMemberValues2(uint8_t str[6]);
	void storeInit();
	void store(std::string name);
	void load(std::string *name);
private:

	std::string name;
	Preferences preferences;
	std::string adjectives[NUMBER_OF_ADJECTIVES] = {
		"Cute",
		"Good",
		"Fast",
		"Mysterious",
		"Pretty",
		"Instinctive",
		"Adorable",
		"Tiny",
		"Clever",
		"Tall",
		"Secret",
		"Large",
		"Small",
		"Futuristict",
		"Wicked",
		"Swift",
		"Quick",
		"Sweet",
		"Lazy",
		"Scary",
		"Brave",
		"Dark",
		"Furry",
		"Cold"
	};
	std::string names[NUMBER_OF_NAMES] =
	{
		//developer names
		"Jole",
		"Fule",
		"Vlax",
		"Tuna",
		"Maricello",
		"Mia",
		"Jan",
		"Peewee",
		"Bobby",
		
		//generic names
		"Beatrix",
		"Blaire",
		"Callie",
		"Cecily",
		"Cleo",
		"Coco",
		"Cosette",
		"Cybil",
		"Daisy",
		"Delaney",
		"Delilah",
		"Eden",
		"Edie",
		"Etta",
		"Everly",
		"Finley",
		"Georgia",
		"Gwendolyn",
		"Hadley",
		"Harper",
		"Holliday",
		"Isla",
		"January",
		"Juniper",
		"Kenzie",
		"Lola",
		"Lulu",
		"Mabel",
		"Mae",
		"Minnie",
		"Nala",
		"Nova",
		"Penelope",
		"Perla",
		"Piper",
		"Poppy",
		"Rebel",
		"Rowan",
		"Sage",
		"Simone",
		"Siobhan",
		"Sloane",
		"Sparrow",
		"Stella",
		"Stevie",
		"Tallulah",
		"Tatum",
		"Quinlynn",
		"Wren",
		"Zelda",
		"Ace",
		"Ajax",
		"Arrow",
		"Ash",
		"August",
		"Axel",
		"Beckett",
		"Booker",
		"Brees",
		"Bruno",
		"Buster",
		"Calloway",
		"Colton",
		"Cormac",
		"Cruz",
		"Dash",
		"Dean",
		"Dexter",
		"Donovan",
		"Elvis",
		"Finn",
		"Echo",
		"Enzo",
		"Felix",
		"Fritz",
		"Griffith",
		"Gunner",
		"Gus",
		"Grayson",
		"Hunter",
		"Jamison",
		"Jax",
		"Lincoln",
		"Maddox",
		"Magnus",
		"Max",
		"Orion",
		"Phoenix",
		"Pierce",
		"Porter",
		"Reid",
		"Rhett",
		"Rhys",
		"Rocco",
		"Ryland",
		"Thorn",
		"Titus",
		"Quinton",
		"Wilder",
		"Zander",
		"Zeke"
	};
};

#endif
