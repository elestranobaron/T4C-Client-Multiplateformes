#include "convertisseur.h"
extern unsigned char vsfkey[];

void
decode_wav(char *file, char *out)
{

 FILE *fid = fopen(file, "rb");
 if (!fid)
 {
   printf("Impossible d'ouvrir le fichier decrypte !\n");
   exit(1);
 }
 long nb_files = 0;
 fread(&nb_files, sizeof(int), 1, fid);

 int i;
 for (i = 0; i < nb_files; i++)
 {
   unsigned char nb_chars;
   fread(&nb_chars, sizeof(char), 1, fid);
   unsigned char nom[nb_chars+1];
   fread(&nom, sizeof(char), nb_chars,fid);
   nom[nb_chars] = '\0';
   unsigned long offset;
   fread(&offset, sizeof(int),1,fid);
   unsigned long taille;
   fread(&taille, sizeof(int),1,fid);
   unsigned short freq;
   fread(&freq, sizeof(short), 1,fid);
   
   char bps;
   fread(&bps, sizeof(char), 1,fid);
   printf("->Extraction de %s\n",nom);
   int pos = ftell(fid);
   
   char n_nom[255];
   sprintf(n_nom,"%s/%s.wav", out, nom);//double backslash ici aussi du a windobb
   FILE *outfile = fopen(n_nom, "wb");
   if (outfile)
   {
    fseek(fid,offset, SEEK_SET);
    char header[5];
    fread(&header, sizeof(char),4,fid);
    header[4] = '\0';
    if (!strcmp(header, "RIFF"))
      fwrite(&header, sizeof(char),4,outfile);
    else
    {
     long _long;
     unsigned short _short;
     char riff[] = "RIFF";
     fwrite(&riff, sizeof(char),4, outfile);
     _long = 350716;
     fwrite(&_long, sizeof(int),1,outfile);
     char fmt[] = "WAVEfmt ";
     fwrite(&fmt, sizeof(char), 8, outfile);
     _long = 16;  //chunk size
     fwrite(&_long, sizeof(int),1,outfile);
     _short = 1;  //format tag
     fwrite(&_short, sizeof(short),1,outfile);
     _short = 1;  //channels
     fwrite(&_short, sizeof(short),1,outfile);
     _long = freq;  //freq
     fwrite(&_long, sizeof(int),1,outfile);
     _long = 176400;  // avg
     fwrite(&_long, sizeof(int),1,outfile);
     _short = 2;  // align
     fwrite(&_short, sizeof(short),1,outfile);
     _short = bps;  //bps
     fwrite(&_short, sizeof(short),1,outfile);
     char data[] = "data";
     fwrite(&data, sizeof(char), 4, outfile);
     fwrite(&header, sizeof(char),4,outfile); 
    }
    char c;
    unsigned long j;
    for (j=0; j < taille-4; j++)
    {
      fread(&c, sizeof(char), 1, fid);
      fwrite(&c, sizeof(char), 1, outfile);
    }
    fclose(outfile);
   }
   fseek(fid,pos, SEEK_SET);
 }
 fclose(fid);

}

bool
extraction_sons(char *src, char *dest)
{ 
 printf(">Extraction des sons\n");
 char dest_dir[255];
 sprintf(dest_dir,"%s/sons", dest);//ET ICI AUSSI DBLE SLASH AFTERSS
 if (mkdir(dest_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) //si on peut pas créer
 {
  if (!opendir(dest_dir))//et qu'il existe pas
  {
    printf("->Impossible de creer le repertoire de destination ! [%s]\n", dest_dir);
    return false;
   }
 }
 
 char file_path[255];
 sprintf(file_path, "%s/T4CGameFile.VSB", src);//enlever les deux \\apres le%s
 
 
 //---> Décryptage
 printf("->Decryptage...\n");
 struct stat sb;
 if (stat(file_path, &sb) == -1) 
 {
  printf("->Impossible d'ouvrir le fichier %s\n", file_path);
  return false;
 }
 
 FILE *fid, *fout;
 fid = fopen(file_path, "rb");
//	std::string soundstr("sounds_dec");//soundstr.c_str()
char	*charsoundstr = (char*)malloc(sizeof(char) * 11);
	//soundstr = "sounds_dec";
	charsoundstr[0] = 's';
	charsoundstr[1] = 'o';
	charsoundstr[2] = 'u';
	charsoundstr[3] = 'n';
	charsoundstr[4] = 'd';
	charsoundstr[5] = 's';
	charsoundstr[6] = '_';
	charsoundstr[7] = 'd';
	charsoundstr[8] = 'e';
	charsoundstr[9] = 'c';
	charsoundstr[10] = '\0';
 fout = fopen(charsoundstr, "wb");
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
 
 //--> Décodage
 decode_wav(charsoundstr, dest_dir);
 
 //Efface fichier temp
 FILE *tmp;
 tmp = fopen(charsoundstr, "w");
 fclose(tmp);
 return true;
}

