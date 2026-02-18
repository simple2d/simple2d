/**
 * SDL3 Test Application
 *
 * Tests SDL3 core functionality along with SDL3_image, SDL3_mixer, and SDL3_ttf.
 * Uses the SDL3 callback-based application model.
 */

#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

/* ============================================================================
 * Constants
 * ============================================================================ */

static const int WINDOW_WIDTH = 640;
static const int WINDOW_HEIGHT = 480;
static const char *WINDOW_TITLE = "hello world";

static const char *FONT_PATH = "./test/media/bitstream_vera/vera.ttf";
static const char *IMG_PATH = "./test/media/image.png";
static const char *MUSIC_PATH = "./test/media/music.wav";

/* ============================================================================
 * Global State
 * ============================================================================ */

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *image_texture = NULL;
static SDL_Texture *message_texture = NULL;

/* ============================================================================
 * Helper Functions
 * ============================================================================ */

static SDL_AppResult SDL_Fail(void) {
  SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error: %s", SDL_GetError());
  return SDL_APP_FAILURE;
}

/* ============================================================================
 * SDL Application Callbacks
 * ============================================================================ */

/**
 * Called once at application startup.
 * Initializes SDL subsystems, creates window/renderer, and loads resources.
 */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  /* Initialize SDL core */
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    return SDL_Fail();
  }

  if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT,
                                   0, &window, &renderer)) {
    return SDL_Fail();
  }

  /* Initialize and load font */
  if (!TTF_Init()) {
    return SDL_Fail();
  }

  TTF_Font *font = TTF_OpenFont(FONT_PATH, 36);
  if (!font) {
    return SDL_Fail();
  }

  /* Render text to texture */
  const char *text_message = "Hello SDL!";
  SDL_Color text_color = {255, 255, 255, 255};
  SDL_Surface *text_surface = TTF_RenderText_Solid(font, text_message,
                                                   strlen(text_message), text_color);
  if (!text_surface) {
    return SDL_Fail();
  }
  message_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
  SDL_DestroySurface(text_surface);
  TTF_CloseFont(font);

  /* Load SVG image */
  SDL_Surface *svg_surface = IMG_Load(IMG_PATH);
  if (!svg_surface) {
    return SDL_Fail();
  }
  image_texture = SDL_CreateTextureFromSurface(renderer, svg_surface);
  SDL_DestroySurface(svg_surface);

  /* Initialize audio mixer and play music */
  if (!MIX_Init()) {
    return SDL_Fail();
  }

  MIX_Mixer *mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
  if (!mixer) {
    return SDL_Fail();
  }

  MIX_Audio *music = MIX_LoadAudio(mixer, MUSIC_PATH, false);
  if (!music) {
    return SDL_Fail();
  }

  MIX_Track *track = MIX_CreateTrack(mixer);
  if (!track) {
    return SDL_Fail();
  }

  if (!MIX_SetTrackAudio(track, music)) {
    return SDL_Fail();
  }

  if (!MIX_PlayTrack(track, 0)) {
    return SDL_Fail();
  }

  return SDL_APP_CONTINUE;
}

/**
 * Called for each SDL event.
 * Handles quit events and escape key to exit.
 */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }

  if (event->type == SDL_EVENT_KEY_DOWN &&
      event->key.scancode == SDL_SCANCODE_ESCAPE) {
    return SDL_APP_SUCCESS;
  }

  return SDL_APP_CONTINUE;
}

/**
 * Called once per frame.
 * Renders animated background color with image and text overlays.
 */
SDL_AppResult SDL_AppIterate(void *appstate) {
  const double now = (double)SDL_GetTicks() / 1000.0;

  /* Animate background color using phase-shifted sine waves */
  const float red   = (float)(0.5 + 0.5 * SDL_sin(now));
  const float green = (float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2.0 / 3.0));
  const float blue  = (float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4.0 / 3.0));

  SDL_SetRenderDrawColorFloat(renderer, red, green, blue, SDL_ALPHA_OPAQUE_FLOAT);
  SDL_RenderClear(renderer);

  /* Draw textures */
  SDL_RenderTexture(renderer, image_texture, NULL, NULL);
  SDL_RenderTexture(renderer, message_texture, NULL, NULL);

  SDL_RenderPresent(renderer);

  return SDL_APP_CONTINUE;
}

/**
 * Called once at application shutdown.
 * SDL automatically cleans up window and renderer.
 */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  SDL_DestroyTexture(image_texture);
  SDL_DestroyTexture(message_texture);
}
