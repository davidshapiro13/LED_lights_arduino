/* LED CODE - David Shapiro - Jan 2024 */
#include <Adafruit_NeoPixel.h>

const int LED_PIN       = 6;
const int BUTTON_PIN    = 11;
const int NUMPIXELS = 150;
const int HSV_WRAP  = 65536;
const int SMALLER_HSV = 13100; //multiple by 5

int buttonState = 0;

Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

uint32_t colorPalette[] = {
  leds.Color(255, 255, 255), //white
  leds.Color(255, 0, 0), //red
  leds.Color(0, 255, 0), //lime
  leds.Color(255, 100, 0), //amber
  leds.Color(0, 0, 255), //blue
  leds.Color(50, 255, 255), //aqua
  leds.Color(255, 255, 0), //yellow
  leds.Color(0, 255, 255), //cyan
  leds.Color(255, 222, 30), //gold
  leds.Color(0, 255, 40), //jade
  leds.Color(255, 0, 20), //pink
  leds.Color(253, 245, 230), //old lace
  leds.Color(192, 192, 192), //silver
  leds.Color(128, 0, 0), //maroon
  leds.Color(255, 0, 20), //sparkly pink
  leds.Color(0, 128, 0), //green, 15
  leds.Color(180, 0, 255), //purple
  leds.Color(0, 128, 128), //teal
  leds.Color(0, 0, 128), //navy
  leds.Color(255, 40, 0), //orange
};

const int LENGTH_COLOR_PALETTE = sizeof(colorPalette) / sizeof(colorPalette[0]);

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  leds.begin();
  leds.setBrightness(random(50, 80));
  Serial.begin(9600);
  randomSeed(analogRead(A0));

}

void loop() {
  routineChoser();
  delay(100);
}

void routineChoser() {
  int react = 0;
  int num_options = 12;
  int choiceNum = random(num_options);

  uint32_t color1 = colorPalette[random(LENGTH_COLOR_PALETTE)];
  uint32_t color2 = colorPalette[random(LENGTH_COLOR_PALETTE)];
  uint32_t color3 = colorPalette[random(LENGTH_COLOR_PALETTE)];
  
  int wait = random(30, 150);
  int wait2 = random(30, 150);
  int wait3 = random(30, 150);
  int shortWait = random(30, 60);
  int increase = random(10, 100);
  int startColorNum = random(SMALLER_HSV) * 5;
  int sectionSize = random(10, 50);
  int dot_num = random(2, 10);
  int repeat_num = random(3, 6);

  
  switch(choiceNum) {
    case 0:
      continuousRainbow(wait, increase);
      break;
    case 1:
      sectionedRainbow(sectionSize);
      break;
    case 2:
      colorWaves(color1, color2, wait);
      break;
    case 3:
      alternatingColors(color1, color2);
      break;
    case 4:
      rainbowCycle(wait);
      break;
    case 5:
      colorSweep(wait, wait2, wait3, color1, color2);
      break;
    case 6:
      everyOther(color1, color2, wait);
      break;
    case 7:
      rainbowStrip(startColorNum, wait, increase);
      break;
    case 8:
      rainbowFullStrip(startColorNum, wait, increase);
      break;
    case 9:
      colorFill(color1, color2, wait);
      break;
    case 10:
      jenniferSpecial(repeat_num, wait);
      break;
    case 11:
      back_and_forth(color1, color2, shortWait, dot_num, repeat_num);
      break;
    case 12:
      threeAlternate(color1, color2, color3, wait);
      break;
    case 13:
      randomLights(wait);
      break;
    case 14:
      center_of_edge(color1, wait);
      break;
    default:
      randomLights(wait);
      break;
  }
}

int center_of_edge(uint32_t color, int wait) {
  leds.setPixelColor(0, color);
  leds.setPixelColor(NUMPIXELS-1, color);
  leds.show();
  center(0, NUMPIXELS-1, color, wait);
}

void center(int edge1, int edge2, uint32_t color, int wait) {
  //Serial.println(edge1);
  //Serial.println(edge2);
  //Serial.println("-----");
  int closeness = edge1 - edge2;
  if (abs(closeness) >= 2) {
    int newLoc = (edge1 + edge2) / 2;
    leds.setPixelColor(newLoc, color);
    leds.show();
    delay(wait);
    center(edge1, newLoc, color, wait);
    center(newLoc, edge2, color, wait);
  }
}

