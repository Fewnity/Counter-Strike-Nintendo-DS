// SPDX-License-Identifier: MIT
//
// Copyright (c) 2021-2022, Fewnity - Grégory Machefer
//
// This file is part of Counter Strike Nintendo DS Multiplayer Edition (CS:DS)

#include "main.h"
#include "sounds.h"
#include <maxmod9.h>
#include <fat.h>
#include <filesystem.h>

// Music remaining time
int musicLength = 0;
// Is music playing
bool isMusicPlaying = false;
// Are sounds loaded
bool soundsLoaded = false;
// Are sound bank file loaded
bool soundBankLoaded = false;
// Is music file is accessible
bool musicFilePresent = false;

// Music file
FILE *musicFile = NULL;

/**
 * @brief Stream callback for maxmod
 *
 * @param length remaining streaming time
 * @param dest
 * @param format
 * @return mm_word
 */
mm_word stream(mm_word length, mm_addr dest, mm_stream_formats format)
{
    if (musicFile)
    {
        size_t samplesize;
        switch (format)
        {
        case MM_STREAM_8BIT_MONO:
            samplesize = 1;
            break;
        case MM_STREAM_8BIT_STEREO:
            samplesize = 2;
            break;
        case MM_STREAM_16BIT_MONO:
            samplesize = 2;
            break;
        case MM_STREAM_16BIT_STEREO:
            samplesize = 4;
            break;
        default:
            samplesize = 2;
            break;
        }

        int res = fread(dest, samplesize, length, musicFile);

        if (res)
        {
            length = res;
        }
        else
        {
            mmStreamClose();
            fclose(musicFile);
            length = 0;
        }
    }
    musicLength = length;
    return length;
}

/**
 * @brief Init sound system and load sound bank and music
 *
 */
void initSoundSystem()
{
    // Init sound system
    // mmInitDefaultMem((mm_addr)soundbank_bin);
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

    //
    if (access("fat:/counter_strike_music.raw", F_OK) == 0 || access("counter_strike_music.raw", F_OK) == 0 || access("sd:/counter_strike_music.raw", F_OK) == 0)
    {
        musicFilePresent = true;
    }
    else
    {
        musicFilePresent = false;
    }

    if (access("fat:/soundbank.bin", F_OK) == 0 || access("soundbank.bin", F_OK) == 0 || access("sd:/soundbank.bin", F_OK) == 0)
    {
        // Load sound back
        mmSoundBankInFiles("soundbank.bin");
        soundBankLoaded = true;
    }
    else
    {
        // file doesn't exist
        soundBankLoaded = false;
    }
}

/**
 * @brief Start the music
 *
 */
void launchMusic()
{
    if (!isMusicPlaying)
    {
        isMusicPlaying = true;
        unloadSounds();
        loadMusic();
    }
}

/**
 * @brief Close music stream
 *
 */
void closeMusicSteam()
{
    if (musicFile)
    {
        mmStreamClose();
        fclose(musicFile);
    }
}

/**
 * @brief Load music file and open stream
 *
 */
void loadMusic()
{
    if (!musicFilePresent)
        return;

    closeMusicSteam();
    // open file
    musicFile = fopen("fat:/counter_strike_music.raw", "rb");
    if (musicFile == NULL)
    {
        musicFile = fopen("counter_strike_music.raw", "rb");
        if (musicFile == NULL)
        {
            musicFile = fopen("sd:/counter_strike_music.raw", "rb");
        }
    }

    // Open stream
    if (musicFile)
    {
        mm_stream mystream;
        mystream.buffer_length = 1024;
        mystream.callback = stream;
        mystream.timer = MM_TIMER1;
        mystream.manual = true;
        mystream.sampling_rate = 32000;
        mystream.format = MM_STREAM_8BIT_MONO;
        mmStreamOpen(&mystream);
    }
}

/**
 * @brief Stop music, close stream and load sounds
 *
 */
void stopMusic()
{
    if (isMusicPlaying)
    {
        closeMusicSteam();
        isMusicPlaying = FALSE;
        loadSounds();
    }
}

/**
 * @brief Load sounds from the sound bank and stop the music
 *
 */
void loadSounds()
{
    stopMusic();

    if (soundsLoaded || !soundBankLoaded)
    {
        return;
    }

    soundsLoaded = true;

    // Load sound effects
    for (int i = 0; i < MSL_NSAMPS; i++)
    {
        mmLoadEffect(i);
    }
}

/**
 * @brief Unload sounds
 *
 */
void unloadSounds()
{
    if (!soundsLoaded || !soundBankLoaded)
    {
        return;
    }

    soundsLoaded = false;

    // Unload sound effects
    for (int i = 0; i < MSL_NSAMPS; i++)
    {
        mmUnloadEffect(i);
    }
}

/**
 * @brief Play a step sound
 *
 * @param Volume Volume
 * @param Panning Panning (0-255) (left-right)
 * @param playerIndex Player index
 */
void DoStepSound(int Volume, int Panning, int playerIndex)
{
    if (!soundBankLoaded)
        return;

    if (Volume > 0)
    {
        mm_sfxhand mysound;

        // Play random sound
        if (AllPlayers[playerIndex].Step == 0)
            mysound = mmEffect(SFX_CONCRETE_CT_1);
        else if (AllPlayers[playerIndex].Step == 1)
            mysound = mmEffect(SFX_CONCRETE_CT_2);
        else if (AllPlayers[playerIndex].Step == 2)
            mysound = mmEffect(SFX_CONCRETE_CT_3);
        else
            mysound = mmEffect(SFX_CONCRETE_CT_4);

        // Set panning and volume
        mmEffectPanning(mysound, Panning);
        mmEffectVolume(mysound, Volume);
        AllPlayers[playerIndex].mapVisivilityTimer = 300;

        AllPlayers[playerIndex].Step++;
        if (AllPlayers[playerIndex].Step == 4)
            AllPlayers[playerIndex].Step = 0;
    }
}

