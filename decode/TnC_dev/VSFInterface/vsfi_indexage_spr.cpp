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
 * \fn VSFInterface::scansprites(char *, struct _file *)
 * \param char * : Nom du fichier � indexer.
 * \param struct _file * : structure d'info sur le fichier � scanner.
 * \brief Cette fonction scanne le fichier pass� en argument et indexe les sprites du vsf.
 * On ne stocke ici que les informations permettant de savoir o� trouver chaque sprite.
 */
void
VSFInterface::scansprites(char *nom_f, struct _file * f)
{
  unsigned int nb_fichiers;
  int i;
  FILE *fid = fopen(nom_f, "rb");
  fseek(fid, f->offset_fichiers, SEEK_SET);
  fread(&nb_fichiers, sizeof(int), 1, fid);
  
  for (i=0; i < (int)nb_fichiers; i++)
  {
   unsigned char  type;
   unsigned int   offset;
   unsigned short lg_nom;
   char           nom[255];

   fread(&type, sizeof(type), 1, fid);
   fread(&offset, sizeof(offset), 1, fid);
   fread(&lg_nom, sizeof(lg_nom), 1, fid);
      //nom = (char*)malloc(sizeof(char)*(lg_nom+1));
   fread(nom, sizeof(char), lg_nom, fid);
   nom[lg_nom] = '\0';//int pos = 0;if (ftell(fid) != -1)
   int pos = ftell(fid);
   
   if (type == 1)
   {
      fseek(fid, offset+20, SEEK_SET); //va lire le flag "sol"
      int sol;
      fread(&sol, sizeof(int), 1, fid);
      add_sprite(nom, offset, sol, f);
   }     
   fseek(fid, pos, SEEK_SET);
      //free(nom);
  }
  fclose(fid);
}

/**
 * \fn VSFInterface::add_sprite(char *, int, struct _file *)
 * \param char * : Nom du sprite � ajouter.
 * \param int : offset du sprite dans le fichier
 * \param struct _file * : pointeur vers la structure du fichier VSF.
 * \brief Cette fonction ajoute un sprite � la liste des sprites. 
 * On calcule ici le hash du nom du sprite pour pouvoir y acc�der rapidement.
 */
void
VSFInterface::add_sprite(char *nom, int offset, int flag_sol, struct _file *f)
{
  struct _sprite *spr = (struct _sprite*) malloc(sizeof(struct _sprite));
  spr->nom = (char *) malloc((strlen(nom)+1)*sizeof(char));
  strcpy(spr->nom, nom);

  spr->offset = offset;
  spr->fichier = f;
  spr->hash = hash(spr->nom);
  spr->miroir = false;
    //printf("Add Sprite > %s [%08x] %s -> hash %08x\n", f->nom, offset, nom, spr->hash);

  spr->palette = NULL;
  flag_sol?spr->sol = false:spr->sol=true;
  spr->largeur = -1;
  spr->hauteur = -1;
  spr->offset_x = 0;
  spr->offset_y = 0;
  spr->transparence = 0;
  spr->pixels = NULL;
  spr->pixels_miroir = NULL;
  spr->sdl_sprite = NULL;
  spr->sdl_sprite_miroir = NULL;
  spr->next = liste_sprites;
  liste_sprites = spr;
}

