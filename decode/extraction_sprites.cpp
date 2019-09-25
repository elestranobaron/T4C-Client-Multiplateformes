#include "convertisseur.h"
//#include "vsfkey.h"
extern unsigned char vsfkey[];
bool
extraction_sprites(char *src, char *dest)
{
 char dest_dir[255];
 sprintf(dest_dir,"%s/sprites", dest);//double backslash avant sprites supprime
 
 if (mkdir(dest_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) //si on peut pas créer
 {
  if (!opendir(dest_dir)) //et qu'il existe pas
  {
    printf("->Impossible de creer le repertoire de destination ! [%s]\n", dest_dir);
    return false;
   }
 }
 
 DIR * dirp;
 struct dirent *d;
 char dir[1024];
 dirp = opendir(src);
 if (dirp == NULL)
 {
     printf("->Impossible de lire les fichiers VSF ! [%s]\n", src);
     return false;
 }
 dir[0] = '\0';

 while ((d = readdir(dirp)))
 {
     if (strstr(d->d_name, ".vsf") != 0)
     {    
          char fname[255];
          strcpy(fname, d->d_name);
          char in_name[255];
          sprintf(in_name, "%s%s", src, fname);//double backslash entre %s pareil en bas
          printf("->Decryptage de %s...\n", in_name);
          char out_name[255];
          sprintf(out_name, "%s/%s", dest_dir, fname);
          out_name[strlen(out_name)-3] = '\0';
          sprintf(out_name, "%sdec", out_name);
          
         //décryptage
         struct stat sb;
         if (stat(in_name, &sb) == -1) 
         {
          printf("->Impossible d'ouvrir le fichier %s\n", in_name);
          return false;
         }
         
         FILE *fid, *fout;
         fid = fopen(in_name, "rb");
         fout = fopen(out_name, "wb");
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

     }
 }
 closedir(dirp);
 return true;
}
