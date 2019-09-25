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

#include "npcmanager.h" // class's header file

// class constructor
NPCManager::NPCManager(char *npc_list, VSFInterface *vsfi)
{
  //fm = new FontManager();
  //fm->load_font("./fonts/sans_bold_12");

  this->vsfi = vsfi;
  this->liste_npc = NULL;
  nb_npc_def = 0;

  //Lecture du fichier d'index des sprites.
  FILE *fid;
  fid = fopen(npc_list, "r");
  fscanf(fid, "%hu\n", &nb_npc_def);

  this->liste_npc_def = new struct _npc_def[nb_npc_def];
  int pos = 0;
  char tmp[255];
  char *sep;
  while ((!feof(fid))&&(pos < nb_npc_def))
  {
    fgets(tmp, 255, fid);
    liste_npc_def[pos].nom = new char[strlen(tmp)+1];
    tmp[strlen(tmp)-1] = '\0';
    strcpy(liste_npc_def[pos].nom, tmp);

    fgets(tmp, 255, fid);
    sep = strchr(tmp, '#');
    *sep = '\0';
    sep++;
    liste_npc_def[pos].deplacement = new char[strlen(tmp)+1];
    strcpy(liste_npc_def[pos].deplacement, tmp);
    liste_npc_def[pos].nb_deplacement = sep[0];

    fgets(tmp, 255, fid);
    sep = strchr(tmp, '#');
    *sep = '\0';
    sep++;
    liste_npc_def[pos].attaque = new char[strlen(tmp)+1];
    strcpy(liste_npc_def[pos].attaque, tmp);
    liste_npc_def[pos].nb_attaque = sep[0];    
    
    fgets(tmp, 255, fid);
    sep = strchr(tmp, '#');
    *sep = '\0';
    sep++;
    liste_npc_def[pos].mort = new char[strlen(tmp)+1];
    strcpy(liste_npc_def[pos].mort, tmp);
    liste_npc_def[pos].nb_mort = sep[0];  
    
    fgets(tmp, 255, fid);  

    
//printf("> [%02d] NPC : %20s  | Deplacement: %20s [a->%c] | Attaque: %20s [a->%c] | Mort: %20s [a->%c]\n",pos, liste_npc_def[pos].nom, liste_npc_def[pos].deplacement, liste_npc_def[pos].nb_deplacement, liste_npc_def[pos].attaque, liste_npc_def[pos].nb_attaque, liste_npc_def[pos].mort, liste_npc_def[pos].nb_mort);
    pos++;
  }
}

// class destructor
NPCManager::~NPCManager()
{
	// insert your code here
}



// No description
void NPCManager::dump_npc()
{
   struct _npc *npc = liste_npc;
   while(npc)
   {
     printf("[%03d] (%04d, %04d)  %s (%d)\n", npc->id, npc->x, npc->y, npc->type->nom, npc->direction);
     npc = npc->next;
   }  
}






// No description
bool
NPCManager::move_to(int id, unsigned int dest_x, unsigned int dest_y, unsigned short speed)
{
  struct _npc *npc = liste_npc;
  while (npc&&(npc->id != id))
    npc = npc->next;
  if(!npc) return false;

  npc->depl_x = - (float)((npc->x*32 - dest_x*32)/(2*speed));
  npc->depl_y = - (float)((npc->y*16 - dest_y*16)/(2*speed));
  npc->duree_depl  = (short) (sqrt((npc->x - dest_x)*(npc->x - dest_x) + (npc->y-dest_y)*(npc->y-dest_y))*32/speed);
  npc->tps_depl  = 0;
  printf("deplacement de [%d, %d] vers [%d, %d] selon (%f,%f) pour une dur�e de %d\n", npc->x, npc->y, dest_x, dest_y, npc->depl_x, npc->depl_y, npc->duree_depl);
    return true;
}

