#pragma once
#include "Particle.h"

/**
 * @class TinyClaw
 * @brief Controls the TinyClaw hardware, including joystick, motors, and gripper.
 */
class TinyClaw
{
public:
    /**
     * @brief Initializes hardware pins and prepares the TinyClaw for operation.
     */
    void setup();

    /**
     * @brief Main loop for TinyClaw operation. Handles input and controls hardware.
     */
    void loop();

    /**
     * @brief Adds lives to the TinyClaw game.
     * @param x Number of lives to add.
     */
    void add_lives(int x) { lives += x; }

    /**
     * @brief Lowers the gripper for a specified duration.
     * @param delay_ms Duration in milliseconds.
     */
    void lowerGripper(unsigned int delay_ms);

    /**
     * @brief Raises the gripper for a specified duration.
     * @param delay_ms Duration in milliseconds.
     */
    void raiseGripper(unsigned int delay_ms);

    /**
     * @brief Opens the gripper for a specified duration.
     * @param delay_ms Duration in milliseconds.
     */
    void openGripper(unsigned int delay_ms);

    /**
     * @brief Closes the gripper for a specified duration.
     * @param delay_ms Duration in milliseconds.
     */
    void closeGripper(unsigned int delay_ms);

    /**
     * @brief Number of lives remaining.
     */
    static int lives;

private:
    int joy_x_pin_ = A0;                ///< Joystick X-axis analog pin
    int joy_y_pin_ = A1;                ///< Joystick Y-axis analog pin

    int trigger_button_ = D17;          ///< Trigger button digital pin

    int motor_x_pin_1_ = D13;           ///< Motor X direction 1 pin
    int motor_y_pin_1_ = D15;           ///< Motor Y direction 1 pin
    int motor_z_pin_1_ = D1;            ///< Motor Z direction 1 pin

    int motor_x_pin_2_ = D14;           ///< Motor X direction 2 pin
    int motor_y_pin_2_ = D16;           ///< Motor Y direction 2 pin
    int motor_z_pin_2_ = D2;            ///< Motor Z direction 2 pin

    int gripper_motor_pin_1_ = D0;       ///< Gripper motor direction 1 pin
    int gripper_motor_pin_2_ = D3;       ///< Gripper motor direction 2 pin
    
    bool sweets_captured_ = false;      ///< State of gripper (true if holding sweets)

    const int JOY_THRESHOLD_ = 200;      ///< Joystick deadzone threshold

    const int Z_MOTOR_SPEED_ = 255;     ///< Z motor speed (0-255)
    const int GRIPPER_RAISING_TIME_ = 500 ; ///< Gripper raising time in ms
    const int GRIPPER_LOWERING_TIME_ = 300; ///< Gripper lowering time in ms
    
    const int GRIPPER_OPENNING_TIME_ = 100; ///< Gripper opening time in ms
    const int GRIPPER_CLOSING_TIME_ = 500;  ///< Gripper closing time in ms

};