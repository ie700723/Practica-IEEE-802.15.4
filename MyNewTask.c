/*
 * MyNewTask.c
 *
 *  Created on: Apr 19, 2020
 *      Author: crist
 */

#include <fsl_os_abstraction.h>
#include "MyNewTask.h"
//#include <TimersManager.h>



osaEventId_t mMyEvents;
/* Global Variable to store our TimerID */
tmrTimerID_t myTimerID = gTmrInvalidTimerID_c;
/* Handler ID for task */
osaTaskId_t gMyTaskHandler_ID;
/* Local variable to store the current state of the LEDs */
static uint8_t ledsState = 0;
/* Main custom task */
/* OSA Task Definition*/

static void myTaskTimerCallback(void *param)
	{
	OSA_EventSet(mMyEvents, gMyNewTaskEvent2_c);
	}



void My_Task(osaTaskParam_t argument)
{
	osaEventFlags_t customEvent;
	myTimerID = TMR_AllocateTimer();
		while(1)
		{
		OSA_EventWait(mMyEvents, osaEventFlagsAll_c, FALSE, osaWaitForever_c,&customEvent);
		if( !gUseRtos_c && !customEvent)
		{
		break;
		}
		/* Depending on the received event */
		switch(customEvent){
			case gMyNewTaskEvent1_c:
				TMR_StartIntervalTimer(myTimerID, 1000,myTaskTimerCallback, NULL);
		TurnOffLeds(); /* Ensure all LEDs are turned off */
		break;
		case gMyNewTaskEvent2_c: /* Event called from myTaskTimerCallback */
		if(!ledsState) {
		TurnOnLeds();
		ledsState = 1;
		}
		else {
		TurnOffLeds();
		ledsState = 0;
		}
		break;
		case gMyNewTaskEvent3_c: /* Event to stop the timer */
		ledsState = 0;
		TurnOffLeds();
		TMR_StopTimer(myTimerID);
		break;
		default:
		break;
		}
		}
		}

OSA_TASK_DEFINE(My_Task, gMyTaskPriority_c, 1, gMyTaskStackSize_c, FALSE );

	/* Function to init the task */
	void MyTask_Init(void)
	{
	mMyEvents = OSA_EventCreate(TRUE);
	/* The instance of the MAC is passed at task creaton */
	gMyTaskHandler_ID = OSA_TaskCreate(OSA_TASK(My_Task), NULL);
	}

	/* This is the function called by the Timer each time it expires */
