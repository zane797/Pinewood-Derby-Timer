//Include libraries
#include <Wire.h>
#include <VL6180X.h>

//Create VL6180X object new
VL6180X sensor;

//Define code constants
#define pinStart 3
#define pinFinish 1
#define pinSDA 2
#define pinSCL 0
#define deadband 10
#define finishCheckSP 2

//Initialize variables
long unsigned int timestart = 0;
int racing = 0;
int finishSP = 0;
int sensor_val = 0;
int finishCheck = 0;  //Range must be below finishSP for finishCheckSP to consider finished

//initializing sensor function
void initialize_rangesensors() {

  //Initialize Sensor with starting values
  sensor.init();
  sensor.configureDefault();

  // Reduce range max convergence time and ALS integration
  sensor.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 5);
  sensor.writeReg(VL6180X::READOUT__AVERAGING_SAMPLE_PERIOD, 10);  //Use a rolling average of the last 10 counts
  sensor.writeReg(VL6180X::SYSRANGE__VHV_REPEAT_RATE, 0);          //Disable the automatic Very High Voltage sensor calibration
  sensor.writeReg(VL6180X::SYSRANGE__VHV_RECALIBRATE, 1);          //Manually run VHV calibration
  sensor.setTimeout(20);
  sensor.setScaling(1);

  //Set sensor ranging mode and start continuous reading
  sensor.stopContinuous();
  delay(10);
  sensor.startRangeContinuous(10);
}

void setup() {
  //Configure DI/DO pins
  pinMode(pinStart, INPUT);
  pinMode(pinFinish, OUTPUT);
  digitalWrite(pinFinish, LOW);  //Set finish pin to low to clear the finish flag
  //I2C Configuration
  Wire.begin(pinSDA, pinSCL);
  delay(50);

  //Sensor Configuration function
  initialize_rangesensors();
}

void loop() {
  //If not racing, and start is triggered, take inital reading a calculate finishSP
  if (racing == 0 && digitalRead(pinStart) == 1) {
    digitalWrite(pinFinish, LOW);               //Clear the finish flag
    sensor_val = sensor.readRangeContinuous();  //Get a baseline reading to act as the "normal" distance reading
    finishSP = sensor_val - deadband;           //Calculate finish SP with baseline reading and deadband
    timestart = millis();
    finishCheck = 0;
    delay(700);  //wait 700 ms for the main board to send and clear start signals. [Optimization] tighten this timing along with the main board delay between start signal activation and clearing
    racing = 1;
  }
  if (sensor.timeoutOccurred() == 1) {
    initialize_rangesensors();
    //[Optimization] consider further if the SP should be recalculated when sensor is reinitialized
    //finishSP=sensor_val-deadband;
  }

  while (racing == 1) {
    //Get sensor reading to test for finish state
    sensor_val = sensor.readRangeContinuous();

    //If sensor times out, reinitialize
    if (sensor.timeoutOccurred() == 1) {
      initialize_rangesensors();
      //finishSP=sensor_val-deadband;
    }

    //If a new start command is sent more than 20 seconds after the last one was sent, reset back to the start of a new race sequence
    if (digitalRead(pinStart) == 1 && millis() - timestart > 20000) {
      racing == 0;
      break;
    }

    delay(1);
    //If the sensor values is below setpoint increment the finishCheck counter
    if (sensor_val < finishSP) {
      finishCheck += 1;
      //If the sensor has read below SP for two consecutive readings, signal the finish bit and clear the ints
      if (finishCheck >= 2) {
        digitalWrite(pinFinish, HIGH);
        racing = 0;
        finishSP = 255;
      }
    }
    //if the sensor value is not below the SP, reset finishCheck to 0
    else {
      finishCheck = 0;
    }
  }
}
