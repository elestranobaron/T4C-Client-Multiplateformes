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
 * \fn VSFInterface::get_sprite_from_hash(unsigned int, bool)
 * \param unsigned int : Hash du sprite � trouver
 * \param bool : charger ou non le sprite ?
 * \return struct _sprite * : Structure du sprite trouv�
 * \brief Cette fonction renvoie la structure du sprite dont le hash est pass� en argument
 */
struct _sprite* 
VSFInterface::get_sprite_from_hash(unsigned int h, bool load)
{
  if (!nb_sprites)
  {
    struct _sprite *spr = liste_sprites;
    struct _sprite *lst = NULL;
  
    while (spr&&(h != spr->hash))
    {
      lst = spr;
      spr = spr->next;
    }  
    if (!spr)
       return NULL;
    else
    {
       if (!spr->sdl_sprite&&load) //pas charg�, on le charge
         read_sprite(spr);
       if (lst)
       {
         lst->next = spr->next;
         spr->next = liste_sprites;
         liste_sprites = spr;
       }    
       return spr;
    }
  }
  else
  {
    struct _sprite *spr = find_sprite(h);
    if (spr&&(!spr->sdl_sprite)&&load) //pas charg�, on le charge
         read_sprite(spr);
    return spr;
  }  
}


/**
 * \fn VSFInterface::get_sprite_from_hash(unsigned int)
 * \param unsigned int : Hash du sprite � trouver
 * \return struct _sprite * : Structure du sprite trouv�
 * \brief Cette fonction renvoie la structure du sprite (charg�) dont le hash est pass� en argument
 */
struct _sprite* 
VSFInterface::get_sprite_from_hash(unsigned int h)
{
  return (get_sprite_from_hash(h, true));
}


/**
 * \fn VSFInterface::get_sprite_by_name(char *, bool)
 * \param char *: nom du sprite
 * \param bool : charger ou non le sprite ?
 * \return struct _sprite * : Structure du sprite trouv�
 * \brief Cette fonction renvoie la structure du sprite dont le nom est pass� en argument
 */
struct _sprite* 
VSFInterface::get_sprite_by_name(char *nom, bool load)
{
  return (get_sprite_from_hash(hash(nom), load));
}


/**
 * \fn VSFInterface::get_sprite_by_name(char *)
 * \param char *: nom du sprite
 * \return struct _sprite * : Structure du sprite trouv�
 * \brief Cette fonction renvoie la structure du sprite (charg�) dont le nom est pass� en argument
 */
struct _sprite* 
VSFInterface::get_sprite_by_name(char *nom)
{
  return (get_sprite_from_hash(hash(nom), true));
}








/**
 * \fn VSFInterface::get_sprite_from_hash(unsigned int, bool)
 * \param unsigned int : Hash du sprite � trouver
 * \param bool : charger ou non le sprite ?
 * \return struct _sprite * : Structure du sprite trouv�
 * \brief Cette fonction renvoie la structure du sprite dont le hash est pass� en argument
 */
struct _sprite* 
VSFInterface::get_mirrored_sprite_from_hash(unsigned int h, bool load)
{
  if (!nb_sprites) //pas de table de hash, recherche � l'ancienne
  {
    struct _sprite *spr = liste_sprites;
    struct _sprite *lst = NULL;
  
    while (spr&&(h != spr->hash))
    {
      lst = spr;
      spr = spr->next;
    }  
    if (!spr)
       return NULL;
    else
    {
       if (!spr->sdl_sprite_miroir&&load) //pas charg�, on le charge
         read_mirrored_sprite(spr);
       if (lst)
       {
         lst->next = spr->next;
         spr->next = liste_sprites;
         liste_sprites = spr;
       }    
       return spr;
    }
  }
  else //recherche avec la table de hash
  {
    struct _sprite *spr = find_sprite(h);
    if (spr&&(!spr->sdl_sprite_miroir)&&load) //pas charg�, on le charge
         read_mirrored_sprite(spr);
    return spr;
  }  
}


