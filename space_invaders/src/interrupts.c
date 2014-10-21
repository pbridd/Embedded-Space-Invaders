/*
 * interrupts.c
 *
 *  Created on: Oct 7, 2014
 *      Author: superman
 */
#include "xgpio.h"          // Provides access to PB GPIO driver.
#include <stdio.h>          // xil_printf and so forth.
#include "platform.h"       // Enables caching and other system stuff.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.
#include "space_invaders.h"
#include "interrupts.h"
#include "time.h"

u32 const TICS_UNTIL_DEBOUNCED = 5;		//the number of ticks needed from the timer for the switches to be debounced
u32 const MS_PER_TIC = 10;
u32 const TICS_PER_SECOND = 100;
u32 const SECONDS_PER_MINUTE = 60;
u32 const MINUTES_PER_HOUR = 60;
u32 const HOURS_PER_DAY = 24;
u32 const FITCOUNTER_MAX = 999999;
u32 const TICS_PER_ALIEN_LOC_UPDATE = 60;			//1/2 a second before every location update
u32 const TICS_PER_TANK_LOC_UPDATE = 4;
u32 const TICS_PER_BULLET_LOC_UPDATE = 4;
u32 const TICS_PER_SCREEN_UPDATE = 2;
u32 const TICS_PER_RED_SPACESHIP_LOC_UPDATE = 4;
u32 const TICS_BETWEEN_DEAD_TANK_OSCILLATION = 10;
u32 const TICS_PER_RED_SPACESHIP_SCORE_CLEAR = 100;

extern int alienInOut;
extern int redSpaceshipStatus;
extern int tankState;
extern int clearRedSpaceshipScoreFlag;
extern int gameOver;
extern DCResult dcResult;

double best_case_time = 10000000;
double worst_case_time = 0;
double total_time = 0;
int num_times_entered = 0;

volatile int timerHandlerCounter = 0;

u32 fitcounter;


XGpio gpPB;   // This is a handle for the push-button GPIO block.

void initInterrupts(){
	//register interrupts
	microblaze_register_handler(interrupt_handler_dispatcher, NULL);
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
			(XPAR_FIT_TIMER_0_INTERRUPT_MASK));
	XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
	microblaze_enable_interrupts();
}


//main interrupt handler--calls the appropriate ISR
void interrupt_handler_dispatcher(){
	timerHandlerCounter++;
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the FIT interrupt first.
	if (intc_status & XPAR_FIT_TIMER_0_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
		timer_interrupt_handler();
	}
}

void timer_interrupt_handler(){
	//get the initial time
	time_t prev_time;
	time(&prev_time);

	incrementDC(1);
	int i;
	for(i=0; i < dcResult.numEntries; i++){
		int evnum = dcResult.triggeredEvents[i];
		//execute the event which has just happened
		if(evnum == EVENT_ALIEN_FIRE){
			fireAlienBullet();
		}
		else if(evnum == EVENT_RED_SPACESHIP_APPEAR){
			redSpaceshipAppear();
		}
		else if(evnum == EVENT_TANK_DEATH){
			tankRevive();
		}
	}


	if(gameOver){
		getButtonInputGameOver();
	}
	//is it time to update the location?
	else if(tankState ==  0){
		if(fitcounter % TICS_PER_TANK_LOC_UPDATE == 0){
			getButtonInput();
		}
		if(fitcounter % TICS_PER_BULLET_LOC_UPDATE == 0){
			moveAlienBullets();
			moveTankBullet();
		}
		if(redSpaceshipStatus == RED_SPACESHIP_ALIVE && fitcounter % TICS_PER_RED_SPACESHIP_LOC_UPDATE == 0){
			moveRedSpaceship();
		}
		if(fitcounter % TICS_PER_RED_SPACESHIP_SCORE_CLEAR == 0 && clearRedSpaceshipScoreFlag){
			clearRedSpaceshipScore();
		}
		if(fitcounter % TICS_PER_ALIEN_LOC_UPDATE == 0){
			alienInOut = !alienInOut;
			moveAliens();
		}
	}
	else{
		if(fitcounter % TICS_BETWEEN_DEAD_TANK_OSCILLATION == 0){
			drawTank();
		}
	}

	if(fitcounter == FITCOUNTER_MAX){
		fitcounter = 0;
	}
	else{
		fitcounter++;
	}



}
