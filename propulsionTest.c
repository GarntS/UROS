#pragma config(Motor,  port1,           left_tank,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           claw,          tmotorServoStandard, openLoop)
#pragma config(Motor,  port3,           lift,          tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port10,          right_tank,    tmotorVex393_HBridge, openLoop, reversed)

/*
 *		File:		propulsionTest.c
 *		Author:	Grant Spencer
 *		Date:		4/13/2017
 *		Desc:		A *very* cut-down and safe version of the full UROS code.
 */

 task main()
{
	motor[left_tank] = vexRT[Ch3];
	motor[right_tank] = vexRT[Ch2];

	while(1) {
		if(vexRT[Btn8D] == 1) {
			break
		}
		// Wait in order to check a regular number of times per second
		wait1Msec(50);
	}
}