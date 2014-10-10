/*
 * movement.c
 *
 *  Created on: Sep 25, 2014
 *      Author: superman
 */
#include "space_invaders.h"
#include "movement.h"
#include <stdlib.h>


extern Bunker bunkerArray[];
extern Bullet bulletArray[];
extern Bullet tankBullet;
extern int alienInOut;
extern int* framePointer0;
extern int* lightningBullet_state1;
extern int* lightningBullet_state2;
extern int* lightningBullet_state3;
extern int* crossBullet_state1;
extern int* crossBullet_state2;
extern int* crossBullet_state3;




int alienOriginX;		//we put the origin on the top left corner
int alienOriginY;
int firstColAliveAliens;
int lastColAliveAliens;
int alienFarRightOffset;
int alienDirection;		//see #defines in movement.h for possible directions



int tankOriginX;
int tankOriginY;





//returns a 0 if successful, returns an error if not
void initializeMovement(){
	firstColAliveAliens = 0;
	lastColAliveAliens = ALIENS_PER_ROW - 1;
	alienOriginX = ORIGIN_X_INITIAL;
	alienOriginY = ORIGIN_Y_INITIAL;
	tankOriginX = TANK_X_INITIAL;
	tankOriginY = TANK_Y_INITIAL;
	alienFarRightOffset = FARRIGHT_OFFSET_INITIAL;
	alienDirection = ALIEN_DIRECTION_INITIAL;

}


//updates the locations of the aliens and bullets between every interval of time
void updateLocations(){
	alienInOut = !alienInOut;
	moveAliens();
	moveTankBullet();
	moveAlienBullets();
}


//updates the origin location by the predefined amount
int moveAliens(){
	//basic implementation, without ups or downs

	//what to do if aliens are on the left side of the screen
	if(alienDirection == LEFT && (alienOriginX - PIXELS_PER_MOVE < -1*(ALIEN_WIDTH * firstColAliveAliens))){
		alienOriginY += PIXELS_PER_MOVE;
		undrawAliens(DOWN);
		alienDirection = RIGHT;

	}
	//aliens are on the right side of the screen
	else if(alienDirection == RIGHT && (alienOriginX + alienFarRightOffset + PIXELS_PER_MOVE > (SCREEN_X_PIXELS + (ALIENS_PER_ROW - lastColAliveAliens - 1)*ALIEN_WIDTH))){
		alienOriginY += PIXELS_PER_MOVE;
		undrawAliens(DOWN);
		alienDirection = LEFT;
	}
	//aliens are free to move as normal
	else{
		alienOriginX += alienDirection == RIGHT ? PIXELS_PER_MOVE : PIXELS_PER_MOVE * -1;
		undrawAliens(alienDirection);
	}
	drawAliens();

	return 0;
}

//updates the position of the tank
int moveTank(int direction){
	if(direction == LEFT){
		if(!(tankOriginX - PIXELS_PER_MOVE < 0)){
			tankOriginX -= PIXELS_PER_MOVE;
		}
		else{
			tankOriginX = 0;
		}

	}
	else{
		if(!(tankOriginX + PIXELS_PER_MOVE + TANK_WIDTH >= SCREEN_X_PIXELS )){
			tankOriginX += PIXELS_PER_MOVE;
		}
		else{
			tankOriginX = SCREEN_X_PIXELS - 1 - TANK_WIDTH;
		}
	}

	undrawTank(direction);
	drawTank();


	return 0;
}



int damageBunker(int bunkerNum, int bunkerBlock){
	switch(bunkerBlock){
	case 0:
		if(bunkerArray[bunkerNum].damageStateTopLeft < 4){
			bunkerArray[bunkerNum].damageStateTopLeft=bunkerArray[bunkerNum].damageStateTopLeft + 1;
		}
		else{
			bunkerArray[bunkerNum].damageStateTopLeft = 0;
		}
		break;
	case 1:
		if(bunkerArray[bunkerNum].damageStateBlockTopLeft < 4){
			bunkerArray[bunkerNum].damageStateBlockTopLeft=bunkerArray[bunkerNum].damageStateBlockTopLeft + 1;
		}
		else{
			bunkerArray[bunkerNum].damageStateBlockTopLeft = 0;
		}
		break;
	case 2:
		if(bunkerArray[bunkerNum].damageStateBlockTopRight < 4){
			bunkerArray[bunkerNum].damageStateBlockTopRight=bunkerArray[bunkerNum].damageStateBlockTopRight + 1;
		}
		else{
			bunkerArray[bunkerNum].damageStateBlockTopRight = 0;
		}
		break;
	case 3:
		if(bunkerArray[bunkerNum].damageStateTopRight < 4){
			bunkerArray[bunkerNum].damageStateTopRight=bunkerArray[bunkerNum].damageStateTopRight + 1;
		}
		else{
			bunkerArray[bunkerNum].damageStateTopRight = 0;
		}
		break;
	case 4:
		if(bunkerArray[bunkerNum].damageStateBlockLeftSide < 4){
			bunkerArray[bunkerNum].damageStateBlockLeftSide=bunkerArray[bunkerNum].damageStateBlockLeftSide + 1;
		}
		else{
			bunkerArray[bunkerNum].damageStateBlockLeftSide = 0;
		}
		break;
	case 5:
		if(bunkerArray[bunkerNum].damageStateBottomLeft < 4){
			bunkerArray[bunkerNum].damageStateBottomLeft=bunkerArray[bunkerNum].damageStateBottomLeft + 1;
		}
		else{
			bunkerArray[bunkerNum].damageStateBottomLeft = 0;
		}
		break;
	case 6:
		if(bunkerArray[bunkerNum].damageStateBottomRight < 4){
			bunkerArray[bunkerNum].damageStateBottomRight=bunkerArray[bunkerNum].damageStateBottomRight + 1;
		}
		else{
			bunkerArray[bunkerNum].damageStateBottomRight = 0;
		}
		break;
	case 7:
		if(bunkerArray[bunkerNum].damageStateBlockRightSide < 4){
			bunkerArray[bunkerNum].damageStateBlockRightSide=bunkerArray[bunkerNum].damageStateBlockRightSide + 1;
		}
		else{
			bunkerArray[bunkerNum].damageStateBlockRightSide = 0;
		}
		break;
	case 8:
		if(bunkerArray[bunkerNum].damageStateBlockBottomLeft < 4){
			bunkerArray[bunkerNum].damageStateBlockBottomLeft=bunkerArray[bunkerNum].damageStateBlockBottomLeft + 1;
		}
		else{
			bunkerArray[bunkerNum].damageStateBlockBottomLeft = 0;
		}
		break;
	case 9:
		if(bunkerArray[bunkerNum].damageStateBlockBottomRight < 4){
			bunkerArray[bunkerNum].damageStateBlockBottomRight=bunkerArray[bunkerNum].damageStateBlockBottomRight + 1;
		}
		else{
			bunkerArray[bunkerNum].damageStateBlockBottomRight = 0;
		}
		break;
	default:
		break;
	}

	drawBunker(bunkerNum);

	return 0;
}

