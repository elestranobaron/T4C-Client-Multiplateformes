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
 * \fn VSFInterface::select_palette(char *path)
 * \param char * : Path vers le fichier "spr_pal.txt"
 * \brief Cette fonction recherche pour chaque sprite index� la meilleure palette possible.
 * On �crit les associations dans un fichier "spr_pal.txt".
 * \todo R�utiliser le fichier "spr_pal.txt" pour ne pas avoir a refaire les asso a chaque fois
 */
void
VSFInterface::select_palette(char *path)
{
  char fname[255];
  sprintf(fname, "%s/spr_pal.txt", path);

  struct stat sb;
  if (stat(fname,&sb) != -1) //fichier de correspondance sprite/palette dispo
  {
    char line[255];
    char *c;
    FILE *fid = fopen(fname, "r");

    while(!feof(fid))
    {
      fgets(line, 255, fid);
      fflush(stdout);
      c = strchr(line, '#');

      if (!c)
      {
         printf("ERREUR > Erreur lors du parsing du fichier de palette [%s] !\n", line);
         fclose(fid);
         return;
      }  
      *c = '\0';
      c++;
      if (c[strlen(c)-1] == '\n') c[strlen(c)-1] = '\0';

      //recherche du sprite
      struct _sprite *spr = this->get_sprite_from_hash(hash(line), false);

      //recherche de la palette
      struct _palette *pal = liste_palettes;
      while ((pal)&&(strcmp(pal->nom, c)))
         pal = pal->next;

      if (pal&&spr)
         spr->palette = pal;
    }
    fclose(fid);
    

    //TODO : Fixer, c'est laid !
    //Certains sprites sont en double, donc un des 2 n'aura pas de palette... ici on v�rifie que tt le monde a bien la sienne
    struct _sprite *spr = liste_sprites;
    while (spr)
    {
     if (!spr->palette)
       spr->palette = find_palette(spr->nom);
     spr = spr->next;
    }

    return;
  }
  else //premier lancement, on g�n�re le fichier de correspondance sprite/palette
  {

    FILE *fid = fopen(fname, "w");
    struct _sprite *spr = liste_sprites;
    while (spr)
    {
     spr->palette = find_palette(spr->nom);
     fprintf(fid, "%s#%s", spr->nom, spr->palette->nom);
     spr = spr->next;
     if (spr) fprintf(fid, "\n");
    }
    fclose(fid);
  }  
}

/**
 * \fn VSFInterface::find_palette(char *)
 * \param char * : Nom du sprite
 * \return struct _palette * : pointeur vers la palette trouv�e.
 * \brief Cette fonction recherche la palette optimale pour le sprite dont le nom est pass� en argument. 
 * G�n�ralement il s'agit d'une palette dont le nom est une variante du nom du sprite. 
 * Sinon, c'est tout simplement 'Bright1'...
 * On calcule ici le hash du nom du sprite pour pouvoir y acc�der rapidement.
 */
