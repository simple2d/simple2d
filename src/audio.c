// audio.c

#include "../include/simple2d.h"


/*
 * Create audio, given an audio file path
 */
S2D_Audio *S2D_CreateAudio(const char *path) {
  // Check if audio file exists
  if (!S2D_FileExists(path)) {
    S2D_Error("S2D_CreateAudio", "Audio file `%s` not found", path);
    return NULL;
  }

  // Allocate the audio structure
  S2D_Audio *aud = (S2D_Audio *) malloc(sizeof(S2D_Audio));
  if (!aud) {
    S2D_Error("S2D_CreateAudio", "Out of memory!");
    return NULL;
  }

  MIX_Audio *mix_audio = MIX_LoadAudio(s2d_app.sdl_mixer, path, false);
  if (!mix_audio) {
    S2D_Error("MIX_LoadAudio", SDL_GetError());
    free(aud);
    return NULL;
  }

  MIX_Track *mix_track = MIX_CreateTrack(s2d_app.sdl_mixer);
  if (!mix_track) {
    S2D_Error("MIX_CreateTrack", SDL_GetError());
    MIX_DestroyAudio(mix_audio);
    free(aud);
    return NULL;
  }

  // Initialize values
  aud->path = strdup(path);
  aud->mix_audio = mix_audio;
  aud->mix_track = mix_track;

  return aud;
}


/*
 * Play the audio
 */
void S2D_PlayAudio(S2D_Audio *aud) {
  if (!aud) return;

  // Assign input to this track before playing (required in SDL3_mixer)
  if (!MIX_SetTrackAudio(aud->mix_track, aud->mix_audio)) {
    S2D_Error("MIX_SetTrackAudio", SDL_GetError());
  }

  if (!MIX_PlayTrack(aud->mix_track, 0)) {
    S2D_Error("MIX_PlayTrack", SDL_GetError());
  }
}


/*
 * Pause the audio
 */
void S2D_PauseAudio(S2D_Audio *aud) {
  if (!aud) return;
  if (!MIX_PauseTrack(aud->mix_track)) {
    S2D_Error("MIX_PauseTrack", SDL_GetError());
  }
}


/*
 * Resume the audio
 */
void S2D_ResumeAudio(S2D_Audio *aud) {
  if (!aud) return;
  if (!MIX_ResumeTrack(aud->mix_track)) {
    S2D_Error("MIX_ResumeTrack", SDL_GetError());
  }
}


/*
 * Stop the audio, optionally fading out over ms_fade milliseconds.
 * Pass 0 for an immediate stop.
 */
void S2D_StopAudio(S2D_Audio *aud, int ms_fade) {
  if (!aud) return;
  if (ms_fade < 0) ms_fade = 0;

  if (!MIX_StopTrack(aud->mix_track, MIX_TrackMSToFrames(aud->mix_track, ms_fade))) {
    S2D_Error("MIX_StopTrack", SDL_GetError());
  }
}


/*
 * Get the audio's volume (percentage, 0-100)
 */
int S2D_GetAudioVolume(S2D_Audio *aud) {
  if (!aud) return 0;
  const float gain = MIX_GetTrackGain(aud->mix_track);  // typically 0.0 .. 1.0
  int volume = (int) SDL_roundf(gain * 100.0f);

  if (volume < 0) volume = 0;
  if (volume > 100) volume = 100;
  return volume;
}


/*
 * Set the audio volume by percentage (0-100)
 */
void S2D_SetAudioVolume(S2D_Audio *aud, int volume) {
  if (!aud) return;
  if (volume < 0) volume = 0;
  if (volume > 100) volume = 100;

  const float gain = (float) volume / 100.0f;
  if (!MIX_SetTrackGain(aud->mix_track, gain)) {
    S2D_Error("MIX_SetTrackGain", SDL_GetError());
  }
}


/*
 * Get the audio mixer volume (percentage, 0-100)
 */
int S2D_GetAudioMixerVolume() {
  const float gain = MIX_GetMixerGain(s2d_app.sdl_mixer);  // typically 0.0 .. 1.0
  int volume = (int) SDL_roundf(gain * 100.0f);

  if (volume < 0) volume = 0;
  if (volume > 100) volume = 100;
  return volume;
}


/*
 * Set the audio mixer volume a given percentage (0-100)
 */
void S2D_SetAudioMixerVolume(int volume) {
  if (volume < 0) volume = 0;
  if (volume > 100) volume = 100;

  const float gain = (float) volume / 100.0f;
  if (!MIX_SetMixerGain(s2d_app.sdl_mixer, gain)) {
    S2D_Error("MIX_SetMixerGain", SDL_GetError());
  }
}


/*
 * Free the audio
 */
void S2D_FreeAudio(S2D_Audio *aud) {
  if (!aud) {
    return;
  }

  if (aud->mix_track) {
    MIX_DestroyTrack(aud->mix_track);
    aud->mix_track = NULL;
  }

  if (aud->mix_audio) {
    MIX_DestroyAudio(aud->mix_audio);
    aud->mix_audio = NULL;
  }

  if (aud->path) {
    free(aud->path);
    aud->path = NULL;
  }

  free(aud);
}
