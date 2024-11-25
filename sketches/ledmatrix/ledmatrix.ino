// ---  NEO PIXELS --- //
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define LED_PIN 9
#define LED_COUNT 240
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
int serpentineMap[LED_COUNT] = {
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
  72, 73, 74, 75, 76, 77, 78, 79,  // Row 5 (left to right)

  // Third Module LED Mapping (Reversed)
  112, 113, 114, 115, 116, 117, 118, 119,  // Row 5 (left to right)
  111, 110, 109, 108, 107, 106, 105, 104,  // Row 4 (right to left)
  96, 97, 98, 99, 100, 101, 102, 103,      // Row 3 (left to right)
  95, 94, 93, 92, 91, 90, 89, 88,          // Row 2 (right to left)
  80, 81, 82, 83, 84, 85, 86, 87,          // Row 1 (left to right)

  // Fourth Module LED Mapping (Reversed)
  120, 121, 122, 123, 124, 125, 126, 127,  // Row 5 (left to right)
  135, 134, 133, 132, 131, 130, 129, 128,  // Row 4 (right to left)
  136, 137, 138, 139, 140, 141, 142, 143,  // Row 3 (left to right)
  151, 150, 149, 148, 147, 146, 145, 144,  // Row 2 (right to left)
  152, 153, 154, 155, 156, 157, 158, 159,  // Row 1 (left to right)


  // fifth Module LED Mapping (Reversed Order)
  192, 193, 194, 195, 196, 197, 198, 199,  // Row 5 (left to right)
  191, 190, 189, 188, 187, 186, 185, 184,  // Row 4 (right to left)
  176, 177, 178, 179, 180, 181, 182, 183,  // Row 3 (left to right)
  175, 174, 173, 172, 171, 170, 169, 168,  // Row 2 (right to left)
  160, 161, 162, 163, 164, 165, 166, 167,   // Row 1 (left to right)

      // Sixth Module LED Mapping (following serpentine layout)
  200, 201, 202, 203, 204, 205, 206, 207,  // Row 1 (left to right)
  215, 214, 213, 212, 211, 210, 209, 208,  // Row 2 (right to left)
  216, 217, 218, 219, 220, 221, 222, 223,  // Row 3 (left to right)
  231, 230, 229, 228, 227, 226, 225, 224,  // Row 4 (right to left)
  232, 233, 234, 235, 236, 237, 238, 239   // Row 5 (left to right)



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

int D_pixels[] = {
  0, 1, 2, 3,      // Top bar of 'D'
  11, 19, 27, 35,  // Right vertical bar of 'D'
  34, 33, 32,      // Bottom bar of 'D'
  0, 8, 16, 24     // Left vertical bar of 'D'
};

int E_pixels[] = {
  0, 1, 2, 3,      // Top bar of 'E'
  11,              // Upper left segment of 'E'
  16, 17, 18, 19,  // Middle bar of 'E'
  27,              // Lower left segment of 'E'
  32, 33, 34, 35   // Bottom bar of 'E'
};

int F_pixels[] = {
  0, 1, 2, 3,      // Top bar of 'F'
  11,              // Upper left segment of 'F'
  16, 17, 18, 19,  // Middle bar of 'F'
  27,              // Lower left segment of 'F'
  35               // Bottom left segment of 'F'
};

int G_pixels[] = {
  0, 1, 2, 3,     // Top bar of 'G'
  11,             // Upper left segment of 'G'
  16, 17, 19,     // Middle segment of 'G'
  24, 27,         // Lower middle segment of 'G'
  32, 33, 34, 35  // Bottom bar of 'G'
};

int H_pixels[] = {
  0, 3,            // Top left and right segments of 'H'
  8, 11,           // Upper middle left and right segments of 'H'
  16, 17, 18, 19,  // Middle bar of 'H'
  24, 27,          // Lower middle left and right segments of 'H'
  32, 35           // Bottom left and right segments of 'H'
};

int I_pixels[] = {
  1, 2,    // Top bar of 'I'
  9, 10,   // Upper middle segment of 'I'
  17, 18,  // Middle segment of 'I'
  25, 26,  // Lower middle segment of 'I'
  33, 34   // Bottom bar of 'I'
};


int J_pixels[] = {
  0,              // Top right segment of 'J'
  8,              // Upper middle right segment of 'J'
  16, 19,         // Middle right and left segments of 'J'
  24, 27,         // Lower middle right and left segments of 'J'
  32, 33, 34, 35  // Bottom bar of 'J'
};

int K_pixels[] = {
  0, 3,    // Top left and right segments of 'K'
  9, 11,   // Upper middle diagonal segments of 'K'
  18, 19,  // Middle segment of 'K'
  25, 27,  // Lower middle diagonal segments of 'K'
  32, 35   // Bottom left and right segments of 'K'
};

int L_pixels[] = {
  3,              // Top left segment of 'L'
  11,             // Upper middle left segment of 'L'
  19,             // Middle left segment of 'L'
  27,             // Lower middle left segment of 'L'
  32, 33, 34, 35  // Bottom bar of 'L'
};

int M_pixels[] = {
  0, 3,          // Top left and right segments of 'M'
  8, 9, 10, 11,  // Second row with inner peak of 'M'
  16, 19,        // Middle row left and right segments of 'M'
  24, 27,        // Lower middle left and right segments of 'M'
  32, 35         // Bottom left and right segments of 'M'
};


int N_pixels[] = {
  0, 3,        // Top left and right segments of 'N'
  8, 11,       // Upper middle left and right segments of 'N'
  16, 18, 19,  // Middle diagonal from left to right of 'N'
  24, 25, 27,  // Lower middle diagonal of 'N'
  32, 35       // Bottom left and right segments of 'N'
};

int O_pixels[] = {
  0, 1, 2, 3,     // Top bar of 'O'
  8, 11,          // Upper left and right segments of 'O'
  16, 19,         // Middle left and right segments of 'O'
  24, 27,         // Lower middle left and right segments of 'O'
  32, 33, 34, 35  // Bottom bar of 'O'
};

int P_pixels[] = {
  0, 1, 2, 3,      // Top bar of 'P'
  8, 11,           // Upper left and right segments of 'P'
  16, 17, 18, 19,  // Middle bar of 'P'
  27,              // Lower left segment of 'P'
  35               // Bottom left segment of 'P'
};

int Q_pixels[] = {
  0, 1, 2, 3,     // Top bar of 'Q'
  8, 11,          // Upper left and right segments of 'Q'
  17, 19,         // Middle left and right segments of 'Q'
  25, 27,         // Lower middle left and right segments of 'Q'
  32, 33, 34, 35  // Bottom bar of 'Q'
};

int R_pixels[] = {
  0, 1, 2, 3,      // Top bar of 'R'
  8, 11,           // Upper left and right segments of 'R'
  16, 17, 18, 19,  // Middle bar of 'R'
  25, 27,          // Lower middle diagonal of 'R'
  32, 35           // Bottom left and right segments of 'R'
};

int S_pixels[] = {
  0, 1, 2, 3,      // Top bar of 'S'
  11,              // Upper left segment of 'S'
  16, 17, 18, 19,  // Middle bar of 'S'
  24,              // Lower right segment of 'S'
  32, 33, 34, 35   // Bottom bar of 'S'
};

int T_pixels[] = {
  0, 1, 2, 3,  // Top bar of 'T'
  9,           // Upper middle segment of 'T'
  17,          // Middle segment of 'T'
  25,          // Lower middle segment of 'T'
  33           // Bottom middle segment of 'T'
};

int U_pixels[] = {
  0, 3,           // Top left and right segments of 'U'
  8, 11,          // Upper middle left and right segments of 'U'
  16, 19,         // Middle left and right segments of 'U'
  24, 27,         // Lower middle left and right segments of 'U'
  32, 33, 34, 35  // Bottom bar of 'U'
};

int V_pixels[] = {
  0, 3,            // Top left and right segments of 'V'
  8, 11,           // Upper middle left and right segments of 'V'
  16, 19,          // Middle left and right segments of 'V'
  24, 25, 26, 27,  // Lower middle bar of 'V'
  33, 34           // Bottom left and right segments of 'V'
};

int W_pixels[] = {
  0, 3,            // Top left and right segments of 'W'
  8, 11,           // Upper middle left and right segments of 'W'
  16, 19,          // Middle left and right segments of 'W'
  24, 25, 26, 27,  // Lower middle bar of 'W'
  32, 35           // Bottom left and right segments of 'W'
};

int X_pixels[] = {
  3,       // Top left segment of 'X'
  8, 10,   // Upper middle left and right segments of 'X'
  17,      // Middle segment of 'X'
  24, 26,  // Lower middle left and right segments of 'X'
  35       // Bottom right segment of 'X'
};

int Y_pixels[] = {
  0, 3,    // Top left and right segments of 'Y'
  8, 11,   // Upper middle left and right segments of 'Y'
  17, 18,  // Middle segment of 'Y'
  25, 26,  // Lower middle segment of 'Y'
  33, 34   // Bottom left and right segments of 'Y'
};

int Z_pixels[] = {
  0, 1, 2, 3,     // Top bar of 'Z'
  9,              // Upper middle segment of 'Z'
  18,             // Middle segment of 'Z'
  27,             // Lower middle segment of 'Z'
  32, 33, 34, 35  // Bottom bar of 'Z'
};



// Define the pixel arrays for each letter (A-Z) and space
int space_pixels[] = {};  // Empty space

// Define lengths of each letter array
int A_length = sizeof(A_pixels) / sizeof(A_pixels[0]);
int B_length = sizeof(B_pixels) / sizeof(B_pixels[0]);
int C_length = sizeof(C_pixels) / sizeof(C_pixels[0]);
int D_length = sizeof(D_pixels) / sizeof(D_pixels[0]);
int E_length = sizeof(E_pixels) / sizeof(E_pixels[0]);
int F_length = sizeof(F_pixels) / sizeof(F_pixels[0]);
int G_length = sizeof(G_pixels) / sizeof(G_pixels[0]);
int H_length = sizeof(H_pixels) / sizeof(H_pixels[0]);
int I_length = sizeof(I_pixels) / sizeof(I_pixels[0]);
int J_length = sizeof(J_pixels) / sizeof(J_pixels[0]);
int K_length = sizeof(K_pixels) / sizeof(K_pixels[0]);
int L_length = sizeof(L_pixels) / sizeof(L_pixels[0]);
int M_length = sizeof(M_pixels) / sizeof(M_pixels[0]);
int N_length = sizeof(N_pixels) / sizeof(N_pixels[0]);
int O_length = sizeof(O_pixels) / sizeof(O_pixels[0]);
int P_length = sizeof(P_pixels) / sizeof(P_pixels[0]);
int Q_length = sizeof(Q_pixels) / sizeof(Q_pixels[0]);
int R_length = sizeof(R_pixels) / sizeof(R_pixels[0]);
int S_length = sizeof(S_pixels) / sizeof(S_pixels[0]);
int T_length = sizeof(T_pixels) / sizeof(T_pixels[0]);
int U_length = sizeof(U_pixels) / sizeof(U_pixels[0]);
int V_length = sizeof(V_pixels) / sizeof(V_pixels[0]);
int W_length = sizeof(W_pixels) / sizeof(W_pixels[0]);
int X_length = sizeof(X_pixels) / sizeof(X_pixels[0]);
int Y_length = sizeof(Y_pixels) / sizeof(Y_pixels[0]);
int Z_length = sizeof(Z_pixels) / sizeof(Z_pixels[0]);
int space_length = sizeof(space_pixels) / sizeof(space_pixels[0]);

// Create an array of pointers to each letter's pixel array
int* alphabetPixels[] = { A_pixels, B_pixels, C_pixels, D_pixels, E_pixels, F_pixels, G_pixels,
                          H_pixels, I_pixels, J_pixels, K_pixels, L_pixels, M_pixels, N_pixels,
                          O_pixels, P_pixels, Q_pixels, R_pixels, S_pixels, T_pixels, U_pixels,
                          V_pixels, W_pixels, X_pixels, Y_pixels, Z_pixels, space_pixels };

// Create an array of lengths for each letter's pixel array
int alphabetLengths[] = { A_length, B_length, C_length, D_length, E_length, F_length, G_length,
                          H_length, I_length, J_length, K_length, L_length, M_length, N_length,
                          O_length, P_length, Q_length, R_length, S_length, T_length, U_length,
                          V_length, W_length, X_length, Y_length, Z_length, space_length };


String textArray = "IT IS FIVE THIRTY PM ON MONDAY DECEMBER TWELFTH AND I WILL CALL THIS WORK SESSION TO ORDER UMM FIRST ON OUR AGENDA OUR CITY UPDATES SO MISS SOLANO PLEASE TAKE IT AWAY THANK YOU MADAM CHAIR COUNSELORS AND MAYOR GRADISAR UM I DO HAVE TWO OR THREE UPDATES THIS EVENING FROM THE CITY AND THE FIRST IS A COVID UPDATE UH THIS MORNING THE PUBLIC DEPARTMENT OF PUBLIC HEALTH AND ENVIRONMENT PIO OFFICER HELD A UNIFIED MESSAGING MEETING WITH UM UPDATES ON COVID THE ONE WEEK CUMULATIVE POSITIVE CASE COUNT HAD DECREASED FROM LAST WEEK AND IT WAS THIS DURING THIS PAST WEEK ONE HUNDRED NINETY FIVE I REPORTED LAST MONDAY A TWO HUNDRED EIGHT AND THE ONE WEEK POSITIVITY RATE IS ALSO DOWN IT IS TEN PERCENT AS OF TODAY AND THIRTEEN PERCENT REPORTED LAST WEEK PUEBLO COUNTY HAS NINE DAYS OF DECLINING OR STABLE PATIENT HOSPITALIZATIONS AND AS OF THE REPORT THIS MORNING THERE ARE SEVENTEEN COVID CASES AT PARKVIEW ELEVEN RSV CASES SIX ADULTS AND FIVE PEDIATRIC CASES AND ONE CASE OF THE INFLUENZA AND THAT IS THE COVID UPDATE WITH RESPECT TO THE PERCENTAGE TOTALS THE NATIONAL WEATHER SERVICE IS PREDICTING LOW TEMPERATURES IN THE COMING DAYS AND ACCORDINGLY MAYOR GRADISAR HAS DECLARED A TEMPORARY HOUSING AND A SHELTER EMERGENCY BEGINNING AT FIVE PM THIS COMING WEDNESDAY DECEMBER THE FOURTEENTH AND ENDING AT SEVEN AM NEXT MONDAY MORNING DECEMBER THE NINETEENTH SO WE ARE IN FOR A STRETCH OF COLD NIGHTS AND THIS DOES ALLOW THE OVERNIGHT WARMING SHELTER AT CRAZY FAITH MINISTRIES TO OPEN WEDNESDAY THURSDAY FRIDAY SATURDAY SUNDAY INTO A MONDAY MORNING COUNCIL WINNER HAS REQUESTED REGULAR UPDATES UM ON THE ARPA TEAM UP TO CLEAN UP TRASH PLAN FOR THE CITY AND I DO HAVE A FEW UPDATES SINCE I LAST REPORTED THE COORDINATOR POSITION INTERVIEWS WERE HELD THIS MORNING AND AN INDIVIDUAL WAS SELECTED FOR THAT POSITION UH THERE WILL BE MORE ON WHO AS IT GOES THROUGH THE CIVIL SERVICE AND UM BACKGROUND CHECK PROCESS THERE ARE TODAY SEVEN APPLICANTS FOR THE PART TIME POSITIONS AND PARKS WILL MOVE FORWARD TO BEGIN TO HIRE THE PART TIMERS NOW THAT THE FULL TIME COORDINATOR HAVE BEEN HIRED UH THE FIRST THE FIRST OF FOUR COMMUNITY BILLBOARDS WERE UP TODAY THAT WERE ADVERTISING THE TRASH REMOVAL PROGRAM AND POSITIONS THERE WILL BE ONE ON MAIN STREET ON BARNFORD AND TWO INITIALLY ON FOURTH STREET WITH THE ONE OF THEM ON FOURTH STREET TO MOVE ON TO NORTHERN IN JANUARY YOU CAN ACTUALLY VIEW THIS EVENING OR TODAY FROM THE INTERSTATE THE BILLBOARD ON SOUTH MAIN HEADING SOUTH ON THE INTERSTATE UM AND THERE WILL BE UM ON FOX TWENTY ONE RUNNING THIS WEEK A PROMOTIONAL COMMERCIAL ABOUT THE TRASH PROGRAM AND THE JOBS AVAILABLE AS PART OF THE WE ARE PUEBLO SPOTLIGHT AND WE WILL BRING THE COMMERCIAL FOR ALL OF YOU AT THE DECEMBER TWENTY SEVENTH MEETING THAT DOES CONCLUDE THE CITY UPDATES THIS EVENING AND DO YOU HAVE ANY QUESTIONS OR DIRECTIONS ON FUTURE INFORMATION TO BRING TO COUNCIL THANK YOU MISS SOLANO FOR THOSE UPDATES WHAT QUESTIONS ARE THERE FROM COUNCIL";

void setup() {
  Serial.begin(9600);
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif


  strip.begin();            // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();             // Turn OFF all pixels ASAP
  strip.setBrightness(50);  // Set BRIGHTNESS to about 1/5 (max = 255)
  clearGrid();
  //setState(SCROLLING_TEXT);
}

void loop() {
  //lightUpOneByOne(100);
  scrollText("HELLO", 500);
  //scrollText(textArray, 500); 
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
        scrollText(textArray, 500); 
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
  for (int i = 0; i < LED_COUNT; i++) {
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
  int numColumns = 48;                             // Total columns for both modules
  int totalColumns = text.length() * letterWidth;  // Calculate total width of the text in columns

  text = reverseString(text);
  // Scroll the entire text from right to left across both modules
  for (int scrollPosition = -totalColumns; scrollPosition <= numColumns; scrollPosition++) {

    //Serial.println(scrollPosition);
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
        int columnOffset = currentLetter[i] % 4;               // Column position within the letter (still 4-pixel width)
        int pixelColumn = letterStartPosition + columnOffset;  // Scrolling column position

        // Only display pixels within the visible grid
        if (pixelColumn >= 0 && pixelColumn < numColumns) {
          int moduleNumber = pixelColumn / 8;
          int rowPosition = currentLetter[i] / 4 / 2;

          // Calculate the ledIndex across the module boundary
          int moduleStart = moduleNumber * 40;
          int columnPositionWithinModule = pixelColumn % 8;
          int ledIndex = moduleStart + rowPosition * 8 + columnPositionWithinModule;

          // Map to the correct LED in the serpentine layout
          if (ledIndex >= 0 && ledIndex < LED_COUNT) {
            int mappedIndex = serpentineMap[ledIndex];
            strip.setPixelColor(mappedIndex, strip.Color(100, 100, 100));  // Set pixel color to white
          }
        }
      }
    }


    strip.show();  // Show the updated LED states
    delay(wait);   // Wait before moving to the next frame
  }
}


int getLetterIndex(char letter) {
  if (letter == ' ')
    return 26;
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
