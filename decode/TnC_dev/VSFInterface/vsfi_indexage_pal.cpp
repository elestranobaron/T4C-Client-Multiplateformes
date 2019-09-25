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
 * \fn VSFInterface::scanpal(char *, struct _file*)
 * \param char * : Nom du fichier � indexer
 * \param struct _file * : structure de fichier 
 * \brief Cette fonction lit le fichier pass� en argument en se basant sur la structure de fichier et indexe les palettes qui s'y trouvent.
 */
void
VSFInterface::scanpal(char *nom_f, struct _file*f)
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
   //   char *nom;

   fread(&type, sizeof(type), 1, fid);
   fread(&offset, sizeof(offset), 1, fid);
   fread(&lg_nom, sizeof(lg_nom), 1, fid);
   //   nom = (char*)malloc((lg_nom+1)*sizeof(char));
   fread(nom, sizeof(char), lg_nom, fid);//sizeof(255)
   nom[lg_nom] = '\0';//printf("toto:%u:toto", ftell(fid));
   int pos = ftell(fid);
   if (type == 2)
      add_pal(nom, offset, fid);
   fseek(fid, pos, SEEK_SET);
      //free(nom);
  }
  fclose(fid);
}

/**
 * \fn VSFInterface::add_pal(char *, int, FILE *)
 * \param char * : Nom de la palette � ajouter.
 * \param int : offset de la palette dans le fichier.
 * \param FILE * : fid du fichier contenant la palette.
 * \brief Cette fonction ajoute la palette � la liste des palettes.
 */
void
VSFInterface::add_pal(char *nom, int offset, FILE *fid)
{
 struct _palette *pal = liste_palettes;
 struct _palette *pres = NULL;
 while (pal)
 {
   if (!strcmp(nom, pal->nom))
     pres = pal;
   pal = pal->next;
 }
 
 if (!pres)
 {
   fseek(fid, offset, SEEK_SET);

   struct _palette *np;
   np = (struct _palette *) malloc(sizeof(struct _palette));
//printf("%s\n", nom);
   int id_parent;
   fread(&id_parent, sizeof(int), 1, fid);
   np->nom = (char *) malloc((strlen(nom)+1)*sizeof(char));
   strcpy(np->nom, nom);

   int i;
   //unsigned char r,g,b;
   for (i=0; i < 256; i++)
   {
     fread(&np->rgb[i].r, sizeof(unsigned char),1, fid);
     fread(&np->rgb[i].g, sizeof(unsigned char),1, fid);
     fread(&np->rgb[i].b, sizeof(unsigned char),1, fid);
   }
   np->next = liste_palettes;
   liste_palettes = np;
 }
}
