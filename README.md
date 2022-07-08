# Counter Strike Nintendo DS
**Counter Strike port on Nintendo DS with online multiplayer** (Works on **emulator** and on **real hardware**).

For **multiplayer** on **emulator**, please use [Melon DS](https://melonds.kuribo64.net/downloads.php).<br/>
There is no emulator with **online** support on **Android** at this time.<br/>
**Ad hoc (Nifi) multiplayer** is not supported (I'm looking for help about that!)<br/>

![image](https://user-images.githubusercontent.com/39272935/176778790-14707835-d15d-473b-baeb-3cb51864b004.png)
![image](https://user-images.githubusercontent.com/39272935/176778935-59555499-c40e-4325-83f8-8d89778a75af.png)

## How to install the game

### On DS
- Put the .nds file in the SD card
- Put the soundbank.bin and the counter_strike_music.raw files the root of the SD card

### On 3DS
- Install [TWiLight Menu++](https://wiki.ds-homebrew.com/twilightmenu/installing-3ds)
- Put the .nds, the soundbank.bin and the counter_strike_music.raw files in the SD card in \roms\nds\

### On emulator (Melon DS) :
To have sounds and musics, you need to use a virtual SD card. Because sounds and music are not included in the .nds file.

- Set in the DLDI settings (Config->Emu settings->DLDI) the counter_strike_sd.raw file as SD card. Or create your own [here](https://melonds.kuribo64.net/board/thread.php?pid=2902) and put the soundbank.bin and the counter_strike_music.raw files in the virtual SD card

## Network Setup
- Create a Wifi without a password or with a WEP protection. You can use your phone/a compatible router or a Linux/Mac OS computer with Wifi adapter to do a hotspot
### On DS
- Start a game that **supports online multiplayer** like **Mario Kart**
- Open the **Wifi setup** and select an available wifi slot
- Search an access point and add your wifi

### On 3DS/2DS
- Open **settings**
- Open **Internet settings**
- Open the **Nintendo DS connection** menu and select an available wifi slot
- Search an access point and add your wifi

## How to compile the game
- Install [DevKitPro](https://github.com/devkitPro/installer/releases/latest) and check the **NDS Development** option
- Download the modified version of Nitro Engine [here](https://github.com/Fewnity/nitro-engine) and put the folder like this : **"C:\devkitPro\nitro-engine"**
- Compile the engine (Open a **CMD**, write **"cd C:\devkitPro\nitro-engine"** then **"make"**)
- Put the game source code folder in **"C:\devkitPro\nitro-engine\examples"**
- Compile the game (Open a **CMD**, write **"cd C:\devkitPro\nitro-engine\examples\Counter-Strike-nds"** then **"make"**)
- Copy the **Counter-Strike-nds.nds** from the project folder and the **soundbank.bin** file from the build folder

Note : To avoid coding collisions, path finding..., there is an Unity project to export code : https://github.com/Fewnity/Counter-Strike-DS-Unity-Project

## TODO
- [ ] Ad hoc (Nifi) multiplayer : **Difficult**
- [ ] Disable sleep mode in multiplayer : **?**
- [ ] Add fall damage : **Easy**
- [ ] Smoke doesn't affect bots at this time : **Easy**
- [ ] Online chat (empty menu already exists) : **Medium**
- [ ] Create a NitroFS version for sounds and musics (and keep the FAT version for better compatibility) : **Medium**
- [ ] Improve bots (make bots stays close to the bomb area for exemple) : **Medium**
- [ ] Add Wingman mode : **Medium**
- [ ] Add input profiles : **Medium**
- [ ] Possibility to change cursor : **Medium**
- [ ] Add bot difficulty : **Medium/Difficult**
- [ ] Translate system : **Medium/Difficult**
- [ ] Add more maps : **Difficult**
- [ ] Optimise the map with a tiled textures system instead of duplicate model faces : **Difficult**
