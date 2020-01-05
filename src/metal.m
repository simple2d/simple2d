#import <Foundation/Foundation.h>
#include "../include/simple2d.h"

void objc_say_hello() {
  NSLog(@"Hello from Objective-C!\n");
  // puts("Hello from Objective-C!\n");
}

int objc_plus(int a, int b) {
  return a + b;
}

void objc_matrix(float orthoMatrix[4]) {
  printf("orthoMatrix[2]: %f\n", orthoMatrix[2]);
}
