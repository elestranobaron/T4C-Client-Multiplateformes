#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include "vsfkey.h"

char *
decode(char *f_name, char*fileout)
{
 printf("->Decryptage du fichier %s\n", f_name);
 if (!fileout)
 {
  char *fileout;
  fileout = (char*) malloc((strlen(f_name)+4) * sizeof(char));
  sprintf(fileout, "%s.dec", f_name);
 }
 

 //recupere la taille du fichier
 struct stat sb;
 if  (stat(f_name, &sb) == -1)
 {
  printf("Impossible de lire le fichier !\n");
  return NULL;
 }

 //decodage
 FILE *fid, *fout;
 fid = fopen(f_name, "rb");
 fout = fopen(fileout, "wb");
 char val;
 int i ;
 for (i = 0; i < sb.st_size; i++)
 {
    fread(&val, sizeof(char), 1, fid);
    val ^= vsfkey[(i/4096)&0xfff];
    fwrite(&val, sizeof(char), 1, fout);
 }
 fclose(fid);
 fclose(fout);
 return fileout;
}
