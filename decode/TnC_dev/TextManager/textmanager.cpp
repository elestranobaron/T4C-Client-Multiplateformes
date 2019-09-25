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

#include "textmanager.h" // class's header file


TextManager::TextManager(FontManager *fm)
{
  this->fm = fm;
  this->liste_textes = NULL;
}


TextManager::~TextManager()
{
  
}


bool TextManager::add_text_at_loc(char *txt, int x, int y, short zone, short timeout)
{
  struct _text *ntxt = (struct _text *) malloc(sizeof(struct _text));
  ntxt->loc_x = x;
  ntxt->loc_y = y;
  ntxt->loc_zone = zone;
  ntxt->screen_x = -1;
  ntxt->screen_y = -1;
  ntxt->texte = fm->get_text(txt);
  ntxt->timeout = SDL_GetTicks()+timeout*1000;
  ntxt->alpha = 0xFF;
  ntxt->next = liste_textes;
  liste_textes = ntxt;
  return true;
}

bool TextManager::add_color_text_at_loc(char *txt, int x, int y, short zone, short timeout, unsigned int color)
{
  struct _text *ntxt = (struct _text *) malloc(sizeof(struct _text));
  ntxt->loc_x = x;
  ntxt->loc_y = y;
  ntxt->loc_zone = zone;
  ntxt->screen_x = -1;
  ntxt->screen_y = -1;
  ntxt->texte = fm->get_text(txt, color);
  ntxt->timeout = SDL_GetTicks()+timeout*1000;
  ntxt->alpha = 0xFF;
  ntxt->next = liste_textes;
  liste_textes = ntxt;
  return true;
}

bool TextManager::add_text_at_pos(char *txt, int x, int y, short timeout)
{
  struct _text *ntxt = (struct _text *) malloc(sizeof(struct _text));
  ntxt->loc_x = -1;
  ntxt->loc_y = -1;
  ntxt->loc_zone = -1;
  ntxt->screen_x = x;
  ntxt->screen_y = y;
  ntxt->texte = fm->get_text(txt);
  ntxt->timeout = SDL_GetTicks()+timeout*1000;
  ntxt->alpha = 0xFF;
  ntxt->next = liste_textes;
  liste_textes = ntxt;
  return true;
}

bool TextManager::add_color_text_at_pos(char *txt, int x, int y, short timeout, unsigned int color)
{
  struct _text *ntxt = (struct _text *) malloc(sizeof(struct _text));
  ntxt->loc_x = -1;
  ntxt->loc_y = -1;
  ntxt->loc_zone = -1;
  ntxt->screen_x = x;
  ntxt->screen_y = y;
  ntxt->texte = fm->get_text(txt, color);
  ntxt->timeout = SDL_GetTicks()+timeout*1000;
  ntxt->alpha = 0xFF;
  ntxt->next = liste_textes;
  liste_textes = ntxt;
  return true;
}

// No description
void TextManager::draw_texts(SDL_Surface *srf, int x, int y)
{
  struct _text *txt = liste_textes;
  SDL_Rect pos;
  int t = SDL_GetTicks();

  struct _text *prev = NULL;
  while (txt)
  {
    if (t > txt->timeout) //besoin de le supprimer
    {
      struct _text *tmp = txt;
      if (!prev)
        liste_textes = txt->next;
      else
        prev->next = txt->next;
      txt = txt->next;
      SDL_FreeSurface(tmp->texte);
      free(tmp);
    }
    else
    {
     prev = txt;
     txt = txt->next;
    }   
  }
  
  txt = liste_textes;
  
  while (txt)
  {
    if (txt->loc_zone ==-1) //position sur l'�cran
    {
      pos.x = txt->screen_x;
      pos.y = txt->screen_y;  
      if (txt->timeout - t <= 1000)
      {
        if (txt->alpha > 0) txt->alpha -=20;
        SDL_SetAlpha(txt->texte, SDL_SRCALPHA, txt->alpha);
        txt->screen_y += 1;
      }
      SDL_BlitSurface(txt->texte, NULL, srf, &pos);
    }
    else //position a une loc
    {
      pos.x = (txt->loc_x - x)*32 + 800/2;
      pos.y = (txt->loc_y - y)*16 + 600/2;
      if (txt->timeout - t <= 1000)
      {
        if (txt->alpha > 0) txt->alpha -=20;
        SDL_SetAlpha(txt->texte, SDL_SRCALPHA, txt->alpha);
        pos.y += (1000-(txt->timeout - t))/100;
      }
      SDL_BlitSurface(txt->texte, NULL, srf, &pos);
    }    
    txt = txt->next;
  }  
}

