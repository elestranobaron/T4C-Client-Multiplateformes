#include <thread>
#include "MapInterface/mapinterface.h"
#include "VSFInterface/vsfinterface.h"
#include "FontManager/fontmanager.h"
#include "NPCManager/npcmanager.h"
#include "TextManager/textmanager.h"
#include "../ext/SDL2SDL2_image-2.0.0/include/SDL2/SDL_image.h"

#define PATH_SPRITES "../data/sprites/"
#define PATH_MAPS "../data/maps/"

//Layers
SDL_Surface * screen;
SDL_Surface *sol;
SDL_Surface *decor;
SDL_Surface *env;

//Son
SDL_AudioSpec audioBufferSpec;
Uint8 *audioBuffer;
Uint32  audioBufferLen, audioLen, audioPos;

//Objets
MAPInterface *mapi;
VSFInterface *vsfi;
FontManager *fm, *fm2;
TextManager *txtm;
NPCManager *npcm;

//Divers
bool map_flag;
bool disp_infos;
char str_infos[1024];
float fps;

void audioCallBack(void *udata, Uint8 *stream, int len)
{(void)udata;
    // On ne lit que s'il reste des données à jouer
    if ( audioLen == 0 )
        return;

    // Remise à zéro du tampon de sortie
    memset(stream, 0, (size_t)len);

    // Lecture du buffer audio
    //if (audioPos < audioBufferSpec.size) {
        /*if (audioPos+len > audioBufferSpec.size)
            len = audioBufferSpec.size = audioPos;*/
        SDL_MixAudio(stream, audioBuffer + audioPos,
                     (Uint32)len, SDL_MIX_MAXVOLUME);
        audioPos += len;
    //}

    // Décrémentation de ce qu'il reste à lire
    audioLen -= len;
}


void soundLoad(char const *title)
{
    if(!SDL_LoadWAV(title, &audioBufferSpec,
                    &audioBuffer, &audioBufferLen))
    {
        printf("Erreur lors du chargement du fichier WAV.\n");
        return ;
    }
    //audioBufferSpec.freq = FREQUENCY;
    /*audioBufferSpec.format = AUDIO_S16SYS;
    audioBufferSpec.channels = 1;
    audioBufferSpec.samples = 2048;*/
    audioBufferSpec.callback = audioCallBack;
    SDL_AudioSpec obtainedSpec;

    // you might want to look for errors here
    if (SDL_OpenAudio(&audioBufferSpec, &obtainedSpec) < 0)
    {
        printf(SDL_GetError());
        return ;
    }

    //audioPos = *audioBuffer;
    audioLen = audioBufferLen;
    // start play audio
    SDL_PauseAudio(0);
    // Attendre que la lecture du son soit terminée
    while ( audioLen > 0 ) {
        SDL_Delay(100);
    }

// Fermeture du module
    SDL_CloseAudio();
    SDL_FreeWAV(audioBuffer);
}

void
redraw(unsigned short zone, unsigned int loc_x, unsigned int loc_y)
{
 int tmptick  = SDL_GetTicks();
 
 //mise a jour des maps
 if (map_flag)
 {
   mapi->get_map(loc_x, loc_y, zone, sol, decor);
   map_flag = false;
 } 
 SDL_BlitSurface(sol,   NULL, screen, NULL); //sol
 SDL_BlitSurface(decor, NULL, screen, NULL); //d�cor

 npcm->draw_npc(screen, loc_x, loc_y);       //PNJ
 SDL_BlitSurface(env, NULL, screen, NULL);   //nuit/jour
 txtm->draw_texts(screen, loc_x, loc_y);     //textes
 
 SDL_Surface *txt_loc;
 char charloc[50];
 sprintf(charloc,"Loc : %d,%d Zone %d FPS : %0.0f", loc_x, loc_y, zone, fps);
 txt_loc = fm->get_text(charloc, 0x00FFFFFF);
 SDL_BlitSurface(txt_loc, NULL, screen, NULL);
 SDL_FreeSurface(txt_loc);

 if (disp_infos)
 {
   SDL_Surface *srf_infos = fm->get_text(str_infos);
   SDL_Rect txtpos;
   txtpos.x = 60;
   txtpos.y = 20;
   txtpos.w = (Uint16)srf_infos->w;
   txtpos.h = (Uint16)srf_infos->h;
   SDL_FillRect(screen, &txtpos, 0x758F75AA);
   SDL_BlitSurface(srf_infos, NULL, screen, &txtpos);
   SDL_FreeSurface(srf_infos);
 }  
	  	     
 SDL_Flip(screen);
 printf("BENCHMARK > Redraw en %f s\n\n",   (float)(SDL_GetTicks()-tmptick)/1000);
 fps = 1000/(SDL_GetTicks()-tmptick);
}

char*
magic_allocator(const char* allocator)
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

