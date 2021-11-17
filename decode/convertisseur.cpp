#include "convertisseur.h"
//#include "vsfkey.h"




int
main(int argc, char *argv[])
{
  if (argc < 3)
  {
    printf("Usage : %s <source> <destination>\n", argv[0]);
    exit(1);
  }

  if (!extraction_sprites(argv[1], argv[2])) printf("Echec lors de l'extraction des sprites !\n");  
  if (!extraction_sons(argv[1], argv[2])) printf("Echec lors de l'extraction des sons !\n");
  if (!extraction_maps(argv[1], argv[2])) printf("Echec lors de l'extraction des maps !\n");  
}