//A set of lights goes back and forth
int back_and_forth(uint32_t color1, uint32_t color2, int wait, int num_dots, int repeats) {

  for (int repeat_num=0; repeat_num < repeats; repeat_num++) {
    for (int i = 0; i < NUMPIXELS - num_dots; i+=1) {
      leds.fill(color1);
      for (int j = 0; j < num_dots; j++) {
        if (digitalRead(BUTTON_PIN) == HIGH) {
          return 1;
        }
        leds.setPixelColor(i+j, color2);
      }
      leds.show();
      delay(wait);
    }
    for (int i = NUMPIXELS - num_dots; i >= 0; i--) {
      leds.fill(color1);
      for (int j = num_dots; j >= 0; j--) {
        if (digitalRead(BUTTON_PIN) == HIGH) {
          return 1;
        }
        leds.setPixelColor(i+j, color2);
      }
      leds.show();
      delay(wait);
    }
  }
  return 0; 

}

int jenniferSpecial(int num_repeats, int wait) {
  leds.clear();
  delay(50);

  for (int i = 0; i < num_repeats; i++) {
    fromTheMiddle(colorPalette[random(LENGTH_COLOR_PALETTE)], wait);
  }
}

int fromTheMiddle(uint32_t color, int wait) {

  int side1 = NUMPIXELS / 2;
  int side2 = side1 + 1;
  for (int i = 0; i < NUMPIXELS / 2; i++) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
       return 1;
     }
    leds.setPixelColor(side1, color);
    leds.setPixelColor(side2, color);
    leds.show();
    delay(wait);
    side1--;
    side2++;
  }
}

int continuousRainbow(int wait, int increment) {
   int num_runs = HSV_WRAP / increment;
   for (int i = 0; i < num_runs; i++) {
     if (digitalRead(BUTTON_PIN) == HIGH) {
       return 1;
     }
    rainbowStrip(i * NUMPIXELS * increment, wait, increment);
    delay(1000);
    return 0;
  }
}

// Baseline routines

//One color goes from both sides, hits and goes the other way
int colorWaves(uint32_t color1, uint32_t color2, int wait) {
  int end = NUMPIXELS;
  int start = 0;
  for (int i = 0; i < NUMPIXELS / 2; i++) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
       return 1;
     }
    leds.setPixelColor(start, color1);
    leds.setPixelColor(end, color1);
    leds.show();
    delay(wait);
    end = end - 1;
    start = start + 1;
  }
  for (int i = 0; i < NUMPIXELS / 2; i++) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
       return 1;
     }
    leds.setPixelColor(start, color2);
    leds.setPixelColor(end, color2);
    leds.show();
    delay(wait);
    end = end + 1;
    start = start - 1;
  }
  delay(1000);
  return 0;
}

int sectionedRainbow(int sectionSize) {
    int numSwatches = NUMPIXELS / sectionSize;
    if (NUMPIXELS % sectionSize != 0) {
      numSwatches++;
    }
    long colorSwatches[numSwatches];
    int index = 0;
    for (int i = 0; i < NUMPIXELS; i+=sectionSize) {
      long rand = random(SMALLER_HSV) * 5;
      colorSwatches[index] = rand;
      for (int j = i; j < i + sectionSize; j++) {
        if (digitalRead(BUTTON_PIN) == HIGH) {
          return 1;
        }
        leds.setPixelColor(j, leds.ColorHSV(colorSwatches[index]));
      }
      index++;
    }
    leds.show();
    delay(1000);
    for (int repeat = 0; repeat < 1000; repeat++) {
      index = 0;
      for (int i = 0; i < NUMPIXELS; i+= sectionSize) {
        colorSwatches[index] = colorSwatches[index] + 100;
        for (int k = i; k < i + sectionSize; k++) {
          if (digitalRead(BUTTON_PIN) == HIGH) {
            return 1;
          }
          leds.setPixelColor(k, leds.ColorHSV(colorSwatches[index]));
        }
        index++;
      }
      leds.show();
      delay(100);
    }
    delay(1000);
    return 0;
}

int alternatingColors(uint32_t color1, uint32_t color2) {
  for (int i=0; i<50; i++) {
    alternate(color1, color2);
  }
}

int alternate(uint32_t color1, uint32_t color2) {
  for (int i =0; i < NUMPIXELS; i+=2) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
       return 1;
     }
    leds.setPixelColor(i, color1);
  }
  for (int i = 1; i < NUMPIXELS; i+=2) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
       return 1;
     }
    leds.setPixelColor(i, color2);
  }
  leds.show();
  delay(200);
  for (int i =0; i < NUMPIXELS; i+=2) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
      return 1;
    }
    leds.setPixelColor(i, color2);
  }
  for (int i = 1; i < NUMPIXELS; i+=2) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
       return 1;
    }
    leds.setPixelColor(i, color1);
  }
  leds.show();
  delay(200);
  return 0;
}

