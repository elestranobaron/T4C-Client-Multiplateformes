Base : http://tnc.sourceforge.net/

Rappel :

HOME

Introduction :

    The Fourth Coming (T4c) is a game developped and originally distributed by Vircom [http://www.the4thcoming.com/].
    The Next Coming (sorry for the name, I know it's stupid..) is a set of C++ classes that can be used to transform T4c's data into SDL surfaces.
    Originally, the main goal was to create a new multiplatform game client.
    So you will find :

        - tools to decode and extract/convert the original game datas (decode sprites packages, decode and convert maps, decode and extract sounds)
        - tools to access any sprite as an SDL_Surface
        - tools to display a specific location of the map
        - tools to extract sounds and music as .wav files
        - tools to display any NPC/Monster at a specific location and display an animation (walking, fighting, dying..)

    Example programs are also provided to show how to use those classes.

State of development :

    TnC is still very incomplete. See the "TODO" section for more details.

Overview :

    TnC do not provide a fully working client for T4c. Nevertheless, several demo applications using the developped set of tools are provided.
    See the "Screenshots" section for more details.

INSTALLATION

Introduction :

    TnC tools are not using directly the T4c game's datas. Several reasons for this :
    - Original datas's are encrypted, decryption "on the fly" is slow.
    - As I said in the TODO section, Map files cannot be used directly (there's no link between the sprite ID written on the map file, and the sprite name specified on the sprite's packages)
    - The Sound pack file cannot be used directly with the SDL sound library.

First Step : Reencoding the datas

    You will need an original copy of the game (distributed freely on the net, try google). The game version does not really matter.
    A simple application is provided to reencode all the game's datas : "convertisseur" (see "Download Section" to get it).
    You need to convert your data this way : "convertisseur original_path new_path".
    Example : convertisseur "C:\games\T4c\Game Files" "C:\TnC\data\"
    Note : The destination path must be a valid directory (create it manually).

    For your information, the reencoder will :

        - Decrypt the sprites files (Every .vsf file will be decrypted and written as a .dec file)
        - Decrypt the sound file (T4CGameFile.VSB) and rewrite every sound and music as a stand-alone .wav file.
        - Decrypt the maps files (*.map files).
        - Reencode the decrypted maps files (to .rmap files) using the "id_list.txt" index file. This file creates a link between the sprite's ID from the map file to the sprite's name (this file is very incomplete). 

Second Step : Compiling sources

    Windows : Initially, the programs have been written with Dev-Cpp under windows. Open one of the projects "MAPInterface.dev" (demo application, using the MAPInterface class) or "test_NPCManager.dev" (demo application displaying several NPC).
    The projects should compile without errors.

    Linux : To compile the MAPInterface demo application, type : make -f Makefile_map
    To compile the NPCManager demo application, type : make -f Makefile_npc

Third Step : Running...

    The demo applications will need to access the game's data (strange uh ?). You can change the path to the data on the two main files "test_vsfinterface.cpp" and "test_mapinterface.cpp".
    Edit the "PATH_*" variables.
    By default, the path to the sprites directory is : "../data/sprites/" and the path to the maps directory is "../data/maps/".

    MAPInterface demo :

        Run it this way : MAPInterface x_loc y_loc zone.
        "x_loc" and "y_loc" are the location on the map (between 0 and 3072).
        "zone" should be 0 (worldmap), 1 (dungeonmap), 2 (cavernmap) or 3 (underworld)
        Example : to visit Lighthaven (loc 2880, 1083) which is on the map "worldmap" (zone 0), type : MAPInterface 2880 1083 0
        Keys :
        - Keypad : Move on the map
        - F12 : Screenshot
        - Escape : quit

    NPCManager demo :

        There is no argument needed for this demo.
        Keys :
        - 0 (keypad) : Idle animation (well... this is not really an animation.. :p)
        - 1 (keypad) : walk animation
        - 2 (keypad) : attack animation
        - 3 (keypad) : death animation
        - Arrows : rotation
        - Escape : quit

DOWNLOAD

Here

TODO

State of development :

    Some part of those tools are working quite well, some still need a lot of work.
    One of the main reason of this is that some datas are hidden somewhere and index files have to be written manually.

    For example, how are managed the sprites animations ?
    You have a basic sprite name, let say "goblin", used to identify the different sprites (frames) for this animation.
    The base name is not always the same (goblin, Goblin, GOBLIN, gob,...), and the different frames are not always on the same package.
    This could have been too simple, so... there is duplicated frames (corresponding to different versions of the game).
    After the base name, you add a prefix, corresponding to the animation's type (walking : no prefix, attack : prefix "A", dying : prefix "C").
    Sometimes the prefix is different : the "BigWorm" dying will be "bigwormc" in spite of "BigWormC"...
    Still too simple ? Ok... Now add to this the fact that the number of frame per animation is not always the same, that some animations are missing...

    So, as you can see, you have to create manually index files specifying for each animation the number of frames, the base name, etc. Those files are probably hidden somewhere in the game datas, but I didn't find them.

    This is just an example, but the problem is similar with sprite's offsets, map's sprites ID, etc.
    I have created most of the files indexes (sprites animations, sprites offsets, map translations..) but some of them are incomplete.

Data extraction/conversion
Module		State		TODO
Sprites pack extraction		Finished		Every sprite is associated to several offsets. Some of them are incorrect... so there's a "modification file" loaded at startup to update the incorrect sprites. This file is incomplete.
Music and sound extraction		Finished		Nothing.
Maps extraction		Incomplete		A map is composed of sprites pick up in the sprites packages. The map is defined as a matrix of sprites ID. Unfortunately, I didn't found the link between the "sprite's ID" and the "sprite's name" (this is probably something like hash("sprite_name") = sprite_id...) So I've written an index file (yes, one again..) with those associations. This index is very incomplete !


C++ classes
Class		State		TODO
VSFInterface
(Sprites interface)		Finished		Check the code for memory leak (should be ok).
NPCManager
(displays an NPC/Monster somewhere with a specific animation)		Finished		Add something to manage NPC movements (moving from point A to point B at a specific speed)
MapInterface
(displays the map at a specific location)		Incomplete		Two problems here :
- sprites missing (see the "Maps extraction" problem)
- Transition between different types of ground incomplete (there is a mask system used to deal with transitions from grass to sand for example)

SCREENSHOTS

First site mentionned

