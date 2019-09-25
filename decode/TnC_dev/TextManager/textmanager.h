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

#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H
#include "../FontManager/fontmanager.h"
#include <stdlib.h>

struct _text
{
  SDL_Surface *texte;
  int timeout;
  int screen_x;
  int screen_y;
  int loc_x;
  int loc_y;
  short loc_zone;
  short alpha;
  struct _text *next;
};  


class TextManager
{
	public:
		TextManager(FontManager *);
		~TextManager();
		bool add_text_at_loc(char *, int, int, short, short);
    bool add_color_text_at_loc(char *, int, int, short, short, unsigned int);
		bool add_text_at_pos(char *, int, int, short);
		bool add_color_text_at_pos(char *, int, int, short, unsigned int);
		void draw_texts(SDL_Surface *, int, int);
	private:
	  FontManager *fm;
	  struct _text *liste_textes;
};

#endif // TEXTMANAGER_H

