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

#include "fontmanager.h" // class's header file

//STRING :  !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_'abcdefghijklmnopqrstuvwxyz{|}~

//TODO : G�rer les chars non pr�vus !!

/**
 * \fn FontManager::FontManager()
 * \brief Constructeur de la classe de Font (ne fait rien).
 */
FontManager::FontManager()
{

}

/**
 * \fn FontManager::~FontManager()
 * \brief Destructeur de la classe de Font (ne fait rien).
 */
FontManager::~FontManager()
{

}

/**
 * \fn FontManager::load_font(char *)
 * \param char * : path ou se trouve la font (  <path>.bmp et <path>.txt  )
 * \return bool : r�ussite ou non
 * \brief Charge une font qui devient alors la font courante. Le fichier d'index permet de parser la bmp pour r�cup�rer les caract�res.
 */
bool FontManager::load_font(char *path)
{
  //charge l'image
  char tmp[255];
  sprintf(tmp,"%s.bmp", path);
  this->font = SDL_LoadBMP(tmp);
  
  
  if (!this->font)
  {
    printf("Erreur : [FontManager] Impossible d'ouvrir la font !! [%s]\n", path);
    return false;
  }
  SDL_SetColorKey(font,    SDL_SRCCOLORKEY, 1);

  //lit l'index des chars
  FILE *fonti;
  sprintf(tmp,"%s.txt", path);
  if ((fonti = fopen(tmp, "r")))
  {
      int i = 0;
      int tmp;
      while (!feof(fonti)&&(i < MAX_CHAR))
      {
        fscanf(fonti,"%d",&tmp);
        index[i] = tmp;
        i++;
      }
      fclose(fonti);
      if (i != MAX_CHAR)
      {
        printf("Erreur : [FontManager] Pas assez de chars dans l'index (%d)!!\n", 1234);
        return false;
      }
  }
  else 
  {
    printf("Erreur : [FontManager] Impossible d'ouvrir l'index !!\n");
    return false;
  }

  return true;
}




/**
 * \fn FontManager::get_text(char *)
 * \param char * : texte � afficher
 * \return SDL_Surface * : Surface contenant le texte � afficher
 * \brief Cette fonction cr�e une SDL_Surface affichant le texte pass� en argument, et en utilisant la Font courante.
 */
SDL_Surface * 
FontManager::get_text(char *text)
{
  int largeur_texte = 0;
  int nb_lignes = 1;
  int i;//,j;
  int tmp_largeur = 0;

 //Mesure les dimensions du texte
  for (i = 0; i < (int)strlen(text); i++)
  {
    if (text[i] == '\n')
    {
      if (tmp_largeur > largeur_texte) largeur_texte = tmp_largeur;
      tmp_largeur = 0;
      nb_lignes ++;
    }
    else
    {
      short larg;
      if ((text[i] - 32) == 0)
        larg = index[text[i]-32];
      else
        larg = index[text[i]-32] - index[text[i]-32-1];
      tmp_largeur += larg;
    }
  }
  if (tmp_largeur > largeur_texte) largeur_texte = tmp_largeur;


  //Cr�e l'image
  SDL_Surface *sdltext;
  sdltext = SDL_CreateRGBSurface(SDL_HWSURFACE, largeur_texte, nb_lignes*(font->h+2), 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);


  //Blit
  SDL_Rect dest_pos;  //destination
  dest_pos.y = 0;
  dest_pos.h = font->h;
  dest_pos.x = 0;
  dest_pos.w = largeur_texte;

  SDL_Rect src_pos;  //Source
  src_pos.x = 0;
  src_pos.w = 0;
  src_pos.y = 0;
  src_pos.h = font->h;
  for (i = 0; i < (int)strlen(text); i++)
  {
    if (text[i] == '\n')
    {
      dest_pos.x = 0;
      dest_pos.y += font->h+2;
    }
    else
    {
      if ((text[i] - 32) == 0)
      {
        src_pos.w = index[text[i]-32];
        src_pos.x = 0;
      }
      else
      {
        src_pos.w = index[text[i]-32] - index[text[i]-32-1];
        src_pos.x = index[text[i]-32-1];
      }

      SDL_BlitSurface(font, &src_pos, sdltext, &dest_pos);
      dest_pos.x += src_pos.w;
    }
  }
  return sdltext;
}


