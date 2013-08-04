#	This file is part of Pidgin Media Emblems Plugin -- Buildable Edition.
#
#	Pidgin Media Emblems Plugin -- Buildable Edition is free software: you can
#	redistribute it and/or modify it under the terms of the GNU General Public
#	License as published by the Free Software Foundation, either version 3 of
#	the License, or (at your option) any later version.
#
#	Pidgin Media Emblems Plugin -- Buildable Edition is distributed in the hope
#	that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
#	warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with Pidgin Media Emblems Plugin -- Buildable Edition.
#	If not, see <http://www.gnu.org/licenses/>.

default: all

all: media_emblems.c
	gcc -Wall -fPIC -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/libpurple -c media_emblems.c
	gcc -shared -o media_emblems.so media_emblems.o

install:
	mkdir -p $${HOME}/.purple/plugins
	cp media_emblems.so $${HOME}/.purple/plugins
