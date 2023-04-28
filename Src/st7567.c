/*
* @Author: nhantt
* @Date:   2020-02-02
* @Last Modified by:   nhantt
* @Last Modified time: 2020-02-02
*/

#include "st7567.h"

u8g2_t u8g2;
static uint8_t *buf;
static uint32_t glcdTick;

static uint8_t u8x8_stm32_gpio_and_delay_cb(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr);
static uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

void ST7567_Init(void)
{
	//MX_SPI2_Init();
	//u8g2_Setup_st7920_s_128x64_f(&u8g2, U8G2_R0, u8x8_byte_4wire_hw_spi, u8x8_gpio_and_delay_stm32); // init u8g2 structure
	u8g2_Setup_st7567_pi_132x64_f(&u8g2, U8G2_R2, u8x8_byte_ks0108, u8x8_stm32_gpio_and_delay_cb);
  buf = (uint8_t *)malloc(u8g2_GetBufferSize(&u8g2)); // dynamically allocate a buffer of the required size
  u8g2_SetBufferPtr(&u8g2, buf);
  u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
  u8g2_SetContrast(&u8g2, 85);
  u8g2_SetPowerSave(&u8g2, 0); // wake up display
  u8g2_ClearBuffer(&u8g2);
  //sys_cfg.backlight?Set_Backlight(sys_cfg.brightness):Backlight_Off();
  glcdTick = HAL_GetTick();
}

/* Set percent backlight */
void Set_Backlight(uint8_t percent)
{
  if((percent < 0) || (percent > 100)) 
  return;
  //TIM1->CCR1 = (uint32_t)(percent*10);
}


/* Backlight off */
void Backlight_Off(void)
{
  Set_Backlight(10);
}

/* Backlight on */
void Backlight_On()
{
  Set_Backlight(99);
}

void ST7567_Refresh(void)
{
	if(HAL_GetTick() - glcdTick > 100)
	{
		u8g2_SendBuffer(&u8g2);
    glcdTick = HAL_GetTick();
	}
}

