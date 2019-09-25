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

#include "mapinterface.h"

/**
 * \fn MAPInterface::get_map(unsigned int, unsigned int, unsigned short, SDL_Surface *, SDL_Surface *)
 * \param unsigned int x,y: coordonn�es de la zone � construire
 * \param unsigned short zone : num�ro de la zone
 * \param SDL_Surface * : SDL_Surface allou�e qui contiendra le layer de sol
 * \param SDL_Surface * : SDL_Surface allou�e qui contiendra le layer de d�cor
 * \return bool : r�ussite ou non
 * \brief Cette fonction met � jour les deux layers, en y collant les sprites pour la loc pass�e en argument. Les sprites sont r�cup�r�s aupr�s de l'objet vsfi.
 */
bool
MAPInterface::get_map(unsigned int x, unsigned int y, unsigned short zone, SDL_Surface *layer_sol, SDL_Surface *layer_decor)
{
  if ((!layer_sol)||(!layer_decor))
  {
    printf("Erreur > Layers de map non initialises !\n");
    exit(1);
  }
 // printf("demande de la loc %d,%d\n",x,y);


  //### MISE A JOUR DU BUFFER (facultatif)
  //on s'est trop �cart� du centre du buffer(map_x, map_y), on recharge un nouveau buffer centr� sur la position actuelle
  if ((x-map_x)*(x-map_x)+(y-map_y)*(y-map_y) > (MAPBUF_W/2-MAP_W)*(MAPBUF_W/2-MAP_W))
  {
    printf("Mise � jour du buffer de map. Ancien centre de map : (%d, %d) Loc actuelle : (%d, %d)\n", map_x, map_y, x,y);
    //R�cup�rage de la matrice des ID a dessiner
  
    FILE *fmap = fopen(tab_maps[zone],"rb");
    unsigned int buf;
    for (int i = 0; i < MAPBUF_H; i++)
    {
       unsigned int offset = ((y-MAPBUF_H/2)+i)*3072+(x-MAPBUF_W/2);
       for (int j = 0; j < MAPBUF_W; j++)
       {
         if (((int)(y+i-MAPBUF_H/2) < 0)||((y+i-MAPBUF_H/2) > 3072)||((int)(x+j-MAPBUF_W/2) < 0)||((x+j-MAPBUF_W/2) > 3072))
           map[i*MAPBUF_W+j] = 0x2aa90650; //d�borde de la map => on colle un "Black Tile".
         else
         {
           fseek(fmap, (offset+j)*sizeof(unsigned int), SEEK_SET);
           fread(&buf, sizeof(unsigned int), 1, fmap);
           map[i*MAPBUF_W+j] = buf;
         }  
       }  
    }  
    map_x = x;
    map_y = y;
    fclose(fmap);
  }

  //### MISE A JOUR DES LAYERS
  //int dep_x = x-last_x;
  //int dep_y = y-last_y;

  SDL_FillRect(layer_sol,    NULL, 0x00000000); //layer transparent
  SDL_FillRect(layer_decor,  NULL, 0x00000000);  //layer transparent  
  SDL_Rect rdr;
  rdr.x = 0;
  rdr.y = 0;
  rdr.w = MAP_W;
  rdr.h = MAP_H;
  full_redraw(layer_sol, layer_decor, x, y, zone, &rdr);
  
  last_x = x;
  last_y = y;
  last_zone = zone;

    return true;
}
