/*
  Sensor.cpp - Library for generic sensor
  Created by Kjetil Asdal, January 07, 2016
*/

#include <Arduino.h>
#include <Sensor.h>

// -- PRIVATE FUNCTION PROTOTYPES ----------------------------------------------

static void defaultReadStateCB(String &state);
static int defaultCompareStateCB(String state, String previousState);
static void defaultPostStateCB(String state);

// -- PUBLIC MEMBER FUNCTIONS --------------------------------------------------

Sensor::Sensor() {
  this->readInterval = 0; // Read as often as possible
  this->postInterval = -1; // Never post updates

  this->state = "";
  this->previousState = "";

  this->lastRead = 0;
  this->lastPost = 0;

  this->readStateCB = defaultReadStateCB;
  this->compareStateCB = defaultCompareStateCB;
  this->postStateCB = defaultPostStateCB;
}

void Sensor::setReadInterval(unsigned long interval) {
  this->readInterval = interval;
}

void Sensor::setPostInterval(unsigned long interval) {
  this->postInterval = interval;
}

void Sensor::setReadStateCB(void (*callback)(String &state)) {
  this->readStateCB = callback;
}

void Sensor::setCompareStateCB(int (*callback)(String state, String previousState)) {
  this->compareStateCB = callback;
}

void Sensor::setPostStateCB(void (*callback)(String state)) {
  this->postStateCB = callback;
}

void Sensor::loop() {
  unsigned long now = millis();
  // Check if it is time to read the sensor
  if ((now - this->lastRead) >= this->readInterval) {
    this->readStateCB(this->state);
    this->lastRead = now;
#ifdef SENSOR_DEBUG
    Serial.print("R:");
    Serial.println(this->state);
#endif
    // Check if state has changed since last read
    if (this->compareStateCB(this->state, this->previousState) != 0) {
      this->previousState = this->state;
#ifdef SENSOR_DEBUG
      Serial.print("C:");
      Serial.println(this->state);
#endif
      // Do not post empty state
      if (this->state != "") {
        this->postStateCB(this->state);
      }
      this->lastPost = now;
    }
  }
  // Check if it is time to post an update
  if ((now - this->lastPost) >= this->postInterval) {
#ifdef SENSOR_DEBUG
    Serial.print("U:");
    Serial.println(this->state);
#endif
    // Do not post empty state
    if (this->state != "") {
      this->postStateCB(this->state);
    }
    this->lastPost = now;
  }
}

// -- PRIVATE MEMBER FUNCTIONS -------------------------------------------------

// -- PRIVATE FUNCTIONS --------------------------------------------------------

static void defaultReadStateCB(String &state) {
  state = ""; // Invalid read
}

static int defaultCompareStateCB(String state, String previousState) {
  return (state != previousState);
}

static void defaultPostStateCB(String state) {
}