struct _palette *
VSFInterface::find_palette(char *nom)
{
//######### recherche de la palette ######### 
 struct _palette *tmp, *goodone = NULL;
 char *palname;

palname = (char *)malloc(sizeof(char) * strlen(nom) + 2);
 //test avec "<nom_sprite>P"
 tmp = liste_palettes;
 strcpy(palname, nom);
 int ln = strlen(palname);
 palname[ln] = 'P';
 palname[ln+1] = '\0';
 while ((tmp)&&(!goodone))
 {
   if (!strcmp(tmp->nom, palname))
      goodone = tmp;
   tmp = tmp->next;
 }
free(palname);
 if (!goodone)
 {
   //test avec "P<nom_sprite>"
   tmp = liste_palettes;
   char *palname;
     palname = (char *)malloc(sizeof(char) * strlen(nom) + 2);
   sprintf(palname,"P%s", nom);
   while ((tmp)&&(!goodone))
   {
     if (!strcmp(tmp->nom, palname))
        goodone = tmp;
     tmp = tmp->next;
   }
     free(palname);
 }

 if (!goodone)
 {
   //test avec "xxxxxxxXXX-yyy" => "xxxxP"
   char *palname;
     palname = (char *)malloc(sizeof(char) * strlen(nom) + 2);
   strcpy(palname, nom);
   char *str;
        str = strstr(palname, "000");
    if (!str)
        str = strstr(palname, "045");
    if (!str)
        str = strstr(palname, "090");
    if (!str)
        str = strstr(palname, "135");
    if (!str)
        str = strstr(palname, "180");
    if (str)
    {
      int pos = strlen(palname) - strlen(str);
      
      palname[pos] = 'P';
      palname[pos+1] = '\0';
      tmp = liste_palettes;
      while ((tmp)&&(!goodone))
      {
        if (!strcmp(tmp->nom, palname))
          goodone = tmp;
        tmp = tmp->next;
      }
    }
     free(palname);
 }

if (!goodone)
 {
   //test avec "xxxxxxx YYY" => "xxxx"
   char *palname;
     palname = (char *)malloc(sizeof(char) * strlen(nom) + 2);
   strcpy(palname, nom);
   char *str;
   str = strchr(palname, ' ');

    if (str)
    {
      int pos = strlen(palname) - strlen(str);
      palname[pos] = '\0';
      tmp = liste_palettes;
      while ((tmp)&&(!goodone))
      {
        if (!strcmp(tmp->nom, palname))
          goodone = tmp;
        tmp = tmp->next;
      }
    }
     free(palname);
 }

 if (!goodone)
 {
   //test avec "xxxxxxx YYY" => "xxxxP"
   char *palname;
     palname = (char *)malloc(sizeof(char) * strlen(nom) + 2);
   strcpy(palname, nom);
   char *str;
   str = strchr(palname, ' ');

    if (str)
    {
      int pos = strlen(palname) - strlen(str);
      palname[pos] = 'P';
      palname[pos+1] = '\0';
      tmp = liste_palettes;
      while ((tmp)&&(!goodone))
      {
        if (!strcmp(tmp->nom, palname))
          goodone = tmp;
        tmp = tmp->next;
      }
    }
     free(palname);
 }
 
 if (!goodone)
 {
   //test avec "xxxxxxx-yyy" => "xxxxP"
   char *palname;
     palname = (char *)malloc(sizeof(char) * strlen(nom) + 2);
   strcpy(palname, nom);
   char *npal = strtok(palname, "-");
   int ln = strlen(npal);
   npal[ln] = 'P';
   npal[ln+1] = '\0';
   tmp = liste_palettes;
   while ((tmp)&&(!goodone))
   {
     if (!strcmp(tmp->nom, npal))
        goodone = tmp;
     tmp = tmp->next;
   }
     free(palname);
 }

 if (!goodone)
 {
   //test avec "Bright1"
   tmp = liste_palettes;
   char *palname;
     palname = (char *)malloc(sizeof(char) * strlen(nom) + 2);
   strcpy(palname,"Bright1");
   while ((tmp)&&(!goodone))
   {
     if (!strcmp(tmp->nom, palname))
        goodone = tmp;
     tmp = tmp->next;
   }
     free(palname);
 }
 
 if (!goodone)
 {
   //test avec "bright1"
   tmp = liste_palettes;
   char *palname;
     palname = (char *)malloc(sizeof(char) * strlen(nom) + 2);
   strcpy(palname,"bright1");
   while ((tmp)&&(!goodone))
   {
     if (!strcmp(tmp->nom, palname))
        goodone = tmp;
     tmp = tmp->next;
   }
     free(palname);
 }
 
 if (!goodone)
 {
   //test avec "brightx"
   tmp = liste_palettes;
   char *palname;
     palname = (char *)malloc(sizeof(char) * strlen(nom) + 2);
   strcpy(palname,"bright");
   while ((tmp)&&(!goodone))
   {
     if (strstr(tmp->nom, palname))
        goodone = tmp;
     tmp = tmp->next;
   }
     free(palname);
 }
//printf("%s -> %s\n", nom, goodone->nom);
 if (!goodone)   //la c'est mal, on en prend une au pif...
   goodone = liste_palettes;
  return goodone;
}
