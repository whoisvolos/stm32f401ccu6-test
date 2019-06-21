#include <stm32f4xx_ll_utils.h>
#include <stm32f4xx_ll_i2c.h>
#include "ssd1306_i2c/ssd1306.h"

#define I2C_REQUEST_WRITE	0x00
#define I2C_REQUEST_READ	0x01

static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

static SSD1306_t SSD1306;

static void __i2c_send_byte(I2C_TypeDef *i2c, uint8_t c, uint8_t addr) {
  LL_I2C_DisableBitPOS(i2c);
  LL_I2C_AcknowledgeNextData(i2c, LL_I2C_ACK);
  LL_I2C_GenerateStartCondition(i2c);
  while (!LL_I2C_IsActiveFlag_SB(i2c)) { }
  (void) i2c->SR1;
  LL_I2C_TransmitData8(i2c, addr | I2C_REQUEST_WRITE);
  while(!LL_I2C_IsActiveFlag_ADDR(i2c)) { }
  LL_I2C_ClearFlag_ADDR(i2c);
  LL_I2C_TransmitData8(i2c, c);
  while(!LL_I2C_IsActiveFlag_TXE(i2c)) { }
  LL_I2C_GenerateStopCondition(i2c);
}

inline static void ssd1306_write_command(uint8_t command) {
  //__i2c_send_byte(SSD1306_I2C_PORT, command, SSD1306_I2C_ADDR);
  LL_I2C_HandleTransfer();
}

//
//	Initialize OLED screen
//
uint8_t ssd1306_Init() {
  // Wait for the screen to boot
  LL_mDelay(100);

  /* Init LCD */
  ssd1306_write_command(0xAE); //display off
  ssd1306_write_command(0x20); //Set Memory Addressing Mode
  ssd1306_write_command(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
  ssd1306_write_command(0xB0); //Set Page Start Address for Page Addressing Mode,0-7
  ssd1306_write_command(0xC8); //Set COM Output Scan Direction
  ssd1306_write_command(0x00); //---set low column address
  ssd1306_write_command(0x10); //---set high column address
  ssd1306_write_command(0x40); //--set start line address
  ssd1306_write_command(0x81); //--set contrast control register
  ssd1306_write_command(0xFF);
  ssd1306_write_command(0xA1); //--set segment re-map 0 to 127
  ssd1306_write_command(0xA6); //--set normal display
  ssd1306_write_command(0xA8); //--set multiplex ratio(1 to 64)
  ssd1306_write_command(0x3F); //
  ssd1306_write_command(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
  ssd1306_write_command(0xD3); //-set display offset
  ssd1306_write_command(0x00); //-not offset
  ssd1306_write_command(0xD5); //--set display clock divide ratio/oscillator frequency
  ssd1306_write_command(0xF0); //--set divide ratio
  ssd1306_write_command(0xD9); //--set pre-charge period
  ssd1306_write_command(0x22); //
  ssd1306_write_command(0xDA); //--set com pins hardware configuration
  ssd1306_write_command(0x12);
  ssd1306_write_command(0xDB); //--set vcomh
  ssd1306_write_command(0x20); //0x20,0.77xVcc
  ssd1306_write_command(0x8D); //--set DC-DC enable
  ssd1306_write_command(0x14); //
  ssd1306_write_command(0xAF); //--turn on SSD1306 panel

  // Clear screen
  ssd1306_fill(Black);

  // Flush buffer to screen
  ssd1306_update_screen();

  // Set default values for screen object
  SSD1306.CurrentX = 0;
  SSD1306.CurrentY = 0;

  SSD1306.Initialized = 1;

  return 1;
}

//
//  Fill the whole screen with the given color
//
void ssd1306_fill(SSD1306_COLOR color) {
  uint8_t fill = (color == Black) ? 0x00 : 0xFF;
  for (uint32_t i = 0; i < sizeof(SSD1306_Buffer); ++i) {
    SSD1306_Buffer[i] = fill;
  }

  LL_I2C_
}