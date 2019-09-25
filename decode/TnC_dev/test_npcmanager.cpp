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
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#include <stdio.h>
#include <SDL/SDL.h>
#include <thread>
#include "NPCManager/npcmanager.h"
#include "VSFInterface/vsfinterface.h"

# define WITDH 800
# define HEIGHT 600

//Son
SDL_AudioSpec audioBufferSpec;
Uint8 *audioBuffer;
Uint32  audioBufferLen, audioLen, audioPos;

SDL_Surface * screen;

void audioCallBack(void *udata, Uint8 *stream, int len)
{(void)udata;
    // On ne lit que s'il reste des données à jouer
    if ( audioLen == 0 )
        return;

    // Remise à zéro du tampon de sortie
    memset(stream, 0, (size_t)len);

    SDL_MixAudio(stream, audioBuffer + audioPos,
                 (Uint32)len, SDL_MIX_MAXVOLUME);
    audioPos += len;
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
{(void)argc;(void)argv;
  if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO) == -1)
  {
   printf("Can't init SDL:  %s\n", SDL_GetError());
   exit(1);
  }
  atexit(SDL_Quit); 
  SDL_ShowCursor(1);
  
  
  screen = SDL_SetVideoMode(WITDH, HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF /*| SDL_NOFRAME | SDL_FULLSCREEN*/);
  SDL_Surface *back;
  SDL_Rect imgpos;
  imgpos.x = 80;
  imgpos.y = 60;
  back = SDL_LoadBMP("LoadingScreen.bmp");
    std::thread t(soundLoad, "../data/sons/Dungeons Music.wav");
    t.detach();
  SDL_BlitSurface(back, NULL, screen, &imgpos);
  SDL_Flip(screen);
  
  VSFInterface *vsfi = new VSFInterface(magic_allocator("../data/sprites/"));
  NPCManager *npcm = new NPCManager(magic_allocator("NPCList.txt"), vsfi);
  SDL_FillRect(screen, NULL, 0xFF000000);
  short direction = 315;

  npcm->ajout_npc(0, magic_allocator("Goblin"),      990 * WITDH / 800, 987 * HEIGHT / 600, direction);
  npcm->ajout_npc(1, magic_allocator("Mummy"),              997 * WITDH / 800, 987 * HEIGHT / 600, direction);
  npcm->ajout_npc(2, magic_allocator("CentaurSkeleton"),           1003 * WITDH / 800, 987 * HEIGHT / 600, direction);
  npcm->ajout_npc(3, magic_allocator("BlackWarrior"),     1010 * WITDH / 800, 987 * HEIGHT / 600, direction);
  
  npcm->ajout_npc(4, magic_allocator("Bat"),            990 * WITDH / 800, 998 * HEIGHT / 600, direction);
  npcm->ajout_npc(5, magic_allocator("Lich"),             997 * WITDH / 800, 998 * HEIGHT / 600, direction);
  npcm->ajout_npc(6, magic_allocator("Zombie"),           1003 * WITDH / 800, 998 * HEIGHT / 600, direction);
  npcm->ajout_npc(7, magic_allocator("Pegasus"),          1010 * WITDH / 800, 998 * HEIGHT / 600, direction);
  
  npcm->ajout_npc(8, magic_allocator("Beholder"),   990 * WITDH / 800, 1007 * HEIGHT / 600, direction);
  npcm->ajout_npc(9, magic_allocator("Orc"),           997 * WITDH / 800, 1007 * HEIGHT / 600, direction);
  npcm->ajout_npc(10, magic_allocator("Demon"),         1003 * WITDH / 800, 1007 * HEIGHT / 600, direction);
  npcm->ajout_npc(11, magic_allocator("GuardModel1"),    1010 * WITDH / 800, 1007 * HEIGHT / 600, direction);

  npcm->ajout_npc(12, magic_allocator("Horse"),                990 * WITDH / 800, 1017 * HEIGHT / 600, direction);
  npcm->ajout_npc(13, magic_allocator("MonsDraconianLeather"), 997 * WITDH / 800, 1017 * HEIGHT / 600, direction);
  npcm->ajout_npc(14, magic_allocator("MonsDraconianPlate"),  1003 * WITDH / 800, 1017 * HEIGHT / 600, direction);
  npcm->ajout_npc(15, magic_allocator("MonsDraconianRobe"),    1010 * WITDH / 800, 1017 * HEIGHT / 600, direction);

  short nbmax = 16;

  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  SDL_Event events;
  while (1)
  {
    SDL_Delay(5);
    if(SDL_PollEvent(&events)) //If events in the queue
	  {
	  	 if(events.key.state == SDL_PRESSED)             //Checks for keyboard
	  	 {
	  	    switch(events.key.keysym.sym)
	  	    {
	  	        case SDLK_ESCAPE:
	  	          exit(1);
        	  	break;
        	  	case SDLK_F1:
      	  	  break;
        	  	case SDLK_F2:
      	  	  break;
      	  	  case SDLK_SPACE :
        	  	  npcm->dump_npc(); 	    
     	  	    break;
     	  	    
     	  	    //action
     	  	    case SDLK_KP0:
     	  	      for (int i=0; (short)i < nbmax; i++)
       	  	      npcm->set_action(i, 'I');
     	  	    break;
     	  	    case SDLK_KP1:
     	  	      for (int i=0; (short)i < nbmax; i++)
     	  	        npcm->set_action(i, 'D');
     	  	    break;
     	  	    case SDLK_KP2:
     	  	      for (int i=0; (short)i < nbmax; i++)
     	  	        npcm->set_action(i, 'A');
     	  	    break;
     	  	    case SDLK_KP3:
     	  	      for (int i=0; (short)i < nbmax; i++)
     	  	        npcm->set_action(i, 'M');
     	  	    break;
     	  	    
     	  	    //direction
     	  	    case SDLK_LEFT:
     	  	      direction = (360+(direction-45))%360;
     	  	      for (int i=0; (short)i < nbmax; i++)
       	  	      npcm->set_direction(i, direction);
   	  	      break;
     	  	    case SDLK_RIGHT:
     	  	      direction = (360+(direction+45))%360;
     	  	      for (int i=0; (short)i < nbmax; i++)
     	  	        npcm->set_direction(i, direction);
   	  	      break;
                default:
                    break ;
        	 } 	
	  	 }
    }
    SDL_FillRect(screen, NULL, 0xFF000000);
    npcm->draw_npc(screen, 1000, 1000);
    SDL_Flip(screen);
  }
  return 0;
}  
