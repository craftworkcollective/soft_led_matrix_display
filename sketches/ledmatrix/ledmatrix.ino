// ---  NEO PIXELS --- // 


// ---  STATE MACHINE --- // 
enum STATES {
  LOADING,
  ATTRACT,
  SCROLLING_TEXT,
  OFF,
  NUM_STATES
}

STATES state = STATES.LOADING;



void setup() {
  // put your setup code here, to run once:
}

void loop() {

  // ---  STATE MACHINE --- // 
  switch (state) {
    case STATES.LOADING:
      {
        break;
      }
    case STATES.ATTRACT:
      {
        break;
      }
    case STATES.SCROLLING_TEXT:
      {
        break;
      }
    case STATES.OFF:
      {
        break;
      }
    default:
      {
        break;
      }
  }
}
//blah blah blah