static uint8_t u8x8_stm32_gpio_and_delay_cb(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
  switch(msg)
  {
    case U8X8_MSG_GPIO_AND_DELAY_INIT: // called once during init phase of u8g2/u8x8 -> Make with HAL_Init
      // __NOP();
    GLCD_CLK();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|
                          GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GLCD_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GLCD_CTR_PORT, &GPIO_InitStruct);
      break;  // can be used to setup pins
    case U8X8_MSG_DELAY_NANO: // delay arg_int * 1 nano second
      __NOP();  
      break;
    case U8X8_MSG_DELAY_100NANO:  // delay arg_init * 100 nano seconds
      __NOP();
      __NOP();
      __NOP();
      break;
    case U8X8_MSG_DELAY_10MICRO:  // delay arg_init * 10 micro seconds
      for(uint16_t n = 0;  n < (320*arg_int); n++)
      {
        __NOP();
      }
      break;
    case U8X8_MSG_DELAY_MILLI:  //delay arg_int * 1 milli second
      HAL_Delay(arg_int);
      break;
    case U8X8_MSG_DELAY_I2C: //arg_int is the I2C speed in 100KHz, e.g 4 = 400 KHz
      // for(uint16_t n = 0; n < (160*arg_int); n++)
      // {
      //   __NOP();
      // }
      break;  // arg_int = 1: delay by 5us, arg_int = 4: delay by 1.25us
    case U8X8_MSG_GPIO_D0:  // D0 or SPI clock pin: Output level in arg_int
    // case U8X8_MSG_GPIO_SPI_CLOCK:
      arg_int?(GLCD_PORT->BSRR = (uint32_t)GLCD_DB0_PIN) : (GLCD_PORT->BSRR = (uint32_t)GLCD_DB0_PIN << 16u);
      break;
    case U8X8_MSG_GPIO_D1:  // D1 or SPI data pin: Output level in arg_int
    // case U8X8_MSG_GPIO_SPI_DATA:
      arg_int?(GLCD_PORT->BSRR = (uint32_t)GLCD_DB1_PIN) : (GLCD_PORT->BSRR = (uint32_t)GLCD_DB1_PIN << 16u);
      break;
    case U8X8_MSG_GPIO_D2:  // D2 pin: Output level in arg_int
      arg_int?(GLCD_PORT->BSRR = (uint32_t)GLCD_DB2_PIN) : (GLCD_PORT->BSRR = (uint32_t)GLCD_DB2_PIN << 16u);
      break;
    case U8X8_MSG_GPIO_D3:  // D3 pin: Output level in arg_int
      arg_int?(GLCD_PORT->BSRR = (uint32_t)GLCD_DB3_PIN) : (GLCD_PORT->BSRR = (uint32_t)GLCD_DB3_PIN << 16u);
      break;
    case U8X8_MSG_GPIO_D4:  // D4 pin: Output level in arg_int
      arg_int?(GLCD_PORT->BSRR = (uint32_t)GLCD_DB4_PIN) : (GLCD_PORT->BSRR = (uint32_t)GLCD_DB4_PIN << 16u);
      break;
    case U8X8_MSG_GPIO_D5:  // D5 pin: Output level in arg_int
      arg_int?(GLCD_PORT->BSRR = (uint32_t)GLCD_DB5_PIN) : (GLCD_PORT->BSRR = (uint32_t)GLCD_DB5_PIN << 16u);
      break;
    case U8X8_MSG_GPIO_D6:  // D6 pin: Output level in arg_int
      arg_int?(GLCD_PORT->BSRR = (uint32_t)GLCD_DB6_PIN) : (GLCD_PORT->BSRR = (uint32_t)GLCD_DB6_PIN << 16u);
      break;
    case U8X8_MSG_GPIO_D7:  // D7 pin: Output level in arg_int
      arg_int?(GLCD_PORT->BSRR = (uint32_t)GLCD_DB7_PIN) : (GLCD_PORT->BSRR = (uint32_t)GLCD_DB7_PIN << 16u);
      break; 
    case U8X8_MSG_GPIO_E:   // E/WR pin: Output levle in arg_int
      arg_int?(GLCD_PORT->BSRR = (uint32_t)GLCD_E_PIN) : (GLCD_PORT->BSRR = (uint32_t)GLCD_E_PIN << 16u);
      break;
    case U8X8_MSG_GPIO_CS:  // CS (chip select) pin: Output level in arg_init
      arg_int?(GLCD_CTR_PORT->BSRR = (uint32_t)GLCD_CS0_PIN) : (GLCD_CTR_PORT->BSRR = (uint32_t)GLCD_CS0_PIN << 16u);
      break;
    case U8X8_MSG_GPIO_DC:  // DC (data/cmd, A0, register select) pin: Output level in arg_int
        if(arg_int)
        {
          GLCD_CTR_PORT->BSRR = (uint32_t)GLCD_RS_PIN;
          GLCD_CTR_PORT->BSRR = (uint32_t)GLCD_RW_PIN << 16u;
        }
        else
        {
          GLCD_CTR_PORT->BSRR = (uint32_t)GLCD_RS_PIN << 16u;
          GLCD_CTR_PORT->BSRR = (uint32_t)GLCD_RW_PIN << 16u;
        }
      break;
    case U8X8_MSG_GPIO_RESET: // Reset pin: Output level in arg_int
      arg_int?(GLCD_CTR_PORT->BSRR = (uint32_t)GLCD_RST_PIN) : (GLCD_CTR_PORT->BSRR = (uint32_t)GLCD_RST_PIN << 16u);
      break;
    // case U8X8_MSG_GPIO_CS1: // CS1 (chip select) pin: Output level in arg_init
    //   arg_int?(GLCD_PORT->BSRR = (uint32_t)GLCD_CS1_PIN) : (GLCD_PORT->BSRR = (uint32_t)GLCD_CS1_PIN << 16u);
    //   break;
    // case U8X8_MSG_GPIO_CS2: // CS2 (chip select) pin: Output level in arg_init
    //   arg_int?(GLCD_PORT->BSRR = (uint32_t)GLCD_CS2_PIN) : (GLCD_PORT->BSRR = (uint32_t)GLCD_CS2_PIN << 16u);
    //   break;
    case U8X8_MSG_GPIO_I2C_CLOCK: // arg_int = 0: Output low at I2C clock pin
      break;                      // arg_int = 1: Input dir with pullup high for I2C clock pin
    case U8X8_MSG_GPIO_I2C_DATA:  // arg_int = 0: Output low at I2C data pin
      break;                      // arg)int = 1: Input dir with pullup high for I2C data pin
  }
  return 1;
}