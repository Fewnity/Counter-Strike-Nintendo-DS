# Counter Strike Nintendo DS
**Counter Strike demake on Nintendo DS with online multiplayer** (Works on **emulator** and on **real hardware**).

# !!! WARNING !!!
This branch has Nifi support, but with issues that I can't fix, I don't know the problem. Maybe you can help me.<br/>
My problem is located in network/nifi.c in the **createRequest** function, when a client is moving, the host sends the client's position to all other clients.<br/>
To create the request buffer I'm using **sprintf** and at the end of the buffer I'm adding the client to update id (clientToUpdate->id).<br/>
But this value is absolutely wrong in the buffer, exemple : clientToUpdate->id = 3 and the text buffer is {POS;....;1205062516} but If I'm doing a printf to print the value alone at screen, clientToUpdate->id looks good.<br/>
I think my code is making a buffer overflow or is trying to acces an unexisting value somewhere (maybe the issue is caused by something else) because some variable is changing randomly.<br/>
**Need help!** (To test the code, you need Melon DS, you can enable the debug mode by holding Select while starting the game (you will get a back screen and some texts)in the main menu you can click on B to join a Nifi Room and Y to create a room, you need to join a team to move).<br/>

I'm using my Nifi template as base, maybe the problem is here but the template alone is working good : https://github.com/Fewnity/Nintendo-DS-Nifi-Template <br/>
The precompiled nifi lib is from here : https://github.com/Fewnity/dswifi

For **multiplayer** on **emulator**, please use [melonDS](https://melonds.kuribo64.net/downloads.php).<br/>
There is no emulator with **online** support on **Android** at this time.<br/>
**Ad hoc (Ni-Fi) multiplayer** is not supported (I'm looking for help with that!)<br/>

![image](https://user-images.githubusercontent.com/39272935/176778790-14707835-d15d-473b-baeb-3cb51864b004.png)
![image](https://user-images.githubusercontent.com/39272935/176778935-59555499-c40e-4325-83f8-8d89778a75af.png)

## How to install the game

### On DS flashcard
1. Put the .nds on the microSD card
2. Put soundbank.bin and counter_strike_music.raw on the root of the microSD card

### On DSi/3DS SD card
1. Install [TWiLight Menu++](https://wiki.ds-homebrew.com/twilightmenu/installing)
2. Put the .nds on the SD card
3. Put soundbank.bin and counter_strike_music.raw on the root of the SD card
4. In TWiLight Menu++, find the game and press Y on it
5. Set **Run in** to **DS mode*

### On emulator (melonDS) :
To have sounds and music, you need to use a virtual SD card, because sounds and music are not included in the .nds file.

- Set in the DLDI settings (**Config**->**Emu settings**->**DLDI**) the counter_strike_sd.raw file as SD card. Or create your own [here](https://melonds.kuribo64.net/board/thread.php?pid=2902) and put soundbank.bin and counter_strike_music.raw in the virtual SD card

## Network Setup
- Create a Wi-Fi network without a password or with WEP security. You can use your Android phone, a compatible router, or a Linux/MacOS computer with Wi-Fi adapter to do a hotspot
### On DS
1. Start a game that **supports online multiplayer** like **Mario Kart DS**
2. Open the **Wi-Fi setup** and select an available Wi-Fi slot
3. Search for an access point and add your Wi-Fi

### On DSi
1. Open **System Settings**
2. Open **Internet**
3. Open **Connection Settings**
4. Select an available Wi-Fi slot (it must be Connection 1, 2, or 3!)
5. Search for an access point and add your Wi-Fi

### On 3DS
1. Open **Settings**
2. Open **Internet Settings**
3. Open the **Nintendo DS Connection** menu and select an available Wi-Fi slot
4. Search for an access point and add your Wi-Fi

## How to compile the game
1. Install [DevKitPro](https://github.com/devkitPro/installer/releases/latest) and check the **NDS Development** option
2. Download the modified version of Nitro Engine [here](https://github.com/Fewnity/nitro-engine) and put the folder like this : **"C:\devkitPro\nitro-engine"**
3. Compile the engine (Open a **CMD**, write **"cd C:\devkitPro\nitro-engine"** then **"make"**)
4. Put the game source code folder in **"C:\devkitPro\nitro-engine\examples"**
5. Compile the game (Open a **CMD**, write **"cd C:\devkitPro\nitro-engine\examples\Counter-Strike-nds"** then **"make"**)
6. Copy the **Counter-Strike-nds.nds** from the project folder and the **soundbank.bin** file from the build folder

Note : To avoid coding collisions, path finding..., there is an Unity project to export code : https://github.com/Fewnity/Counter-Strike-DS-Unity-Project

## TODO
- [ ] Ad hoc (Ni-Fi) multiplayer : **Difficult**
- [ ] Disable sleep mode in multiplayer : **?**
- [ ] Add fall damage : **Easy**
- [ ] Smoke doesn't affect bots at this time : **Easy**
- [ ] Online chat (empty menu already exists) : **Medium**
- [ ] Create a NitroFS version for sounds and musics (and keep the FAT version for better compatibility) : **Medium**
- [ ] Put data in JSON files (weapons/equipments/bots names/ai waypoints) : **Medium**
- [ ] Add Wingman mode : **Medium**
- [ ] Add input profiles : **Medium**
- [ ] Possibility to change cursor : **Medium**
- [ ] Add bot difficulty : **Medium/Difficult**
- [ ] Translate system : **Medium/Difficult**
- [ ] Add more maps : **Difficult**
- [ ] Optimise the map with a tiled textures system instead of duplicate model faces : **Difficult**