/**
 * \fn FontManager::get_text(char *)
 * \param char * : texte � afficher
 * \param unsigned int : couleur du texte
 * \return SDL_Surface * : Surface contenant le texte � afficher
 * \brief Cette fonction cr�e une SDL_Surface affichant le texte pass� en argument, et en utilisant la Font courante.
 */
SDL_Surface * 
FontManager::get_text(char *text, unsigned int color)
{
  SDL_Surface *srf = get_text(text);
  SDL_LockSurface(srf);
  unsigned int *ptr = NULL;
  ptr = (unsigned int *) srf->pixels;

  for (int i = 0; i < srf->h; i++)
   for (int j = 0; j < srf->w; j++)
     if (ptr[i*srf->w + j])
       ptr[i*srf->w + j] = color;
  SDL_UnlockSurface(srf);
  return srf;
}



/**
 * \fn FontManager::get_text(char *, unsigned int)
 * \param char * : texte � afficher
 * \param unsigned int : largeur maximale du texte
 * \return SDL_Surface * : Surface contenant le texte � afficher
 * \brief Cette fonction cr�e une SDL_Surface affichant le texte pass� en argument, et en utilisant la Font courante.
 */
SDL_Surface * 
FontManager::get_text_w(char *text, unsigned int largeur)
{
  int largeur_texte = 0;
  int nb_lignes = 1;
  int i;//,j;
  int tmp_largeur = 0;

 //Mesure les dimensions du texte
  for (i = 0; i < (int)strlen(text); i++)
  {
    if ((text[i] == '\n')||(tmp_largeur > (int)largeur))
    {
      if (tmp_largeur > largeur_texte) largeur_texte = tmp_largeur;
      tmp_largeur = 0;
      nb_lignes ++;
    }
    else
    {
      short larg;
      if ((text[i] - 32) == 0)
        larg = index[text[i]-32];
      else
        larg = index[text[i]-32] - index[text[i]-32-1];
      tmp_largeur += larg;
    }
  }
  if (tmp_largeur > largeur_texte) largeur_texte = tmp_largeur;


  //Cr�e l'image
  SDL_Surface *sdltext;
  sdltext = SDL_CreateRGBSurface(SDL_HWSURFACE, largeur_texte, nb_lignes*(font->h+2), 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);


  //Blit
  SDL_Rect dest_pos;  //destination
  dest_pos.y = 0;
  dest_pos.h = font->h;
  dest_pos.x = 0;
  dest_pos.w = largeur_texte;


  SDL_Rect src_pos;  //Source
  src_pos.x = 0;
  src_pos.w = 0;
  src_pos.y = 0;
  src_pos.h = font->h;
  for (i = 0; i < (int)strlen(text); i++)
  {
    if (text[i] == '\n')
    {
      dest_pos.x = 0;
      dest_pos.y += font->h+2;
    }
    else
    {
      if ((unsigned int)dest_pos.x > largeur)
      {
        dest_pos.x = 0;
        dest_pos.y += font->h+2;
      }

      if ((text[i] - 32) == 0)
      {
        src_pos.w = index[text[i]-32];
        src_pos.x = 0;
      }
      else
      {
        src_pos.w = index[text[i]-32] - index[text[i]-32-1];
        src_pos.x = index[text[i]-32-1];
      }

      SDL_BlitSurface(font, &src_pos, sdltext, &dest_pos);
      dest_pos.x += src_pos.w;
    }
  }
  return sdltext;
}

/**
 * \fn FontManager::get_text(char *, unsigned int, unsigned int)
 * \param char * : texte � afficher
 * \param unsigned int : largeur maximale du texte
 * \param unsigned int : couleur du texte
 * \return SDL_Surface * : Surface contenant le texte � afficher
 * \brief Cette fonction cr�e une SDL_Surface affichant le texte pass� en argument, et en utilisant la Font courante.
 */
SDL_Surface * 
FontManager::get_text_w(char *text, unsigned int largeur, unsigned int color)
{
  SDL_Surface *srf = get_text_w(text,largeur);
  SDL_LockSurface(srf);
  unsigned int *ptr = NULL;
  ptr = (unsigned int *) srf->pixels;

  for (int i = 0; i < srf->h; i++)
   for (int j = 0; j < srf->w; j++)
     if (ptr[i*srf->w + j])
       ptr[i*srf->w + j] = color;
  SDL_UnlockSurface(srf);
  return srf;
}




