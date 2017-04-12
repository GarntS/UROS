#pragma config(Motor,  port1,           left_tank,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           claw,          tmotorServoStandard, openLoop)
#pragma config(Motor,  port3,           lift,          tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port10,          right_tank,    tmotorVex393_HBridge, openLoop, reversed)

/*
 *		File:		uros.c
 *		Author:	Grant Spencer
 *		Date:		4/12/2017
 *		Desc:		Driver for the UROS robot.
 */

// Constant Definitions
#define CLAW_OPEN -127
#define CLAW_CLOSED 127
#define CLAW_STEPS 15 // Note that 255%15=0
#define SPEED_FULL 1
#define SPEED_CAREFUL 0.5
#define SPEED_OFF 0
#define TICK_DELAY_MILLIS 50 // 20Hz

/*
 *	get_movement_multiplier() - Used to scale movement speeds
 *	In:		None
 *	Out:	A float bounded by [.2, 1] depending on
 *				the number of shoulder buttons currently down
 */
float get_movement_multiplier()
{
	float nShoulderButtons = 1;
	if(vexRT[Btn5D] == 1)
		nShoulderButtons++;
	if(vexRT[Btn5U] == 1)
		nShoulderButtons++;
	if(vexRT[Btn6D] == 1)
		nShoulderButtons++;
	if(vexRT[Btn6U] == 1)
		nShoulderButtons++;
	return 1 / nShoulderButtons;
}

/*
 *	map_motor() - Used to map a scaled joystick input to a motor
 *	In:		The Motor object, A Joystick object, and a float to
 *				control the sensitivity of the mapping.
 *	Out:	None
 */
void map_motor(tMotor target, TVexJoysticks joy, float scale) {
	motor[target] = vexRT[joy] * scale;
}

/*
 *	configure_joysticks() - Configures the joysticks to control the motors
 *	In:		References to: the motor object containing the left motor being driven,
 *				and the float objects containing the speed scales for both motors.
 *	Out:	None
 */
void configure_joysticks(tMotor &lJoy, float &lScale, float &rScale) {
		// Figure out which motor we're controlling
		lJoy = vexRT[Btn8D] == 1 ? lift : left_tank;
		lScale = vexRT[Btn8D] == 1 ? SPEED_CAREFUL : SPEED_FULL;
		rScale = vexRT[Btn8D] == 1 ? SPEED_OFF : SPEED_FULL;
}

/*
 *	main() - The entry point
 *	In:		None
 *	Out:	None
 */
task main()
{
	// Motor control variables
	float multiplier = 1,
				prevMultiplier = 0,
				lScale = 1,
				rScale = 1,
				prevRScale = 0;

	// Position of the claw servo, default open
	int clawPos = CLAW_OPEN;

	// Which motor is currently mapped to which stick?
	tMotor	lJoy = left_tank,
					rJoy = right_tank;

	// Main loop
	while(1) {
		// Update cached multiplier value
		multiplier = get_movement_multiplier();

		// Configure the joysticks
		configure_joysticks(lJoy, lScale, rScale);

		// If the multiplier or selected motor changed, remap the joysticks
		if(multiplier != prevMultiplier || rScale != prevRScale) {

			// Update prevMultiplier/prevRScale with the new values
			prevMultiplier = multiplier;
			prevRScale = rScale;

			// Actually do the mapping
			map_motor(lJoy, Ch3, multiplier * lScale); // Left Joy
			map_motor(rJoy, Ch2, multiplier * rScale); // Right Joy
		}

		// Update claw position
		if(vexRT[Btn7D] == 1 && clawPos + (255.0 / CLAW_STEPS) < CLAW_CLOSED)
			clawPos += (255.0 / CLAW_STEPS);
		else if(vexRT[Btn7L] == 1 && clawPos - (255.0 / CLAW_STEPS) > CLAW_OPEN)
			clawPos -= (255.0 / CLAW_STEPS);

		// Set the servo to the new Position
		motor[claw] = clawPos;

		// Wait in order to check a regular number of times per second
		wait1Msec(TICK_DELAY_MILLIS);
	}
}
