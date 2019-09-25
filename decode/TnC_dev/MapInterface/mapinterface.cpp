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

#include "mapinterface.h" // class's header file


/**
 * \fn MAPInterface::MAPInterface(char *, VSFInterface *)
 * \param char * : path vers le r�pertoire contenant les cartes
 * \param VSFInterface * : pointeur vers un objet de type vsfi, qui se chargera de fournir les sprites
 * \return Rien.
 * \brief Constructeur de la classe MapInterface. On indexe ici les cartes et on initialise certaines variables.
 */
char*
magic_allocator_deux(const char* allocator)
{
    char *good;
    int i;
    size_t len;

    len = strlen(allocator);
    i = 0;
    good = (char*)malloc(sizeof(char) * len + 1);
    while ((unsigned int)i < len)
    {
        good[i] = allocator[i];
        i++;
    }
    good[i] = '\0';

    return (good);
}
MAPInterface::MAPInterface(char *maps_dir, VSFInterface *vsfi)
{
    //listing des cartes, on v�rifie que tout le monde est l�...
    sprintf(tab_maps[0],"%s/worldmap.rmap", maps_dir);
    sprintf(tab_maps[1],"%s/dungeonmap.rmap", maps_dir);
    sprintf(tab_maps[2],"%s/cavernmap.rmap", maps_dir);
    sprintf(tab_maps[3],"%s/underworld.rmap", maps_dir);
    struct stat sb;
    if (stat(tab_maps[0],&sb) == -1) 
    {
      printf("Erreur : Carte %s introuvable !\n", tab_maps[0]);
      exit(1);
    }
    if (stat(tab_maps[1],&sb) == -1) 
    {
      printf("Erreur : Carte %s introuvable !\n", tab_maps[1]);
      exit(1);
    }
    if (stat(tab_maps[2],&sb) == -1) 
    {
      printf("Erreur : Carte %s introuvable !\n", tab_maps[2]);
      exit(1);
    }
    if (stat(tab_maps[3],&sb) == -1) 
    {
      printf("Erreur : Carte %s introuvable !\n", tab_maps[3]);
      exit(1);
    }
    
    this->vsfi = vsfi;
    

    fm = new FontManager();
    fm->load_font(magic_allocator_deux("./fonts/font_trebuchet_12"));
    
    this->map_x  = 999999999;
    this->map_y  = 999999999;
    this->last_x = 999999999;
    this->last_y = 999999999;
    this->debug = 0;
}



/**
 * \fn MAPInterface::~MAPInterface()
 * \brief Destructeur. Ne fait rien.
 */
MAPInterface::~MAPInterface()
{
	// insert your code here
}


/**
 * \fn MAPInterface::set_debug(bool flag)
 * \param bool : flag debug
 * \brief Change le flag de debug.
 */
void MAPInterface::set_debug(bool flag)
{
  debug = flag;
}

/**
 * \fn MAPInterface::get_loc(int*, int*, short*)
 * \param int * : x
 * \param int * : y
 * \param short * : zone
 * \brief Renvoie la loc actuelle.
 */
void MAPInterface::get_loc(int *x, int*y, short*zone)
{
  *x    = last_x;
  *y    = last_y;
  *zone = last_zone;
}


