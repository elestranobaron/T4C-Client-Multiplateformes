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
#include "vsfinterface.h"

/**
 * \fn VSFInterface::read_sprite(struct _sprite *)
 * \param struct _sprite * : pointeur vers la structure du sprite � charger
 * \brief Cette fonction charge en m�moire le sprite dont la structure est pass�e en argument. 
 * Si le sprite est d�ja charg�, on ne fait rien.. 
 * Sinon, on va lire le fichier VSF, on charge en m�moire tout ce qui n'a pas �t� charg� lors de l'indexage (offsets, data, etc)
 * Si le sprite est flagg� comme �tant un sprite miroir, on inverse la zone de pixels
 */
void
VSFInterface::read_mirrored_sprite(struct _sprite *spr)
{
  read_sprite(spr, true);
}

/**
 * \fn VSFInterface::read_sprite(struct _sprite *)
 * \param struct _sprite * : pointeur vers la structure du sprite � charger
 * \brief Cette fonction charge en m�moire le sprite dont la structure est pass�e en argument. 
 * Si le sprite est d�ja charg�, on ne fait rien.. 
 * Sinon, on va lire le fichier VSF, on charge en m�moire tout ce qui n'a pas �t� charg� lors de l'indexage (offsets, data, etc)
 * Si le sprite est flagg� comme �tant un sprite miroir, on inverse la zone de pixels
 */
void
VSFInterface::read_sprite(struct _sprite *spr)
{
  read_sprite(spr, false);
}
  
/**
 * \fn VSFInterface::read_sprite(struct _sprite *)
 * \param struct _sprite * : pointeur vers la structure du sprite � charger
 * \param bool : cr�er le miroir du sprite ?
 * \brief Cette fonction charge en m�moire le sprite dont la structure est pass�e en argument. 
 * Si le sprite est d�ja charg�, on ne fait rien.. 
 * Sinon, on va lire le fichier VSF, on charge en m�moire tout ce qui n'a pas �t� charg� lors de l'indexage (offsets, data, etc)
 * Si le sprite est flagg� comme �tant un sprite miroir, on inverse la zone de pixels
 */
