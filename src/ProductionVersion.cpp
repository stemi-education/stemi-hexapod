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


#include "ProductionVersion.h"


ProductionVersion::ProductionVersion()
{
	preferences.begin("my-app", false);
	version = preferences.getUInt("hwVersionMajor", 0);
}

void ProductionVersion::check()
{
	Serial.printf("\n*** STEMI Hexapod ***\n");
	if (version == 0)
	{
		
		Serial.printf("Hardware version not burned. ");
#ifdef ENABLE_VERSION_BURNING
		Serial.printf("Burning hardware v%d.%d.%d", HEXAPOD_HW_VERSION_MAJOR, HEXAPOD_HW_VERSION_MINOR, HEXAPOD_HW_VERSION_PATCH);
		preferences.putUInt("hwVersionMajor", HEXAPOD_HW_VERSION_MAJOR);
		preferences.putUInt("hwVersionMinor", HEXAPOD_HW_VERSION_MINOR);
		preferences.putUInt("hwVersionPatch", HEXAPOD_HW_VERSION_PATCH);
#endif // ENABLE_VERSION_BURNING
		Serial.printf("\n");
	}
	else
	{
		Serial.printf("Hardware v%d.%d.%d\n",
			HEXAPOD_HW_VERSION_MAJOR, HEXAPOD_HW_VERSION_MINOR, HEXAPOD_HW_VERSION_PATCH);
	}
	Serial.printf("Software v%d.%d.%d\n\n", HEXAPOD_SW_VERSION_MAJOR, HEXAPOD_SW_VERSION_MINOR, HEXAPOD_SW_VERSION_PATCH);
}