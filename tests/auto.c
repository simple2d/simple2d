#include <simple2d.h>

#define FAIL false
#define PASS true

int tests = 0;
int failures = 0;


void start_test_suite() {
  puts("\n\033[1;39mStarting test suite...\033[1;39m\n");
}

void start_test(char *test) {
  printf("\033[1;34m==>\033[1;39m Running test\033[0m %s\n", test);
  tests++;
}

void end_test(bool status) {
  if (status == FAIL) {
    failures++;
    puts("\033[1;31m==> Test failed!\033[0m");
  }
}

void end_test_suite() {
  if (failures == 0) {
    puts("\n\033[1;32mAll tests passed!\033[0m");
  } else {
    puts("\n\033[1;31mTest suite failed!\033[0m");
  }
  printf("%i examples, %i failures\n\n", tests, failures);
}


int main() {
  
  // Set Up ////////////////////////////////////////////////////////////////////
  
  S2D_Diagnostics(true);
  
  start_test_suite();
  
  // Images ////////////////////////////////////////////////////////////////////
  
  start_test("(S2D_CreateImage) create images with supported formats");
  S2D_Image *img1 = S2D_CreateImage("media/image.bmp");
  S2D_Image *img2 = S2D_CreateImage("media/image.jpg");
  S2D_Image *img3 = S2D_CreateImage("media/image.png");
  end_test(img1 != NULL && img2 != NULL && img3 != NULL);
  
  start_test("(S2D_CreateImage) bad image file path");
  S2D_Image *img4 = S2D_CreateImage("image.bmp");
  S2D_Image *img5 = S2D_CreateImage("");
  S2D_Image *img6 = S2D_CreateImage(NULL);
  end_test(img4 == NULL && img5 == NULL && img6 == NULL);
  
  start_test("(S2D_FreeImage) free images");
  S2D_FreeImage(img1); S2D_FreeImage(img2); S2D_FreeImage(img3);
  S2D_FreeImage(img4); S2D_FreeImage(img5); S2D_FreeImage(img6);
  end_test(PASS);
  
  start_test("(S2D_FreeImage) free bad image pointer");
  S2D_FreeImage(NULL);
  end_test(PASS);
  
  // Sprites ///////////////////////////////////////////////////////////////////
  
  start_test("(S2D_CreateSprite) create sprites with supported formats");
  S2D_Sprite *spr1 = S2D_CreateSprite("media/image.bmp");
  S2D_Sprite *spr2 = S2D_CreateSprite("media/image.jpg");
  S2D_Sprite *spr3 = S2D_CreateSprite("media/image.png");
  end_test(spr1 != NULL && spr2 != NULL && spr3 != NULL);
  
  start_test("(S2D_CreateSprite) bad sprite image file path");
  S2D_Sprite *spr4 = S2D_CreateSprite("image.bmp");
  S2D_Sprite *spr5 = S2D_CreateSprite("");
  S2D_Sprite *spr6 = S2D_CreateSprite(NULL);
  end_test(spr4 == NULL && spr5 == NULL && spr6 == NULL);
  
  start_test("(S2D_CropSprite) crop sprites");
  S2D_ClipSprite(spr3, 0, 0, 10, 20);
  S2D_ClipSprite(spr3, -10, -20, 100, 200);
  S2D_ClipSprite(spr3, 100, 200, -10, -20);
  end_test(PASS);
  
  start_test("(S2D_FreeSprite) free sprites");
  S2D_FreeSprite(spr1); S2D_FreeSprite(spr2); S2D_FreeSprite(spr3);
  S2D_FreeSprite(spr4); S2D_FreeSprite(spr5); S2D_FreeSprite(spr6);
  end_test(PASS);
  
  start_test("(S2D_FreeSprite) free bad sprite pointer");
  S2D_FreeSprite(NULL);
  end_test(PASS);
  
  // Text //////////////////////////////////////////////////////////////////////
  
  start_test("(S2D_CreateText) good font file path");
  S2D_Text *txt1 = S2D_CreateText("./media/bitstream_vera/vera.ttf", "Hello World", 20);
  S2D_Text *txt2 = S2D_CreateText("media/bitstream_vera/vera.ttf", "Hello World", 20);
  end_test(txt1 != NULL && txt2 != NULL);
  
  start_test("(S2D_CreateText) bad font file path");
  S2D_Text *txt3 = S2D_CreateText("font.ttf", "Hello World", 20);
  S2D_Text *txt4 = S2D_CreateText("", "Hello World", 20);
  S2D_Text *txt5 = S2D_CreateText(NULL, "Hello World", 20);
  end_test(txt3 == NULL && txt4 == NULL && txt5 == NULL);
  
  start_test("(S2D_CreateText) empty text message");
  S2D_Text *txt6 = S2D_CreateText("media/bitstream_vera/vera.ttf", "", 20);
  S2D_Text *txt7 = S2D_CreateText("media/bitstream_vera/vera.ttf", NULL, 20);
  end_test(txt6 != NULL && txt7 != NULL);
  
  start_test("(S2D_SetText) empty text message");
  S2D_SetText(txt1, "A Different Message");
  S2D_SetText(txt1, "");
  S2D_SetText(txt1, NULL);
  end_test(txt1 != NULL);
  
  start_test("(S2D_FreeText) free text");
  S2D_FreeText(txt1); S2D_FreeText(txt2); S2D_FreeText(txt3);
  S2D_FreeText(txt4); S2D_FreeText(txt5); S2D_FreeText(txt6);
  S2D_FreeText(txt7);
  end_test(PASS);
  
  start_test("(S2D_FreeText) free bad text pointer");
  S2D_FreeText(NULL);
  end_test(PASS);
  
  // Sound /////////////////////////////////////////////////////////////////////
  
  start_test("(S2D_CreateSound) create sound with supported formats");
  S2D_Sound *snd1 = S2D_CreateSound("media/sound.wav");
  S2D_Sound *snd2 = S2D_CreateSound("media/sound.mp3");
  S2D_Sound *snd3 = S2D_CreateSound("media/sound.ogg");
  end_test(snd1 != NULL && snd2 != NULL && snd3 != NULL);
  
  start_test("(S2D_CreateSound) bad sound file path");
  S2D_Sound *snd4 = S2D_CreateSound("sound.wav");
  S2D_Sound *snd5 = S2D_CreateSound("");
  S2D_Sound *snd6 = S2D_CreateSound(NULL);
  end_test(snd4 == NULL && snd5 == NULL && snd6 == NULL);
  
  start_test("(S2D_FreeSound) free sound");
  S2D_FreeSound(snd1); S2D_FreeSound(snd2); S2D_FreeSound(snd3);
  S2D_FreeSound(snd4); S2D_FreeSound(snd5); S2D_FreeSound(snd6);
  end_test(PASS);
  
  start_test("(S2D_FreeSound) free bad sound pointer");
  S2D_FreeSound(NULL);
  end_test(PASS);
  
  // Music /////////////////////////////////////////////////////////////////////
  
  start_test("(S2D_CreateMusic) create music with supported formats");
  S2D_Music *mus1 = S2D_CreateMusic("media/music.wav");
  S2D_Music *mus2 = S2D_CreateMusic("media/music.mp3");
  S2D_Music *mus3 = S2D_CreateMusic("media/music.ogg");
  S2D_Music *mus4 = S2D_CreateMusic("media/music.flac");
  end_test(mus1 != NULL && mus2 != NULL && mus3 != NULL && mus4 != NULL);
  
  start_test("(S2D_CreateMusic) bad music file path");
  S2D_Music *mus5 = S2D_CreateMusic("music.wav");
  S2D_Music *mus6 = S2D_CreateMusic("");
  S2D_Music *mus7 = S2D_CreateMusic(NULL);
  end_test(mus5 == NULL && mus6 == NULL && mus7 == NULL);
  
  start_test("(S2D_FreeMusic) free music");
  S2D_FreeMusic(mus1); S2D_FreeMusic(mus2); S2D_FreeMusic(mus3);
  S2D_FreeMusic(mus4); S2D_FreeMusic(mus5); S2D_FreeMusic(mus6);
  S2D_FreeMusic(mus7);
  end_test(PASS);
  
  start_test("(S2D_FreeMusic) free bad music pointer");
  S2D_FreeMusic(NULL);
  end_test(PASS);
  
  // Window ////////////////////////////////////////////////////////////////////
  
  S2D_Window *window = S2D_CreateWindow("Hello World", 100, 100, NULL, NULL, 0);
  
  start_test("(S2D_FreeWindow) free window");
  S2D_FreeWindow(window);
  end_test(PASS);
  
  start_test("(S2D_FreeWindow) create multiple windows");
  S2D_Window *win1 = S2D_CreateWindow("Window 1", 300, 100, NULL, NULL, 0);
  S2D_Window *win2 = S2D_CreateWindow("Window 2", 300, 100, NULL, NULL, 0);
  S2D_FreeWindow(win1); S2D_FreeWindow(win2);
  end_test(PASS);
  
  start_test("(S2D_Show) show a bad window pointer");
  S2D_Show(NULL);
  end_test(PASS);
  
  // Clean Up //////////////////////////////////////////////////////////////////
  
  end_test_suite();
  
  return failures;
}
