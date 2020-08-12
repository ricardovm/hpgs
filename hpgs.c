/******************************************************************************
 * hpgs - HP 6xxC Printer GhostScript
 *
 * The HP 6xxC series printers uses the ^[*o-1M command to turn to
 * EconoFast mode. GhostScript (gs) uses the -dDepletion parameter
 * to control this, but it can't send a -1M, only -1.
 * This programs gets the output from gs, find the *o-1d2Q string
 * and translates to *o-1M.
 *
 * Author:	Ricardo Vaz Mannrich (husk-le)
 * 		<mannrich@hotmail.com>
 * Date:	April, 16th 2001.
 *
 *                         -------------------------
 *                         
 * Copyright (C) 2000, 2001 Ricardo Vaz Mannrich
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *    GNU General Public License for more details.
 *
 *    Tou should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *                         -------------------------
 * 
 *  TODO: Create a .configure script
 *        Use of Pipes
 *
 *        With this, I think I can chenge to the stable version.
 ******************************************************************************/
#include <stdio.h>
#include <string.h>

#define VERSION 0.45
#define GS "/usr/bin/gs-run "	// keep the ending space

#define DEBUG
/*-----------------------------------------------------------------------------
 * Variables
 *----------------------------------------------------------------------------*/
FILE *file_in;
FILE *file_out;

/*-----------------------------------------------------------------------------
 * void econo_mode()
 *
 * This converts the input to a econo-mode output.
 *----------------------------------------------------------------------------*/
void econo_mode()
{
	char *in_str="*o-1";
	char pos=0;
	char in_char;
	
	in_char = fgetc(file_in);
	while (!feof(file_in)) {
		if (in_char==in_str[pos]) {
			pos++;
		} else
			pos=0;

		fputc(in_char, file_out);

		if (in_str[pos]=='\0') {
			/*
			 * This reads the "d2q" from file
			 */

			in_char = fgetc(file_in);
			in_char = fgetc(file_in);
			in_char = fgetc(file_in);

			/*
			 * Puts the endign "M"
			 */
			fputc('M', file_out);
			pos=0;
		}
		
		in_char = fgetc(file_in);
	}

	fflush(file_out);
}

/*-----------------------------------------------------------------------------
 * int main(int argc, char **argv)
 *
 * This calls the GhostScript, ant puts its output in a temporary file. This
 * calls the econo_mode() function too.
 *----------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
	int i;
	char cmdbuf[512];
	char *useroutputfile=NULL;
	char *tmpname;

	cmdbuf[0] = '\0';

	strcat(cmdbuf, GS);

	tmpname = tmpnam(NULL);
	strcat(cmdbuf, "-sOutputFile=");
	strcat(cmdbuf, tmpname);
	strcat(cmdbuf, " ");

	/*
	 * This replaces the -sOutputFile parameter with a temp file
	 */
	for (i=1; i < (argc); i++) {
		if (strncasecmp("-sOutputFile=", argv[i], 13)==0) {
			useroutputfile = argv[i]+13;

			if (strcmp(useroutputfile, "-")!=0)
				file_out = fopen(useroutputfile, "w+");
			else
				file_out = stdout;
		} else
			strcat(cmdbuf, argv[i]);

		strcat(cmdbuf, " ");
	}
	
	if (useroutputfile==NULL)
		file_out = stdout;

	/*
	 * Forces GhostScript output to a pipe
	 */

	system(cmdbuf);		// call GhostScript

	/*
	 * FIXME: Better using:
	 *
	 * cmdbuf = "gs-default --help";
	 * system(cmdbuf);
	 *
	 * in the while above instead the if below
	 */
	if ((strcmp(argv[1], "-help")==0) || (strcmp(argv[1], "--help")==0)) {
		return(0);
	}

	file_in = fopen(tmpname, "r");

	econo_mode();

	fclose(file_in);

	if (file_out!=stdout)
		fclose(file_out);

	remove(tmpname);
}
