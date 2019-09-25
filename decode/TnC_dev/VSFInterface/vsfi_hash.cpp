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

/**
 * \fn VSFInterface::hash(char *)
 * \param char * : Chaine � hasher.
 * \return unsigned int : hash de la chaine pass�e en argument.
 * \brief Cette fonction renvoie un hash sur 4 octets de la chaine pass�e en argument.
 */
unsigned int
VSFInterface::hash(char *str)
{
   short maxlen = 128;           //on hash sur des chaines de 64 octets 
   short strl = (short)strlen(str);    //les bits en trop sur la chaine sont mis a 0
   unsigned int hash = 5381;
   unsigned int i    = 0;

   for(i = 0; (short)i < maxlen; str++, i++)
   {
       (short)i<strl?hash = ((hash << 5) + hash) + (*str): ((hash << 5) + hash);
   }
   return (hash & 0x7FFFFFFF);

}
