#ifndef _Snake_
#define _Snake_

#include "ST7735_1284.h"
#include <stdlib.h>

//srand((unsigned int)(1));
int clock_counter = 0;

uint8_t is_multiplayer = 0;

short xPos1, yPos1, xPos2, yPos2;
short xLast1, yLast1, xLast2, yLast2;
uint8_t lastDir1 = 0;
uint8_t lastDir2 = 0;
uint8_t needFood = 0;

uint8_t foodSize = 5;

uint8_t foodX = 0, foodY = 0;

#define max_length 200
uint8_t current_segment1 = 0;
uint8_t current_segment2 = 0;

uint8_t game_over = 0;

uint8_t snake_score_1 = 0;
uint8_t snake_score_2 = 0;

typedef struct SnakeSegment
{
	int x, y;
} ;
struct SnakeSegment segments1[max_length];
struct SnakeSegment segments2[max_length];

void init_Snake1(uint8_t _length, uint8_t _pos){
	game_over = 0;
	snake_score_1 = 0;
	snake_score_2 = 0;
	xPos1 = width/2; yPos1 = _pos;
	
	//initialize x and y pos to -1
	for(int i = 0; i < max_length; i++){
		segments1[i].x = -1;
		segments1[i].y = -1;
	}
	
	//initialize with length
	for(int i = 0; i < _length; i++){
		segments1[i].x = width/2 - i;
		segments1[i].y = _pos/2;// -i;
	}
	current_segment1 = _length;
	
	for(int i = 0; i < current_segment1; i++){
		fillRect(segments1[i].x, segments1[i].y, 1, 1, 0xFF);
		//fillRect(xLast, yLast, 1, 1, 0x0000);
	}
}

void init_Snake2(uint8_t _length, uint8_t _pos){
	xPos2 = width/2; yPos2 = _pos;
	
	//initialize x and y pos to -1
	for(int i = 0; i < max_length; i++){
		segments2[i].x = -1;
		segments2[i].y = -1;
	}
	
	//initialize with length
	for(int i = 0; i < _length; i++){
		segments2[i].x = width/2 - i;
		segments2[i].y = _pos/2;// -i;
	}
	current_segment2 = _length;
	
	for(int i = 0; i < current_segment2; i++){
		fillRect(segments2[i].x, segments2[i].y, 1, 1, COLOR_GREEN);
		//fillRect(xLast, yLast, 1, 1, 0x0000);
	}
}

enum Directions1{still1, right1, up1, down1, left1  }direction1;
void update_Snake1(uint8_t direction1){
	xLast1 = segments1[current_segment1-1].x;
	yLast1 = segments1[current_segment1-1].y;
	
	if(direction1){
		for(int i = current_segment1-1; i > 0; i--){
			segments1[i].x = segments1[i-1].x;
			segments1[i].y = segments1[i-1].y;
		}
	
	
		switch(direction1){
			case right1:
				xPos1 = segments1[0].x+1;
				if(xPos1 >= 127)
					xPos1 = 0;
				segments1[0].x = xPos1;
				break;
		
			case up1:
				yPos1 = segments1[0].y-1;
				if(yPos1 < 10)
					yPos1 = 159;
				segments1[0].y = yPos1;
			break;
		
			case down1:
				yPos1 = segments1[0].y+1;
				if(yPos1 > 159)
					yPos1 = 10;
				segments1[0].y = yPos1;
			break;
		
			case left1:
				xPos1 = segments1[0].x-1;
					if(xPos1 < 0)
				xPos1 = 127;
				segments1[0].x = xPos1;
			break;
		
			case still1:
				break;
		
			default:
				break;
		}
	}
}

enum Directions2{still2, right2, up2, down2, left2  }direction2;
void update_Snake2(uint8_t direction2){
	xLast2 = segments2[current_segment2-1].x;
	yLast2 = segments2[current_segment2-1].y;
	
	if(direction2){
		for(int i = current_segment2-1; i > 0; i--){
			segments2[i].x = segments2[i-1].x;
			segments2[i].y = segments2[i-1].y;
		}
		
		
		switch(direction2){
			case right2:
			xPos2 = segments2[0].x+1;
			if(xPos2 >= 127)
			xPos2 = 0;
			segments2[0].x = xPos2;
			break;
			
			case up2:
			yPos2 = segments2[0].y-1;
			if(yPos2 < 10)
			yPos2 = 159;
			segments2[0].y = yPos2;
			break;
			
			case down2:
			yPos2 = segments2[0].y+1;
			if(yPos2 > 159)
			yPos2 = 10;
			segments2[0].y = yPos2;
			break;
			
			case left2:
			xPos2 = segments2[0].x-1;
			if(xPos2 < 0)
			xPos2 = 127;
			segments2[0].x = xPos2;
			break;
			
			case still2:
			break;
			
			default:
			break;
		}
	}
}


void add_segment1(){
	
	if(current_segment1 < max_length){
		current_segment1++;
			segments1[current_segment1].x = segments1[current_segment1-1].x;
			segments1[current_segment1].y = segments1[current_segment1-1].y;
		current_segment1++;
		segments1[current_segment1].x = segments1[current_segment1-1].x;
		segments1[current_segment1].y = segments1[current_segment1-1].y;
		current_segment1++;
		segments1[current_segment1].x = segments1[current_segment1-1].x;
		segments1[current_segment1].y = segments1[current_segment1-1].y;
		current_segment1++;
		segments1[current_segment1].x = segments1[current_segment1-1].x;
		segments1[current_segment1].y = segments1[current_segment1-1].y;
		current_segment1++;
		segments1[current_segment1].x = segments1[current_segment1-1].x;
		segments1[current_segment1].y = segments1[current_segment1-1].y;
	}
}

