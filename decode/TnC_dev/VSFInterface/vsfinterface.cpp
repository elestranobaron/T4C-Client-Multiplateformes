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
#include "vsfinterface.h" // class's header file

/**
 * \fn VSFInterface::VSFInterface(char*)
 * \param char * : path contenant les fichiers vsf d�cod�s
 * \brief Constructeur de la classe VSFI. On cr�e les index des palettes et des sprites. 
 * On identifie pour chaque sprite la palette la plus appropri�e. 
 * On charge les modifications sp�cifiques � certains sprites.
 */
char*
magic_allocator_trois(const char* allocator)
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
VSFInterface::VSFInterface(char*vsfdir)
{ 
  ////int tk = SDL_GetTicks();
  nb_sprites = 0;


  //### Generation de la liste des fichiers VSF
  liste_fichiers = NULL;
  liste_dossiers = NULL;
  liste_sprites  = NULL;
  liste_palettes = NULL;
  
  char liste_f[14][255];
  sprintf(liste_f[0],"%s/t4cgamefile.dec", vsfdir);
  sprintf(liste_f[1],"%s/t4cgamefile2.dec", vsfdir);
  sprintf(liste_f[2],"%s/t4cgamefile3.dec", vsfdir);
  sprintf(liste_f[3],"%s/t4cgamefile4.dec", vsfdir);
  sprintf(liste_f[4],"%s/t4cgamefile5.dec", vsfdir);
  sprintf(liste_f[5],"%s/t4cgamefile6.dec", vsfdir);
  sprintf(liste_f[6],"%s/t4cgamefile7.dec", vsfdir);
  sprintf(liste_f[7],"%s/t4cgamefile8.dec", vsfdir);
  sprintf(liste_f[8],"%s/t4cgamefile9.dec", vsfdir);  
  sprintf(liste_f[9],"%s/t4cgamefile10.dec", vsfdir);
  sprintf(liste_f[10],"%s/t4cgamefile11.dec", vsfdir);
  sprintf(liste_f[11],"%s/t4cgamefile12.dec", vsfdir);
  sprintf(liste_f[12],"%s/t4cgamefile13.dec", vsfdir);
  sprintf(liste_f[13],"%s/t4cgamefile14.dec", vsfdir);
  struct stat sb;
  for (int i=  0; i < 14; i++)
  {
    if (stat(liste_f[i],&sb) == -1) 
    {
      printf("VSFI> Erreur ! Fichier de donn�es %s introuvable.\n", liste_f[i]);
      exit(1);
    }
    struct _file *f = (struct _file*) malloc(sizeof(struct _file));
    f->nom = (char *) malloc((strlen(liste_f[i])+1)*sizeof(char));
    strcpy(f->nom, liste_f[i]);
    f->next = liste_fichiers;
    liste_fichiers = f;
  }  
  
  /*
  DIR * dirp;
  struct dirent *d;
  char dir[1024];
  dirp = opendir(vsfdir);
  if (dirp == NULL)
      return;
  dir[0] = '\0';
  char tmpname[255];
  while (d = readdir(dirp))
  {
     if (strstr(d->d_name, ".dec") != 0)
     {
          sprintf(tmpname, "%s/%s", vsfdir, d->d_name);
          struct _file *f;
          f = (struct _file*) malloc(sizeof(struct _file));
          f->nom = (char *) malloc((strlen(tmpname)+1)*sizeof(char));
          strcpy(f->nom, tmpname);
          f->next = liste_fichiers;
          liste_fichiers = f;
     }
  }
  closedir(dirp);
*/


  //### Indexage des r�pertoires
  
  struct _file*f;
  
  f = liste_fichiers;
  while (f)
  {
    printf("%s:\n", f->nom);
    scandirs(f->nom, f);
    f = f->next;
  }



  //generation des path
  struct _dossier *dsr;
  dsr = liste_dossiers;
  char fullpath[512];
  while (dsr)
  {
    strcpy(fullpath, vsfdir);
    char * pth = get_full_path(dsr, fullpath);
    dsr->path = (char *) malloc((strlen(pth)+1)*sizeof(char));
    strcpy(dsr->path, pth);
    dsr->path[strlen(pth)] = '\0';
    dsr = dsr->next;
  }

  //### Generation de la liste des palettes.
  f = liste_fichiers;
  while (f)
  {
    printf("%s:\n", f->nom);
    scanpal(f->nom, f);
    f = f->next;
  }
  //optimisation magique : on colle Bright 1 en premier...
  struct _palette *p = liste_palettes;
  struct _palette *p2 = NULL;
  while ((p)&&(strcmp(p->nom, "Bright1")))
  {
    p2 = p;
    p = p->next;
  }
  if (p && p2)
  {
    p2->next = p->next;
    p->next = liste_palettes;
    liste_palettes = p;
  }




  //### Generation de la liste des sprites.
  ////FILE *tmp;
  f = liste_fichiers;
  while (f)
  {
    printf("%s:\n", f->nom);
    scansprites(f->nom, f);
    f = f->next;
  }

  //cr�ation table de hash (utilis�e pour l'asso des palettes)
  create_hash();

  //identification des palettes pour les sprites
  select_palette(vsfdir);

  //chargement des modifications sur les sprites
  load_modif(magic_allocator_trois("modif_sprites.txt"));

  //cr�ation table de hash, en rajoutant ceux modifs
  free(hash_table);
  nb_sprites = 0;
  create_hash();
  
  //suppression des doublons
  //on scanne la table de hash. Si 2 prites ont le meme hash (meme nom), 
  //on garde celui dont le fichier d'origine est le plus grand (plus r�cent)
  struct _sprite *spr;
  int prev_hash = -1;

  ////struct _hash *h = hash_table;
  for (int i = 0; (unsigned int)i < nb_sprites; i++)
  {
    if (prev_hash == (int)hash_table[i].hash)
    {
//      printf(">Doublon : %s [%s] | %s [%s]\n", hash_table[i].addr->nom,hash_table[i].addr->fichier->nom, spr->nom, spr->fichier->nom);
      if (strcmp(hash_table[i].addr->fichier->nom, spr->fichier->nom)>0)  // on garde hash_table[i], on vire hash_table[i-1]
      {
        free(hash_table[i-1].addr->nom);
        free(hash_table[i-1].addr);
        hash_table[i-1].addr = hash_table[i].addr;
      }  
      else                                                                // on garde hash_table[i-1], on vire hash_table[i]
      {
        free(hash_table[i].addr->nom);
        free(hash_table[i].addr);
        hash_table[i].addr = spr;
      }  
    }    
    prev_hash = (int)hash_table[i].hash;
    spr = hash_table[i].addr;
  }
}




