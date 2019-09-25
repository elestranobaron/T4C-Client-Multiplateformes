cmake_minimum_required(VERSION 3.7)
project(TnC_dev)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        FontManager/fontmanager.cpp
        FontManager/fontmanager.h
        MapInterface/mapi_full_redraw.cpp
        MapInterface/mapi_get_map.cpp
        MapInterface/mapi_move_map.cpp
        MapInterface/mapinterface.cpp
        MapInterface/mapinterface.h
        NPCManager/npc_ajout.cpp
        NPCManager/npc_config.cpp
        NPCManager/npc_draw.cpp
        NPCManager/npcmanager.cpp
        NPCManager/npcmanager.h
        TextManager/textmanager.cpp
        TextManager/textmanager.h
        VSFInterface/vsfi_create_hash.cpp
        VSFInterface/vsfi_hash.cpp
        VSFInterface/vsfi_indexage_pal.cpp
        VSFInterface/vsfi_indexage_rep.cpp
        VSFInterface/vsfi_indexage_spr.cpp
        VSFInterface/vsfi_palettes.cpp
        VSFInterface/vsfi_read_sprite.cpp
        VSFInterface/vsfi_save_bmp.cpp
        VSFInterface/vsfi_sprites.cpp
        VSFInterface/vsfinterface.cpp
        VSFInterface/vsfinterface.h
        VSFInterface/vsfkey.h
        funfact.c
        test_mapinterface.cpp
        test_npcmanager.cpp)

add_executable(TnC_dev ${SOURCE_FILES})