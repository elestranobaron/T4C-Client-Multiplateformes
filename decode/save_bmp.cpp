#include "convertisseur.h"



void
write_bmp(char *f_name, long largeur, long hauteur, unsigned long *matrix, struct _palette *pal)
{
  int i,j;
  FILE *spr;
  spr = fopen(f_name, "wb");
 if (!spr)
  return;
 long _long;
 short _short;
 char _char;
 int pad_largeur = largeur;
 while (pad_largeur - ((int)(pad_largeur/4))*4 != 0)
   pad_largeur ++;
   
 //BMP
 //int full_size = 0;
 fprintf(spr, "BM");
 _long = 1078 + largeur*hauteur;
 fwrite(&_long, sizeof(long) / 2,1,spr);          //taille totale X
 fwrite(&_long, sizeof(long) / 2,1,spr);         //NC
 _long = 1078;
 fwrite(&_long, sizeof(long) / 2,1,spr);         //offset data X
 _long = 40;
 fwrite(&_long, sizeof(long) / 2,1,spr);         //taille header
 _long = largeur;
 fwrite(&_long, sizeof(long) / 2,1,spr);         //largeur
 _long = hauteur;
 fwrite(&_long, sizeof(long) / 2,1,spr);         //hauteur
 _short = 1;
 fwrite(&_short, sizeof(short), 1, spr);     //nb plans
 short huit = 8;
 fwrite(&huit, sizeof(short), 1, spr);     //bits palette
 _long = 0;
 fwrite(&_long, sizeof(long) / 2,1,spr);                     //compression


 
 _long = pad_largeur*hauteur;
 fwrite(&_long, sizeof(long) / 2,1,spr);       //taille de la zone de donnees (arondi 4)

 _long = largeur;
 fwrite(&_long, sizeof(long) / 2, 1, spr);         //largeur/m
 _long = hauteur;
 fwrite(&_long, sizeof(long) / 2, 1, spr);         //hauteur/m
 _long = 256;
 fwrite(&_long, sizeof(long) / 2,1,spr);    //bpp
 _long  = 0;//couleurs importantes
 fwrite(&_long, sizeof(long) / 2,1,spr); 
 for (i=0; i < 256; i++)                   //palette
 {
   _char = pal->B[i];
   fwrite(&_char, sizeof(char), 1, spr);
   _char = pal->G[i];
   fwrite(&_char, sizeof(char), 1, spr);
   _char = pal->R[i];
   fwrite(&_char, sizeof(char), 1, spr);
   _char = 0;
   fwrite(&_char, sizeof(char), 1, spr);
 }


 for (i=hauteur-1; i >= 0; i--)
 {
  for (j = 0; j < largeur; j++)
  {
    _char = matrix[i*largeur+j];
    fwrite(&_char, sizeof(char), 1, spr);
  }
  for (j = 0; j < pad_largeur - largeur; j++)
  {
    _char = 0;
    fwrite(&_char, sizeof(char), 1, spr);
  }
 }

  fclose(spr);
}
