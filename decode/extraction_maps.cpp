#include "convertisseur.h"
void write_bmp(char *, long , long , unsigned long *, struct _palette *);
//unsigned long get_hash_val(unsigned long, unsigned int, unsigned int);
//bool load_index();
int
decode_zone(unsigned char *pt_pack, unsigned int *pt_unpack)
{
	int		val,i,val2;
	unsigned int *pt_org = pt_unpack;

	do
	{
		val = *(unsigned short *)pt_pack;
        pt_pack += 2;

		if (val < 0x1000)
			*pt_unpack++=(val* 4 + 0x354);
		else
		{
			val2 = *(unsigned short *)pt_pack;
            pt_pack += 2;
			for (i = val - 0x1000; i > 0; i--)
				*pt_unpack ++= (val2* 4 + 0x354);
		}
	}
	while(pt_unpack - pt_org < 128*128);

	return pt_unpack - pt_org;
}

void
decodage_map(char *in_name, char *out_name)
{
  //Colle les données de la carte dans un buffer
  FILE *fmap;
  fmap = fopen(in_name,"rb");
  int taille;
  fseek(fmap, 0, SEEK_END);
  taille = ftell(fmap);
  fseek(fmap, 0, SEEK_SET);
  
  unsigned char *data;
  data = (unsigned char *) malloc(taille);
  fread(data, taille, 1, fmap);
  fclose(fmap);
  
  
  //Buffer de sortie (image de 3072x3072, id sur des long)
  unsigned char *decode = (unsigned char *) malloc(3072*3072*sizeof(int));
  unsigned char *tmp_unpack = (unsigned char *) malloc(128*128*sizeof(int));
  unsigned char *pt_data = data;
  unsigned int *pt_decode = (unsigned int *)decode;
  unsigned int *ptr = (unsigned int *)tmp_unpack;
  
  int	num, offset;
  
  
  //pour chacune des 24x24 zones de 128x128
  for (num = 0; num < 24*24; num++)
  {
	offset = *(unsigned int *) &pt_data[num*4]; //lit l'offset ou est stocké la zone ('num' ieme zone sur un long)
	decode_zone(&pt_data[offset],(unsigned int *)tmp_unpack);

	int zone_x = num%24;
	int zone_y = (int)num/24;

	for (int y = 0; y < 128; y++)
  	 for (int x = 0; x < 128; x++)
  	 {
  	   int posx = zone_x*128 + x;
  	   int posy = zone_y*128 + y;
  	   pt_decode[posx + 3072*posy] = get_hash_val(ptr[x+y*128],x,y);
  	 }
  }
  free(tmp_unpack);
  
  /*
  struct _palette pal;
  for (int i = 0; i < 256; i++)
  {
    pal.R[i] = (unsigned char) i;
    pal.G[i] = (unsigned char) i;
    pal.B[i] = (unsigned char) i;
  }
  char nom[255];
  sprintf(nom, "%s.bmp", out_name);
  write_bmp(nom, 3072, 3072, (unsigned long *)decode, &pal);
  */

  
  FILE *fout = fopen(out_name,"wb");
  fwrite(pt_decode, sizeof(unsigned int), 3072*3072, fout);
  fclose(fout);
/*
  gzFile fout = gzopen(out_name,"wb");
  gzwrite(fout, pt_decode, sizeof(unsigned long)* 3072*3072);
  gzclose(fout);
  */
}


bool
extraction_maps(char *src, char *dest)
{
 char dest_dir[255];
 sprintf(dest_dir,"%s/maps", dest);// deux backslashs par un slash
 
 if (mkdir(dest_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) //si on peut pas créer
 {
  if (!opendir(dest_dir))//et qu'il existe pas
  {
    printf("->Impossible de creer le repertoire de destination ! [%s]\n", dest_dir);
    return false;
   }
 }
 if (!load_index())
 {
   printf("->Impossible de lire le ficchier de mappage !\n");
   return false;
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
     if (strstr(d->d_name, ".map") != 0 || strstr(d->d_name, ".Map") != 0)
     {    
        char fname[255];
        strcpy(fname, d->d_name);
        char in_name[255];
        sprintf(in_name, "%s/%s", src, fname);//deux backslash par un slash
        printf("->Reencodage de %s...\n", in_name);
        char out_name[255];
        sprintf(out_name, "%s/%s", dest_dir, fname);// deux backslash par un slash
        out_name[strlen(out_name)-3] = '\0';
        sprintf(out_name, "%srmap", out_name); 
        decodage_map(in_name, out_name);
     }
 }
 return true;
}
