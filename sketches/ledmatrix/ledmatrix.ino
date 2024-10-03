// ---  NEO PIXELS --- //
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define LED_PIN 6
#define LED_COUNT 30
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// ---  STATE MACHINE --- //
enum STATES {
  LOADING,
  ATTRACT,
  SCROLLING_TEXT,
  OFF,
  NUM_STATES
};


STATES state = LOADING;
float startTime = 0.0;
float attractDuration = 2.0 * 1000.0;

// --- SCROLLING TEXT --- //
int serpentineMap[30] = {
  4, 3, 2, 1, 0,       // Row 1 (right to left)
  5, 6, 7, 8, 9,       // Row 2 (left to right)
  14, 13, 12, 11, 10,  // Row 3 (right to left)
  15, 16, 17, 18, 19,  // Row 4 (left to right)
  24, 23, 22, 21, 20,  // Row 5 (right to left)
  25, 26, 27, 28, 29   // Row 6 (left to right)
};

int A_pixels[] = { 0, 1, 2, 3, 4, 5, 9, 10, 11, 12, 13, 14, 15, 19, 20, 24, 25, 29 };
int B_pixels[] = { 0, 1, 2, 3, 4, 5, 9, 10, 11, 12, 13, 14, 15, 19, 20, 24, 25, 26, 27, 28, 29 };
int C_pixels[] = { 0, 1, 2, 3, 4, 5, 10, 15, 20, 25, 26, 27, 28, 29 };

// Define the lengths of each letter pixel array
int A_length = sizeof(A_pixels) / sizeof(A_pixels[0]);
int B_length = sizeof(B_pixels) / sizeof(B_pixels[0]);
int C_length = sizeof(C_pixels) / sizeof(C_pixels[0]);

// Define a 2D array to hold all letters and their lengths
int* letters[] = {A_pixels, B_pixels, C_pixels};  // Add all letters
int letterLengths[] = {A_length, B_length, C_length};  // Add all letter lengths

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif


  strip.begin();            // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();             // Turn OFF all pixels ASAP
  strip.setBrightness(50);  // Set BRIGHTNESS to about 1/5 (max = 255)
  setState(SCROLLING_TEXT);
}

void loop() {

  // ---  STATE MACHINE --- //
  switch (state) {
    case LOADING:
      {
        colorWipe(strip.Color(0, 255, 0), 50);  // green
        setState(ATTRACT);
        break;
      }
    case ATTRACT:
      {
        rainbow(10);
        break;
      }
    case SCROLLING_TEXT:
      {
        scrollText("ABC", 1000);
        break;
      }
    case OFF:
      {
        clearGrid();
        break;
      }
    default:
      {
        break;
      }
  }
}

void setState(STATES newState) {
  startTime = millis();
  state = newState;

  switch (state) {
    case LOADING:
      {
        break;
      }
    case ATTRACT:
      {
        break;
      }
    case SCROLLING_TEXT:
      {
        break;
      }
    case OFF:
      {
        break;
      }
    default:
      {
        break;
      }
  }
}

// --- SCROLLING TEXT --- //
void clearGrid() {
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));  // Turn off all pixels
  }
  strip.show();
}

void scrollText(String text, int wait) {
  int totalColumns = text.length() * 5; // Each letter is 5 columns wide

  // For each step in the scrolling animation (each column shift)
  for (int scrollPosition = 0; scrollPosition < totalColumns + 5; scrollPosition++) {
    clearGrid();  // Clear the grid before each new frame

    // For each character in the text
    for (int charIndex = 0; charIndex < text.length(); charIndex++) {
      char c = text[charIndex];
      int letterIndex = c - 'A';  // Map character to letter array
      int* currentLetter = letters[letterIndex];  // Get current letter pixel data
      int currentLetterLength = letterLengths[letterIndex];  // Get the letter's length

      // Determine the starting column for this letter in the grid
      int letterStartPosition = charIndex * 5 - scrollPosition;

      // Only display columns that are currently visible in the 5x6 grid
      if (letterStartPosition >= -5 && letterStartPosition < 5) {
        // For each pixel of the current letter
        for (int i = 0; i < currentLetterLength; i++) {
          // Calculate the actual grid position using the serpentine map
          int ledIndex = serpentineMap[currentLetter[i] % LED_COUNT];
          int columnPosition = (currentLetter[i] % 5) + letterStartPosition;

          // Check if the column is within the visible grid range
          if (columnPosition >= 0 && columnPosition < 5) {
            strip.setPixelColor(ledIndex, strip.Color(255, 255, 255));  // Turn on the pixel
          }
        }
      }
    }

    strip.show();  // Update the strip with the new frame
    delay(wait);   // Wait before moving to the next frame
  }
}


// --- ANIMATIONS --- //
void checkTimerInRainbow() {
  float currentDuration = millis() - startTime;
  if (currentDuration > attractDuration) {
    setState(SCROLLING_TEXT);
  }
}

void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) {  // For each pixel in strip...
    strip.setPixelColor(i, color);               //  Set pixel's color (in RAM)
    strip.show();                                //  Update strip to match
    delay(wait);                                 //  Pause for a moment
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
    strip.rainbow(firstPixelHue);
    strip.show();  // Update strip with new contents
    delay(wait);   // Pause for a moment

    // check timer
    checkTimerInRainbow();
  }
}
