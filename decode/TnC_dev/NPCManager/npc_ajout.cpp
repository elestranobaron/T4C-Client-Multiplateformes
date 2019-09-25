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
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#include "npcmanager.h"

// No description
bool NPCManager::ajout_npc(int id, char *nom, int x, int y, short direction)
{
  //cr�ation de la structure
  struct _npc *npc = (struct _npc *) malloc(sizeof(struct _npc));
  npc->id = id;
  npc->x = x;
  npc->y = y;
  npc->depl_x = 0;
  npc->depl_y = 0;
  npc->duree_depl = 0;
  npc->direction = direction;
  npc->action = 'D';
  npc->frame = 'a';
  npc->type = NULL;

  //recherche du type de NPC
  for (int i = 0; i < nb_npc_def; i++) {
    if (strchr(liste_npc_def[i].nom, '\r'))
      liste_npc_def[i].nom[strlen(liste_npc_def[i].nom) - 1] = 0;
    if (!strcmp(liste_npc_def[i].nom, nom))
      npc->type = &liste_npc_def[i];
  }

  if (!npc->type) //type de npc pas trouv� !
  {
    free(npc);
    return false;
  }  

  npc->timer = SDL_GetTicks();

  //ajout � sa position dans la liste  : tri� par loc y croissant (1er plan : y grand)
  struct _npc *tmp = liste_npc;
  struct _npc *tmp2 = NULL;
  printf("NPCManager > Ajout du npc %s\n", npc->type->nom);
  while ((tmp)&&(tmp->y < npc->y))
  {
    tmp2 = tmp;
    tmp = tmp->next;
  }
  if (tmp2)  //on en a un avant
  {
    npc->next = tmp;
    tmp2->next = npc;
  }
  else if (!tmp) //la liste �tait vide
  {
    npc->next = NULL;
    liste_npc = npc;
  }
  else    //1er de la liste
  {
    npc->next = liste_npc;
    liste_npc = npc;
  }  
  return true;
}