int moveAlienBullets(){
	int i;
	for(i = 0; i < NUM_ALIEN_BULLETS; i++){
		if(bulletArray[i].type != INACTIVE_BULLET){ //bullet is active
			bulletArray[i].y += ALIEN_BULLETS_PIXELS_PER_MOVE;
			bulletArray[i].state = (bulletArray[i].state + 1) % 3;
			undrawAlienBullet();
			if(bulletArray[i].y >= 480 - BULLET_HEIGHT){
				bulletArray[i].type = INACTIVE_BULLET; //bullet has gone offscreen so deactivate the bullet
				clearAlienBullet(i);

				//schedule the next bullet fire
				insertDC(rand() % MAX_TICS_BETWEEN_ALIEN_FIRE, EVENT_ALIEN_FIRE);
			}

		}
	}


	drawAlienBullet(-1);
	return 0;
}

int alienBulletCollision(int i){
	//check if in the range of the bunkers
	if((bulletArray[i].y >= BUNKER_INITIAL_Y) && (bulletArray[i].y <= BUNKER_HEIGHT*3)){
		return 0;
	}
	//check if in range of the tank
	else if((bulletArray[i].y >= tankOriginY) && (bulletArray[i].y <= tankOriginY+TANK_HEIGHT) && (bulletArray[i].x >= tankOriginX) && (bulletArray[i].x <= tankOriginX+TANK_WIDTH)){
		int row, col;
		int type = bulletArray[i].type;
		for(row = 0; row < BULLET_HEIGHT; row++){
			for(col = 0; col < BULLET_WIDTH; col++){
				switch (type){
				case SQUIGGLY_BULLET:
					if(bulletArray[i].state == 0){
						if((lightningBullet_state1[row % BULLET_HEIGHT] & (1<<(BULLET_WIDTH-1-col)))){
							if(framePointer0[(bulletArray[i].y + row)*640 + (bulletArray[i].x+col)] != BLACK){
								return 1;
							}
						}
					}
					else if(bulletArray[i].state == 1){
						if((lightningBullet_state2[row % BULLET_HEIGHT] & (1<<(BULLET_WIDTH-1-col)))){
							if(framePointer0[(bulletArray[i].y + row)*640 + (bulletArray[i].x+col)] != BLACK){
								return 1;
							}
						}
					}
					else{
						if((lightningBullet_state3[row % BULLET_HEIGHT] & (1<<(BULLET_WIDTH-1-col)))){
							if(framePointer0[(bulletArray[i].y + row)*640 + (bulletArray[i].x+col)] != BLACK){
								return 1;
							}
						}
					}
					break;
				case CROSS_BULLET:
					if(bulletArray[i].state == 0){
						if((crossBullet_state1[row % BULLET_HEIGHT] & (1<<(BULLET_WIDTH-1-col)))){
							if(framePointer0[(bulletArray[i].y + row)*640 + (bulletArray[i].x+col)] != BLACK){
								return 1;
							}
						}
					}
					else if(bulletArray[i].state == 1){
						if((crossBullet_state2[row % BULLET_HEIGHT] & (1<<(BULLET_WIDTH-1-col)))){
							if(framePointer0[(bulletArray[i].y + row)*640 + (bulletArray[i].x+col)] != BLACK){
								return 1;
							}
						}
					}
					else{
						if((crossBullet_state3[row % BULLET_HEIGHT] & (1<<(BULLET_WIDTH-1-col)))){
							if(framePointer0[(bulletArray[i].y + row)*640 + (bulletArray[i].x+col)] != BLACK){
								return 1;
							}
						}
					}
					break;
				default:
					break;
				}
			}
		}
	}
	else{
		return 0; //no collision
	}
	return 0;

}

int moveTankBullet(){
	if(tankBullet.type != INACTIVE_BULLET){ //bullet is active
		tankBullet.y -= TANK_BULLET_PIXELS_PER_MOVE;
		undrawTankBullet();
		if(tankBullet.y < 0){
			tankBullet.type = INACTIVE_BULLET; //bullet has gone offscreen so deactivate the bullet
			clearTankBullet();
		}

	}


	drawTankBullet();
	return 0;
}


