//Include libraries
#include <Adafruit_Protomatter.h>
#include <Wire.h>

//System control variables
#define debug 0               //Full debug. USB Serial enabled. May delay code sufficiently to impact finish timing.
#define debug_no_serial 0     //Put into debug mode to skip the start line but do not send USB serial debug in race run loop
#define button 22             //GPIO Pin for the Chassis Button
#define config_start_input 1  //1=Starting line sensor present, Set to see if the starting line has a sensor. If not, a 3 second delay will be started after hitting the button before transitioning to racing mode
#define sim_race 0            //1=Run the race and DNF all lanes after 10 seconds
#define start_line 26         //GPIO Pin for the starting line IO.
#define BRIGHTNESS 75         //Range 0-255
uint8_t mode = 0;   //0=Ready to Race, 1= Start Race, 2=Running Race,3=Displaying Times
long current_time;  //initialize timing variable

//Sensor IO pins
int startPins[] = { 0, 5, 16, 18, 20 }; 
int finishPins[] = { 0, 4, 6, 17, 19 };

//Assign LED Matrix Variables
uint8_t rgbPins[] = { 8, 7, 9, 11, 10, 12 }; //R1,G1,B1,R2,G2,B2
uint8_t addrPins[] = { 14, 15, 2, 3 }; //A, B, C, D
uint8_t clockPin = 13;
uint8_t latchPin = 1;
uint8_t oePin = 0;

Adafruit_Protomatter matrix(
  64,                         // Width of matrix (or matrix chain) in pixels
  4,                          // Bit depth, 1-6
  1, rgbPins,                 // # of matrix chains, array of 6 RGB pins for each
  4, addrPins,                // # of address pins (height is inferred), array of pins
  clockPin, latchPin, oePin,  // Other matrix control pins
  false);                     // No double-buffering here (see "doublebuffer" example)

//Establish timer variables
uint timerraw = 0;
String timerstr = "";
String decimals[5][5];
uint race_times[5];
int finish[5] = { 0, 0, 0, 0, 0 };
uint start_time;
char times[] = "";


//Function to display the times for a given lane [Optimization] if the time is less than 1 second, append a leading 0 to it to display correctly
void displaytimes(int ln, int int_time) {
  //Establish Function Variables
  int x_offset = (ln - 1) * 16 + 2;
  String lane_name = "L" + String(ln);
  //Run if the time is valid
  if (int_time < 10000) {
    //Convert time to string
    itoa(int_time, times, 10);
    /*Proposed optimization code
    if (int_time<1000){
      times="0"+times;
    }
    */

    //Clear the matrix
    matrix.fillRect(x_offset, 10, 16, 22, matrix.color565(0, 0, 0));
    //Print the lane details
    matrix.setTextColor(matrix.color565(0, 0, BRIGHTNESS));
    matrix.setCursor(x_offset, 10);
    matrix.println(lane_name);
    matrix.setTextColor(matrix.color565(0, BRIGHTNESS, 0));
    matrix.setCursor(x_offset, 18);
    matrix.println(times[0]);
    matrix.drawPixel(x_offset + 5, 24, matrix.color565(BRIGHTNESS, 0, 0));
    matrix.setCursor(x_offset + 6, 18);
    matrix.println(times[1]);
    matrix.setCursor(x_offset, 25);
    matrix.println(String(times[2]) + String(times[3]));
    matrix.show();
  } else {
    //Clear the matrix
    matrix.fillRect(x_offset, 10, 16, 22, matrix.color565(0, 0, 0));
    //Print the lane details
    matrix.setTextColor(matrix.color565(0, 0, 100));
    matrix.setCursor(x_offset, 10);
    matrix.println(lane_name);
    matrix.setTextColor(matrix.color565(0, 100, 0));
    matrix.setCursor(x_offset, 18);
    matrix.println("DN");
    matrix.setCursor(x_offset + 3, 25);
    matrix.println("F");
    matrix.show();
  }
}

//Function to reset the screen to a prerace state
void reset_screen() {
  //Blank the entire screen
  matrix.fillScreen(matrix.color565(0, 0, 0));
  //Show Startup Screen
  for (int x = 0; x < matrix.width(); x++) {
    int r = 0;
    int b = 0;
    int g = 0;
    if (x < 16) {
      r = BRIGHTNESS;
    } else if (x >= 16 && x < 32) {
      g = BRIGHTNESS;
    } else if (x >= 32 && x < 48) {
      b = BRIGHTNESS;
    } else {
      r = BRIGHTNESS;
      g = BRIGHTNESS;
      b = BRIGHTNESS;
    }
    matrix.drawPixel(x, 0, matrix.color565(r, g, b));
    matrix.drawPixel(x, 8, matrix.color565(r, g, b));
  }
  matrix.setCursor(1, 1);
  matrix.setTextSize(1);
  matrix.setTextColor(matrix.color565(BRIGHTNESS, BRIGHTNESS, BRIGHTNESS));
  matrix.println("RACE");
  matrix.setCursor(34, 1);
  matrix.println("READY");
  matrix.show();  // Copy data to matrix buffers

  //Set mode to 0, Ready to Race
  mode = 0;
}

