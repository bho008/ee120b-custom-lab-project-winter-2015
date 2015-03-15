#ifndef _Ai__
#define _Ai__

#include "Snake.h"

void AI_singleplayer(){
	//go up
	if((segments1[0].y > foodY+2)){// && (segments1[0].y != foodY)){
		if(lastDir1 != down1){
			lastDir1 = up1;
			//return;
		}
	}
	//go right
	else if(segments1[0].x < foodX+2){
		if(lastDir1!=left1){
			lastDir1 = right1;
			//return;
		}
	}
	
	//go left
	else if(segments1[0].x > foodX+2){
		if(lastDir1 != right1){
			lastDir1 = left1;
			//return;
		}
	}
	
	//go down
	else if(segments1[0].y < foodY+2){
		if(lastDir1 != up1){
			lastDir1 = down1;
			//return;
		}
	}
	
	//detect collision
	for(uint8_t i = 1; i < current_segment1; i++){
		if(lastDir1 == right1){
			if(segments1[0].x + 1 == segments1[i].x && segments1[0].y == segments1[i].y){
				//if(segments[0].x + 1 == 128)
				//lastDir1 = down1;
				lastDir1 = down1;
			}
			if(segments1[0].x + 1 == 128 && segments1[0].x == 0){
				lastDir1 = down1;
			}
		}
		else if(lastDir1 == down1){
			if(segments1[0].y + 1 == segments1[i].y && segments1[0].x == segments1[i].x){
				lastDir1 = right1;
			}
			if(segments1[0].y+1 == 160 && segments1[i].y == 0){
				lastDir1 == right1;
			}
		}
		else if(lastDir1 == left1){
			if(segments1[0].x - 1 == segments1[i].x && segments1[0].y == segments1[i].y){
				//lastDir1 = up1;
			}
		}
	}
	
	
	
}

//controls snake2
void AI_multiplayer(){
	if(foodX > segments2->x){
		if(lastDir2 != up2)
		lastDir2 = down2;
	}
}

#endif