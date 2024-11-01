// ---  NEO PIXELS --- //
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define LED_PIN 6
#define LED_COUNT 80
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
int serpentineMap[80] = {
  // First 40 LEDs (Module 1)
  32, 33, 34, 35, 36, 37, 38, 39,  // Row 5 (right to left)
  31, 30, 29, 28, 27, 26, 25, 24,  // Row 4 (left to right)
  16, 17, 18, 19, 20, 21, 22, 23,  // Row 3 (right to left)
  15, 14, 13, 12, 11, 10, 9, 8,    // Row 2 (left to right)
  0, 1, 2, 3, 4, 5, 6, 7,          // Row 1 (right to left)

  // Second 40 LEDs (Module 2)
  40, 41, 42, 43, 44, 45, 46, 47,  // Row 1 (left to right)
  55, 54, 53, 52, 51, 50, 49, 48,  // Row 2 (right to left)
  56, 57, 58, 59, 60, 61, 62, 63,  // Row 3 (left to right)
  71, 70, 69, 68, 67, 66, 65, 64,  // Row 4 (right to left)
  72, 73, 74, 75, 76, 77, 78, 79   // Row 5 (left to right)
};


// 'A' mapped to first module (4x5 grid)

int A_pixels[] = {
  0, 1, 2, 3,      // Top row of 'A'
  8, 24, 32,       // Right vertical bar of 'A'
  19, 18, 17, 16,  // Middle bar of 'A'
  11, 27, 35       // Left vertical bar of 'A'
};

int B_pixels[] = {
  0, 1, 2, 3,      // Top bar of 'B'
  8, 24, 32,       // Left vertical bar of 'B'
  19, 18, 17, 16,  // Middle bar of 'B'
  11, 27, 35,      // Right vertical bar of 'B'
  34, 33           // Bottom bar of 'B'
};

int C_pixels[] = {
  0, 1, 2, 3,     // Top bar of 'C'
  11, 19, 27,     // Left vertical bar of 'C'
  35, 34, 33, 32  // Bottom bar of 'C'
};

int space_pixels[] = {};

// Define lengths of each letter array
int A_length = sizeof(A_pixels) / sizeof(A_pixels[0]);
int B_length = sizeof(B_pixels) / sizeof(B_pixels[0]);
int C_length = sizeof(C_pixels) / sizeof(C_pixels[0]);
int space_length = sizeof(C_pixels) / sizeof(space_pixels[0]);

// Create an array of pointers to each letter's pixel array
int* alphabetPixels[] = { A_pixels, B_pixels, C_pixels, space_length };  // You can expand this with more letters

// Create an array of lengths for each letter's pixel array
int alphabetLengths[] = { A_length, B_length, C_length };


void setup() {
  Serial.begin(9600);
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif


  strip.begin();            // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();             // Turn OFF all pixels ASAP
  strip.setBrightness(50);  // Set BRIGHTNESS to about 1/5 (max = 255)
  setState(SCROLLING_TEXT);
}

