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

#ifndef MAPINTERFACE_H
#define MAPINTERFACE_H
#include <zlib.h>
#include "../VSFInterface/vsfinterface.h"
#include "../FontManager/fontmanager.h"

#define SCREEN_W 1800//800
#define SCREEN_H 1000//600
#define MARGE_W 5
#define MARGE_H 20
#define MAP_W (SCREEN_W/32+MARGE_W+MARGE_W)    //nb de sprites de largeur (20 affich�s)
#define MAP_H (SCREEN_H/16+1+MARGE_H+MARGE_H)    //nb de sprites de hauteur (30 affich�s)
#define MAPBUF_W 512 //largeur du buffer (en nb de sprites)
#define MAPBUF_H 512 //hauteur du buffer (en nb de sprites)

/**
 * \class MAPInterface
 * \brief Classe de gestion des cartes.
 */
class MAPInterface
{
	public:
		MAPInterface(char *, VSFInterface *);
		~MAPInterface();
		bool get_map(unsigned int, unsigned int, unsigned short, SDL_Surface *, SDL_Surface *);
		void set_debug(bool);
		void get_loc(int *, int*, short*);
	private:
	  int map_x; /** centre (x) de la zone du buffer de map */
	  int map_y; /** centre (y) de la zone du buffer de map */
	  int last_x;
	  int last_y;
	  short last_zone;
	  unsigned int map[MAPBUF_W*MAPBUF_H];
		char tab_maps[4][128];
		VSFInterface * vsfi;
    FontManager *fm;
		// No description
		bool move_map(SDL_Surface *, SDL_Surface *, int, int);
		// No description
		void full_redraw(SDL_Surface *, SDL_Surface *, unsigned int, unsigned int, unsigned short, SDL_Rect*);
		bool debug;
		// No description
		inline SDL_Surface * fusion(struct _sprite*, struct _sprite *, struct _sprite *, unsigned char, unsigned char);
};

#endif // MAPINTERFACE_H