/**
 * @brief Get panning between the local player and a player
 *
 * @param PlayerId Player index
 * @param OutPanning Out Panning (0-255) (left-right)
 * @param OutVolume Out Volume
 * @param xWithoutYForAudio
 * @param zWithoutYForAudio
 * @param MaxSoundDistance Max sound distance
 */
void GetPanning(int PlayerId, int *OutPanning, int *OutVolume, float xWithoutYForAudio, float zWithoutYForAudio, float MaxSoundDistance)
{
    // float MaxSoundDistance = 0.15; //0 = 0 meter; 0.05 = 7.65 meters; 1 = 255 meters
    float CenterOffset = 10;
    float Panning = -1;

    // With players positions, calculate volume and panning
    for (int i = 1; i < MaxPlayer; i++)
        if (AllPlayers[i].Id == PlayerId)
        {
            if (CurrentCameraPlayer != 0 && CurrentCameraPlayer != i)
                CalculatePlayerPosition(CurrentCameraPlayer);
            CalculatePlayerPosition(i);
            float Dis = sqrtf(powf(AllPlayers[CurrentCameraPlayer].position.x - AllPlayers[i].position.x, 2.0) + powf(AllPlayers[CurrentCameraPlayer].position.y - AllPlayers[i].position.y, 2.0) + powf(AllPlayers[CurrentCameraPlayer].position.z - AllPlayers[i].position.z, 2.0));
            *OutVolume = (int)fmax(255 - (Dis * 2.0) / MaxSoundDistance, 0);
            Panning = (xWithoutYForAudio * (AllPlayers[i].position.x - AllPlayers[CurrentCameraPlayer].position.x)) + (zWithoutYForAudio * (AllPlayers[i].position.z - AllPlayers[CurrentCameraPlayer].position.z));
            break;
        }

    // Panning ajustements
    if (Panning > CenterOffset)
        Panning = CenterOffset;
    else if (Panning < -CenterOffset)
        Panning = -CenterOffset;

    Panning += CenterOffset;
    Panning /= CenterOffset * 2.0;
    Panning *= 255.0;

    // return panning
    *OutPanning = (int)Panning;
}

/**
 * @brief
 * @brief Get panning between the camera and position
 *
 * @param OutPanning Out Panning (0-255) (left-right)
 * @param OutVolume Out Volume (0-255)
 * @param PositionB Position
 * @param xWithoutYForAudio
 * @param zWithoutYForAudio
 * @param MaxSoundDistance Max sound distance
 */
void GetPanningByPosition(int *OutPanning, int *OutVolume, Vector4 PositionB, float xWithoutYForAudio, float zWithoutYForAudio, float MaxSoundDistance)
{
    // float MaxSoundDistance = 0.15; //0 = 0 meter; 0.05 = 7.65 meters; 1 = 255 meters
    float CenterOffset = 10;
    float Panning = -1;

    if (CurrentCameraPlayer != 0)
        CalculatePlayerPosition(CurrentCameraPlayer);

    // With players positions, calculate volume and panning
    float Dis = sqrtf(powf(AllPlayers[CurrentCameraPlayer].position.x - PositionB.x, 2.0) + powf(AllPlayers[CurrentCameraPlayer].position.y - PositionB.y, 2.0) + powf(AllPlayers[CurrentCameraPlayer].position.z - PositionB.z, 2.0));
    *OutVolume = (int)fmax(255 - (Dis * 2.0) / MaxSoundDistance, 0);
    Panning = (xWithoutYForAudio * (PositionB.x - AllPlayers[CurrentCameraPlayer].position.x)) + (zWithoutYForAudio * (PositionB.z - AllPlayers[CurrentCameraPlayer].position.z));

    // Panning ajustements
    if (Panning > CenterOffset)
        Panning = CenterOffset;
    else if (Panning < -CenterOffset)
        Panning = -CenterOffset;

    Panning += CenterOffset;
    Panning /= CenterOffset * 2.0;
    Panning *= 255.0;

    // return panning
    *OutPanning = (int)Panning;
}

/**
 * @brief Play a sound in 3D space
 *
 * @param sound Sound to play
 * @param Volume Volume (0-255)
 * @param Panning Panning (0-255) (left-right)
 * @param player Player pointer
 */
void Play3DSound(mm_word sound, int Volume, int Panning, Player *player)
{
    if (!soundBankLoaded)
        return;

    // If the volume is 0, don't play the sound
    if (Volume > 0)
    {
        mm_sfxhand mysound = mmEffect(sound);

        // Set effect panning and volume
        mmEffectPanning(mysound, Panning);
        mmEffectVolume(mysound, Volume);
    }

    // Show the player on the map if the sound is loud
    if (Volume >= 25 && player != NULL)
        player->mapVisivilityTimer = 300;
}

/**
 * @brief Play a sound in 2D space
 *
 * @param sound Sound to play
 * @param Volume Volume (0-255)
 */
void Play2DSound(mm_word sound, int Volume)
{
    if (!soundBankLoaded)
        return;

    // If the volume is 0, don't play the sound
    if (Volume > 0)
    {
        mm_sfxhand mysound = mmEffect(sound);

        // Set effeft volume
        mmEffectVolume(mysound, Volume);
    }
}

/**
 * @brief Play a sound
 *
 * @param sound Sound to play
 */
void PlayBasicSound(mm_word sound)
{
    if (!soundBankLoaded)
        return;

    mmEffect(sound);
}
