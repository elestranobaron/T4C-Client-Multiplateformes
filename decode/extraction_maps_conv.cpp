#include "convertisseur.h"
#define MAX_SPRITES 1526

struct _sprite_index
{
  unsigned int id;  //ID origine
  char *nom;        //nom du sprite associé
  short modx;       //modulo éventuel sur x (après un #)
  short mody;       //modulo éventuel sur y (après un #)
  short rand1;      //random eventuel
  short rand2;      //random eventuel
};

struct _sprite_index tab_index[MAX_SPRITES];




bool load_index()
{
  FILE *index_file;
  if (!(index_file = fopen("id_list.txt.txt", "r")))
    return false;
  int pos = 0;
  char line[255];
  while ((!feof(index_file))&&(pos < MAX_SPRITES))
  {
    fgets(line, 255, index_file);
    if (line[strlen(line)-1] == '\n') 
        line[strlen(line)-1] = '\0';

    tab_index[pos].modx  = 0;
    tab_index[pos].mody  = 0;
    tab_index[pos].rand1 = 0;
    tab_index[pos].rand2 = 0;

    //parsing de la ligne
    char *tmp;
    tmp = strchr(line, '=');        //séparation ID|STR au "="
    *tmp = '\0';
    sscanf(line, "0x%x",&tab_index[pos].id); //récupère l'ID
    
    tmp ++;
    
    char *sep;
    char *sep2;
    if ((sep = strchr(tmp, '#'))) //SPRITE AVEC MODULO
    {
     *sep = '\0';
     sep++;
     if (!strchr(sep,'#')) //un seul modulo
       sscanf(sep, "%hd", &tab_index[pos].modx);
     else
     {
       sep2 = strchr(sep,'#');
       *sep2 = '\0';
       sscanf(sep, "%hd", &tab_index[pos].modx);
       sep2++;
       sscanf(sep2, "%hd", &tab_index[pos].mody);
     }  
    }  
    
    if ((sep = strchr(tmp, '$'))) //SPRITE AVEC RANDOM
    {
     *sep = '\0';
     sep++;
     if (!strchr(sep,'$')) //on a qu'un random
       sscanf(sep,"%hd", &tab_index[pos].rand1);
     else
     {
       sep2 = strchr(sep,'$');
       *sep2 = '\0';
       sscanf(sep,"%hd", &tab_index[pos].rand1);
       sep2 ++;
       sscanf(sep2,"%hd", &tab_index[pos].rand2);
     }  
    }  
    tab_index[pos].nom = (char *) malloc((strlen(tmp)+1)*sizeof(char));
    strcpy(tab_index[pos].nom, tmp);
    /*
    if (tab_index[pos].modx&&tab_index[pos].modx)
        printf("[%s] modulo %d, %d\n", tab_index[pos].nom, tab_index[pos].modx, tab_index[pos].mody);
    else if (tab_index[pos].modx)
        printf("[%s] modulo %d\n", tab_index[pos].nom, tab_index[pos].modx);
    else if (tab_index[pos].rand1&&tab_index[pos].rand2)
        printf("[%s] rand %d, %d\n", tab_index[pos].nom, tab_index[pos].rand1, tab_index[pos].rand2);
    else if (tab_index[pos].rand1)
        printf("[%s] rand %d\n", tab_index[pos].nom, tab_index[pos].rand1);
        */
    pos ++;
  }
  fclose(index_file);
  return true;
}

unsigned int
hash(char *str)
{
   short maxlen = 128;           //on hash sur des chaines de 64 octets 
   short strl = strlen(str);    //les bits en trop sur la chaine sont mis a 0
   unsigned int hash = 5381;
   unsigned int i    = 0;

   for(i = 0; (short)i < maxlen; str++, i++)
   {
      (short)i<strl?hash = ((hash << 5) + hash) + (*str): ((hash << 5) + hash);
   }
   return (hash & 0x7FFFFFFF);

}

unsigned int
get_hash_val(unsigned int id_spr, unsigned int x, unsigned int y) //recherche le sprite correspondant a id_sprite et renvoie le hash du nom
{
  int i=0;
  while ((i < MAX_SPRITES)&&(tab_index[i].id != id_spr))
    i++;

  if (i == MAX_SPRITES) //on ne trouve pas le sprite dans l'index, c'est mal !
  {
    printf("Sprite non recense !! %04lx\n", id_spr);
    return id_spr;
  }

  char tmpname[128];
  if (tab_index[i].modx&&tab_index[i].mody)
  {
    sprintf(tmpname,tab_index[i].nom, x%tab_index[i].modx+1, y%tab_index[i].mody+1);
    //printf("[%s] modulo %d, %d\n", tab_index[i].nom, tab_index[i].modx, tab_index[i].mody);
    return hash(tmpname);
  }
  else if (tab_index[i].modx)
  {
    sprintf(tmpname, tab_index[i].nom, x%tab_index[i].modx+1);
    //printf("[%s] modulo %d\n", tab_index[i].nom, tab_index[i].modx);
    return hash(tmpname);
  }  
  else if (tab_index[i].rand1&&tab_index[i].rand2)
  {
    sprintf(tmpname,  tab_index[i].nom, rand()%tab_index[i].rand1+1, rand()%tab_index[i].rand2+1);
    //printf("[%s] rand %d, %d\n", tab_index[i].nom, tab_index[i].rand1, tab_index[i].rand2);
    return hash(tmpname);
  }  
  else if (tab_index[i].rand1)
  {
    sprintf(tmpname,  tab_index[i].nom, rand()%tab_index[i].rand1+1);
    //printf("[%s] rand %d\n", tab_index[i].nom, tab_index[i].rand1);
    return hash(tmpname);
  }

  if (hash(tab_index[i].nom) == hash("None"))  //Sprite non identifié -> on laisse l'ancienne valeur
    return (unsigned int)id_spr;
  else                                         //Sprite identifié, on renvoie son hash
    return hash(tab_index[i].nom);
}

