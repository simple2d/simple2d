// music.c

#include "../include/simple2d.h"


/*
 * Create the music
 */
S2D_Music *S2D_CreateMusic(const char *path) {
  S2D_Init();

  // Check if music file exists
  if (!S2D_FileExists(path)) {
    S2D_Error("S2D_CreateMusic", "Music file `%s` not found", path);
    return NULL;
  }

  // Allocate the music structure
  S2D_Music *mus = (S2D_Music *) malloc(sizeof(S2D_Music));
  if (!mus) {
    S2D_Error("S2D_CreateMusic", "Out of memory!");
    return NULL;
  }

  // Load the music data from file
  mus->data = Mix_LoadMUS(path);
  if (!mus->data) {
    S2D_Error("Mix_LoadMUS", Mix_GetError());
    free(mus);
    return NULL;
  }

  // Initialize values
  mus->path = path;

  return mus;
}


/*
 * Play the music
 */
void S2D_PlayMusic(S2D_Music *mus, bool loop) {
  if (!mus) return;

  // If looping, set to -1 times; else 0
  int times = loop ? -1 : 0;

  // times: 0 == once, -1 == forever
  if (Mix_PlayMusic(mus->data, times) == -1) {
    // No music for you
    S2D_Error("S2D_PlayMusic", Mix_GetError());
  }
}


/*
 * Pause the playing music
 */
void S2D_PauseMusic() {
  Mix_PauseMusic();
}


/*
 * Resume the current music
 */
void S2D_ResumeMusic() {
  Mix_ResumeMusic();
}


/*
 * Stop the playing music; interrupts fader effects
 */
void S2D_StopMusic() {
  Mix_HaltMusic();
}


/*
 * Get the music volume
 */
int S2D_GetMusicVolume() {
  // Get music volume as percentage of maximum mix volume
  return ceil(Mix_VolumeMusic(-1) * (100.0 / MIX_MAX_VOLUME));
}


/*
 * Set the music volume a given percentage
 */
void S2D_SetMusicVolume(int volume) {
  // Set volume to be a percentage of the maximum mix volume
  Mix_VolumeMusic((volume / 100.0) * MIX_MAX_VOLUME);
}


/*
 * Fade out the playing music
 */
void S2D_FadeOutMusic(int ms) {
  Mix_FadeOutMusic(ms);
}


/*
 * Free the music
 */
void S2D_FreeMusic(S2D_Music *mus) {
  if (!mus) return;
  Mix_FreeMusic(mus->data);
  free(mus);
}