//Function to initialize the race and store necessary variables
void racing_start() {
  //Blank the entire screen
  matrix.fillScreen(matrix.color565(0, 0, 0));
  //Show Startup Screen
  for (int x = 0; x < matrix.width(); x++) {
    int r = 0;
    int b = 0;
    int g = 0;
    if (x < 16) {
      r = BRIGHTNESS;
    } else if (x >= 16 && x < 32) {
      g = BRIGHTNESS;
    } else if (x >= 32 && x < 48) {
      b = BRIGHTNESS;
    } else {
      r = BRIGHTNESS;
      g = BRIGHTNESS;
      b = BRIGHTNESS;
    }
    matrix.drawPixel(x, 0, matrix.color565(r, g, b));
    matrix.drawPixel(x, 8, matrix.color565(r, g, b));
  }
  matrix.setCursor(1, 1);
  matrix.setTextSize(1);
  matrix.setTextColor(matrix.color565(BRIGHTNESS, BRIGHTNESS, BRIGHTNESS));
  matrix.println("READY SET!");
  matrix.show();  // Copy data to matrix buffers

  mode = 1;
}

//Function to run during the actual race
void running_race() {
  //Record the start time
  start_time = millis();
  
  //Zero Out the finish variable
  for (int x = 1; x < 5; x++) {
    finish[x] = 0;
  }
  //Send the race start signal
  for (int x = 1; x < 5; x++) {
    digitalWrite(startPins[x], HIGH);
  }
  delay(500); //[Optimization] Lower this delay with trial testing on the esp8266 main boards, I'd like to see this at <100 ms

  //Clear the race start signal
  for (int x = 1; x < 5; x++) {
    digitalWrite(startPins[x], LOW);
  }

  if (debug == 1) { Serial.println("Start time: " + String(start_time)); }

  //Keep running the loop until all lanes have finished
  while (finish[1] != 1 || finish[2] != 1 || finish[3] != 1 || finish[4] != 1) {

    //Increment through the sensors and evaluate states
    for (int x = 1; x < 5; x++) {
      //if the lane has not finished previously, evaluate the IO to see if it has now
      if (finish[x] != 1) {
        if (digitalRead(finishPins[x]) == 1) {
          race_times[x] = millis() - start_time;
          finish[x] = 1;
        }
      }
    }
    //If the race has run for more than 10 seconds, end the loop
    if ((millis() - start_time) > 10000) {
      break;
    }
  }
  //Check all lanes, if one did not finish assign it a time of 10000 (DNF)
  for (int x = 1; x < 5; x++) {
    if (finish[x] != 1) {
      finish[x] = 1;
      race_times[x] = 10000;
    }
  }
  //Run the display times function for all lanes
  for (int x = 1; x < 5; x++) {
    displaytimes(x, race_times[x]);
  }
  //Change the display to say "Finish"
  matrix.fillRect(0, 1, 64, 7, matrix.color565(0, 0, 0));
  matrix.setTextColor(matrix.color565(0, 0, BRIGHTNESS));
  matrix.setCursor(14, 1);
  matrix.println("FINISH!");
  matrix.show();

  //Set mode to 3, displaying times
  mode = 3;

  for (int x = 1; x < 5; x++) { digitalWrite(startPins[x], LOW); }
}

void setup() {
  // put your setup code here, to run once:
  if (debug == 1) {
    Serial.begin(9600);
  }

  // Initialize matrix...
  ProtomatterStatus status = matrix.begin();
  if (debug == 1) {
    Serial.print("Protomatter begin() status: ");
    Serial.println((int)status);
  }
  if (status != PROTOMATTER_OK) {
    // DO NOT CONTINUE if matrix setup encountered an error.
    for (;;)
      ;
  }

  //Show Startup Screen
  reset_screen();

  //Set Pin Modes
  pinMode(button, INPUT_PULLUP);
  pinMode(start_line, INPUT_PULLUP);
  for (int x = 1; x < 5; x++) {
    pinMode(startPins[x], OUTPUT);
    digitalWrite(startPins[x], LOW);
    pinMode(finishPins[x], INPUT);
  }

  delay(100);
  //Set mode to 0, ready to race
  mode = 0;
}

void loop() {
  //If in mode 0, prepared to start, pushing the button will prep the system to run looking for debug or start signal
  if (digitalRead(button) == 0 && mode == 0) {
    racing_start();
  } 
  else if ((digitalRead(start_line) == 0 || config_start_input == 0) && mode == 1 && sim_race == 0) {
    if (config_start_input == 0) { delay(3000); }
    matrix.fillRect(0, 1, 64, 7, matrix.color565(0, 0, 0));
    matrix.setTextColor(matrix.color565(0, BRIGHTNESS, 0));
    matrix.setCursor(26, 1);
    matrix.println("GO!");
    matrix.show();
    mode = 2;
    running_race();
  }
  //Debug mode, skip start line input
  else if ((debug == 1 || debug_no_serial == 1) && mode == 1 && sim_race == 1) {
    Serial.println("Entering Sim Racing Mode, skipping start_line input");
    delay(500);
    matrix.fillRect(0, 1, 64, 7, matrix.color565(0, 0, 0));
    matrix.setTextColor(matrix.color565(0, BRIGHTNESS, 0));
    matrix.setCursor(26, 1);
    matrix.println("GO!");
    matrix.show();
    mode = 2;
    Serial.println("Mode: " + String(mode));
    running_race();
    delay(50);
  } 
  else if (digitalRead(button) == 0 && mode == 3) {
    matrix.fillRect(0, 1, 64, 7, matrix.color565(0, 0, 0));
    matrix.fillRect(0, 9, 64, 23, matrix.color565(0, 0, 0));
    matrix.setTextColor(matrix.color565(BRIGHTNESS, BRIGHTNESS, BRIGHTNESS));
    matrix.setCursor(20, 1);
    matrix.println("RESET");
    matrix.show();
    mode = 4;
    delay(500);
  } else if (digitalRead(start_line) == 1 && mode == 4) {
    reset_screen();
  }
}
