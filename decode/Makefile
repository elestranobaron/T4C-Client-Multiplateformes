#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By:        <      @student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/06/06 23:12:37 by                   #+#    #+#              #
#    Updated: 2015/09/17 23:55:05 by                  ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME =	convert2
INCS =	$(subst .cpp,.h,$(SRC))
OBJ =	$(subst .cpp,.o,$(SRC))
SRC =	extraction_maps.cpp extraction_sons.cpp extraction_maps_conv.cpp\
	extraction_sprites.cpp decode.cpp convertisseur.cpp save_bmp.cpp
DWLDR	=	curl -O 
CC		=	clang++ -g
#FLAGS	=	-Wall -Werror -Wextra -std=c++11

INC		=	-I .  #-Iinc -I. #/ttf #../../inc
INC		+= 	-I/usr/include/SDL2
INC		+=	-Iext/SDL2SDL2-2.0.3/include/SDL2 
INC		+=	-Iext/SDL2SDL2_image-2.0.0/include/SDL2
INC		+=	-Iext/SDL2SDL2_ttf-2.0.12/include/SDL2 

LIBS	=  -L./ext/SDL2SDL2-2.0.3/lib
LIBS	+= -L./ext/SDL2SDL2_image-2.0.0/lib
LIBS	+= -L./ext/SDL2SDL2_ttf-2.0.12/lib
LIBS	+= -lSDL2 -lSDL2_ttf -lSDL2_image
LIBS	+= `ext/SDL2SDL2-2.0.3/sdl2-config --cflags --libs -lSDL2 -lSDL2_ttf -lSDL2_image`

SDL2_DIR_CLEAN= ext/SDL2

all: ext $(NAME)

$(NAME): $(OBJ)
	$(CC)  -o $@ $^ $(LIBS) $(INC)

%.o: %.cpp
#	$(CC) $(FLAGS) -o $@ -c $< $(INC)
	$(CC)  $(FLAGS) -o $@ -c $< $(INC)

re: fclean all

clean:
	rm -rf $(OBJ)

fclean:
	rm -rf $(OBJ)
	rm -rf $(NAME)
	rm -fr ext/SDL2
	mkdir -p ext/SDL2

ext: ext/SDL2SDL2-2.0.3 ext/SDL2SDL2_image-2.0.0 ext/SDL2SDL2_ttf-2.0.12

$(SDL2_DIR_CLEAN):
	rm -fr ext/SDL2
	mkdir -p ext/SDL2

ext/SDL2SDL2-2.0.3:
	@mkdir -p ext/SDL2
	$(DWLDR) http://www.libsdl.org/release/SDL2-2.0.3.tar.gz
	@echo "$(VERT)~> [ SDL library downloaded. ]$(NCOL)"
	tar xf SDL2-2.0.3.tar.gz --no-same-owner
	mv -f SDL2-2.0.3.tar.gz ext/SDL2
	mv -f SDL2-2.0.3 ext/SDL2
#	cp -fr SDL2-2.0.3.tar.gz ext/SDL2
#	mv -f SDL2-2.0.3 ext/SDL2
	( cd ext/SDL2/SDL2-2.0.3 \
	&& ./configure --prefix=$(shell pwd)/ext/SDL2SDL2-2.0.3/ \
	&& $(MAKE) && $(MAKE) install )
	@echo "$(VERT)~> [ SDL library set up. ]$(NCOL)"

ext/SDL2SDL2_image-2.0.0:
	@mkdir -p ext/SDL2
	$(DWLDR) http://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.0.tar.gz
	@echo "$(VERT)~> [ SDL_Image library downloaded. ]$(NCOL)"
	tar xf SDL2_image-2.0.0.tar.gz --no-same-owner
	mv -f SDL2_image-2.0.0.tar.gz ext/SDL2
	mv -f SDL2_image-2.0.0 ext/SDL2
#	cp -fr SDL2_image-2.0.0.tar.gz ext/SDL2
#	mv -f SDL2_image-2.0.0 ext/SDL2
	( export SDL2_CONFIG='$(shell pwd)/ext/SDL2SDL2-2.0.3/sdl2-config' \
	&& cd ext/SDL2/SDL2_image-2.0.0 \
	&& ./configure --prefix=$(shell pwd)/ext/SDL2SDL2_image-2.0.0/ \
	&& $(MAKE) && $(MAKE) install );
	@echo "$(VERT)~> [ SDL_Image library set up. ]$(NCOL)"

ext/SDL2SDL2_ttf-2.0.12:
	@mkdir -p ext/SDL2
	$(DWLDR) http://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.12.tar.gz
	@echo "$(VERT)~> [ SDL_ttf library downloaded. ]$(NCOL)"
	tar xf SDL2_ttf-2.0.12.tar.gz --no-same-owner
	mv -f SDL2_ttf-2.0.12.tar.gz ext/SDL2
	mv -f SDL2_ttf-2.0.12 ext/SDL2
#	cp -fr SDL2_ttf-2.0.12.tar.gz ext/SDL2
#	mv -f SDL2_ttf-2.0.12 ext/SDL2
	( export SDL2_CONFIG='$(shell pwd)/ext/SDL2SDL2-2.0.3/sdl2-config' \
	&& cd ext/SDL2/SDL2_ttf-2.0.12 \
	&& ./configure --prefix=$(shell pwd)/ext/SDL2SDL2_ttf-2.0.12/ \
	&& $(MAKE) && $(MAKE) install );
	@echo "$(VERT)~> [ SDL_ttf library set up. ]$(NCOL)"

.PHONY: all re clean fclean
