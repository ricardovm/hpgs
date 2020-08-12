# hpgs - HP 6xxC Printer GhostScript
#
# Author:	Ricardo Vaz Mannrich (husk-le)
# 		<mannrich@hotmail.com>
# Date:		April, 16th 2001.
#
# Copyright (C) 2000, 2001 Ricardo Vaz Mannrich
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#    GNU General Public License for more details.
#
#    Tou should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

all: hpgs

hpgs: hpgs.c
	gcc -g2 -ohpgs hpgs.c

really-install: hpgs
	mv /usr/bin/gs /usr/bin/gs-run
	cp hpgs /usr/bin/gs-hpgs-0.45
	ln -s /usr/bin/gs-hpgs-0.45 /usr/bin/gs

clean:
	rm hpgs

uninstall-old:
	mv /usr/bin/gs /usr/bin/gs-hpgs-0.02.old
	mv /usr/bin/gs.default /usr/bin/gs

uninstall-old-remove:
	rm -f /usr/bin/gs
	mv /usr/bin/gs.default /usr/bin/gs

update: hpgs uninstall-old really-install
