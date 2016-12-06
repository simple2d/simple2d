// music.c

#include "../include/simple2d.h"


/*
 * Create the music
 */
S2D_Music *S2D_CreateMusic(const char *path) {
  S2D_Init();
  
  // Check if music file exists
  if (!S2D_FileExists(path)) {
    S2D_Error("S2D_CreateMusic", "Music file not found");
    return NULL;
  }
  
  // Allocate the music structure
  S2D_Music *music = (S2D_Music *) malloc(sizeof(S2D_Music));
  if (!music) {
    S2D_Error("S2D_CreateMusic", "Out of memory!");
    return NULL;
  }
  
  // Load the music data from file
  music->data = Mix_LoadMUS(path);
  if (!music->data) {
    S2D_Error("Mix_LoadMUS", Mix_GetError());
    free(music);
    return NULL;
  }
  
  return music;
}


/*
 * Play the music
 */
void S2D_PlayMusic(S2D_Music *music, bool loop) {
  if (!music) return;
  
  // If looping, set to -1 times; else 0
  int times = loop ? -1 : 0;
  
  // times: 0 == once, -1 == forever
  if (Mix_PlayMusic(music->data, times) == -1) {
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
 * Stops the playing music; interrupts fader effects
 */
void S2D_StopMusic() {
  Mix_HaltMusic();
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
void S2D_FreeMusic(S2D_Music *music) {
  if (!music) return;
  Mix_FreeMusic(music->data);
  free(music);
}
