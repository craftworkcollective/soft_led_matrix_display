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
float attractDuration = 10.0 * 1000.0;

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif


  strip.begin();            // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();             // Turn OFF all pixels ASAP
  strip.setBrightness(50);  // Set BRIGHTNESS to about 1/5 (max = 255)
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
        colorWipe(strip.Color(0, 0, 255), 50);  // Blue
        break;
      }
    case OFF:
      {
        colorWipe(strip.Color(0, 0, 0), 0);
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
