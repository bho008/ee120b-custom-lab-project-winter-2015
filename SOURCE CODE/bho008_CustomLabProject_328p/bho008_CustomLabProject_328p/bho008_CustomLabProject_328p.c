/*
 * bho008_CustomLabProject_328p.c
 *
 * Created: 3/1/2015 1:08:45 PM
 *  Author: Brian
 */ 


#include <avr/io.h>
#include "Snake.h"
#include "timer.h"
#include "ST7735_1284.h"
#include "SPI_1284.h"
#include "usart_328p_PRIMARY.h"
#include <stdio.h>
#include "Enable_ADC.h"
#include "Color_cycle_rgb.h"
#include "Ai.h"
#define F_CPU 20000000UL

//scl = PB5
//sda = PB3

uint16_t periodMultiplier = 1;
uint8_t setPeriod = 1;
uint16_t game_speed;
//main buttons
	unsigned char button1 = 0x00; // for pinc
	unsigned char button1_temp = 0x00; // for pind
	unsigned char upButton1 = 0x00;
	unsigned char downButton1 = 0x00;
	unsigned char leftButton1 = 0x00;
	unsigned char rightButton1 = 0x00;
	unsigned char aux_a_1 = 0x00;	//pd2
	unsigned char aux_b_1 = 0x00;	//pd3
	unsigned char aux_start_1 = 0x00;	//pd4
	uint8_t prev_button1 = 0x00;

//wireless controller buttons
	unsigned char button2 = 0x00;
	unsigned char upButton2 = 0x00;
	unsigned char downButton2 = 0x00;
	unsigned char leftButton2 = 0x00;
	unsigned char rightButton2 = 0x00;
	unsigned char aux_a_2 = 0x00;
	unsigned char aux_b_2 = 0x00;
	unsigned char aux_start_2 = 0x00;

uint8_t usart_temp = 0x00;

void setTimerPeriod(uint16_t period){
	TimerSet(period);
	periodMultiplier = period;
}

enum Clock_states{init_clock, update_clock}clock_state;


char convert_buffer[16];

void Clock_sm(){
	switch(clock_state){
		case init_clock:
			clock_counter = 0;
			clock_state = update_clock;
			break;
		
		case update_clock:
			
			sprintf(convert_buffer, "%d", clock_counter);
			drawString(1, 1, convert_buffer, 0, 1);
			
			clock_counter++;
			
			sprintf(convert_buffer, "%d", clock_counter);
			drawString(1, 1, convert_buffer, 0xff, 1);
			break;
			
	}
}

void getButton(){
	button1 = (~PINC & 0x0f);
	button1_temp = (~PIND & 0x1c);
	
	if(button1){
		if(button1 & 0x01){
			button1 = 3;
			if(lastDir1!= up1)
			lastDir1 = down1;
			
		}
		else if(button1 & 0x02){
			button1 = 2;
			if(lastDir1 != left1)
			lastDir1 = right1;
			
		}
		else if(button1 & 0x04){
			 button1 = 4;
			 if(lastDir1 != right1)
			 lastDir1 = left1;
		}
		else if(button1 & 0x08){
			button1 = 1;
			if(lastDir1 != down1)
			lastDir1 = up1;
		}
	}
	
	else if(button1_temp){
		if(button1_temp & 0x4){
			button1 = 5;
		}
		else if(button1_temp & 0x08){
			button1 = 6;
		}
		else if(button1_temp & 0x10){
			button1 = 7;
		}
	}
	else button1 = 0x0;
	
	drawChar(120, 1, prev_button1+ '0', 0, 1);
	drawChar(120, 1, button1 + '0', 0xff, 1);
	prev_button1 = button1;
}

enum Game_states{wait, selection_screen, selection_button_wait, credit_screen, initialize_game, run_single, run_multi, pause} game_state;


	uint8_t temp = 0;
