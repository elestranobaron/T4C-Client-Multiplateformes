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
#include "npcmanager.h"

// No description
bool NPCManager::set_action(int id, char action)
{
  struct _npc *npc = liste_npc;
  while (npc&&(npc->id != id))
    npc = npc->next;
  if(!npc) return false;
  
  npc->frame  =  'a';
  npc->action = action;
  return true;
}

// No description
bool NPCManager::set_direction(int id, int direction)
{
  struct _npc *npc = liste_npc;
  while (npc&&(npc->id != id))
    npc = npc->next;
  if(!npc) return false;

  npc->frame  =  'a';
  npc->direction = direction;
  return true;
}
