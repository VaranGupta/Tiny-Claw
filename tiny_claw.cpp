#include "Particle.h"
#include "tiny_claw.hpp"

int TinyClaw::lives = 5;

void TinyClaw::setup()
{
  pinMode(joy_x_pin_, INPUT);
  pinMode(joy_y_pin_, INPUT);
  pinMode(trigger_button_, INPUT_PULLUP);

  pinMode(motor_x_pin_1_, OUTPUT);
  pinMode(motor_y_pin_1_, OUTPUT);
  pinMode(motor_z_pin_1_, OUTPUT);

  pinMode(motor_x_pin_2_, OUTPUT);
  pinMode(motor_y_pin_2_, OUTPUT);
  pinMode(motor_z_pin_2_, OUTPUT);

  pinMode(gripper_motor_pin_1_, OUTPUT);
  pinMode(gripper_motor_pin_2_, OUTPUT);
  Log.info("TinyClaw Setup completed..");
}

void TinyClaw::loop()
{
  if (lives > 0)
  {
    int dx = -(analogRead(joy_x_pin_) - 2048); // read the analogPin
    int dy = (analogRead(joy_y_pin_) - 2048);  // read the analogPin

    // Map the joystick values to motor commands so that the platform moves
    // in the direction of the joystick on X-Y mechanism
    int motor_1_cmd = (dx + dy) / 8;
    int motor_2_cmd = (dx - dy) / 8;

    // NOTE: analogRead values go from 0 to 4095, analogWrite values from 0 to 255
    // Set direction of the motor based on joystick direction
    if (motor_1_cmd > JOY_THRESHOLD_)
    {
      digitalWrite(motor_x_pin_1_, LOW);
      analogWrite(motor_x_pin_2_, abs(motor_1_cmd));
    }
    else if (motor_1_cmd < -JOY_THRESHOLD_)
    {
      digitalWrite(motor_x_pin_2_, LOW);
      analogWrite(motor_x_pin_1_, abs(motor_1_cmd));
    }
    else
    {
      digitalWrite(motor_x_pin_1_, LOW);
      digitalWrite(motor_x_pin_2_, LOW);
    }

    // Set direction of the motor based on joystick direction
    if (motor_2_cmd > JOY_THRESHOLD_)
    {
      digitalWrite(motor_y_pin_1_, LOW);
      analogWrite(motor_y_pin_2_, abs(motor_2_cmd));
    }
    else if (motor_2_cmd < -JOY_THRESHOLD_)
    {
      digitalWrite(motor_y_pin_2_, LOW);
      analogWrite(motor_y_pin_1_, abs(motor_2_cmd));
    }
    else
    {
      digitalWrite(motor_y_pin_1_, LOW);
      digitalWrite(motor_y_pin_2_, LOW);
    }

    if (!digitalRead(trigger_button_))
    {
      Log.info("Button pressed!!");

      if (!sweets_captured_)
      {
        // To capture sweets, we need to lower the gripper, close it and then raise it
        Log.info("Capturing sweets.....");
        lowerGripper(GRIPPER_LOWERING_TIME_);
        delay(1000);
        closeGripper(GRIPPER_CLOSING_TIME_);
        delay(1000);
        raiseGripper(GRIPPER_RAISING_TIME_);
        delay(1000);

        sweets_captured_ = true;
      }
      else
      {
        Log.info("Releasing Sweets.....");
        openGripper(GRIPPER_OPENNING_TIME_);
        delay(2000);
        sweets_captured_ = false;
        lives--;
      }
    }
  }
  else
  {
    Log.warn("Not enough lives.....");
  }
  delay(100);
}

void TinyClaw::lowerGripper(unsigned int delay_ms)
{
  Log.info("Lowering gripper for %d ms", delay_ms);
  analogWrite(motor_z_pin_1_, 0);
  digitalWrite(motor_z_pin_2_, HIGH);

  delay(delay_ms);

  analogWrite(motor_z_pin_1_, 0);
  digitalWrite(motor_z_pin_2_, LOW);
}

void TinyClaw::raiseGripper(unsigned int delay_ms)
{
  Log.info("Raising gripper for %d ms", delay_ms);
  analogWrite(motor_z_pin_1_, Z_MOTOR_SPEED_);
  digitalWrite(motor_z_pin_2_, LOW);

  delay(delay_ms);

  analogWrite(motor_z_pin_1_, 0);
  digitalWrite(motor_z_pin_2_, LOW);
}

void TinyClaw::openGripper(unsigned int delay_ms)
{
  Log.info("Openning gripper for %d ms", delay_ms);
  digitalWrite(gripper_motor_pin_1_, LOW);
  digitalWrite(gripper_motor_pin_2_, HIGH);

  delay(delay_ms);

  digitalWrite(gripper_motor_pin_1_, LOW);
  digitalWrite(gripper_motor_pin_2_, LOW);
}

void TinyClaw::closeGripper(unsigned int delay_ms)
{
  Log.info("Closing gripper for %d ms", delay_ms);
  digitalWrite(gripper_motor_pin_1_, HIGH);
  digitalWrite(gripper_motor_pin_2_, LOW);

  delay(delay_ms);

  digitalWrite(gripper_motor_pin_1_, LOW);
  digitalWrite(gripper_motor_pin_2_, LOW);
}