/**
 * \fn VSFInterface::~VSFInterface()
 * \brief Destructeur de la classe. Ne fait rien.
 */
VSFInterface::~VSFInterface()
{

}





/**
 * \fn VSFInterface::load_modif(char *)
 * \param char * : Nom du fichier contenant les modifications
 * \brief Cette fonction lit le fichier pass� en argument et effectue certaines modifications sur les sprites (ajout de miroirs, modifications d'offsets, etc.)
 */
void VSFInterface::load_modif(char *modif_file)
{
 FILE *modif;
 if (!(modif = fopen(modif_file, "r"))) return;
 char line[255];
 char *sep;
 char *sep2;
 while (!feof(modif))
 {
   fgets(line, 255, modif);
   if ((sep = strchr(line, '#'))) //changement d'offset
   {
     *sep = '\0';
     sep++;
     if ((sep2 = strchr(sep, '#'))) //si on en a qu'un, on laisse tomber
     {
      *sep2 = '\0';
      sep2++;
      int offx, offy;
      sscanf(sep,  "%d", &offx);
      sscanf(sep2, "%d", &offy);
      unsigned int h = hash(line);

      struct _sprite *spr = liste_sprites;
      while (spr)
      {
        if (h == spr->hash)
        {
         spr->offset_x += offx;
         spr->offset_y += offy;
         printf("> D�calage d'offset pour %s : (%d,%d)\n", spr->nom, offx, offy);
         spr = NULL;
        }
        if (spr) spr = spr->next;
      }

     }  
   }
   else if ((sep = strchr(line, '$'))) //ajout d'un sprite miroir
   {
     *sep = '\0';
     struct _sprite *spr = get_sprite_from_hash(hash(line)); //r�cup�re le sprite a dupliquer
     if (spr)
     {
       struct _sprite *spr2 = liste_sprites; //va a la fin de la liste
       while (spr2->next) spr2 = spr2->next;
       
       struct _sprite *nspr = (struct _sprite *) malloc(sizeof(struct _sprite));
       nspr->nom = (char *) malloc((strlen(spr->nom)+2)*sizeof(char));
       sprintf(nspr->nom,"%sM", spr->nom);
       nspr->hash         = hash(nspr->nom);
       nspr->fichier      = spr->fichier;
       nspr->miroir       = true;
       nspr->offset       = spr->offset;
       nspr->palette      = spr->palette;
       nspr->sol          = false;
       nspr->hauteur      = -1;
       nspr->largeur      = -1;
       nspr->offset_x     = 0;
       nspr->offset_y     = 0;
       nspr->transparence = 0;
       nspr->pixels       = NULL;
       nspr->sdl_sprite   = NULL;
       nspr->next         = NULL;
       spr2->next         = nspr;
       printf("> ajout du miroir : %s (hash : %08x) palette : %s\n", nspr->nom, nspr->hash, nspr->palette->nom);
     }  
   }
 }
 fclose(modif);
}




