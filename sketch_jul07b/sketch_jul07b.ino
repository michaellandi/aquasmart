#include <avr/pgmspace.h>

/* COMPILER CONSTANTS */
#define DATA_1 (PORTC |=  0x01)
#define DATA_0 (PORTC &=  0xfe)
#define STRIP_PINOUT (DDRC = 0xff)

/* FLASH MEMORY CONSTANTS */
PROGMEM const unsigned long COLOR_WHITE = 0xffffff;
PROGMEM const unsigned long COLOR_BLUE = 0x001700;
PROGMEM const unsigned long COLOR_RED = 0x0000ff;
PROGMEM const unsigned long COLOR_GREEN = 0xff0000;
PROGMEM const unsigned long COLOR_BLACK = 0x000000;
PROGMEM const unsigned long COLOR_PURPLE = 0x008040;

unsigned long _current_color;
unsigned long _timeout = 10;

void setup() {
  /* initialize light strip */
  STRIP_PINOUT; 
  reset_strip();
  
  /* initialize debug */
  Serial.begin(9600);
}

void loop() { 
  send_1M_color(COLOR_BLACK);
  
  while (1) {
    if (Serial.available() > 0) {
      delay(2);
      // read the incoming byte:
      char incomingByte = (char)Serial.read();
      if (incomingByte == NULL || incomingByte == ' ' || incomingByte == '\0') {
        continue;
      }
      
      unsigned long color;
      switch (incomingByte)
      {
         case '0':
          Serial.print("black");
          color = COLOR_BLACK;
          break;
        case '1':
          Serial.print("blue");
          color = COLOR_BLUE;
          break;
        case '2':
          Serial.print("white");
          color = COLOR_WHITE;
          break;
        case '3':
          Serial.print("purple");
          color = COLOR_PURPLE;
          break;
        case '4':
          Serial.print("red");
          color = COLOR_RED;
          break;
        case '5':
          Serial.print("green");
          color = COLOR_GREEN;
          break;
        default:
          continue;
      }
      
      send_1M_fade(color, _timeout);
      Serial.print("\n");
    }
  }
}

void send_1M_fade(unsigned long newColor, int timeout)
{
  unsigned long currentColor = _current_color;
  
  /* calculate the current RGB source and destination values */
  unsigned long src_red = currentColor & 0x0000FF;
  unsigned long src_green = (currentColor & 0xFF0000) >> 16;
  unsigned long src_blue = (currentColor & 0x00FF00) >> 8;
  unsigned long dst_red = newColor & 0x0000FF;
  unsigned long dst_green = (newColor & 0xFF0000) >> 16;
  unsigned long dst_blue = (newColor & 0x00FF00) >> 8;
  
  /* calculate the distances between source and destination for each RGB value */
  int tmp_red = src_red - dst_red;
  int tmp_green = src_green - dst_green;
  int tmp_blue = src_blue - dst_blue;
  unsigned int dist_red = abs(tmp_red);
  unsigned int dist_green = abs(tmp_green);
  unsigned int dist_blue = abs(tmp_blue);
  
  /* maximum distance for looping */
  unsigned long maximum = max(max(dist_red, dist_green), dist_blue);
  
  /* loop variables to mantain between interations */
  unsigned long red = src_red;
  unsigned long green = src_green;
  unsigned long blue = src_blue;

  /* begin fade loop */
  for (int i = 0; i < maximum; i++) {
    /* increment or decrement red */
    if (dist_red > 0 && i < dist_red) {
      if (src_red > dst_red) {
        red--;
      }
      else {
        red++;
      }
    }
    
    /* increment or decrement green */
    if (dist_green > 0 && i < dist_green) {
      if (src_green > dst_green) {
        green--;
      }
      else {
        green++;
      }
    }
    
    /* increment or decrement blue */
    if (dist_blue > 0 && i < dist_blue) {
      if (src_blue > dst_blue) {
        blue--;
      }
      else {
        blue++;
      }
    }
    
    /* calculate new color and send to device */
    unsigned long color = red + (blue << 8) + (green << 16);
    send_1M_color(color);
    
    /* wait the timeout period */
    delay(timeout);
  }
}

void send_1M_color(unsigned long color) {
   _current_color = color;
  
   noInterrupts();
   for (int i = 0; i < 10; i++) {
     send_strip(color);
   }
   interrupts();
}

/**************************** RADIO SHACK ****************************/
void send_1M_pattern(const unsigned long data[][10], int pattern_no, int frame_rate) {
  int i=0;
  int j=0;
  uint32_t temp_data;

  for (i=0;i<pattern_no;i++) {
    noInterrupts();
    for (j=0;j<10;j++) {
      temp_data = pgm_read_dword_near(&data[i][j]);
      send_strip(temp_data);
    }
    interrupts();

    delay(frame_rate);
  }
}

void send_strip(uint32_t data) {
  int i;
  unsigned long j=0x800000;
  
  for (i=0;i<24;i++) {
    if (data & j) {
      DATA_1;
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");    
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      
/*----------------------------*/
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");  
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");  
      __asm__("nop\n\t");  
      __asm__("nop\n\t");        
/*----------------------------*/      
      DATA_0;
    }
    else {
      DATA_1;
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");    
      DATA_0;
/*----------------------------*/      
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");      
/*----------------------------*/         
    }

    j>>=1;
  }
}

void reset_strip() {
  DATA_0;
  delayMicroseconds(20);
}
