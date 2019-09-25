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
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#include "vsfinterface.h"

inline void
VSFInterface::hash_swap(long a, long b)
{
  struct _hash tmp;
  tmp = hash_table[a];
  hash_table[a] = hash_table[b];
  hash_table[b] = tmp;
}  
  
long VSFInterface::hash_partition (long gauche,long droit)
{
  long i,pivot_pos,pivot;

  pivot = hash_table[gauche].hash;
  pivot_pos = gauche;
  for (i=gauche+1;i<=droit;i++)
    if (hash_table[i].hash && hash_table[i].hash < pivot)
      hash_swap(++pivot_pos, i);
  hash_swap(pivot_pos,gauche);

  return(pivot_pos);
}


void
VSFInterface::hash_quicksort(long gauche,long droit)
{
  long pos_pivot;
  if (gauche < droit)
  {
    pos_pivot = hash_partition(gauche,droit);
    hash_quicksort(gauche,pos_pivot-1);
    hash_quicksort(pos_pivot+1,droit);
  }
}



/**
 * \fn VSFInterface::create_hash()
 * \brief Cette fonction cree la table de hash pour acc�der aux sprites
 */
void VSFInterface::create_hash()
{
  struct _sprite *spr;

  //compte le nb d'elements et alloue la table de hash
  spr = liste_sprites;
  while (spr)
  {
    spr = spr->next;
    nb_sprites++;
  }
  nb_sprites--;
  this->hash_table = (struct _hash *)malloc(nb_sprites*sizeof(struct _hash));
  
  //ajoute les sprites ds la table
  spr = liste_sprites;
  for (int i = 0; (unsigned int)i < nb_sprites; i++)
  {
    hash_table[i].hash = spr->hash;
    hash_table[i].addr = spr;
    spr = spr->next;
  }
 
  //tri de la table
  hash_quicksort(0, nb_sprites);
  
/*
  for (int i=0; i < nb_sprites; i++)
    printf("%08x -> %08x\n", hash_table[i].hash,  hash_table[i].addr);
    */
}


