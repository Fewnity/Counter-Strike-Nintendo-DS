#include "main.h"
#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"
#include <math.h>

int Step = 0;

void initSoundSystem()
{
    //Init sound system
    //mmInitDefaultMem((mm_addr)soundbank_bin);
    mm_ds_system sys;

    // number of modules in your soundbank (defined in output header)
    sys.mod_count = MSL_NSONGS;

    // number of samples in your soundbank (defined in output header)
    sys.samp_count = MSL_NSAMPS;

    // memory bank, allocate BANKSIZE (or NSONGS+NSAMPS) words
    sys.mem_bank = malloc(MSL_BANKSIZE * 4);

    // select fifo channel
    sys.fifo_channel = FIFO_MAXMOD;

    // initialize maxmod
    mmInit(&sys);
    //mmSelectMode(MM_MODE_C);

    //Load sound back
    mmSoundBankInFiles("soundbank.bin");

    //Load sound effects
    mmLoadEffect(SFX_TERRORISTSWIN);
    mmLoadEffect(SFX_COUNTERTERRORISTWIN);
    mmLoadEffect(SFX_DETONATE);
    mmLoadEffect(SFX_BOMBPLANTING);
    mmLoadEffect(SFX_BOMBEXPLODE);
    mmLoadEffect(SFX_BOMBHASBEENDEFUSED);
    mmLoadEffect(SFX_BOMBHASBEENPLANTED);
    mmLoadEffect(SFX_BOMBBIP);
    /*mmLoadEffect(SFX_AWP);
	mmLoadEffect(SFX_GLOCK);
	mmLoadEffect(SFX_M4A1);
	mmLoadEffect(SFX_MP7);
	mmLoadEffect(SFX_MP9);
	mmLoadEffect(SFX_NOVA);
	mmLoadEffect(SFX_USP);
	mmLoadEffect(SFX_XM1014);*/
    mmLoadEffect(SFX_KNIFE_HIT_PLAYER);
    mmLoadEffect(SFX_KNIFE_HIT_WALL);
    mmLoadEffect(SFX_RIC);
    mmLoadEffect(SFX_HEADSHOT1);
    mmLoadEffect(SFX_DEATH);
    mmLoadEffect(SFX_FLESH_IMPACT);

    mmLoadEffect(SFX_CONCRETE_CT_1);
    mmLoadEffect(SFX_CONCRETE_CT_2);
    mmLoadEffect(SFX_CONCRETE_CT_3);
    mmLoadEffect(SFX_CONCRETE_CT_4);

    mmLoadEffect(SFX_LAND);

    mmLoadEffect(SFX_HEGRENADE_EXPLOSE);
    mmLoadEffect(SFX_HEGRENADE_BOUNCE);

    mmLoadEffect(SFX_FLASHBANG_BOUNCE);
    mmLoadEffect(SFX_SMOKE_EMIT);
    mmLoadEffect(SFX_MOLOTOV_DETONATE);
    mmLoadEffect(SFX_FLASHBANG_EXPLODE);
    mmLoadEffect(SFX_FIRE_LOOP);

    mmLoadEffect(SFX_AK47);
    mmLoadEffect(SFX_AUG);
    mmLoadEffect(SFX_DEAGLE);
    mmLoadEffect(SFX_ELITE);
    mmLoadEffect(SFX_FAMAS);
    mmLoadEffect(SFX_FIVESEVEN);
    mmLoadEffect(SFX_G3SG1);
    mmLoadEffect(SFX_GALIL);
    mmLoadEffect(SFX_GLOCK18);
    mmLoadEffect(SFX_M249);
    mmLoadEffect(SFX_M3);
    mmLoadEffect(SFX_M4A1);
    mmLoadEffect(SFX_MAC10);
    mmLoadEffect(SFX_MP5);
    mmLoadEffect(SFX_P228);
    mmLoadEffect(SFX_P90);
    mmLoadEffect(SFX_SCOUT);
    mmLoadEffect(SFX_SG550);
    mmLoadEffect(SFX_SG552);
    mmLoadEffect(SFX_TMP);
    mmLoadEffect(SFX_UMP45);
    mmLoadEffect(SFX_USP);
    mmLoadEffect(SFX_XM1014);
    mmLoadEffect(SFX_XM1014);
    mmLoadEffect(SFX_ZOOM);

    /*file = fopen("awp_01.raw", "rb");

	mm_stream mystream;
	mystream.buffer_length = 1024;
	mystream.callback = stream;
	mystream.timer = MM_TIMER0;
	mystream.manual = true;
	mystream.sampling_rate = 22050;
	mystream.format = MM_STREAM_16BIT_STEREO;
	mmStreamOpen(&mystream);

	while (1)
	{

		mmStreamUpdate();

		mm_word position = mmStreamGetPosition() / 22050;
		iprintf("\x1b[12;13H%02d:%02d\n", position / 60, position % 60);

		swiWaitForVBlank();
	}*/
}

