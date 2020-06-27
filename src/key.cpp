#ifndef KEY_HPP
#define KEY_HPP
#include "key.hpp"
#include <iostream>
key::key(keyType myKeyType, double deadTime)
    : type(myKeyType), inputDeadTime(deadTime) {}

bool key::keyPressed(std::chrono::steady_clock::time_point time) {
  if (this->type == continuous) {
    if (this->pressed) {
      // if is currently pressed
      std::chrono::duration<double> diff = time - inputTime;
      if (diff.count() > this->inputDeadTime / ((this->continuousRepeatCount>>2)+1)) {
        if(this->continuousRepeatCount <= 3){
          this->continuousRepeatCount++;
        }
        return true;
      } else {
        return false;
      }
    } else {
      this->pressed = true;
      this->continuousRepeatCount = 0;
      this->inputTime = time;
      return true;
    }
  } else {
    if (this->pressed == false) {
      this->pressed = true;
      return true;
    } else {
      //this->pressed = false;
      return false;

    }
  }
}

void key::keyUnpressed()
{
  this->pressed = false;
  this->continuousRepeatCount = 0; 
}
#endif KEY_HPP
