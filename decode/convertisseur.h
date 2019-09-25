#ifndef CONVERTISSEUR_H
# define CONVERTISSEUR_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <io.h>
#include <sys/io.h>
#include <string.h>
#include <dirent.h>
#include <zlib.h>
#include <iostream>




struct _palette
{
  unsigned char R[256];
  unsigned char G[256];
  unsigned char B[256];
};

char * decode(char *, char*);
bool extraction_sons(char *, char *);
bool extraction_sprites(char *, char *);
bool extraction_maps(char *, char *);
unsigned int get_hash_val(unsigned int id_spr, unsigned int x, unsigned int y); //recherche le sprite correspondant a id_sprite et renvoie le hash du nom
bool load_index();

#endif
