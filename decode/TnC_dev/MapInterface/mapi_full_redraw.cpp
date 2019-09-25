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

// No description
void
MAPInterface::full_redraw(SDL_Surface *layer_sol, SDL_Surface *layer_decor, unsigned int x, unsigned int y, unsigned short zone, SDL_Rect *rdr_zone)
{
//  printf("recolle les sprites sur les colonnes : %d->%d Lignes %d->%d\n", rdr_zone->x, rdr_zone->x+rdr_zone->w, rdr_zone->y, rdr_zone->y+rdr_zone->h);
    (void)zone;
  int tmptick  = SDL_GetTicks();

  SDL_Rect clr_zone;
  clr_zone.x = rdr_zone->x * 32; 
  clr_zone.w = rdr_zone->w * 32;
  clr_zone.y = rdr_zone->y * 16;
  clr_zone.h = rdr_zone->h * 16;
  SDL_FillRect(layer_sol,   &clr_zone, 0x000000FF); 
  SDL_FillRect(layer_decor, &clr_zone, 0x00000000); 

   SDL_Rect rec;
   rec.x = 0;
   rec.y = 0;
   rec.w = 32;
   rec.h = 16;
   struct _sprite *spr = NULL;
   struct _sprite *spr_sol = NULL;
   unsigned int last_id = 0;

  int tot_get = 0;
  ////int tik;
  
  //calcul de la loc, dans le buffer de map
   int offy = y - map_y + MAPBUF_H/2 - MAP_H/2;
   int offx = x - map_x + MAPBUF_W/2 - MAP_W/2;

   for (int j = rdr_zone->y; j < rdr_zone->y+rdr_zone->h; j++)
     for (int i = rdr_zone->x; i < rdr_zone->x+rdr_zone->w; i++)
     {
       //printf("redessine sprite loc %d,%d\n", i,j);
        int OFF = (j + offy)*MAPBUF_W + (i + offx);            //offset du sprite dans le buffer de map
  
        //on recharge pas un sprite qu'on vient d'afficher
        if (map[OFF] != last_id)
        {
          last_id = map[OFF];
          int tik;
          tik = SDL_GetTicks();
          spr = vsfi->get_sprite_from_hash(map[OFF]); 
          tot_get += SDL_GetTicks() - tik;
        }

        if (!debug&&spr) //-->sprite connu
        {
            if (spr->sol&&(i >= MARGE_W)&&(i < MAP_W-MARGE_W)&&(j >= MARGE_H)&&(j < MAP_H - MARGE_H)) //->Sol
            {
              rec.x = i*32+spr->offset_x-MARGE_W*32;
              rec.y = j*16+spr->offset_y-MARGE_H*16;

              //Gestion des sprites masques (pour les transitions de terrain)
              if ((map[OFF] == 0x54d9a4de)||(map[OFF] == 0x54d9a4f7)||(map[OFF] == 0x54d9a4f9)||(map[OFF] == 0x54d9a4fb)||(map[OFF] == 0x54d9a4fd)||(map[OFF] == 0x54d9a4fe)||(map[OFF] == 0x54d9a500)||(map[OFF] == 0x4c44a7e8)||(map[OFF] == 0x54d9a519))        //s�paration verticale (B|V)
              {
                //prend les 2 sprites a fusionner
                struct _sprite *prev = vsfi->get_sprite_from_hash(map[(j + offy)*MAPBUF_W + (i + offx - 1)]);
                struct _sprite *next = vsfi->get_sprite_from_hash(map[(j + offy)*MAPBUF_W + (i + offx + 1)]);
                SDL_Surface *f = fusion(spr, prev, next, 0, 168);
                SDL_BlitSurface(f, NULL, layer_sol, &rec);
                SDL_FreeSurface(f);
              }
              else if ((map[OFF] == 0x4c44a7e9)||(map[OFF] == 0x54d9a4dd)||(map[OFF] == 0x54d9a4df)||(map[OFF] == 0x54d9a4f8)||(map[OFF] == 0x54d9a4fa)||(map[OFF] == 0x54d9a4fc)||(map[OFF] == 0x54d9a4ff)||(map[OFF] == 0x54d9a518)||(map[OFF] == 0x54d9a51a)||(map[OFF] == 0x54d9a51c))   //s�paration verticale (V|B)
              {
                //prend les 2 sprites a fusionner
                struct _sprite *prev = vsfi->get_sprite_from_hash(map[(j + offy)*MAPBUF_W + (i + offx - 1)]);
                struct _sprite *next = vsfi->get_sprite_from_hash(map[(j + offy)*MAPBUF_W + (i + offx + 1)]);
                SDL_Surface *f = fusion(spr, next, prev, 0, 168);
                SDL_BlitSurface(f, NULL, layer_sol, &rec);
                SDL_FreeSurface(f);
              }
              else if ((map[OFF] == 0x4c44a7e6)||(map[OFF] == 0x54d9a4d6)||(map[OFF] == 0x54d9a4d9)||(map[OFF] == 0x54d9a4da)||(map[OFF] == 0x4c44a7ea)||(map[OFF] == 0x4c44a7ee))        //s�paration horizontale (B/V)
              {
                //prend les 2 sprites a fusionner
                struct _sprite *prev = vsfi->get_sprite_from_hash(map[(j - 1 + offy)*MAPBUF_W + (i + offx)]);
                struct _sprite *next = vsfi->get_sprite_from_hash(map[(j + 1 + offy)*MAPBUF_W + (i + offx)]);
                SDL_Surface *f = fusion(spr, next, prev, 0, 168);
                SDL_BlitSurface(f, NULL, layer_sol, &rec);
                SDL_FreeSurface(f);
              }
              else if ((map[OFF] == 0x54d9a4d7)||(map[OFF] == 0x54d9a4d6)||(map[OFF] == 0x54d9a4d8)||(map[OFF] == 0x54d9a4db)||(map[OFF] == 0x54d9a4dc)||(map[OFF] == 0x4c44a7e7)||(map[OFF] == 0x4c44a7ec)||(map[OFF] == 0x4c44a7ed))   //s�paration horizontale (V/B)
              {
                //prend les 2 sprites a fusionner
                struct _sprite *prev = vsfi->get_sprite_from_hash(map[(j - 1 + offy)*MAPBUF_W + (i + offx)]);
                struct _sprite *next = vsfi->get_sprite_from_hash(map[(j + 1 + offy)*MAPBUF_W + (i + offx)]);
                SDL_Surface *f = fusion(spr, prev, next, 0, 168);
                SDL_BlitSurface(f, NULL, layer_sol, &rec);
                SDL_FreeSurface(f);
              }  
              else
                SDL_BlitSurface(spr->sdl_sprite, NULL, layer_sol, &rec);
             }
             else if (!spr->sol)         //->D�cor
             {
               //recolle sous le d�cor le sprite de sol d'en dessous
               if (j != rdr_zone->y+rdr_zone->h-1)
               {
                  int OFFsol = (j+1 + offy)*MAPBUF_W + (i + offx);
                  spr_sol = vsfi->get_sprite_from_hash(map[OFFsol]); 
                  if (spr_sol&&spr_sol->sol)
                  {
                   rec.x = i*32+spr_sol->offset_x-MARGE_W*32;
                   rec.y = j*16+spr_sol->offset_y-MARGE_H*16;
                   SDL_BlitSurface(spr_sol->sdl_sprite, NULL, layer_sol, &rec);
                 }  
               }  
               rec.x = i*32+spr->offset_x-MARGE_W*32;
               rec.y = j*16+spr->offset_y-MARGE_H*16;
               SDL_BlitSurface(spr->sdl_sprite, NULL, layer_decor, &rec);
             }
        }
        else //--> sprite inconnu (affiche la valeur en hexa)
        {
          rec.x = i*32-MARGE_W*32;
          rec.y = j*16-MARGE_H*16;
          rec.w = 32;
          rec.h = 16;
          char bla[10];
          if (debug||(map[OFF] < 0x0000FFFF))          //sprite sur un short non mapp�
            sprintf(bla,"%04x", map[OFF]);
          else                                //sprite mapp� non trouv� ! (c'est mal.)
          {
            printf("ERREUR > Sprite de hash 8octets non trouv� : %08x\n", map[OFF]);
            sprintf(bla,"ERR!"); //< - L�, c'est grave !
          }
          SDL_Surface *txt_srf = fm->get_text(bla);
          if (map[OFF] < 0x0000FFFF)
            SDL_FillRect(layer_decor, &rec, 0x4B68C4FF);
          else
            SDL_FillRect(layer_decor, &rec, 0xD6A81DFF);
          SDL_BlitSurface(txt_srf, NULL, layer_decor, &rec);
          SDL_FreeSurface(txt_srf);    
        }
     }
   printf("BENCHMARK > [total get_sprite_from_hash] : %f s\n", (float) tot_get/1000);
   printf("BENCHMARK > [mapi_full_redraw] en %f s\n",   (float)(SDL_GetTicks()-tmptick)/1000);
}


