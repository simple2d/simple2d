#include <simple2d.h>

typedef enum { TEST_FAIL = 0, TEST_PASS = 1 } TestStatus;

int tests = 0;
int failures = 0;


void start_test_suite() {
  puts("\033[1;39mStarting test suite...\033[1;39m\n");
}

void start_test(char *test) {
  printf("\033[1;34m==>\033[1;39m Running test\033[0m %s\n", test);
  tests++;
}

// Improved: Accepts a message for failed tests for better output consistency
void end_test(TestStatus status, const char *fail_msg) {
  if (status == TEST_FAIL) {
    failures++;
    printf("\033[1;31m==> Test failed: %s\033[0m\n", fail_msg);
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


void S2D_Init() {

  // Set Up ////////////////////////////////////////////////////////////////////
  S2D_Diagnostics(true);
  S2D_Windows_EnableTerminalColors();
  start_test_suite();
  S2D_CreateWindow("Simple 2D", 640, 480);

  // Logging ///////////////////////////////////////////////////////////////////
  // Test: Logging info, warning, and error messages
  start_test("(S2D_Log) log info, warning, and error messages");
  S2D_Log(S2D_INFO, "(S2D_Log) %s", "An `S2D_INFO` message");
  S2D_Log(S2D_WARN, "(S2D_Log) %s", "An `S2D_WARN` message");
  S2D_Log(S2D_ERROR, "(S2D_Log) %s", "An `S2D_ERROR` message");
  S2D_Error("S2D_Error", "An error message");
  end_test(TEST_PASS, "");

  // Images ////////////////////////////////////////////////////////////////////
  // Test: Create images with supported formats
  start_test("(S2D_CreateImage) create images with supported formats");
  S2D_Image *img1 = S2D_CreateImage("media/image.bmp");
  S2D_Image *img2 = S2D_CreateImage("media/image.jpg");
  S2D_Image *img3 = S2D_CreateImage("media/image.png");
  end_test((img1 != NULL && img2 != NULL && img3 != NULL) ? TEST_PASS : TEST_FAIL,
           "Failed to create images with supported formats");

  // Test: Create images with bad file paths (expect errors)
  start_test("(S2D_CreateImage) bad image file path (expect errors)");
  S2D_Image *img4 = S2D_CreateImage("image.bmp");
  S2D_Image *img5 = S2D_CreateImage("");
  S2D_Image *img6 = S2D_CreateImage(NULL);
  end_test((img4 == NULL && img5 == NULL && img6 == NULL) ? TEST_PASS : TEST_FAIL,
           "Bad image file paths did not return NULL as expected");

  // Test: Free images
  start_test("(S2D_FreeImage) free images");
  S2D_FreeImage(img1); S2D_FreeImage(img2); S2D_FreeImage(img3);
  S2D_FreeImage(img4); S2D_FreeImage(img5); S2D_FreeImage(img6);
  end_test(TEST_PASS, "");

  // Test: Free bad image pointer (NULL)
  start_test("(S2D_FreeImage) free bad image pointer");
  S2D_FreeImage(NULL);
  end_test(TEST_PASS, "");

  // Sprites ///////////////////////////////////////////////////////////////////
  // Test: Create sprites with supported formats
  start_test("(S2D_CreateImage) create sprites with supported formats");
  S2D_Image *spr1 = S2D_CreateImage("media/image.bmp");
  S2D_Image *spr2 = S2D_CreateImage("media/image.jpg");
  S2D_Image *spr3 = S2D_CreateImage("media/image.png");
  end_test((spr1 != NULL && spr2 != NULL && spr3 != NULL) ? TEST_PASS : TEST_FAIL,
           "Failed to create sprites with supported formats");

  // Test: Create sprites with bad file paths (expect errors)
  start_test("(S2D_CreateImage) bad sprite image file path (expect errors)");
  S2D_Image *spr4 = S2D_CreateImage("image.bmp");
  S2D_Image *spr5 = S2D_CreateImage("");
  S2D_Image *spr6 = S2D_CreateImage(NULL);
  end_test((spr4 == NULL && spr5 == NULL && spr6 == NULL) ? TEST_PASS : TEST_FAIL,
           "Bad sprite image file paths did not return NULL as expected");

  // Test: Crop sprites with S2D_ClipImage
  start_test("(S2D_ClipImage) crop sprites");
  S2D_ClipImage(spr3, 0, 0, 10, 20);
  S2D_ClipImage(spr3, -10, -20, 100, 200);
  S2D_ClipImage(spr3, 100, 200, -10, -20);
  end_test(TEST_PASS, "");

  // Test: Free sprites
  start_test("(S2D_FreeSprite) free sprites");
  S2D_FreeImage(spr1); S2D_FreeImage(spr2); S2D_FreeImage(spr3);
  S2D_FreeImage(spr4); S2D_FreeImage(spr5); S2D_FreeImage(spr6);
  end_test(TEST_PASS, "");

  // Test: Free bad sprite pointer (NULL)
  start_test("(S2D_FreeSprite) free bad sprite pointer");
  S2D_FreeImage(NULL);
  end_test(TEST_PASS, "");

  // Text //////////////////////////////////////////////////////////////////////
  // Test: Create text with good font file path
  start_test("(S2D_CreateText) good font file path");
  S2D_Text *txt1 = S2D_CreateText("./media/bitstream_vera/vera.ttf", "Hello World", 20);
  S2D_Text *txt2 = S2D_CreateText("media/bitstream_vera/vera.ttf", "Hello World", 20);
  end_test((txt1 != NULL && txt2 != NULL) ? TEST_PASS : TEST_FAIL,
           "Failed to create text with good font file path");

  // Test: Create text with bad font file path (expect errors)
  start_test("(S2D_CreateText) bad font file path (expect errors)");
  S2D_Text *txt3 = S2D_CreateText("font.ttf", "Hello World", 20);
  S2D_Text *txt4 = S2D_CreateText("", "Hello World", 20);
  S2D_Text *txt5 = S2D_CreateText(NULL, "Hello World", 20);
  end_test((txt3 == NULL && txt4 == NULL && txt5 == NULL) ? TEST_PASS : TEST_FAIL,
           "Bad font file paths did not return NULL as expected");

  // Test: Create text with empty message
  start_test("(S2D_CreateText) empty text message");
  S2D_Text *txt6 = S2D_CreateText("media/bitstream_vera/vera.ttf", "", 20);
  S2D_Text *txt7 = S2D_CreateText("media/bitstream_vera/vera.ttf", NULL, 20);
  end_test((txt6 != NULL && txt7 != NULL) ? TEST_PASS : TEST_FAIL,
           "Failed to create text with empty or NULL message");

  // Test: Set text to empty and NULL
  start_test("(S2D_SetText) empty text message");
  S2D_SetText(txt1, "A Different Message");
  S2D_SetText(txt1, "");
  S2D_SetText(txt1, NULL);
  end_test((txt1 != NULL) ? TEST_PASS : TEST_FAIL,
           "Failed to set text to empty or NULL");

  // Test: Free text
  start_test("(S2D_FreeText) free text");
  S2D_FreeText(txt1); S2D_FreeText(txt2); S2D_FreeText(txt3);
  S2D_FreeText(txt4); S2D_FreeText(txt5); S2D_FreeText(txt6);
  S2D_FreeText(txt7);
  end_test(TEST_PASS, "");

  // Test: Free bad text pointer (NULL)
  start_test("(S2D_FreeText) free bad text pointer");
  S2D_FreeText(NULL);
  end_test(TEST_PASS, "");

  // Audio /////////////////////////////////////////////////////////////////////
  // Test: Create audio with supported formats
  start_test("(S2D_CreateAudio) create audio with supported formats");
  S2D_Audio *aud1 = S2D_CreateAudio("media/sound.wav");
  S2D_Audio *aud2 = S2D_CreateAudio("media/sound.mp3");
  S2D_Audio *aud3 = S2D_CreateAudio("media/sound.ogg");
  S2D_Audio *aud4 = S2D_CreateAudio("media/sound.flac");
  end_test((aud1 != NULL && aud2 != NULL && aud3 != NULL && aud4 != NULL) ? TEST_PASS : TEST_FAIL,
           "Failed to create audio with supported formats");

  // Test: Create audio with bad file path (expect errors)
  start_test("(S2D_CreateAudio) bad audio file path (expect errors)");
  S2D_Audio *aud5 = S2D_CreateAudio("sound.wav");
  S2D_Audio *aud6 = S2D_CreateAudio("");
  S2D_Audio *aud7 = S2D_CreateAudio(NULL);
  end_test((aud5 == NULL && aud6 == NULL && aud7 == NULL) ? TEST_PASS : TEST_FAIL,
           "Bad audio file paths did not return NULL as expected");

  // Test: Free audio
  start_test("(S2D_FreeAudio) free audio");
  S2D_FreeAudio(aud1); S2D_FreeAudio(aud2); S2D_FreeAudio(aud3);
  S2D_FreeAudio(aud4); S2D_FreeAudio(aud5); S2D_FreeAudio(aud6);
  S2D_FreeAudio(aud7);
  end_test(TEST_PASS, "");

  // Test: Free bad sound pointer (NULL)
  start_test("(S2D_FreeSound) free bad sound pointer");
  S2D_FreeAudio(NULL);
  end_test(TEST_PASS, "");

  // Clean Up //////////////////////////////////////////////////////////////////
  end_test_suite();
  S2D_Close();
}
