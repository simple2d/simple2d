// sound.c

#include "../include/simple2d.h"


/*
 * Create a sound, given an audio file path
 */
S2D_Sound *S2D_CreateSound(const char *path) {
  S2D_Init();

  // Check if sound file exists
  if (!S2D_FileExists(path)) {
    S2D_Error("S2D_CreateSound", "Sound file `%s` not found", path);
    return NULL;
  }

  // Allocate the sound structure
  S2D_Sound *snd = (S2D_Sound *) malloc(sizeof(S2D_Sound));
  if (!snd) {
    S2D_Error("S2D_CreateSound", "Out of memory!");
    return NULL;
  }

  // Load the sound data from file
  snd->data = Mix_LoadWAV(path);
  if (!snd->data) {
    S2D_Error("Mix_LoadWAV", Mix_GetError());
    free(snd);
    return NULL;
  }

  // Initialize values
  snd->path = path;

  return snd;
}


/*
 * Play the sound
 */
void S2D_PlaySound(S2D_Sound *snd) {
  if (!snd) return;
  Mix_PlayChannel(-1, snd->data, 0);
}


/*
 * Get the sound's volume
 */
int S2D_GetSoundVolume(S2D_Sound *snd) {
  if (!snd) return -1;
  return ceil(Mix_VolumeChunk(snd->data, -1) * (100.0 / MIX_MAX_VOLUME));
}


/*
 * Set the sound's volume a given percentage
 */
void S2D_SetSoundVolume(S2D_Sound *snd, int volume) {
  if (!snd) return;
  // Set volume to be a percentage of the maximum mix volume
  Mix_VolumeChunk(snd->data, (volume / 100.0) * MIX_MAX_VOLUME);
}


/*
 * Get the sound mixer volume
 */
int S2D_GetSoundMixVolume() {
  return ceil(Mix_Volume(-1, -1) * (100.0 / MIX_MAX_VOLUME));
}


/*
 * Set the sound mixer volume a given percentage
 */
void S2D_SetSoundMixVolume(int volume) {
  // This sets the volume value across all channels
  // Set volume to be a percentage of the maximum mix volume
  Mix_Volume(-1, (volume / 100.0) * MIX_MAX_VOLUME);
}


/*
 * Free the sound
 */
void S2D_FreeSound(S2D_Sound *snd) {
  if (!snd) return;
  Mix_FreeChunk(snd->data);
  free(snd);
}
