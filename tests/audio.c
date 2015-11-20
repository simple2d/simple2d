#include <simple2d.h>

bool is_sound = true;
int sound = 1;
int music = 2;
Sound snd_1;
Sound snd_2;
Music mus_1;
Music mus_2;
Window *window;

void print_help() {
  // ┌ ┬ ┐ ┤ ┘ ┴ └ ├ ┼ ─ │
  puts("┌───────────────────────────────┐");
  puts("│ AUDIO TESTS KEYBOARD COMMANDS │");
  puts("├───────────────────────────────┤");
  puts("│ Q -> Switch between           │");
  puts("│      `Sound` and `Music`      │");
  puts("│                               │");
  puts("│ Sound:                        │");
  puts("│   P -> S2D_PlaySound          │");
  puts("│                               │");
  puts("│ Music:                        │");
  puts("│   P -> S2D_PlayMusic          │");
  puts("│   A -> S2D_PauseMusic         │");
  puts("│   S -> S2D_StopMusic          │");
  puts("│   R -> S2D_ResumeMusic        │");
  puts("│   F -> S2D_FadeOutMusic       │");
  puts("│                               │");
  puts("│ Switching Samples:            │");
  puts("│   1 -> note.wav               │");
  puts("│   2 -> music.ogg              │");
  puts("│                               │");
  puts("│ H -> Print this help message  │");
  puts("└───────────────────────────────┘");
  puts("");
}

void on_key(const char *key) {
  
  switch (*key) {
    case 'H':
      print_help();
      break;
    case 'Q':
      is_sound = is_sound ? false: true;
      if (is_sound) {
        puts("Switching to Sound...");
      } else {
        puts("Switching to Music...");
      }
      break;
    case 'P':
      if (is_sound) {
        puts("S2D_PlaySound...");
        if (sound == 1) {
          S2D_PlaySound(snd_1);
        } else {
          S2D_PlaySound(snd_2);
        }
      } else {
        puts("S2D_PlayMusic...");
        if (music == 1) {
          S2D_PlayMusic(mus_1, -1);
        } else {
          S2D_PlayMusic(mus_2, -1);
        }
      }
      break;
    case 'A':
      puts("S2D_PauseMusic...");
      S2D_PauseMusic();
      break;
    case 'R':
      puts("S2D_ResumeMusic...");
      S2D_ResumeMusic();
      break;
    case 'S':
      puts("S2D_StopMusic...");
      S2D_StopMusic();
      break;
    case 'F':
      puts("S2D_FadeOutMusic...");
      S2D_FadeOutMusic(2000);
      break;
    case '1':
      puts("Switching to sound.wav...");
      if (is_sound) {
        sound = 1;
      } else {
        music = 1;
      }
      break;
    case '2':
      puts("Switching to music.ogg...");
      if (is_sound) {
        sound = 2;
      } else {
        music = 2;
      }
      break;
  }
}

int main(int argc, char const *argv[]) {
  
  window = S2D_CreateWindow(
    "Audio", 200, 150, NULL, NULL, 0
  );
  
  window->on_key = on_key;
  
  snd_1 = S2D_CreateSound("media/sound.wav");
  snd_2 = S2D_CreateSound("media/music.ogg");
  
  mus_1 = S2D_CreateMusic("media/sound.wav");
  mus_2 = S2D_CreateMusic("media/music.ogg");
  
  print_help();
  
  S2D_Show(window);
  return 0;
}
