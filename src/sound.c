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
 * Free the sound
 */
void S2D_FreeSound(S2D_Sound *snd) {
  if (!snd) return;
  Mix_FreeChunk(snd->data);
  free(snd);
}