void loop() {
  //lightUpOneByOne(100);
  //lightUpColumns(1000);
  //highlightRows(1000);

  //displayLetterOnFirstModule('A', 1000);  // Show 'A' for 1 second
  //displayLetterOnFirstModule('B', 1000);  // Show 'B' for 1 second
  //displayLetterOnFirstModule('C', 1000);  // Show 'C' for 1 second
  //displayStringOnFirstModule("AB", 1000);

  scrollText("AB C", 500);
  //scrollLetterA(500);
  return;


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
        //scrollText("ABC", 1000);
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

// --- DEBUG TEXT --- //
void displayStringOnFirstModule(String text, int wait) {
  clearGrid();  // Clear the grid before displaying the string

  for (int charIndex = 0; charIndex < text.length(); charIndex++) {
    char letter = text[charIndex];  // Get the current letter
    int* currentLetter;
    int letterLength;

    // Select the correct letter pixels based on the current letter
    if (letter == 'A') {
      currentLetter = A_pixels;
      letterLength = A_length;
    } else if (letter == 'B') {
      currentLetter = B_pixels;
      letterLength = B_length;
    } else if (letter == 'C') {
      currentLetter = C_pixels;
      letterLength = C_length;
    }

    // Offset for each letter in the string (each letter is 4 columns wide)
    int offset = charIndex * 4;

    // Display each pixel of the current letter
    for (int i = 0; i < letterLength; i++) {
      int ledIndex = serpentineMap[currentLetter[i] + offset];    // Shift letter by 4 columns per letter
      strip.setPixelColor(ledIndex, strip.Color(255, 255, 255));  // Set pixel color to white
    }
  }

  strip.show();  // Show the updated LED states
  delay(wait);   // Wait for a specified time before clearing or updating the grid
}

void displayLetterOnFirstModule(char letter, int wait) {
  clearGrid();  // Clear the grid before displaying a letter

  int* currentLetter;
  int letterLength;

  // Choose the correct letter pixel map
  if (letter == 'A') {
    currentLetter = A_pixels;
    letterLength = A_length;
  } else if (letter == 'B') {
    currentLetter = B_pixels;
    letterLength = B_length;
  } else if (letter == 'C') {
    currentLetter = C_pixels;
    letterLength = C_length;
  }

  // Display each pixel of the current letter on the first module
  for (int i = 0; i < letterLength; i++) {
    int ledIndex = serpentineMap[currentLetter[i]];             // Map the letter's pixels to the serpentine layout
    strip.setPixelColor(ledIndex, strip.Color(255, 255, 255));  // Set pixel color to white
  }

  strip.show();  // Show the updated LED states
  delay(wait);   // Wait for a specified time before clearing or updating the grid
}

void lightUpColumns(int wait) {
  // Loop through all 8 columns (since you have 8 columns in the grid)
  for (int col = 0; col < 8; col++) {
    clearGrid();  // Clear the grid before lighting up the next column
    // Loop through all 5 rows (since you have 5 rows)
    for (int row = 0; row < 5; row++) {
      // Calculate the index in the serpentine map using the row and column
      int ledIndex = serpentineMap[row * 8 + col];            // Each row has 8 LEDs, hence row * 8 + col
      strip.setPixelColor(ledIndex, strip.Color(255, 0, 0));  // Light up the LED in red (you can change the color)
    }
    strip.show();  // Show the updated LED states
    delay(wait);   // Wait before lighting the next column
  }
}

void lightUpOneByOne(int wait) {
  // Loop through all 80 LEDs in the serpentineMap
  for (int i = 0; i < 80; i++) {
    clearGrid();  // Clear the grid before lighting up the next LED
    // Get the LED index from the serpentine map
    int ledIndex = serpentineMap[i];
    // Light up the current LED
    strip.setPixelColor(ledIndex, strip.Color(255, 0, 0));  // Light up in red (you can change color)
    strip.show();                                           // Show the updated LED states
    delay(wait);                                            // Wait before lighting the next LED
  }
}

void lightUpSerpentineGrid() {
  for (int i = 0; i < 80; i++) {
    int mappedIndex = serpentineMap[i];                            // Get the correct LED index from the serpentine map
    strip.setPixelColor(mappedIndex, strip.Color(255, 255, 255));  // Set each pixel to white
  }

  strip.show();  // Show all LEDs lit
  delay(2000);   // Keep them lit for 2 seconds
}

// --- SCROLLING TEXT ---
void scrollLetterA(int wait) {
  int letterWidth = 4;  // The width of the letter 'A'

  // Scroll from right to left across 16 columns
  for (int scrollPosition = -letterWidth; scrollPosition <= 16; scrollPosition++) {
    clearGrid();  // Clear the grid before each new frame

    // Loop through each pixel in the letter 'A'
    for (int i = 0; i < A_length; i++) {
      int columnOffset = A_pixels[i] % 4;               // Column position within the letter 'A'
      int pixelColumn = scrollPosition + columnOffset;  // Overall position in the scrolling grid

      // FIRST MODULE
      int ledIndex;
      if (pixelColumn >= 0 && pixelColumn < 8) {
        ledIndex = A_pixels[i] + scrollPosition;  // Get the pixel index with scroll offset
      } else if (pixelColumn >= 8 && pixelColumn < 16) {
        ledIndex = A_pixels[i] + scrollPosition + 32;  // Get the pixel index with scroll offset
      }

      if (pixelColumn >= 0 && pixelColumn < 16) {
        if (ledIndex >= 0 && ledIndex < 80) {
          int mappedIndex = serpentineMap[ledIndex];                 // Map to the correct LED in the serpentine layout
          strip.setPixelColor(mappedIndex, strip.Color(0, 255, 0));  // Set pixel color to red
        }
      }
    }



    strip.show();
    delay(wait);  // Wait before moving to the next frame
  }
}

String reverseString(String input) {
  String reversed = "";
  for (int i = input.length() - 1; i >= 0; i--) {
    reversed += input[i];
  }
  return reversed;
}

void scrollText(String text, int wait) {
  int letterWidth = 5;                             // Each letter is now 5 columns wide (4 for pixels, 1 for spacing)
  int numColumns = 16;                             // Total columns for both modules
  int totalColumns = text.length() * letterWidth;  // Calculate total width of the text in columns

  text = reverseString(text);

  // Scroll the entire text from right to left across both modules
  for (int scrollPosition = -totalColumns - numColumns; scrollPosition <= numColumns; scrollPosition++) {
    clearGrid();  // Clear the grid before each new frame

    // Loop through each character in the text
    for (int charIndex = 0; charIndex < text.length(); charIndex++) {
      char letter = text[charIndex];             // Get the current letter
      int letterIndex = getLetterIndex(letter);  // Get the index of the letter in the alphabet array

      // Retrieve the pixel map and length for the current letter
      int* currentLetter = alphabetPixels[letterIndex];
      int letterLength = alphabetLengths[letterIndex];

      // Calculate the starting column for this letter in the scroll
      int letterStartPosition = (charIndex * letterWidth) + scrollPosition;

      // Loop through each pixel in the current letter
      for (int i = 0; i < letterLength; i++) {
        int columnOffset = alphabetPixels[letterIndex][i] % 4;  // Column position within the letter (still 4-pixel width)
        int pixelColumn = letterStartPosition + columnOffset;   // Scrolling column position

        // Only display pixels within the visible grid (0 to 15 columns)
        if (pixelColumn >= 0 && pixelColumn < numColumns) {
          int ledIndex;

          // Determine if the pixel is in the first or second module
          if (pixelColumn < 8) {
            // First module (columns 0-7)
            ledIndex = currentLetter[i] + letterStartPosition;
          } else {
            // Second module (columns 8-15)
            ledIndex = currentLetter[i] + letterStartPosition + 32;  // Offset by 32 for the second module
          }

          // Map to the correct LED in the serpentine layout
          if (ledIndex >= 0 && ledIndex < 80) {
            int mappedIndex = serpentineMap[ledIndex];
            strip.setPixelColor(mappedIndex, strip.Color(255, 255, 255));  // Set pixel color to white
          }
        }
      }
    }

    strip.show();  // Show the updated LED states
    delay(wait);   // Wait before moving to the next frame
  }
}



int getLetterIndex(char letter) {
  if (letter == " ")
    return 4;
  else
    return letter - 'A';  // Returns 0 for 'A', 1 for 'B', etc.
}

void clearGrid() {
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));  // Turn off all pixels
  }
  strip.show();
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
