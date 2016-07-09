/*
  Sensor.h - Library for generic sensor
  Created by Kjetil Asdal, January 07, 2016
*/

#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

#define SENSOR_DEBUG

class Sensor {
  public:
    Sensor();
    void setReadInterval(unsigned long interval);
    void setPostInterval(unsigned long interval);

    void setReadStateCB(void (*callback)(String &state));
    void setCompareStateCB(int (*callback)(String state, String previousState));
    void setPostStateCB(void (*callback)(String state));

    void setup();
    void loop();
  private:
    unsigned long readInterval;
    unsigned long postInterval;

    String state;
    String previousState;

    unsigned long lastRead;
    unsigned long lastPost;

    void (*readStateCB)(String &state);
    int (*compareStateCB)(String state, String previousState);
    void (*postStateCB)(String state);
};

#endif
