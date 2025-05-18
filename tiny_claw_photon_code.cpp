/* 
 * Project Tiny Claw
 * Author: Varan Gupta and Conor Kirby
 * Date: 18-05-2025
 */

#include "Particle.h"
#include "tiny_claw.hpp"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

SerialLogHandler logHandler(LOG_LEVEL_INFO);

TinyClaw tiny_claw;

/**
 * @brief Adds lives to the TinyClaw game via Particle function.
 * @param command String containing the number of lives to add.
 * @return 1 on success, -1 if the command is not a valid integer.
 */
int add_lives(String command)
{
  int lives = command.toInt(); // Convert the string to an integer

  if (lives == 0 && command != "0")
  {
    // Handle the error: command was not a valid integer string
    return -1;
  }

  tiny_claw.add_lives(lives);
  Log.info("%d lives added. Total lives left: %d", lives,TinyClaw::lives);

  return 1;
}

/**
 * @brief Lowers the gripper for the specified duration via Particle function.
 * @param command String containing the duration in milliseconds.
 * @return 1 on success.
 */
int lowerGripper(String command)
{
  tiny_claw.lowerGripper(command.toInt());
  return 1;
}

/**
 * @brief Raises the gripper for the specified duration via Particle function.
 * @param command String containing the duration in milliseconds.
 * @return 1 on success.
 */
int raiseGripper(String command)
{
  tiny_claw.raiseGripper(command.toInt());
  return 1;
}

/**
 * @brief Opens the gripper for the specified duration via Particle function.
 * @param cmd String containing the duration in milliseconds.
 * @return 1 on success.
 */
int openGripper(String cmd)
{
  tiny_claw.openGripper(cmd.toInt());
  return 1;
}

/**
 * @brief Closes the gripper for the specified duration via Particle function.
 * @param cmd String containing the duration in milliseconds.
 * @return 1 on success.
 */
int closeGripper(String cmd)
{
  tiny_claw.closeGripper(cmd.toInt());
  return 1;
}

/**
 * @brief Sets up the TinyClaw hardware and registers Particle functions and variables.
 */
void setup() {
  tiny_claw.setup();
  Particle.function("addLives", add_lives);
  Particle.function("raiseGripper", raiseGripper);
  Particle.function("lowerGripper", lowerGripper);
  Particle.function("openGripper", openGripper);
  Particle.function("closeGripper", closeGripper);
  Particle.variable("lives", TinyClaw::lives);

  Log.info("Photon setup completed..");
}

void loop() {
  tiny_claw.loop();
}