void
VSFInterface::read_sprite(struct _sprite *spr, bool creer_miroir)
{

  if (!creer_miroir&&spr->sdl_sprite) return;       //chargement d'un sprite simple, d�ja charg�..
  if (creer_miroir&&spr->sdl_sprite_miroir) return; //chargement d'un sprite miroir d�ja charg� 
  //printf("chargement du sprite%s %s Offset init = [%d, %d]\n", creer_miroir?" miroir":"",spr->nom, spr->offset_x, spr->offset_y);
  FILE *fid = fopen(spr->fichier->nom, "rb");
  fseek(fid, spr->offset, SEEK_SET);


  int id_parent;
  fread(&id_parent, sizeof(int), 1, fid);
  unsigned char type;
  fread(&type, sizeof(char), 1, fid);
  unsigned char pad;
  fread(&pad, sizeof(char), 1, fid);
  fread(&spr->largeur, sizeof(short), 1, fid);
  fread(&spr->hauteur, sizeof(short), 1, fid);
  
  short tmpoff;
  fread(&tmpoff, sizeof(short), 1, fid);
  if ((!spr->sdl_sprite)&&(!spr->sdl_sprite_miroir))
    spr->offset_x += tmpoff;

  fread(&tmpoff, sizeof(short), 1, fid);
  if ((!spr->sdl_sprite)&&(!spr->sdl_sprite_miroir))
    spr->offset_y += tmpoff;

  fread(&tmpoff, sizeof(short), 1, fid);
  if ((!spr->sdl_sprite)&&(!spr->sdl_sprite_miroir))
    spr->offset_mx += tmpoff;
  fread(&tmpoff, sizeof(short), 1, fid);
  if ((!spr->sdl_sprite)&&(!spr->sdl_sprite_miroir))
    spr->offset_my += tmpoff;

  
  unsigned char pad2;
  fread(&pad2, sizeof(char), 1, fid);
  fread(&pad, sizeof(char), 1, fid);

  int sol;
  fread(&sol, sizeof(int), 1, fid);
  sol==0?spr->sol=true:spr->sol=false;
  fread(&spr->transparence, sizeof(char), 1, fid);
  fread(&pad, sizeof(char), 1, fid);
  fread(&pad, sizeof(char), 1, fid);
  fread(&pad, sizeof(char), 1, fid);
  int data_size;
  fread(&data_size, sizeof(int), 1, fid);

  //lecture des pixels
  spr->pixels_miroir = NULL;
  spr->pixels = (unsigned char*) malloc(spr->hauteur*spr->largeur*sizeof(unsigned char));

  int i,j;
   for (i=0; i < spr->hauteur; i++)
    for (j = 0; j < spr->largeur; j++)
     spr->pixels[i*spr->largeur+j] = spr->transparence;

  //char c;
  if (type == 1)  //non compresse
  {
   int i,j;
   for (i=0; i < spr->hauteur; i++)
    for (j = 0; j < spr->largeur; j++)
     fread(&spr->pixels[i*spr->largeur+j], sizeof(char), 1, fid);
  }
  else if (type == 2)  //compresse RLE
  {
   unsigned char data[data_size];
   fread(&data, sizeof(char), data_size, fid);
   
   
   unsigned short x = 0;
   unsigned short y = 0;
   unsigned int nb_pix;
   //unsigned char *pt_dpack = &spr->pixels[0];
   unsigned char *pt_pack = data;

   while(1)
   {
     // Index du premier pixel � decompresser
     x=*(short *)pt_pack;
     pt_pack+=2;
     // Nombre de pixels � decompresser
     nb_pix = pt_pack[0]*4+pt_pack[1];
     pt_pack+=2;
     //pt_dpack = &data[x+(y*spr->largeur)];

     if (*pt_pack++ != 1)                 //pas un, on repete
     {
        for (i=0; i<(int)nb_pix; i++)
          if (i+x < spr->largeur*spr->hauteur)
             spr->pixels[y*spr->largeur+i+x] = *pt_pack++;
     }
     else                                 //1 => on laisse comme c'est (transparent)
        x+=nb_pix;

     if (*pt_pack == 0)   // Fin de sprite
        break;

     if (*pt_pack++ == 2) // Fin de ligne
        y++;

     if (y==spr->hauteur)           // Par s�curit�
        break;
    }
  }



  //miroir a la place du sprite normal ?
  if (spr->miroir)
  {
    //inverse les offsets
    int t;
    t = spr->offset_mx;
    spr->offset_mx = spr->offset_x;
    spr->offset_x = t;
    t = spr->offset_my;
    spr->offset_my = spr->offset_y;
    spr->offset_y = t;

    //miroir du sprite
    unsigned char c;
    for (int i = 0; i < spr->hauteur; i++)
      for (int j = 0; j < spr->largeur/2; j++)
      {
        c = spr->pixels[i*spr->largeur + j];
        spr->pixels[i*spr->largeur + j] = spr->pixels[i*spr->largeur + spr->largeur - j -1];
        spr->pixels[i*spr->largeur + spr->largeur - j -1] = c;
      }
  } 
  spr->sdl_sprite = SDL_CreateRGBSurfaceFrom(spr->pixels, spr->largeur, spr->hauteur, 8, spr->largeur, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
  SDL_SetColors(spr->sdl_sprite, spr->palette->rgb, 0, 256);             //cree la palette
  SDL_SetColorKey(spr->sdl_sprite, SDL_SRCCOLORKEY, spr->transparence);  //transparence


  //miroir en plus du sprite normal ?
  if (creer_miroir)
  {
    //lecture des pixels
    if (spr->sdl_sprite_miroir) exit(1);
    spr->pixels_miroir = (unsigned char*) malloc(spr->hauteur*spr->largeur*sizeof(unsigned char));
    memcpy(spr->pixels_miroir, spr->pixels, spr->hauteur*spr->largeur*sizeof(unsigned char));

    unsigned char c;
    for (int i = 0; i < spr->hauteur; i++)
      for (int j = 0; j < floor(spr->largeur/2); j++)
      {
        c = spr->pixels_miroir[i*spr->largeur + j];
        spr->pixels_miroir[i*spr->largeur + j] = spr->pixels_miroir[i*spr->largeur + spr->largeur - j -1];
        spr->pixels_miroir[i*spr->largeur + spr->largeur - j -1] = c;
      }
    spr->sdl_sprite_miroir = SDL_CreateRGBSurfaceFrom(spr->pixels_miroir, spr->largeur, spr->hauteur, 8, spr->largeur, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    SDL_SetColors(spr->sdl_sprite_miroir, spr->palette->rgb, 0, 256);
    SDL_SetColorKey(spr->sdl_sprite_miroir, SDL_SRCCOLORKEY, spr->transparence);
  }
  
  fclose(fid);
}
