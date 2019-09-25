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

#ifndef NPCMANAGER_H
#define NPCMANAGER_H

#include <stdio.h>
#include <string.h>
#include "../VSFInterface/vsfinterface.h"
#include "../FontManager/fontmanager.h"
#define ANIM_FPS 15

struct _npc_def
{
  char *nom;
  char *deplacement;
  char nb_deplacement;
  char *attaque;
  char nb_attaque;
  char *mort;
  char nb_mort;
};

struct _npc
{
  int id;
  struct _npc_def *type;
  int x;
  int y;
  float   depl_x;
  float   depl_y;
  short duree_depl;
  short tps_depl;
  char action;
  short direction;
  char frame;
  int  timer;
  struct _npc *next;
};  
 
/*
 * No description
 */
class NPCManager
{
	public:
		// class constructor
		NPCManager(char *, VSFInterface *);
		// class destructor
		~NPCManager();
		// No description
		bool ajout_npc(int, char *, int, int, short);
		// No description
		bool draw_npc(SDL_Surface *, int, int);
		// No description
		void dump_npc();
		// No description
		bool set_direction(int, int);
		// No description
		bool set_action(int, char);
		// No description
		bool move_to(int, unsigned int, unsigned int, unsigned short);
	private:
		struct _npc_def * liste_npc_def;
		short nb_npc_def;
		VSFInterface *vsfi;
		struct _npc *liste_npc;
		//FontManager *fm;
};

#endif // NPCMANAGER_H