uint8_t drawSelectionScreen = 1;
uint8_t drawCreditScreen = 1;
uint8_t game_mode = 1;
//uint8_t is_multiplayer = 0;

uint8_t speed[] = {50, 40, 20, 5};
uint8_t speed_choice = 0;

void Game_sm(){
	switch(game_state){
		case wait:
			//init_Snake(50);
			//draw_Snake();
			
			//direction = right;
			fillScreen(0);
			if(game_over){
				drawString(5, 10, "GAME OVER", COLOR_RED, 2);
				drawString(10, 30, "Player 1 score: ", 0xFF, 1);
				sprintf(convert_buffer, "%d", snake_score_1);
				drawString(100, 30, convert_buffer, 0xff, 1);
				if(is_multiplayer){
					drawString(10, 50, "Player 2 score: ", 0xFF, 1);
					sprintf(convert_buffer, "%d", snake_score_2);
					drawString(100, 50, convert_buffer, 0xff, 1);
				}
				drawCreditScreen = 1;
				drawSelectionScreen = 1;
			}
			game_state = credit_screen;
			break;
		
		case credit_screen:
			//uint8_t start = ~PIND;
			drawString(5, height/2-10, "SNAKE GAME", Color565(r, g, b), 2);
			if(drawCreditScreen){
				drawString(5, height/2-10, "SNAKE GAME", COLOR_GREEN, 2);
				drawString(10, height/2+8, "By: Brian Ho", COLOR_CYAN, 1);
				drawString(5, height/2+20, "EE120B Custom Lab", COLOR_WHITE, 1);
				drawString(5, height/2+32, "Project", COLOR_WHITE, 1);
				
				drawString(5, height/2 + 8*8-2, "Press any button to ", COLOR_BLUE, 1);
				drawString(width/2-25, height/2 + 8*9-2, "continue ", COLOR_BLUE, 1);
				
				drawCreditScreen = 0;
			}
			if((~PIND & 0x0c) || (~PINC & 0x0f) || usart_temp){
				game_state = selection_screen;
				fillScreen(0);
				drawCreditScreen = 1;
			}
			else
			game_state = credit_screen;
			break;
			
		case selection_screen:
			if(setPeriod){
				TimerSet(50);
				periodMultiplier = 50;
				setPeriod = 0;
			}
			//button1 = ~PINC & 0x0f;
			
			drawString(5, 12, "SNAKE GAME", Color565(r, g, b), 2);
			
			if((button1 == 2) || (button1 == 4) && (! drawSelectionScreen)){// || usart_temp == 2 || usart_temp == 4){ 
				game_mode++;
				if(game_mode % 2 != 0){
					drawString(25, height/2, "single player", 0, 1);
					drawString(25, height/2, "multiplayer", Color565(r, g, b), 1);
					is_multiplayer = 1;
					game_state = selection_button_wait;
					return;
				}
				
				else if(game_mode % 2 == 0){
					drawString(25, height/2, "multiplayer", 0, 1);
					drawString(25, height/2, "single player", Color565(r, g, b), 1);
					is_multiplayer = 0;
					game_state = selection_button_wait;
					return;
				}
			}
			
			if((button1 == 1) || (button1 == 3) && !drawSelectionScreen){
				if(button1 == 1)
				speed_choice++;
				if(button1 == 3)
				speed_choice--;
				
				if(speed_choice>4)
					speed_choice = 0;
					
				if(speed_choice == 0){
					//drawString(65, height/2+15, "insane", 0, 1);
					fillRect(65, height/2+15, 50, 20, 0);
					drawString(65, height/2+15, "normal", COLOR_WHITE, 1);
					game_state = selection_button_wait;
				}
				
				if(speed_choice == 1){	
					fillRect(65, height/2+15, 50, 20, 0);
					drawString(65, height/2+15, "medium", COLOR_CYAN, 1);
					game_state = selection_button_wait;
				}
				
				if(speed_choice == 2){
					fillRect(65, height/2+15, 50, 20, 0);
					drawString(65, height/2+15, "fast", COLOR_RED, 1);
					game_state = selection_button_wait;
				}
				
				if(speed_choice == 3){
					drawString(65, height/2+15, "fast", 0, 1);
					//drawString(65, height/2+15, "insane", COLOR_WHITE, 1);
					drawChar(65+5*0, height/2+15, 'i', COLOR_CYAN, 1);
					drawChar(65+5*1+4, height/2+15, 'n', COLOR_GREEN, 1);
					drawChar(65+5*2+2+6, height/2+15, 's', COLOR_MAGENTA, 1);
					drawChar(65+5*3+3+8, height/2+15, 'a', COLOR_YELLOW, 1);
					drawChar(65+5*4+4+10, height/2+15, 'n', COLOR_RED, 1);
					drawChar(65+5*5+5+12, height/2+15, 'e', COLOR_CYAN, 1);
					game_state = selection_button_wait;
				}
			}
			
			if(drawSelectionScreen){
				drawString(5, 12, "SNAKE GAME", COLOR_GREEN, 2);
				drawString(25, height/2, "single player", COLOR_MAGENTA, 1);
				drawString(25, height/2+15, "speed: ", COLOR_WHITE, 1);
				drawString(65, height/2+15, "normal", COLOR_WHITE, 1);
				is_multiplayer = 0;
				speed_choice = 0;
				drawSelectionScreen = 0;
			}
			if(button1 == 5 || usart_temp == 5){
				game_state = initialize_game;
				fillScreen(0);
				drawSelectionScreen = 1;
				setPeriod = 1;
			}
			break;
		
		case selection_button_wait:
			if(!button1){
				game_state = selection_screen;
			}
			break;
		
		case initialize_game:
			/*
			if(setPeriod){
				TimerSet(20);
				periodMultiplier = 20;
				setPeriod = 0;
			}
			*/
			setTimerPeriod(speed[speed_choice]);
			lastDir1 = lastDir2 = still1;
			if(is_multiplayer){
				init_Snake1(50, height-50);
				draw_Snake1();
				init_Snake2(50, height+50);
				draw_Snake2();
				
				direction1 = right1;
				direction2 = right2;
				
				add_food();
				game_state = run_multi;
				sprintf(convert_buffer, "%d", snake_score_1);
				drawString(1, 150, convert_buffer, 0xff, 1);
				sprintf(convert_buffer, "%d", snake_score_2);
				drawString(110, 150, convert_buffer, 0xff, 1);
			}
			else{
				init_Snake1(50, height);
				draw_Snake1();
				add_food();
				sprintf(convert_buffer, "%d", snake_score_1);
				drawString(1, 150, convert_buffer, 0xff, 1);
				//drawString(1, 150, "Player 1: ", 0xFF, 1);
				//drawString(1, 150, "Player 1: ", 0xFF, 1);
				direction1 = right1;
				game_state = run_single;
			}
			
			uint16_t h_pos = width/2;
			uint16_t v_pos = height/2;
			//fillRect(h_pos, v_pos, 2, 2, 0xFFFF);
			setPeriod = 1;
			break;
		
		case pause:
			getButton();
			if(button1 == 7){
				game_state = wait;
			}
			break;;
			
		case run_single:
			if(game_over){
				game_state = pause;
				break;
			}
			
			if(needFood){
				add_food();
				add_segment1();
				sprintf(convert_buffer, "%d", snake_score_1);
				//drawString(1, 150, "Player 1: ", 0xFF, 1);
				drawString(1, 150, convert_buffer, 0, 1);
				snake_score_1++;
				sprintf(convert_buffer, "%d", snake_score_1);
				drawString(1, 150, convert_buffer, 0xff, 1);
			}
			detect_collision_food_1();

			if(button1 == 7){
				game_state = wait;
				break;
			}
			
			if(button1 == 1){
				upButton1 = 1;
			}
			else if(button1 == 2){
				rightButton1 = 1;
			}
			else if(button1 == 3){
				downButton1 = 1;
			}
			else if(button1 == 4){
				leftButton1 = 1;
			}
			else{
				upButton1 = downButton1 = leftButton1 = rightButton1 = 0;
			}
			
			//AI_singleplayer();
        
			if(usart_temp== 1){
				upButton1 = 1;
			}
			else if(usart_temp== 2){
				rightButton1 = 1;
			}
			else if(usart_temp == 3){
				downButton1 = 1;	
			}
			else if(usart_temp == 4){
				leftButton1 = 1;
			}
			//if(temp)
        
        
			if(upButton1){
				//drawString(20, 20, "upButton", 0xff, 1);
				//update_Snake1(up1);
				if(lastDir1 != down1)
				lastDir1 = up1;
			}
			else if(downButton1){
				//update_Snake1(down1);
				if(lastDir1 != up1)
				lastDir1 = down1;
			}
			else if(rightButton1){
				//update_Snake1(right1);
				if(lastDir1!=left1)
				lastDir1 = right1;
			}
			else if(leftButton1){
				//update_Snake1(left1);
				if(lastDir1!= right1)
				lastDir1 = left1;
			}
			
			if(button1 == 6){
				add_segment1();
			}
			
			update_Snake1(lastDir1);
			detect_collision_self1();
		
			//if(button1 || usart_temp){
				draw_Snake1(lastDir1);
			//}
			
			break;
/* ---------------------------------------------------------------------------*/
		
		case run_multi:
			if(game_over){
				game_state = pause;
				break;
			}
			
			detect_collision_food_1();
			if(needFood){
				add_food();
				add_segment1();
				sprintf(convert_buffer, "%d", snake_score_1);
				//drawString(1, 150, "Player 1: ", 0xFF, 1);
				drawString(1, 150, convert_buffer, 0, 1);
				snake_score_1++;
				sprintf(convert_buffer, "%d", snake_score_1);
				drawString(1, 150, convert_buffer, 0xff, 1);
			}
			
			detect_collision_food_2();
			if(needFood){
				add_food();
				add_segment2();
				sprintf(convert_buffer, "%d", snake_score_2);
				drawString(110, 150, convert_buffer, 0, 1);
				snake_score_2++;
				sprintf(convert_buffer, "%d", snake_score_2);
				drawString(110, 150, convert_buffer, 0xff, 1);
			}
			
			if(button1 == 7){
				game_state = wait;
				break;
			}
		
			if(button1 == 1){
				upButton1 = 1;
			}
			else if(button1 == 2){
				rightButton1 = 1;
			}
			else if(button1 == 3){
				downButton1 = 1;
			}
			else if(button1 == 4){
				leftButton1 = 1;
			}
			else{
				upButton1 = downButton1 = leftButton1 = rightButton1 = 0;
			}
		
		
			//player 2 snake
			//AI_multiplayer();
			
			if(usart_temp== 1){
				//drawString(40, 40, "hello", 0xff, 1);
				//upButton2 = 1;
				if(lastDir2 != down2)
				lastDir2 = up2;
			}
			else if(usart_temp== 2){
				//rightButton2 = 1;
				if(lastDir2 != left2)
				lastDir2 = right2;
			}
			else if(usart_temp == 3){
				//downButton2 = 1;
				if(lastDir2 != up2)
				lastDir2 = down2;
			}
			else if(usart_temp == 4){
				//leftButton2 = 1;
				if(lastDir2!= right2)
				lastDir2 = left2;
			}
			//if(temp)
		
		
			if(upButton1){
				//drawString(20, 20, "upButton", 0xff, 1);
				//update_Snake1(up1);
				if(lastDir1 != down1)
				lastDir1 = up1;
			}
			else if(downButton1){
				//update_Snake1(down1);
				if(lastDir1 != up1)
				lastDir1 = down1;
			}
			else if(rightButton1){
				//update_Snake1(right1);
				if(lastDir1!=left1)
				lastDir1 = right1;
			}
			else if(leftButton1){
				//update_Snake1(left1);
				if(lastDir1!= right1)
				lastDir1 = left1;
			}
		
			if(button1 == 6){
				add_segment1();
			}
		
			update_Snake1(lastDir1);
			update_Snake2(lastDir2);
			detect_collision_self1();
			detect_collision_self2();
			detect_collision_both();
		
			//if(button1 || usart_temp){
			draw_Snake1(lastDir1);
			draw_Snake2(lastDir2);
			//}
		
			break;
	}
}