/**
 * \fn VSFInterface::get_mirrored_sprite_from_hash(unsigned int)
 * \param unsigned int : Hash du sprite � trouver
 * \return struct _sprite * : Structure du sprite trouv�
 * \brief Cette fonction renvoie la structure du miroir du sprite (charg�) dont le hash est pass� en argument
 */
struct _sprite* 
VSFInterface::get_mirrored_sprite_from_hash(unsigned int h)
{
  return (get_mirrored_sprite_from_hash(h, true));
}


/**
 * \fn VSFInterface::get_mirrored_sprite_by_name(char *, bool)
 * \param char *: nom du sprite
 * \param bool : charger ou non le sprite ?
 * \return struct _sprite * : Structure du sprite trouv�
 * \brief Cette fonction renvoie la structure du miroir du sprite dont le nom est pass� en argument
 */
struct _sprite* 
VSFInterface::get_mirrored_sprite_by_name(char *nom, bool load)
{
  return (get_mirrored_sprite_from_hash(hash(nom), load));
}

/**
 * \fn VSFInterface::get_mirrored_sprite_by_name(char *)
 * \param char *: nom du sprite
 * \return struct _sprite * : Structure du sprite trouv�
 * \brief Cette fonction renvoie la structure du miroir du sprite (charg�) dont le nom est pass� en argument
 */
struct _sprite* 
VSFInterface::get_mirrored_sprite_by_name(char *nom)
{
  return (get_mirrored_sprite_from_hash(hash(nom), true));
}
























/**
 * \fn VSFInterface::unload_sprite(char *)
 * \param char * : Nom du sprite � d�charger.
 * \brief Cette fonction supprime le sprite charg� en m�moire dont le nom est pass� en argument (il reste dans l'index, on efface les datas, etc.)
 */
void 
VSFInterface::unload_sprite(char *sprite_name)
{
  struct _sprite *spr = get_sprite_by_name(sprite_name);
  if (!spr)
     return;
  if (!spr->sdl_sprite)
     return;
  SDL_FreeSurface(spr->sdl_sprite);
  free(spr->pixels);
  spr->pixels = NULL;
  spr->sdl_sprite = NULL;
  spr->offset_x = 0;
  spr->offset_y = 0;
}


/**
 * \fn VSFInterface::unload_sprite(struct _sprite *)
 * \param struct _sprite * : Structure du sprite � d�charger.
 * \brief Cette fonction supprime le sprite charg� en m�moire dont la structure est pass�e en argument (il reste dans l'index, on efface les datas, etc.)
 */
void VSFInterface::unload_sprite(struct _sprite * spr)
{
  if (!spr)
     return;
  if (!spr->sdl_sprite)
     return;
  SDL_FreeSurface(spr->sdl_sprite);
  free(spr->pixels);
  spr->pixels = NULL;
  spr->sdl_sprite = NULL;
}


/**
 * \fn VSFInterface::get_list()
 * \return struct _sprite * : La liste des sprites.
 * \warning Ne pas utiliser ! C'est sale !
 * \brief Cette fonction renvoie la liste des structures de sprites de l'index.
 */
struct _sprite *
VSFInterface::get_list()
{
  return liste_sprites;
}


/**
 * \fn VSFInterface::find_sprite(unsigned)
 * \param unsigned int : hash du sprite
 * \return struct _sprite : pointeur vers le sprite
 * \brief Cette fonction recherche un sprite dans la table de hash
 */
struct _sprite * VSFInterface::find_sprite(unsigned int hash)
{
  unsigned int min = 0;
  unsigned int max = nb_sprites-1;

  unsigned int pivot = (unsigned int) floor((max - min)/2);
  while ((hash_table[pivot].hash != hash)&&(max-min != 1))
  {
    if (hash > hash_table[pivot].hash)
      min = pivot;
    else
      max = pivot;
    pivot = (unsigned int) floor((max - min)/2) + min;
  }

  if (hash_table[pivot].hash == hash)
    return hash_table[pivot].addr;
  else
    return NULL;
}
