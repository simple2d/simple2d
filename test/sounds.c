#include <simple2d.h>

bool is_music = true;
S2D_Audio *music;
S2D_Audio *sound;
S2D_Window *window;

void print_help() {
  puts("");
  puts(" |-------------------------------|");
  puts(" | AUDIO TESTS KEYBOARD COMMANDS |");
  puts(" |-------------------------------|");
  puts(" | Q -> Switch between music and |");
  puts(" |      sound samples            |");
  puts(" |                               |");
  puts(" | P -> S2D_PlayAudio            |");
  puts(" | A -> S2D_PauseAudio           |");
  puts(" | S -> S2D_StopAudio            |");
  puts(" | R -> S2D_ResumeAudio          |");
  puts(" | F -> S2D_FadeOutAudio         |");
  puts(" |                               |");
  puts(" | H -> Print this help message  |");
  puts(" | Esc -> Exit                   |");
  puts(" |-------------------------------|");
  puts("");
}

void S2D_Init() {
  window = S2D_CreateWindow("Simple 2D — Audio", 200, 150);

  music = S2D_CreateAudio("media/music.ogg");
  sound = S2D_CreateAudio("media/sound.wav");

  print_help();
}

void S2D_OnKey(S2D_Event e) {
  if (e.type != S2D_KEY_DOWN) return;

  if (S2D_KeyIs(e, "Escape")) S2D_Close();

  switch (*e.key) {
    case 'H':
      print_help();
      break;
    case 'Q':
      is_music = !is_music;
      puts(is_music ? "Switching to Music..." : "Switching to Sound...");
      break;
    case 'P':
      puts("S2D_PlayAudio...");
      S2D_PlayAudio(is_music ? music : sound);
      break;
    case 'A':
      puts("S2D_PauseAudio...");
      S2D_PauseAudio(is_music ? music : sound);
      break;
    case 'R':
      puts("S2D_ResumeAudio...");
      S2D_ResumeAudio(is_music ? music : sound);
      break;
    case 'S':
      puts("S2D_StopAudio...");
      S2D_StopAudio(is_music ? music : sound, 0);
      break;
    case 'F':
      puts("S2D_StopAudio with fade...");
      S2D_StopAudio(is_music ? music : sound, 2000);
      break;
  }
}

void S2D_Quit() {
  S2D_FreeAudio(music);
  S2D_FreeAudio(sound);
}