enum Usart_states{usart_wait} Usart_state;

//uint8_t usart_temp = 0x00;
char usart_char[8];
char prev_temp;

void USART_sm(){
	switch(Usart_state){
		case usart_wait:
			
			if(USART_HasReceived()){
				//drawChar(10, 100, '1', 0xFF, 1);
				usart_temp = USART_Receive();
				
				USART_Flush();
				//itoa(usart_temp, usart_char, 10);
				//sprintf(usart_char, "%d", usart_temp);
				drawChar(100, 1, prev_temp+'0', 0, 1);
				
				//clock_counter++;
				
				//sprintf(convert_buffer, "%d", usart_temp);
				//drawChar(100, 1, prev_temp+'0', 0, 1);
				drawChar(100, 1, usart_temp+'0', 0xff, 1);
				prev_temp = usart_temp;
				
			}
			
			break;
			
	}
}

void Console_sm(){
	//drawString(1, 120, itoa(xPos1), 0xff, 1);
	
	sprintf(convert_buffer, "%d", xLast1);
	drawString(1, 150, convert_buffer, 0, 1);
	sprintf(convert_buffer, "%d", xPos1);
	drawString(1, 150, convert_buffer, 0xFF, 1);
}


int main(void)
{
	_delay_ms(3000);
	USART_Init();
	SPI_MasterInit();
	srand(99);
	
	initADC();
	
	InitDisplay();
	
	fillScreen(0);
	
	//DDRD = 0x00; PORTD = 0xFF;
	
	//drawChar(1, 1, 'Q', 0xFF, 22);
	//drawChar(30, 30, 'Q', 0xFF, 8);
	

	DDRC = 0x00; PORTC = 0xFf;
	DDRD = 0x00; PORTD = 0xff;
	
	_delay_ms(100);
	
	
	TimerSet(1);
	TimerOn();
	
	uint16_t timer = 0;
	uint16_t clock_period = 0;
	uint32_t count = 0;
	uint16_t game_period = 0;
	
	uint16_t prev_adc = 0;
	float adc_display = 0.0;
	
	game_state = wait;
	clock_state = init_clock;
	Usart_state = usart_wait;
    while(1)
    {
        //TODO:: Please write your application code 
		
		//USART_sm();
		updateColor();
		if(timer >= 10){
			//drawChar(1, 100, '0', 0xFF, 1);
			USART_sm();getButton();
			//Clock_sm();
			
		}
		if(clock_period > 200){
			Clock_sm();
			
			clock_period = 0;
			adc_value = ADCW;
			adc_display = adc_value;///1023;// * 5;
			//drawString(40, 1, prev_adc, 0, 1);
			fillRect(40, 1, 40, 8, 0);
			
			sprintf(convert_buffer, "%d", (adc_value-25));
			drawString(40, 1, convert_buffer, 0xff, 1);
			prev_adc = convert_buffer;
			
		}
		
		if(game_period > game_speed){
			Game_sm();
		}
		
		//Console_sm();
		usart_temp = 0;
		
		//drawChar(100, 1, PIND+'0', 0, 1);
		//drawChar(100, 1, usart_temp+'0', 0xff, 1);
		
		while(!TimerFlag){}
		TimerFlag = 0;
		timer+= periodMultiplier;
		clock_period+= periodMultiplier;
		game_period += periodMultiplier;
    }
}