void detect_collision_self1(){
	for(uint8_t i = 2; i < current_segment1; i++){
		if((segments1[0].x == segments1[i].x) && (segments1[0].y == segments1[i].y)){
			game_over = 1;
		}
		if(is_multiplayer)
		if((segments1[0].x == segments2[i].x) && (segments1[0].y == segments2[i].y)){
			game_over = 1;
		}
	}
}

void add_segment2(){
	
	if(current_segment2 < max_length){
		current_segment2++;

			segments2[current_segment2].x = segments2[current_segment2-1].x;//-30;
			segments2[current_segment2].y = segments2[current_segment2-1].y;//+30;
		current_segment1++;
		segments1[current_segment1].x = segments1[current_segment1-1].x;
		segments1[current_segment1].y = segments1[current_segment1-1].y;
		current_segment1++;
		segments1[current_segment1].x = segments1[current_segment1-1].x;
		segments1[current_segment1].y = segments1[current_segment1-1].y;
		current_segment1++;
		segments1[current_segment1].x = segments1[current_segment1-1].x;
		segments1[current_segment1].y = segments1[current_segment1-1].y;
		current_segment1++;
		segments1[current_segment1].x = segments1[current_segment1-1].x;
		segments1[current_segment1].y = segments1[current_segment1-1].y;
	}
}

void detect_collision_self2(){
	for(uint8_t i = 2; i < current_segment2; i++){
		if((segments2[0].x == segments2[i].x) && (segments2[0].y == segments2[i].y)){
			game_over = 1;
		}
		if(is_multiplayer)
		if((segments2[0].x == segments1[i].x) && (segments2[0].y == segments1[i].y)){
			game_over = 1;
		}
	}
}

void detect_collision_both(){
	
}

void detect_collision_food_1(){
	//check top
	for(uint8_t i = 0; i < foodSize; i++){
		if(segments1[0].x == (foodX+i) && segments1[0].y == (foodY)){
			fillRect(foodX, foodY, foodSize, foodSize, 0);
			needFood = 1;
			//add_segment1();
			break;
		}
	}
	//check bottom
	for(uint8_t i = 0; i < foodSize; i++){
		if(segments1[0].x == (foodX+i) && segments1[0].y == (foodY+foodSize)){
			fillRect(foodX, foodY, foodSize, foodSize, 0);
			needFood = 1;
			//add_segment1();
			break;
		}
	}
	//check left
	for(uint8_t i = 0; i < foodSize; i++){
		if(segments1[0].x == (foodX) && segments1[0].y == (foodY+i)){
			fillRect(foodX, foodY, foodSize, foodSize, 0);
			needFood = 1;
			//add_segment1();
			break;
		}
	}
	//check right
	for(uint8_t i = 0; i < foodSize; i++){
		if(segments1[0].x == (foodX + foodSize) && segments1[0].y == (foodY+i)){
			fillRect(foodX, foodY, foodSize, foodSize, 0);
			needFood = 1;
			//add_segment1();
			break;
		}
	}
}

void detect_collision_food_2(){
	//check top
	for(uint8_t i = 0; i < foodSize; i++){
		if(segments2[0].x == (foodX+i) && segments2[0].y == (foodY)){
			fillRect(foodX, foodY, foodSize, foodSize, 0);
			needFood = 1;
			add_segment2();
			break;
		}
	}
	//check bottom
	for(uint8_t i = 0; i < foodSize; i++){
		if(segments2[0].x == (foodX+i) && segments2[0].y == (foodY+foodSize)){
			fillRect(foodX, foodY, foodSize, foodSize, 0);
			needFood = 1;
			add_segment2();
			break;
		}
	}
	//check left
	for(uint8_t i = 0; i < foodSize; i++){
		if(segments2[0].x == (foodX) && segments2[0].y == (foodY+i)){
			fillRect(foodX, foodY, foodSize, foodSize, 0);
			needFood = 1;
			add_segment2();
			break;
		}
	}
	//check right
	for(uint8_t i = 0; i < foodSize; i++){
		if(segments2[0].x == (foodX + foodSize) && segments2[0].y == (foodY+i)){
			fillRect(foodX, foodY, foodSize, foodSize, 0);
			needFood = 1;
			add_segment2();
			break;
		}
	}
}

void add_food(){
	//if(clock_counter)
	//foodX = clock_counter % 1 + 10;
	//foodY = rand() % 1 - 10;
	foodX = clock_counter*2;
	foodY = clock_counter*3;
	while(foodX > 120){
		foodX >>= 1;
	}
	while(foodY >= 150){
		foodY >>= 1;
	}
	
	if(foodY < 11){
		foodY += 20;
	}
	//check collision with snake
	
	
	fillRect(foodX, foodY, foodSize, foodSize, 0xFFFF);
	needFood = 0;
}


void draw_Snake1(){
	/*
	for(int i = 0; i < current_segment1; i++){
		fillRect(segments1[i].x, segments1[i].y, 1, 1, 0xFF);
		fillRect(xLast1, yLast1, 1, 1, 0x0000);
	}
	*/
	fillRect(segments1[0].x, segments1[0].y, 1, 1, 0xFF);
	fillRect(xLast1, yLast1, 1, 1, 0x0000);
}

void draw_Snake2(){
	/*
	for(int i = 0; i < current_segment2; i++){
		fillRect(segments2[i].x, segments2[i].y, 1, 1, COLOR_GREEN);
		fillRect(xLast2, yLast2, 1, 1, 0x0000);
	}
	*/
	fillRect(segments2[0].x, segments2[0].y, 1, 1, COLOR_GREEN);
	fillRect(xLast2, yLast2, 1, 1, 0x0000);
}

#endif