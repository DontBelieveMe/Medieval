unistd.h and getopt.c/.h are here purely for zlib to work. It means I do not have to go messing around in 
the zlib source/headers. In fact im not sure if they are needed for anyone else, especially MinGW. 
TL;DR Here as MSVC is a little shit, and decided not to have unix header files.

getopt.c/.h:
	https://gist.github.com/ashelly/7776712

/* Getopt for GNU.
   NOTE: getopt is now part of the C library, so if you don't know what
   "Keep this file name-space clean" means, talk to drepper@gnu.org
   before changing it!
   Copyright (C) 1987,88,89,90,91,92,93,94,95,96,98,99,2000,2001
        Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

uistd.h:
	http://stackoverflow.com/questions/341817/is-there-a-replacement-for-unistd-h-for-windows-visual-c