#ifndef _Color_cycle_rgb.h_
#define _Color_cycle_rgb.h_

enum colors{red_to_green, green_to_blue, blue_to_red} color_set;
//color_set = red_to_green;
uint8_t r = 0, g = 0, b = 0;

void updateColor(){
	switch(color_set){
		case red_to_green:
		g++;
		r = 255 - g;
		if(g >= 255){
			b = 0;
			color_set = green_to_blue;
		}
		break;
		case green_to_blue:
		b++;
		g = 255-b;
		if(b >= 255){
			r = 0;
			color_set = blue_to_red;
		}
		break;
		case blue_to_red:
		r++;
		b = 255-r;
		if(r >= 255){
			g = 0;
			color_set = red_to_green;
		}
		break;
		default:
		color_set = red_to_green;
		break;
	}
}

#endif