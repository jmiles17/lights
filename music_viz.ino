// music viz program
// Jason Miles

#include <FastLED.h>

// spectrum connections
int strobe = 4;
int reset = 5;
int audio0 = A0;
int audio1 = A1;

// audio arrays
// left and right hold the past 3 readings from left and right input 
//   for each of the 7 frequency bands
int left[3][7];
int right[3][7];

float averages[4][7];

int band_count[7];

// LED LIGHTING SETUP
#define LED_PIN     8
#define NUM_LEDS    300
#define BRIGHTNESS  32
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

int midway = 150;

void setup() {
  
  // set spectrum pin configurations
  pinMode(strobe, OUTPUT);
  pinMode(reset, OUTPUT);
  pinMode(audio0, INPUT);
  pinMode(audio1, INPUT);

  // init audio analyzers
  digitalWrite(strobe, HIGH);
  digitalWrite(reset, LOW);
  // delay(5);

  // LED LIGHTING SETUP
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  // CLEAR LEDS
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CRGB(0, 0, 0);

  delay(1000);

  FastLED.show();

  Serial.begin(19200);

}

int read_freqs() {

  digitalWrite(reset, HIGH);
  digitalWrite(reset, LOW);

  int average_total;
  
  // read freqs for each band 
  for (int band = 0; band < 7; band++) {
    digitalWrite(strobe, LOW);
    delayMicroseconds(50);

    average_total = 0;

    // move old freqs back and sum
    for (int t = 1; t < 3; t++) {
      left[t][band] = left[t-1][band];
      right[t][band] = right[t-1][band];

      averages[t][band] = (left[t][band] + right[t][band]) / 2;

      average_total += averages[t][band];

    }

    // get reading
    int left_read = analogRead(audio0);
    int right_read = analogRead(audio1);
    left[0][band] = left_read;
    right[0][band] = right_read;
    averages[0][band] = (left_read + right_read) / 2;

    // get averages
    average_total += averages[0][band];
    averages[3][band] = averages[0][band] / average_total / 5;

    // strobe
    digitalWrite(strobe, HIGH);
    //delayMicroseconds(50);

  }

  // find highest percent change
  int max_band = 0;
  for (int b = 0; b < 7; b++) {
    if (averages[3][b] > averages[3][max_band]) {
      max_band = b;
    }
  }

  return max_band;
  
}

// colors takes the largest changed band
// and changes the color accordingly
void colors(int max_band) {

  // move previous colors
  for (int l = midway; l >= 10; l--) {
    // 300-158
    leds[150 + l] = leds[150 + l - 10];
    // 142-0
    leds[150 - l] = leds[150 - l + 10];
  }

  // new color based on band
  if (max_band == 0) {
    for (int l = 0; l < 10; l++) {
      leds[150 + l] = CRGB(250, 250, 20);
      leds[150 - l] = CRGB(250, 250, 20);
    }
  } else if (max_band == 1) {
    for (int l = 0; l < 10; l++) {
      leds[150 + l] = CRGB(250, 250, 20);
      leds[150 - l] = CRGB(250, 250, 20);
    }
  } else if (max_band == 4) {
    for (int l = 0; l < 10; l++) {
      leds[150 + l] = CRGB(50, 250, 200);
      leds[150 - l] = CRGB(50, 250, 200);
    }
  } else if (max_band == 3) {
    for (int l = 0; l < 10; l++) {
      leds[150 + l] = CRGB(50, 250, 200);
      leds[150 - l] = CRGB(50, 250, 200);
    }
  } else if (max_band == 2) {
    for (int l = 0; l < 10; l++) {
      leds[150 + l] = CRGB(50, 250, 200);
      leds[150 - l] = CRGB(50, 250, 200);
    }
  } else if (max_band == 5) {
    for (int l = 0; l < 10; l++) {
      leds[150 + l] = CRGB(200, 50, 250);
      leds[150 - l] = CRGB(200, 50, 250);
    }
  } else if (max_band == 6) {
    for (int l = 0; l < 10; l++) {
      leds[150 + l] = CRGB(200, 50, 250);
      leds[150 - l] = CRGB(200, 50, 250);
    }
  }

  FastLED.show();
  
}

void loop() {
  // find largest changed frequency
  int max_band = read_freqs();
  // output colors to led lights
  colors(max_band);
  // graph
  // graph();
  // delay to avoid flickering
  delay(120);
}

// function to graph the readings on the serial monitor
// used for debugging purposes
void graph() {
  for (int i = 0; i < 7; i++)
  {
    Serial.print(averages[3][i]);
    Serial.print("    ");
  }
  Serial.println();
}