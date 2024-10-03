#include <Adafruit_NeoPixel.h>

// Define the pin for the NeoPixel data line
#define PIN 6

// Define the number of NeoPixels
#define NUMPIXELS 30 // 5x6 grid

// Initialize the NeoPixel library
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Adjusted pixel mapping for the new serpentine layout
int serpentineMap[30] = {
  4, 3, 2, 1, 0,    // Row 1 (right to left)
  5, 6, 7, 8, 9,    // Row 2 (left to right)
  14, 13, 12, 11, 10, // Row 3 (right to left)
  15, 16, 17, 18, 19, // Row 4 (left to right)
  24, 23, 22, 21, 20, // Row 5 (right to left)
  25, 26, 27, 28, 29  // Row 6 (left to right)
};

void setup() {
  pixels.begin(); // Initialize the NeoPixel strip
  pixels.setBrightness(50); // Set brightness level (0-255)
  pixels.show();  // Ensure all pixels are off
}

// Function to clear the grid
void clearGrid() {
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Turn off all pixels
  }
  pixels.show();
}

// Function to display a specific letter by passing pixel array
void displayLetter(int letter_pixels[], int size) {
  clearGrid();
  for(int i=0; i<size; i++) {
    int mappedIndex = serpentineMap[letter_pixels[i]];
    pixels.setPixelColor(mappedIndex, pixels.Color(173, 316, 200)); // color
  }
  pixels.show();
}

// Functions for each letter

void displayLetterA() {
  int A_pixels[] = {0, 1, 2, 3, 4, 5, 9, 10, 11, 12, 13, 14, 15, 19, 20, 24, 25, 29};
  displayLetter(A_pixels, sizeof(A_pixels)/sizeof(int));
}

void displayLetterB() {
  int B_pixels[] = {0, 1, 2, 3, 4, 5, 9, 10, 11, 12, 13, 14, 15, 19, 20, 24, 25, 26,27,28, 29};
  displayLetter(B_pixels, sizeof(B_pixels)/sizeof(int));
}

void displayLetterC() {
  int C_pixels[] = {0, 1, 2, 3, 4, 5, 10, 15, 20, 25, 26, 27, 28, 29};
  displayLetter(C_pixels, sizeof(C_pixels)/sizeof(int));

}

void loop() {
  displayLetterA();
  delay(4000); // Pause for 4 seconds

  displayLetterB();
  delay(4000); // Pause for 4 seconds

  displayLetterC();
  delay(4000); // Pause for 4 seconds

  // After Z, the loop repeats from A
}