int randomLights(int sectionSize) {
  for (int i = 0; i < NUMPIXELS; i+=sectionSize) {
    long rand = random(SMALLER_HSV);
    for (int j = i; j < i + sectionSize; j++) {
      if (digitalRead(BUTTON_PIN) == HIGH) {
       return 1;
      }
      leds.setPixelColor(j, leds.ColorHSV(rand * 5));
    }
  }
  leds.show();
  delay(0.1);
  for (int j = 0; j < 100; j++) {
    for (int i = 0; i < NUMPIXELS; i+=sectionSize) {
      for (int j = i; j < i + sectionSize; j++) {
        if (digitalRead(BUTTON_PIN) == HIGH) {
          return 1;
        }
        leds.setPixelColor(j, leds.getPixelColor(i) + random(-2, 2));
      }
    }
    delay(0.001);
  }
  leds.show();
  delay(1000);
  return 0;
}

// Rainbow cycle function
//Chat GPT written
int rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < leds.numPixels(); i++) {
      if (digitalRead(BUTTON_PIN) == HIGH) {
       return 1;
      }
      leds.setPixelColor(i, Wheel((i + j) & 255));
    }
    leds.show();
    delay(wait);
  }
  delay(1000);
  return 0;
}

// Generate rainbow colors across 0-255 positions
// ChatGPT Written
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return leds.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return leds.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return leds.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

//one color fills, then another
int colorSweep(uint8_t wait, uint8_t wait2, uint8_t wait3, uint32_t color, uint32_t color2) {
  leds.show();
  for (int i =0; i < leds.numPixels(); i++) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
       return 1;
    }
    leds.setPixelColor(i, color);
    leds.show();
    delay(wait);
  }
  delay(wait3);
  for (int i = leds.numPixels(); i >= 0; i--) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
       return 1;
    }
    leds.setPixelColor(i, color2);
    leds.show();
    delay(wait2);
  }
  delay(wait3);
  return 0;
}

int flash(uint32_t color, uint32_t color2, int wait, int times) {

  for (int i =0; i < times; i++) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
       return 1;
    }
    leds.fill(color);
    leds.show();
    delay(wait);
    leds.fill(color2);
    leds.show();
    delay(wait);
  }
  delay(1000);
  return 0;
}

//Fills strip one by one
int colorFill(uint32_t startColor, uint32_t color, int wait) {
  int lastSpot = leds.numPixels() - 1;
  leds.fill(startColor);
  leds.show();
  for (int j=0; j < leds.numPixels(); j++) {
    for (int i=0; i < lastSpot-1; i++) {
      if (digitalRead(BUTTON_PIN) == HIGH) {
       return 1;
      }
      leds.setPixelColor(i, color);
      leds.show();
      delay(wait);
      leds.setPixelColor(i, startColor);
      leds.show();
    }
    leds.setPixelColor(lastSpot, color);
    delay(wait);
    leds.show();
    lastSpot = lastSpot - 1;
  }
  delay(1000);
  return 0;
}

//Colors alternate
int everyOther(uint32_t color1, uint32_t color2, int wait) {
  leds.clear();
  for (int i = 0; i < NUMPIXELS; i+=2) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
       return 1;
    }
    leds.setPixelColor(i, color1);
    leds.show();
    delay(wait);
  }
  for (int i = NUMPIXELS + 1; i > 0; i-=2) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
       return 1;
    }
    leds.setPixelColor(i, color2);
    leds.show();
    delay(wait);
  }
  delay(1000);
  return 0;
}

//Colors alternate
int threeAlternate(uint32_t color1, uint32_t color2, uint32_t color3, int wait) {
  leds.clear();
  for (int i = 0; i < NUMPIXELS; i+=3) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
       return 1;
    }
    leds.setPixelColor(i, color1);
    leds.show();
    delay(wait);
  }
  for (int i = NUMPIXELS + 1; i > 0; i-=3) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
       return 1;
    }
    leds.setPixelColor(i, color2);
    leds.show();
    delay(wait);
  }
  for (int i = 2; i < NUMPIXELS; i+=3) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
       return 1;
    }
    leds.setPixelColor(i, color3);
    leds.show();
    delay(wait);
  }
  delay(1000);
  return 0;
}

//Shows color gradient of rainbow across strip
int rainbowStrip(int startColorNum, int wait, int increase) {
  int colorNum = startColorNum;
  uint32_t color = leds.ColorHSV(colorNum);
  for (int i = 0; i < NUMPIXELS; i++) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
       return 1;
    }
    leds.setPixelColor(i, color);
    leds.show();
    delay(wait);
    colorNum += increase;
    color = leds.ColorHSV(colorNum);
  }
  delay(1000);
  return 0;
}

int rainbowFullStrip(int startColorNum, int wait, int increase) {
    int currColorNum = startColorNum;
    uint32_t color = leds.ColorHSV(currColorNum);
    for (int i = 0; i < HSV_WRAP/increase; i++) {
      if (digitalRead(BUTTON_PIN) == HIGH) {
       return 1;
      }
      color = leds.ColorHSV(currColorNum);
      leds.fill(color);
      leds.show();
      delay(wait);
      currColorNum += increase;
    }
    delay(1000);
    return 0;
  }