int
main(int argc, char *argv[])
{
  if (argc <4)
  {
   printf("Usage : %s x y zone\n", argv[0]);
   exit(0);
  }

  if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO) == -1)
  {
   printf("Can't init SDL:  %s\n", SDL_GetError());
   exit(1);
  }
  atexit(SDL_Quit); 
  SDL_ShowCursor(1);

  //dump des infos
  const SDL_VideoInfo *infos;
  infos = SDL_GetVideoInfo();
  sprintf(str_infos,"######################################################\n");
  sprintf(str_infos,"%s###                               -- INFOS SYSTEME --\n",str_infos);
  sprintf(str_infos,"%s######################################################\n",str_infos);
  sprintf(str_infos,"%s# HW surfaces                            : %s\n",str_infos, infos->hw_available?"oui":"non");
  sprintf(str_infos,"%s# Window manager                         : %s\n",str_infos, infos->wm_available?"oui":"non");
  sprintf(str_infos,"%s# Blits HW->HW acceleres                 : %s\n",str_infos, infos->blit_hw?"oui":"non");
  sprintf(str_infos,"%s# Blits HW->HW acceleres (avec colorkey) : %s\n",str_infos, infos->blit_hw_CC?"oui":"non");
  sprintf(str_infos,"%s# Blits HW->HW acceleres (avec alpha)    : %s\n",str_infos, infos->blit_hw_A?"oui":"non");
  sprintf(str_infos,"%s# Blits SW->HW acceleres                 : %s\n",str_infos, infos->blit_sw?"oui":"non");
  sprintf(str_infos,"%s# Blits SW->HW acceleres (avec colorkey) : %s\n",str_infos, infos->blit_sw_CC?"oui":"non");
  sprintf(str_infos,"%s# Blits SW->HW acceleres (avec alpha)    : %s\n",str_infos, infos->blit_sw_A?"oui":"non");
  sprintf(str_infos,"%s# Color fill acceleres                   : %s\n",str_infos, infos->blit_fill?"oui":"non");
  sprintf(str_infos,"%s# Memoire Video                          : %06d Ko\n",str_infos, infos->video_mem);
  sprintf(str_infos,"%s######################################################",str_infos);
  printf("%s",str_infos);

  //Init des coordonn�es
  unsigned int loc_x = (unsigned int)atoi(argv[1]);
  unsigned int loc_y = (unsigned int)atoi(argv[2]);
  unsigned short zone = (unsigned short)atoi(argv[3]);
  
  //Cr�ation des layers
  screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE | SDL_DOUBLEBUF /*| SDL_NOFRAME | SDL_FULLSCREEN*/);
  sol    = SDL_CreateRGBSurface(SDL_HWSURFACE, SCREEN_W, SCREEN_H, 32, 0xFF000000,0x00FF0000,0x0000FF00,0x00000000); //pas de couche alpha pour le sol
  decor  = SDL_CreateRGBSurface(SDL_HWSURFACE, SCREEN_W, SCREEN_H, 32, 0xFF000000,0x00FF0000,0x0000FF00,0x000000FF);
  env    = SDL_CreateRGBSurface(SDL_HWSURFACE, SCREEN_W, SCREEN_H, 32, 0xFF000000,0x00FF0000,0x0000FF00,0x00000000);

  //Image temp
  SDL_Surface *back;
  SDL_Rect imgpos;
  imgpos.x = 80;
  imgpos.y = 60;
  back = IMG_Load("LoadingScreen.bmp");
    std::thread t(soundLoad, "../data/sons/Caverns Music.wav");
    t.detach();
  SDL_BlitSurface(back, NULL, screen, &imgpos);
  SDL_Flip(screen);

  //Font Manager
  unsigned int tmptick = SDL_GetTicks();
  fm = new FontManager();
  if (!fm->load_font(magic_allocator("./fonts/font_trebuchet_12")))
  {
    printf("FontManager> Impossible de charger la font !\n");
    exit(1);
  }  
  fm2 = new FontManager();
  if (!fm2->load_font(magic_allocator("./fonts/sans_bold_12")))
  {
    printf("FontManager> Impossible de charger la font !\n");
    exit(1);
  }    
  printf("BENCHMARK > Gestionnaire de font charg� en %f s\n", (float)(SDL_GetTicks()-tmptick)/1000);
  
  //VSF Interface
  tmptick  = SDL_GetTicks();
  vsfi = new VSFInterface(magic_allocator(PATH_SPRITES));
  printf("BENCHMARK > VSFI charg� en %f s\n",   (float)(SDL_GetTicks()-tmptick)/1000);

  //MAPInterface
  tmptick  = SDL_GetTicks();
  mapi = new MAPInterface(magic_allocator(PATH_MAPS), vsfi);
  printf("BENCHMARK > MAPI charg� en %f s\n",   (float)(SDL_GetTicks()-tmptick)/1000);

  //Text Manager
  txtm = new TextManager(fm2);
  txtm->add_color_text_at_pos(magic_allocator("[ Zone prete ]"), 380, 300, 5, 0xBBCC6699);
  txtm->add_color_text_at_loc(magic_allocator("Youpi, je suis un petit bonhomme rouge !"), loc_x-4, loc_y-5, 0, 10, 0x00CC6699);
  txtm->add_color_text_at_loc(magic_allocator("Bouh je suis une vilaine momie !"), loc_x-11, loc_y-6, 0, 10, 0xFFCC6699);
  
  //NPC Manager
  npcm = new NPCManager(magic_allocator("NPCList.txt"), vsfi);
  npcm->ajout_npc(0, magic_allocator("CentaurWarrior"),      loc_x, loc_y, 135);//PaysanModel&
  npcm->set_action(0, 'D');
  npcm->move_to(0, loc_x - 5, loc_y - 5, 15);
  npcm->ajout_npc(1, magic_allocator("Lich"),      loc_x - 5, loc_y - 5, 45);//Mummy
  npcm->set_action(1, 'I');


  SDL_FreeSurface(back);


  //jour/nuit
  short luminosite = 0x00;
  SDL_Surface *torche = IMG_Load("torche.png");
  SDL_BlitSurface(torche, NULL, env, NULL);
  SDL_FreeSurface(torche);
  SDL_SetAlpha(env, SDL_SRCALPHA|SDL_RLEACCEL, (Uint8)luminosite);
  
 //SDL_Event events;
 map_flag = true;
 disp_infos = false;
 SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  //SDL_EnableKeyRepeat(5, 5);
    SDL_Event events;
 while (1)
 {
    SDL_Delay(5);
    redraw(zone, loc_x, loc_y);

    if(SDL_PollEvent(&events)) //If events in the queue
	  {
	  	 if(events.key.state == SDL_PRESSED)             //Checks for keyboard
	  	 {
	  	    switch(events.key.keysym.sym)
	  	    {
	  	        //divers
	  	        case SDLK_ESCAPE:
	  	             exit(1);
        	  	break;
        	  	case SDLK_F1:
        	  	     mapi->set_debug(true);
        	  	     map_flag = true;
        	  	break;
        	  	case SDLK_F2:
        	  	     mapi->set_debug(false);
        	  	     map_flag = true;
        	  	break;
        	  	case SDLK_F3:
                    //soundLoad("../data/sons/Caverns Music.wav");
        	  	  disp_infos = !disp_infos;
        	  	break;
        	  	case SDLK_F4:
        	  	    if (luminosite > 0x00) luminosite -= 10;
        	  	    SDL_SetAlpha(env, SDL_SRCALPHA, (Uint8)luminosite);
        	  	break;
        	  	case SDLK_F5:
        	  	    if (luminosite < 0xFF) luminosite += 10;
        	  	    SDL_SetAlpha(env, SDL_SRCALPHA, (Uint8)luminosite);
        	  	break;
        	  	case SDLK_F12:
	  	             SDL_SaveBMP(screen, "screen.bmp");
        	  	break;
                case SDLK_SPACE:
                    //soundLoad("../data/sons/Caverns Music.wav");
        	  	
        	  	//controles au Keypad
        	  	case SDLK_KP7:
        	  	  loc_x--;
        	  	  loc_y--;
      	  	    map_flag = true;
      	  	  break;
        	  	case SDLK_KP8:
        	  	  loc_y--;
     	  	      map_flag = true;
      	  	  break;
        	  	case SDLK_KP9:
        	  	  loc_x++;
        	  	  loc_y--;
       	  	    map_flag = true;
      	  	  break;
        	  	case SDLK_KP4:
        	  	  loc_x--;
       	  	    map_flag = true;
      	  	  break;
        	  	case SDLK_KP6:
        	  	  loc_x++;
       	  	    map_flag = true;
      	  	  break;
        	  	case SDLK_KP1:
        	  	  loc_x--;
        	  	  loc_y++;
       	  	    map_flag = true;
      	  	  break;
        	  	case SDLK_KP2:
        	  	  loc_y++;
       	  	    map_flag = true;
      	  	  break;
        	  	case SDLK_KP3:
        	  	  loc_x++;
        	  	  loc_y++;
       	  	    map_flag = true;
      	  	  break;
      	  	  
      	  	  //controles fleches
        	  	case SDLK_LEFT :
      	  	    loc_x--;
       	  	    map_flag = true;
        	  	break;
        	  	case SDLK_RIGHT :
          	  	loc_x++;
       	  	    map_flag = true;
        	  	break;
        	  	case SDLK_UP :
                loc_y--;
       	  	    map_flag = true;
        	  	break;
        	  	case SDLK_DOWN :
                loc_y++;
       	  	    map_flag = true;
        	  	break;
                default:
                    break ;
	  	     }
		 }
   }
 }
}
