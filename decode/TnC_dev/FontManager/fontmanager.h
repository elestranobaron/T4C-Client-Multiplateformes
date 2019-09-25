/***********************************************************************
* This file is a part of TnC The Next Coming (The 4th Coming rebuild with SDL)
* Copyright (C) 2005  Noth
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* aint with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef FONTMANAGER_H
#define FONTMANAGER_H
#include <SDL/SDL.h>
//#include <SDL/SDL_image.h>
#include <string.h>
#define MAX_CHAR 95 /** Nombre de chars dans une font */

/**
 * \class FontManager
 * \brief Classe de gestion des Fonts.
 */
class FontManager
{
	public:
		FontManager();
		~FontManager();
		bool load_font(char *);
		SDL_Surface * get_text(char *);
		SDL_Surface * get_text(char *, unsigned int);
		SDL_Surface * get_text_w(char *, unsigned int);
		SDL_Surface * get_text_w(char *, unsigned int, unsigned int);
	private:
		/** Surface contenant la font actuelle */
		SDL_Surface * font;
		/** Tableau d'index servant a parser la surface de font */
		int index[MAX_CHAR];
};

#endif // FONTMANAGER_H