void DoStepSound(int Volume, int Panning, int playerIndex)
{
    if (Volume > 0)
    {
        mm_sfxhand mysound;

        //Play random sound
        if (AllPlayers[playerIndex].Step == 0)
            mysound = mmEffect(SFX_CONCRETE_CT_1);
        else if (Step == 1)
            mysound = mmEffect(SFX_CONCRETE_CT_1);
        else if (Step == 2)
            mysound = mmEffect(SFX_CONCRETE_CT_2);
        else
            mysound = mmEffect(SFX_CONCRETE_CT_3);

        //Set panning and volume
        mmEffectPanning(mysound, Panning);
        mmEffectVolume(mysound, Volume);

        AllPlayers[playerIndex].Step++;
        if (AllPlayers[playerIndex].Step == 4)
            AllPlayers[playerIndex].Step = 0;
    }
}

void GetPanning(int PlayerId, int *OutPanning, int *OutVolume, Player AllPlayers[], float xWithoutYForAudio, float zWithoutYForAudio, float MaxSoundDistance)
{
    //float MaxSoundDistance = 0.15; //0 = 0 meter; 0.05 = 7.65 meters; 1 = 255 meters
    float CenterOffset = 10;
    float Panning = -1;

    //With players positions, calculate volume and panning
    for (int i = 1; i < MaxPlayer; i++)
        if (AllPlayers[i].Id == PlayerId)
        {
            if (CurrentCameraPlayer != 0 && CurrentCameraPlayer != i)
                CalculatePlayerPosition(CurrentCameraPlayer);
            CalculatePlayerPosition(i);
            float Dis = sqrtf(powf(AllPlayers[CurrentCameraPlayer].xPos - AllPlayers[i].xPos, 2.0) + powf(AllPlayers[CurrentCameraPlayer].yPos - AllPlayers[i].yPos, 2.0) + powf(AllPlayers[CurrentCameraPlayer].zPos - AllPlayers[i].zPos, 2.0));
            *OutVolume = (int)fmax(255 - (Dis * 2.0) / MaxSoundDistance, 0);
            Panning = (xWithoutYForAudio * (AllPlayers[i].xPos - AllPlayers[CurrentCameraPlayer].xPos)) + (zWithoutYForAudio * (AllPlayers[i].zPos - AllPlayers[CurrentCameraPlayer].zPos));
            break;
        }

    //Panning ajustements
    if (Panning > CenterOffset)
        Panning = CenterOffset;
    else if (Panning < -CenterOffset)
        Panning = -CenterOffset;

    Panning += CenterOffset;
    Panning /= CenterOffset * 2.0;
    Panning *= 255.0;

    //return panning
    *OutPanning = (int)Panning;
}

void GetPanningByPosition(int *OutPanning, int *OutVolume, Player AllPlayers[], Vector4 PositionB, float xWithoutYForAudio, float zWithoutYForAudio, float MaxSoundDistance)
{
    //float MaxSoundDistance = 0.15; //0 = 0 meter; 0.05 = 7.65 meters; 1 = 255 meters
    float CenterOffset = 10;
    float Panning = -1;

    if (CurrentCameraPlayer != 0)
        CalculatePlayerPosition(CurrentCameraPlayer);

    //With players positions, calculate volume and panning
    float Dis = sqrtf(powf(AllPlayers[CurrentCameraPlayer].xPos - PositionB.x, 2.0) + powf(AllPlayers[CurrentCameraPlayer].yPos - PositionB.y, 2.0) + powf(AllPlayers[CurrentCameraPlayer].zPos - PositionB.z, 2.0));
    *OutVolume = (int)fmax(255 - (Dis * 2.0) / MaxSoundDistance, 0);
    Panning = (xWithoutYForAudio * (PositionB.x - AllPlayers[CurrentCameraPlayer].xPos)) + (zWithoutYForAudio * (PositionB.z - AllPlayers[CurrentCameraPlayer].zPos));

    //Panning ajustements
    if (Panning > CenterOffset)
        Panning = CenterOffset;
    else if (Panning < -CenterOffset)
        Panning = -CenterOffset;

    Panning += CenterOffset;
    Panning /= CenterOffset * 2.0;
    Panning *= 255.0;

    //return panning
    *OutPanning = (int)Panning;
}

void Play3DSound(mm_word sound, int Volume, int Panning)
{
    if (Volume > 0)
    {
        mm_sfxhand mysound = mmEffect(sound);

        //Set effect panning and volume
        mmEffectPanning(mysound, Panning);
        mmEffectVolume(mysound, Volume);
    }
}

void Play2DSound(mm_word sound, int Volume)
{
    if (Volume > 0)
    {
        mm_sfxhand mysound = mmEffect(sound);

        //Set effeft volume
        mmEffectVolume(mysound, Volume);
    }
}

void PlayBasicSound(mm_word sound)
{
    mmEffect(sound);
}
