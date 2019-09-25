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
 * \fn VSFInterface::scandirs(char *, struct _file *)
 * \param char * : Nom du fichier � scanner.
 * \param struct _file * : structure d'info sur le fichier � scanner
 * \brief Cette fonction scanne le fichier pass� en argument et indexe les r�pertoires du vsf.
 */
void
VSFInterface::scandirs(char *nom_f, struct _file *f)
{
 FILE *fid = fopen(nom_f, "rb");

  //lecture du header
 int lng;
 fread(&lng, sizeof(int), 1, fid);

 
 int nb_chunks;
 fread(&nb_chunks, sizeof(int), 1, fid);
 
 int c_dossiers;
 fread(&c_dossiers, sizeof(int), 1, fid);
 f->offset_dossiers = c_dossiers;

 int c_fichiers;
 fread(&c_fichiers, sizeof(int), 1, fid);
 f->offset_fichiers = c_fichiers;

 
 //lecture des dossiers
 fseek(fid, c_dossiers, SEEK_SET);
 fread(&f->nb_dossiers, sizeof(int), 1, fid);;


//printf("nb dossiers : %d  off d %d, off f : %d chunks : %d\n", f->nb_dossiers, c_dossiers, c_fichiers, nb_chunks);
 int i;
 for (i=0; i < (int)f->nb_dossiers; i++)
 {
   unsigned char  type;
   unsigned int   index;
   unsigned int   parent;
   unsigned short lg_nom;
   char           *nom;

   fread(&type, sizeof(unsigned char),1, fid);
   fread(&index, sizeof(unsigned int),1, fid);
   fread(&parent, sizeof(unsigned int),1, fid);
   fread(&lg_nom, sizeof(unsigned short),1, fid);
   nom = (char *) malloc((lg_nom+1)*sizeof(char));
   fread(nom, sizeof(char),lg_nom, fid);
   nom[lg_nom] = '\0';

   if (type == 1)
      add_dir(nom, index, parent);
     free(nom);
 }

 fclose(fid);
}

/**
 * \fn VSFInterface::add_dir(char *, int, int)
 * \param char * : Nom du fichier � ajouter.
 * \param int : Id du dossier
 * \param int : Id du parent
 * \brief Cette fonction ajoute le r�pertoire � la liste des r�pertoires
 */
void
VSFInterface::add_dir(char * nom, int id, int id_parent)
{
  //recherche si on l'a d�ja
  struct _dossier *d = liste_dossiers;
  struct _dossier *pres = NULL;
  while (d)
  {
    if ((!strcmp(nom, d->nom))&&(d->id == id)&&(d->id_parent == id_parent))
       pres = d;
    d = d->next;
  }
  if (!pres)
  {
    struct _dossier *nd;
    nd = (struct _dossier*) malloc(sizeof(struct _dossier));
    nd->nom = (char *) malloc((strlen(nom)+1) *sizeof(char));
    strcpy(nd->nom, nom);
    nd->id = id;
    nd->id_parent = id_parent;
    nd->next = liste_dossiers;
    liste_dossiers = nd;
  }
}

/**
 * \fn VSFInterface::get_full_path(struct _dossier *, char *)
 * \param struct _dossier * : structure du dossier dont il faut trouver le parent.
 * \param char * path courant.
 * \brief Fonction r�cursive permettant de retrouver le path complet d'un r�pertoire.
 */
char *
VSFInterface::get_full_path(struct _dossier *doss, char *path)
{
  if (doss->id_parent == 0)
  {
   sprintf(path, "%s\\%s", path, doss->nom);
   return path;
   }
  else
  {
   struct _dossier *d = liste_dossiers;
   while (d&&(d->id != doss->id_parent))
     d = d->next;

   sprintf(path, "%s\\%s",get_full_path(d, path), doss->nom);
   return path;
  }
}
