#ifndef _ST7735_1284__
#define _ST7735_1284__

#include <util/delay.h>
#include <avr/pgmspace.h>
#include "SPI_1284.h"
#include "glcdfont.h"

#define RSPORT PORTB
#define rspin 0
#define RS 0			// RS/DC PIN
uint8_t width = 128;
uint8_t height = 160;

#define INITR_GREENTAB 0x0
#define INITR_REDTAB   0x1
#define INITR_BLACKTAB   0x2

#define ST7735_TFTWIDTH  128
#define ST7735_TFTHEIGHT 160

#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5

#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

#define ST7735_PWCTR6  0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

// Color definitions
#define	ST7735_BLACK   0x0000
#define	ST7735_BLUE    0x001F
#define	ST7735_RED     0xF800
#define	ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0
#define ST7735_WHITE   0xFFFF

#define	COLOR_BLACK   0x0000
#define	COLOR_BLUE    0x001F
#define	COLOR_RED     0xF800
#define	COLOR_GREEN   0x07E0
#define COLOR_CYAN    0x07FF
#define COLOR_MAGENTA 0xF81F
#define COLOR_YELLOW  0xFFE0
#define COLOR_WHITE   0xFFFF

void InitDisplay(){
	SoftwareReset();
	writecommand(ST7735_SLPOUT);
	_delay_ms(150);
	
	writecommand(ST7735_FRMCTR1);  // frame rate control - normal mode
	writedata(0x01);  // frame rate = fosc / (1 x 2 + 40) * (LINE + 2C + 2D)
	writedata(0x2C);
	writedata(0x2D);

	writecommand(ST7735_FRMCTR2);  // frame rate control - idle mode
	writedata(0x01);  // frame rate = fosc / (1 x 2 + 40) * (LINE + 2C + 2D)
	writedata(0x2C);
	writedata(0x2D);

	writecommand(ST7735_FRMCTR3);  // frame rate control - partial mode
	writedata(0x01); // dot inversion mode
	writedata(0x2C);
	writedata(0x2D);
	writedata(0x01); // line inversion mode
	writedata(0x2C);
	writedata(0x2D);
	
	writecommand(ST7735_INVCTR);  // display inversion control
	writedata(0x07);  // no inversion

	writecommand(ST7735_PWCTR1);  // power control
	writedata(0xA2);
	writedata(0x02);      // -4.6V
	writedata(0x84);      // AUTO mode

	writecommand(ST7735_PWCTR2);  // power control
	writedata(0xC5);      // VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD

	writecommand(ST7735_PWCTR3);  // power control
	writedata(0x0A);      // Opamp current small
	writedata(0x00);      // Boost frequency

	writecommand(ST7735_PWCTR4);  // power control
	writedata(0x8A);      // BCLK/2, Opamp current small & Medium low
	writedata(0x2A);

	writecommand(ST7735_PWCTR5);  // power control
	writedata(0x8A);
	writedata(0xEE);

	writecommand(ST7735_VMCTR1);  // power control
	writedata(0x0E);

	writecommand(ST7735_INVOFF);    // don't invert display
	
	writecommand(ST7735_COLMOD);
	writedata(0x05);
	
	
	writecommand(ST7735_MADCTL);  // memory access control (directions)
	writedata(0xC0);  // row address/col address, bottom to top refresh
	
	writecommand(ST7735_CASET);  // column addr set
	writedata(0x00);
	writedata(0x00);   // XSTART = 0
	writedata(0x00);
	writedata(0x7F);   // XEND = 127

	writecommand(ST7735_RASET);  // row addr set
	writedata(0x00);
	writedata(0x00);    // XSTART = 0
	writedata(0x00);
	writedata(0x9F);    // XEND = 159
	
	writecommand(ST7735_GMCTRP1);
	writedata(0x0f);
	writedata(0x1a);
	writedata(0x0f);
	writedata(0x18);
	writedata(0x2f);
	writedata(0x28);
	writedata(0x20);
	writedata(0x22);
	writedata(0x1f);
	writedata(0x1b);
	writedata(0x23);
	writedata(0x37);
	writedata(0x00);
	writedata(0x07);
	writedata(0x02);
	writedata(0x10);
	writecommand(ST7735_GMCTRN1);
	writedata(0x0f);
	writedata(0x1b);
	writedata(0x0f);
	writedata(0x17);
	writedata(0x33);
	writedata(0x2c);
	writedata(0x29);
	writedata(0x2e);
	writedata(0x30);
	writedata(0x30);
	writedata(0x39);
	writedata(0x3f);
	writedata(0x00);
	writedata(0x07);
	writedata(0x03);
	writedata(0x10);
	
	writecommand(ST7735_DISPON);
	_delay_ms(100);
		writecommand(ST7735_NORON);  // normal display on
		_delay_ms(10);
}