inline SDL_Surface *MAPInterface::fusion(struct _sprite *spr, struct _sprite * prev, struct _sprite *next, unsigned char C1, unsigned char C2)
{
    //cr�e un sprite temporaire
    SDL_Surface *out = SDL_CreateRGBSurface(SDL_HWSURFACE, 32, 16, 32, 0xFF000000,0x00FF0000,0x0000FF00,0x00000000);

    //cree une copie du masque
    SDL_Surface *mask = SDL_ConvertSurface(spr->sdl_sprite, spr->sdl_sprite->format, SDL_HWSURFACE);
    
    //cr�e un sprite temporaire
    SDL_Surface *tmp = SDL_CreateRGBSurface(SDL_HWSURFACE, 32, 16, 32, 0xFF000000,0x00FF0000,0x0000FF00,0x00000000);


    //colle le 1er
    if (prev) SDL_BlitSurface(prev->sdl_sprite, NULL, out, NULL);
    SDL_SetColorKey(mask, SDL_SRCCOLORKEY, SDL_MapRGB(mask->format, 0, C1, C2));
    SDL_BlitSurface(mask, NULL, out, NULL);
    
    if (next) SDL_BlitSurface(next->sdl_sprite, NULL, tmp, NULL);
    SDL_FreeSurface(mask);
    mask = SDL_ConvertSurface(spr->sdl_sprite, spr->sdl_sprite->format, SDL_HWSURFACE);
    SDL_SetColorKey(mask, SDL_SRCCOLORKEY, SDL_MapRGB(mask->format, 0, C2, C1));
    SDL_BlitSurface(mask, NULL, tmp, NULL);
    SDL_SetColorKey(tmp, SDL_SRCCOLORKEY, SDL_MapRGB(tmp->format, 0, C1, C2));
    SDL_BlitSurface(tmp, NULL, out, NULL);
    SDL_FreeSurface(tmp);
    SDL_FreeSurface(mask);
    return out;
}

