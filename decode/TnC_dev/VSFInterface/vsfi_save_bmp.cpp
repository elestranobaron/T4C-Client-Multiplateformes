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
 * \fn VSFInterface::save_bmp(struct _sprite *, char *)
 * \return bool : r�ussite ou non
 * \param struct _sprite * : pointeur vers la structure du sprite � enregistrer
 * \param char * : nom du bmp a cr�er
 * \brief Cette fonction enregistre le sprite pass� en argument dans un fichier bmp.
 */
bool VSFInterface::save_bmp(struct _sprite *spr, char *out_name)
{
  if (!spr) 
     return false;
  if (!spr->pixels)
    read_sprite(spr);
    
  char fname[255];
  sprintf(fname, "%s\\%s.bmp", out_name, spr->nom);
  write_bmp(fname, spr->largeur, spr->hauteur, spr->pixels, spr->palette);
  unload_sprite(spr);

    return true;
}

/**
 * \fn VSFInterface::save_bmp(char *, char *)
 * \return bool : r�ussite ou non
 * \param char * : nom du sprite � enregistrer
 * \param char * : nom du bmp a cr�er
 * \brief Cette fonction enregistre le sprite pass� en argument dans un fichier bmp.
 */
bool VSFInterface::save_bmp(char *sprite_name, char *out_name)
{
  struct _sprite *spr = get_sprite_by_name(sprite_name);
  if (!spr) 
     return false;
  if (!spr->pixels)
    read_sprite(spr);
    
  char fname[255];
  sprintf(fname, "%s\\%s.bmp", out_name, sprite_name);
  write_bmp(fname, spr->largeur, spr->hauteur, spr->pixels, spr->palette);
  unload_sprite(spr);
  return true;
  /*
  //Sauvegarde avec ttes les palettes
  struct _palette *pal = liste_palettes;
  while (pal)
  {
   char fname[255];
   sprintf(fname, "%s\\%s %s.bmp", out_name, sprite_name, pal->nom);
   write_bmp(fname, spr->data->largeur, spr->data->hauteur, spr->data->pixels, pal);
   pal = pal->next;
  }*/
}

/**
 * \fn VSFInterface::write_bmp(char *, int, int, unsigned char *, struct _palette *)
 * \param char * : Nom du fichier � cr�er
 * \param int : largeur
 * \param int : hauteur
 * \param unsigned char * :  data
 * \param struct _palette * : palette � utiliser
 * \brief Cette fonction �crit les datas pass�es en argument dans un fichier bmp.
 */
void
VSFInterface::write_bmp(char *f_name, int largeur, int hauteur, unsigned char *matrix, struct _palette *pal)
{

 int i,j;
 FILE *spr;
 spr = fopen(f_name, "wb");
 if (!spr)
  return;
 int _int;
 short _short;
 char _char;
 int pad_largeur = largeur;
 while (pad_largeur - ((int)(pad_largeur/4))*4 != 0)
   pad_largeur ++;
   
 //BMP
 ////int full_size = 0;
 fprintf(spr, "BM");
 _int = 1078 + largeur*hauteur;
 fwrite(&_int, sizeof(int),1,spr);          //taille totale X
 fwrite(&_int, sizeof(int),1,spr);         //NC
 _int = 1078;
 fwrite(&_int, sizeof(int),1,spr);         //offset data X
 _int = 40;
 fwrite(&_int, sizeof(int),1,spr);         //taille header
 _int = largeur;
 fwrite(&_int, sizeof(int),1,spr);         //largeur
 _int = hauteur;
 fwrite(&_int, sizeof(int),1,spr);         //hauteur
 _short = 1;
 fwrite(&_short, sizeof(short), 1, spr);     //nb plans
 short huit = 8;
 fwrite(&huit, sizeof(short), 1, spr);     //bits palette
 _int = 0;
 fwrite(&_int, sizeof(int),1,spr);                     //compression


 
 _int = pad_largeur*hauteur;
 fwrite(&_int, sizeof(int),1,spr);       //taille de la zone de donnees (arondi 4)

 _int = largeur;
 fwrite(&_int, sizeof(int), 1, spr);         //largeur/m
 _int = hauteur;
 fwrite(&_int, sizeof(int), 1, spr);         //hauteur/m
 _int = 256;
 fwrite(&_int, sizeof(int),1,spr);    //bpp
 _int  = 0;//couleurs importantes
 fwrite(&_int, sizeof(int),1,spr);
 for (i=0; i < 256; i++)                   //palette
 {
   _char = pal->rgb[i].b;
   fwrite(&_char, sizeof(char), 1, spr);
   _char = pal->rgb[i].g;
   fwrite(&_char, sizeof(char), 1, spr);
   _char = pal->rgb[i].r;
   fwrite(&_char, sizeof(char), 1, spr);
   _char = 0;
   fwrite(&_char, sizeof(char), 1, spr);
 }


 for (i=hauteur-1; i >= 0; i--)
 {
  for (j = 0; j < largeur; j++)
  {
    _char = matrix[i*largeur+j];
    fwrite(&_char, sizeof(char), 1, spr);
  }
  for (j = 0; j < pad_largeur - largeur; j++)
  {
    _char = 0;
    fwrite(&_char, sizeof(char), 1, spr);
  }
 }

  fclose(spr);
}