void InitDisplay1(){
	
	//writecommand(ST7735_SWRESET); // software reset
	//_delay_ms(150);
	
	SoftwareReset();
	writecommand(ST7735_SLPOUT);  // out of sleep mode
	_delay_ms(500);

	writecommand(ST7735_FRMCTR1);  // frame rate control - normal mode
	writedata(0x01);  // frame rate = fosc / (1 x 2 + 40) * (LINE + 2C + 2D)
	writedata(0x2C);
	writedata(0x2D);

	writecommand(ST7735_FRMCTR2);  // frame rate control - idle mode
	writedata(0x01);  // frame rate = fosc / (1 x 2 + 40) * (LINE + 2C + 2D)
	writedata(0x2C);
	writedata(0x2D);

	writecommand(ST7735_FRMCTR3);  // frame rate control - partial mode
	writedata(0x01); // dot inversion mode
	writedata(0x2C);
	writedata(0x2D);
	writedata(0x01); // line inversion mode
	writedata(0x2C);
	writedata(0x2D);
	
	writecommand(ST7735_INVCTR);  // display inversion control
	writedata(0x07);  // no inversion

	writecommand(ST7735_PWCTR1);  // power control
	writedata(0xA2);
	writedata(0x02);      // -4.6V
	writedata(0x84);      // AUTO mode

	writecommand(ST7735_PWCTR2);  // power control
	writedata(0xC5);      // VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD

	writecommand(ST7735_PWCTR3);  // power control
	writedata(0x0A);      // Opamp current small
	writedata(0x00);      // Boost frequency

	writecommand(ST7735_PWCTR4);  // power control
	writedata(0x8A);      // BCLK/2, Opamp current small & Medium low
	writedata(0x2A);

	writecommand(ST7735_PWCTR5);  // power control
	writedata(0x8A);
	writedata(0xEE);

	writecommand(ST7735_VMCTR1);  // power control
	writedata(0x0E);

	writecommand(ST7735_INVOFF);    // don't invert display

	writecommand(ST7735_MADCTL);  // memory access control (directions)
	//writedata(0xC8);  // row address/col address, bottom to top refresh
	writedata(0xC0);  // row address/col address, top to bottom refresh
	//madctl = 0xC8;
	
	writecommand(ST7735_COLMOD);  // set color mode
	writedata(0x05);        // 16-bit color

	writecommand(ST7735_CASET);  // column addr set
	writedata(0x00);
	writedata(0x00);   // XSTART = 0
	writedata(0x00);
	writedata(0x7F);   // XEND = 127

	writecommand(ST7735_RASET);  // row addr set
	writedata(0x00);
	writedata(0x00);    // XSTART = 0
	writedata(0x00);
	writedata(0x9F);    // XEND = 159

	writecommand(ST7735_GMCTRP1);
	writedata(0x0f);
	writedata(0x1a);
	writedata(0x0f);
	writedata(0x18);
	writedata(0x2f);
	writedata(0x28);
	writedata(0x20);
	writedata(0x22);
	writedata(0x1f);
	writedata(0x1b);
	writedata(0x23);
	writedata(0x37);
	writedata(0x00);
	writedata(0x07);
	writedata(0x02);
	writedata(0x10);
	writecommand(ST7735_GMCTRN1);
	writedata(0x0f);
	writedata(0x1b);
	writedata(0x0f);
	writedata(0x17);
	writedata(0x33);
	writedata(0x2c);
	writedata(0x29);
	writedata(0x2e);
	writedata(0x30);
	writedata(0x30);
	writedata(0x39);
	writedata(0x3f);
	writedata(0x00);
	writedata(0x07);
	writedata(0x03);
	writedata(0x10);
	
	writecommand(ST7735_DISPON);
	_delay_ms(100);

	writecommand(ST7735_NORON);  // normal display on
	_delay_ms(10);
}

void setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	writecommand(ST7735_CASET);  // column addr set
	writedata(0x00);
	writedata(x0+0);   // XSTART
	writedata(0x00);
	writedata(x1+0);   // XEND

	writecommand(ST7735_RASET);  // row addr set
	writedata(0x00);
	writedata(y0+0);    // YSTART
	writedata(0x00);
	writedata(y1+0);    // YEND

	writecommand(ST7735_RAMWR);  // write to RAM
}

  // Pass 8-bit (each) R,G,B, get back 16-bit packed color
  inline uint16_t Color565(uint8_t r, uint8_t g, uint8_t b) {
	  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
  }

void fillScreen(uint16_t color) {
	//width, height

	setAddrWindow(0, 0, width-1, height-1);

	// setup for data
	RSPORT |= (1 << RS);
	//*portOutputRegister(csport) &= ~ cspin;

	for (uint8_t x=0; x < width; x++) {
		for (uint8_t y=0; y < height; y++) {
			spiwrite(color >> 8);
			spiwrite(color);
		}
	}

	//*portOutputRegister(csport) |= cspin;
}

void drawPixel(int16_t x, int16_t y, uint16_t color) {
	if((x < 0) ||(x >= width) || (y < 0) || (y >= height)) return;

	setAddrWindow(x,y,x+1,y+1);
	
	//RSPORT |= (1 << RS);
	RSPORT |= (1 << RS);

	spiwrite(color >> 8);
	spiwrite(color);

}


void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
	// Rudimentary clipping
	if((x >= width) || (y >= height)) return;
	if((y+h-1) >= height) h = height-y;
	setAddrWindow(x, y, x, y+h-1);

	uint8_t hi = color >> 8, lo = color;
	RSPORT |= (1 << RS);

	while (h--) {
		spiwrite(hi);
		spiwrite(lo);
	}
	RSPORT |= (0 << RS);
}

void drawFastHLine(int16_t x, int16_t y, int16_t w,
                   uint16_t color) {

    // Rudimentary clipping
    if((x >= width) || (y >= height)) return;
    if((x+w-1) >= width)  w = width-x;
    setAddrWindow(x, y, x+w-1, y);

    uint8_t hi = color >> 8, lo = color;
    RSPORT |= (1 << RS);
    while (w--) {
        spiwrite(hi);
        spiwrite(lo);
    }
}

// fill a rectangle
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color) {
	// rudimentary clipping (drawChar w/big text requires this)
	if((x >= width) || (y >= height)) return;
	if((x + w - 1) >= width)  w = width  - x;
	if((y + h - 1) >= height) h = height - y;

	setAddrWindow(x, y, x+w-1, y+h-1);

	uint8_t hi = color >> 8, lo = color;
	RSPORT |= (1 << RS);

	for(y=h; y>0; y--) {
		for(x=w; x>0; x--) {
			spiwrite(hi);
			spiwrite(lo);
		}
	}
}
// draw a character
void drawChar(uint8_t x, uint8_t y, char c,
uint16_t color, uint8_t size) {
	for (uint8_t i =0; i<5; i++ ) {
		uint8_t line = pgm_read_byte(font+(c*5)+i);
		for (uint8_t j = 0; j<8; j++) {
			if (line & 0x1) {
				if (size == 1) // default size
				drawPixel(x+i, y+j, color);
				else {  // big size
					fillRect(x+i*size, y+j*size, size, size, color);
				}
			}
			line >>= 1;
		}
	}
}

void drawString(uint8_t x, uint8_t y, char *c,
uint16_t color, uint8_t size) {
	while (c[0] != 0) {
		drawChar(x, y, c[0], color, size);
		x += size*6;
		c++;
		if (x + 5 >= width) {
			y += 10;
			x = 0;
		}
	}
}


#endif