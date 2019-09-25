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

#ifndef VSFINTERFACE_H
#define VSFINTERFACE_H
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <SDL/SDL.h>
//#include <SDL/SDL_image.h>

/*
 * Structure contenant les infos sur les fichiers vsf
*/
struct _file
{
  char *nom;                /** nom */
  int offset_dossiers;     /** offset de la zone des dossiers */
  int offset_fichiers;     /** offset de la zone des fichiers  */
  int nb_dossiers;         /** nombre de dossiers */
  struct _file*next;        /** dossier suivant */
};

/**
 * Structure contenant les infos sur les dossiers contenus dans les vsf
 */
struct _dossier
{
  int id;                   /** id du dossier */
  int id_parent;            /** id du parent (0 : root) */
  char *nom;                /** nom du dossier */
  char *path;               /** chemin complet du dossier */
  struct _dossier *next; /** dossier suivant */
};

/**
 * Structure de palette.
*/
struct _palette
{
  char *nom;                /** nom de la palette */
  SDL_Color rgb[256];           /** palette */
  struct _palette *next; /** palette suivante*/
};

/**
 * Structure stockant un sprite
 */
struct _sprite
{ 

  //charg� au d�part
  char *nom;                /** nom du sprite */
  int offset;              /** offset du sprite dans son fichier */
  struct _file *fichier;    /** pointeur vers la structure du fichier */
  struct _palette *palette; /** palette pour le sprite */
  unsigned int hash;        /** Hash du nom */
  bool miroir;              /** sprite miroir ? */
  
  //mis a jour apres chargement du sprite
  bool sol; /** flag sol */
  short largeur; /** largeur du sprite */
  short hauteur; /** hauteur du sprite */
  short offset_x; /** offset sur x du sprite */
  short offset_y; /** offset sur y du sprite */
  short offset_mx; /** offset sur x du sprite */
  short offset_my; /** offset sur y du sprite */
  unsigned char transparence; /** couleur de transparence du sprite */
  unsigned char *pixels; /** data du sprite */
  unsigned char *pixels_miroir; /** data du sprite miroir*/
  SDL_Surface *sdl_sprite; /** SDL_Surface du sprite */
  SDL_Surface *sdl_sprite_miroir; /** SDL_Surface du sprite */
  struct _sprite *next; /** sprite suivant */
};


struct _hash
{
  long hash;
  struct _sprite *addr;
};  

/**
 * \class VSFInterface
 * \brief Classe de gestion des Sprites.
 */
class VSFInterface
{
	public:
		VSFInterface(char*);
		~VSFInterface();
		bool save_bmp(char*, char*);
		bool save_bmp(struct _sprite *, char*);
		struct _sprite *get_list();
		void unload_sprite(char *);
		void unload_sprite(struct _sprite * spr);
		void write_bmp(char *, int, int, unsigned char *, struct _palette *);
		
		struct _sprite* get_sprite_from_hash(unsigned int, bool);
		struct _sprite* get_sprite_from_hash(unsigned int);
		struct _sprite* get_sprite_by_name(char *);
		struct _sprite* get_sprite_by_name(char *, bool);

		struct _sprite* get_mirrored_sprite_from_hash(unsigned int, bool);
		struct _sprite* get_mirrored_sprite_from_hash(unsigned int);
		struct _sprite* get_mirrored_sprite_by_name(char *);
		struct _sprite* get_mirrored_sprite_by_name(char *, bool);
		
	private:
	  unsigned int nb_sprites;
	  struct _hash *hash_table; /** table de hash (Id_sprite | addr sprite) */
		struct _file    * liste_fichiers;
		struct _dossier * liste_dossiers;
		struct _palette * liste_palettes;
		struct _sprite  * liste_sprites;
		
		void scandirs(char *, struct _file *);
	  void add_dir(char *, int, int);
		char * get_full_path(struct _dossier *, char *);
		void scanpal(char *, struct _file*);
		void add_pal(char *, int, FILE *);
		void scansprites(char *, struct _file*);
		void add_sprite(char *, int, int, struct _file *);
		struct _palette *find_palette(char *);
		void select_palette(char *);
    void read_sprite(struct _sprite *);
    void read_mirrored_sprite(struct _sprite *);
    void read_sprite(struct _sprite *, bool);
		unsigned int hash(char *);
		void load_modif(char *);
		void create_hash();
		void hash_quicksort(long, long);//VSFinterface:: => Extra-Qualification
		long hash_partition (long, long);//
		inline void hash_swap(long, long);//
		struct _sprite * find_sprite(unsigned int);
};

#endif // VSFINTERFACE_H

