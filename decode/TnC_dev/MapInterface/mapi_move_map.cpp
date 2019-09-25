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
 * \fn MAPInterface::move_map(SDL_Surface *, SDL_Surface *, int, int)
 * \param SDL_Surface *: layer sol
 * \param SDL_Surface *: layer decor
 * \param int : deplacement sur x
 * \param int : deplacement sur y
 * \return bool : r�ussite ou non
 * \brief D�placement des 2 layers selon les vecteurs dep_x, et dep_y.
 */
bool
MAPInterface::move_map(SDL_Surface *layer_sol, SDL_Surface *layer_decor, int dep_x, int dep_y)
{

  
  SDL_Rect src;
  SDL_Rect dst;
  if (dep_x > 0) //d�placement a dte
  { 
    src.x = 32*dep_x;
    src.w = MAP_W*32 - 32*dep_x;
    dst.x = 0;
    dst.w = MAP_W*32 - 32*dep_x;
  }  
  else
  {
    src.x = 0;
    src.w = MAP_W*32 + 32*dep_x;
    dst.x = -32*dep_x;
    dst.w = MAP_W*32 + 32*dep_x;
  }
  if (dep_y > 0)
  {
    src.y = 16*dep_y;
    src.h = MAP_H*16 - 16*dep_y;
    dst.y = 0;
    dst.h = MAP_H*16 - 16*dep_y;
  }
  else
  {
    src.y = 0;
    src.h = MAP_H*16 + 16*dep_y;
    dst.y = -16*dep_y;
    dst.h = MAP_H*16 + 16*dep_y;
  }
int tmptick  = SDL_GetTicks();
  SDL_Surface *tmp = SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_SRCCOLORKEY, MAP_W*32, MAP_H*16, 32, 0xFF000000,0x00FF0000,0x0000FF00,0x00000000);
  SDL_SetColorKey(tmp, SDL_SRCCOLORKEY, 0);

  SDL_BlitSurface(layer_sol, NULL, tmp, NULL);
  SDL_BlitSurface(tmp, &src, layer_sol, &dst);

  SDL_BlitSurface(layer_decor, NULL, tmp, NULL);
  SDL_BlitSurface(tmp, &src, layer_decor, &dst);

  SDL_FreeSurface(tmp);
  printf("BENCHMARK > [mapi_move_map] en %f s\n",   (float)(SDL_GetTicks()-tmptick)/1000);
}
