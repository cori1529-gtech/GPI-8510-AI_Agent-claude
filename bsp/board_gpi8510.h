/*******************************************************************************
 * board_gpi8510.h - BSP (Board Support Package) pin map for GPI-8510
 *
 * Board    : GTECH-PMA-8510 V2.0  (Schematic: GPI-8510-ET-000-00 R0)
 * MCU      : STM32F103ZET6 (144pin, Flash 512KB, RAM 64KB)
 * Clock    : HSE 8MHz x PLL9 = 72MHz / LSE 32.768kHz
 * Library  : STM32 Standard Peripheral Library (NOT HAL)
 *
 * RULE : This file is the ONLY place where physical pin numbers appear.
 *        All .c files must reference these macros, never GPIO_Pin_x directly.
 *        For a new PCB, copy this file, rename, and edit pins here only.
 *
 * NOTE : Comments are in English on purpose (avoid CP949/UTF-8 mixing issues).
 * TODO : Items marked [TODO] must be verified against schematic / source code
 *        before use. Verified items are marked [OK: source].
 ******************************************************************************/

#ifndef __BOARD_GPI8510_H
#define __BOARD_GPI8510_H

#include "stm32f10x.h"

/*==============================================================================
 * 1. IAP / MEMORY MAP                                    [OK: hw_config.c]
 *============================================================================*/
#define BOARD_APP_VECTOR_OFFSET     0x3000              /* NVIC_SetVectorTable */
#define BOARD_BOOTLOADER_BASE       0x08000000          /* 12KB bootloader     */
#define BOARD_APP_BASE              0x08003000          /* application start   */
/* WARNING: never write flash below BOARD_APP_BASE (bootloader area)          */

/*==============================================================================
 * 2. UART                                                 [OK: usart.c]
 *============================================================================*/
/* USART1 : Debug / Junction board RS-232, 115200bps (MAX3232 IC10) */
#define UART_DEBUG                  USART1
#define UART_DEBUG_BAUD             115200
#define UART_DEBUG_TX_PORT          GPIOA
#define UART_DEBUG_TX_PIN           GPIO_Pin_9
#define UART_DEBUG_RX_PORT          GPIOA
#define UART_DEBUG_RX_PIN           GPIO_Pin_10

/* USART2 : PC program / RS-422/485 / BLE(BOT-CLE310), 9600bps (MAX489 IC13) */
#define UART_RS485                  USART2
#define UART_RS485_BAUD             9600
#define UART_RS485_TX_PORT          GPIOA
#define UART_RS485_TX_PIN           GPIO_Pin_2
#define UART_RS485_RX_PORT          GPIOA
#define UART_RS485_RX_PIN           GPIO_Pin_3
#define UART_RS485_DE_PORT          GPIOA               /* TX enable, normal LOW */
#define UART_RS485_DE_PIN           GPIO_Pin_4          /* see USART2TX_enable() */
#define UART_RS485_DE_HOLD_TICKS    6                   /* ~6ms auto-disable     */

/* USART3 : CDMA/LTE modem (AT command, SMS), 115200bps (MAX3232 IC12) */
#define UART_MODEM                  USART3
#define UART_MODEM_BAUD             115200
#define UART_MODEM_TX_PORT          GPIOB
#define UART_MODEM_TX_PIN           GPIO_Pin_10
#define UART_MODEM_RX_PORT          GPIOB
#define UART_MODEM_RX_PIN           GPIO_Pin_11
#define UART_MODEM_DE_PORT          GPIOB               /* init only, unused?  */
#define UART_MODEM_DE_PIN           GPIO_Pin_12

/* UART4 : Zigbee / Bluetooth (relay-switched), 115200bps */
#define UART_ZIGBEE                 UART4
#define UART_ZIGBEE_BAUD            115200
#define UART_ZIGBEE_TX_PORT         GPIOC
#define UART_ZIGBEE_TX_PIN          GPIO_Pin_10
#define UART_ZIGBEE_RX_PORT         GPIOC
#define UART_ZIGBEE_RX_PIN          GPIO_Pin_11

/* UART5 : RS-485 temperature sensor (MAX489 IC14)
   [TODO] hardware exists but not initialized in USARTx_Initial()             */

/*==============================================================================
 * 3. EEPROM (AT24C16, IC9)                                [OK: hw_config.c]
 *============================================================================*/
#define EEPROM_I2C                  I2C1
#define EEPROM_SCL_PORT             GPIOB
#define EEPROM_SCL_PIN              GPIO_Pin_6          /* AF open-drain       */
#define EEPROM_SDA_PORT             GPIOB
#define EEPROM_SDA_PIN              GPIO_Pin_7          /* AF open-drain       */
/* WARNING: call FSMC_DISABLE (def.h) before any EEPROM access                */

/*==============================================================================
 * 4. SPI FLASH (W25Q128, IC8) - Korean font + IAP image    [OK: sflash.c]
 *============================================================================*/
/* CORRECTION: earlier version of this file assumed SPI1/PA5-7/PA4 (NSS),
 * flagging a conflict with the RS485 DE pin. That was wrong - sflash.c has
 * TWO SPI_FLASH_Init() definitions: the SPI1/PA5-7/PA4 one is wrapped in
 * "#ifdef gcr1000" (a different, unrelated product - the macro is not
 * defined anywhere in this project, so that block never compiles). The
 * REAL, active init (unguarded, and the only one SPI_FLASH_CS_LOW/HIGH()
 * actually agree with) uses SPI2 on GPIOB: PB13=SCK, PB14=MISO, PB15=MOSI,
 * PB12=CS. No conflict with RS485 DE (PA4) after all.                       */
#define SFLASH_SPI                  SPI2
#define SFLASH_SCK_PORT             GPIOB
#define SFLASH_SCK_PIN              GPIO_Pin_13
#define SFLASH_MISO_PORT            GPIOB
#define SFLASH_MISO_PIN             GPIO_Pin_14
#define SFLASH_MOSI_PORT            GPIOB
#define SFLASH_MOSI_PIN             GPIO_Pin_15
#define SFLASH_NSS_PORT             GPIOB
#define SFLASH_NSS_PIN              GPIO_Pin_12

/*==============================================================================
 * 5. RTC (HT1381, IC7) - 3-wire                    [OK: main.c, see NOTE]
 *============================================================================*/
/* Signal names from schematic: CLOCK_SC, CLOCK_IO, CLOCK_RE                  */
/* NOTE: rtc.c does NOT drive the HT1381. It wraps the STM32 *internal* RTC   */
/* peripheral (RTC_Configuration()/RTC_SetCounter()/RTC_IRQHandler()), which  */
/* appears to be a separate, parallel time source not the field-used clock.  */
/* The real HT1381 3-wire bit-bang driver (WriteByte/ReadByte,               */
/* W8bit_HT1381/R8bit_HT1381) lives in main.c (~line 633, repeated ~4208),   */
/* using the macros below. Pin modes (Output PP) are implied by the          */
/* GPIO_SetBits/ResetBits macros but no explicit GPIO_Init() for PB5/PB9 was */
/* found in adc.c/lcd.c/rtc.c/key.c - only PB8 toggles mode via Rtc_Write()  */
/* (Out_PP) / Rtc_Read() (IPU) in key.c, since it is the shared I/O data pin.*/
#define RTC_SCLK_PORT               GPIOB               /* [OK: main.c RTC_CLOCK_HIGH/LOW] */
#define RTC_SCLK_PIN                GPIO_Pin_5
#define RTC_IO_PORT                 GPIOB               /* [OK: main.c RTC_DATA_HIGH/LOW, key.c Rtc_Write/Read */
#define RTC_IO_PIN                  GPIO_Pin_8
#define RTC_RST_PORT                GPIOB               /* [OK: main.c RTC_RESET_HIGH/LOW] */
#define RTC_RST_PIN                 GPIO_Pin_9

/*==============================================================================
 * 6. LOADCELL ADC                                  [OK: adc.c + key.c]
 *============================================================================*/
/* LC1 : CS5555-SS (U1) - bit-banged serial    [OK: adc.c ADC_SLCK_HIGH/LOW_1,
 *                                               key.c bsp_key_gpio_init LC1 block] */
#define LC1_SCLK_PORT               GPIOE               /* Out_OD */
#define LC1_SCLK_PIN                GPIO_Pin_14
#define LC1_DOUT_PORT               GPIOE               /* IN_FLOATING, also DRDY */
#define LC1_DOUT_PIN                GPIO_Pin_13
/* [TODO] no PDWN (power-down) pin control was found anywhere in adc.c or
 * main.c for CS5555 U1 - it may be permanently tied on the PCB. Verify
 * against the schematic before assuming a software-controlled PDWN exists. */
#define LC1_PDWN_PORT               GPIOx
#define LC1_PDWN_PIN                GPIO_Pin_x

/* LC2 : CS5555-SS (U2) - bit-banged serial    [OK: adc.c ADC_SLCK_HIGH/LOW_2,
 *                                               key.c bsp_key_gpio_init LC2 block] */
#define LC2_SCLK_PORT               GPIOD               /* Out_OD */
#define LC2_SCLK_PIN                GPIO_Pin_9
#define LC2_DOUT_PORT               GPIOD               /* IN_FLOATING, also DRDY */
#define LC2_DOUT_PIN                GPIO_Pin_8
/* [TODO] same as LC1_PDWN - no PDWN control found in source.               */
#define LC2_PDWN_PORT               GPIOx
#define LC2_PDWN_PIN                GPIO_Pin_x

/* LC3 : ADS1251U (IC15), silo 1 - bit-banged serial   [OK: adc.c RDDATA()/adc_value()] */
/* NOTE: only 2 signals are actually driven (SCLK + DOUT/DRDY). The stale
 * comment in adc.c ("PC1: CLK  PC2: DOUT  PC3: SCLK") does not match the
 * code and a separate CLK pin is not toggled anywhere - dropped from this
 * header. DOUT doubles as the /DRDY (data-ready) line, polled before clocking. */
#define LC3_SCLK_PORT               GPIOC
#define LC3_SCLK_PIN                GPIO_Pin_0
#define LC3_DOUT_PORT               GPIOC               /* also /DRDY */
#define LC3_DOUT_PIN                GPIO_Pin_2

/* LC3_2 : second ADS1251, silo 2 - bit-banged serial  [OK: adc.c RDDATA_1()/adc_value_1()] */
/* NOTE: not documented in CLAUDE.md 3.2 (which lists only one ADS1251, IC15),
 * but a fully separate read path exists in adc.c for a second channel -
 * consistent with the dual-silo (_2 suffix) convention used elsewhere. */
#define LC3_2_SCLK_PORT             GPIOB
#define LC3_2_SCLK_PIN              GPIO_Pin_1
#define LC3_2_DOUT_PORT             GPIOC               /* also /DRDY */
#define LC3_2_DOUT_PIN              GPIO_Pin_13

/*==============================================================================
 * 7. LCD (128x64 dual-panel GLCD, KS0108/HD61202-style)   [OK: lcd.c]
 *============================================================================*/
/* IMPORTANT - this does NOT match the OLED_RD/WR/DC/CS bus this template
 * originally assumed. The panel is actually driven with an E / R-W / DI(RS) /
 * CS1 / CS2 protocol (classic dual-chip 128x64 GLCD, left/right half-screen
 * page select), confirmed from the ONLY functions actually called from
 * lcd_initial() and every draw routine: lcd_command() / lcd_data_write().
 * Their bit-shuffle (cmd<<1 & 0x0180 | cmd & 0x003f, OR'd with CS1/CS2/E/RW/DI)
 * decodes to:
 *   D0..D5 -> PG0..PG5, D6 -> PG7, D7 -> PG8   (PG6 is intentionally skipped)
 *   PG11 = CS1, PG12 = CS2, PG13 = E, PG14 = R/W, PG15 = DI (RS)
 * A second function, lcdu8() (PG9=E/PG10=RW/PG11=RS, contiguous PG0..PG7
 * data), exists in lcd.c but is never called anywhere - dead code, ignore it.
 * All of this matches the 13-pin GPIOG output block in key.c
 * bsp_key_gpio_init() ("128 * 64 LCD" comment): pins 0,1,2,3,4,5,7,8,11,12,13,14,15. */
#define LCD_DATA_PORT               GPIOG
#define LCD_D0_PIN                  GPIO_Pin_0
#define LCD_D1_PIN                  GPIO_Pin_1
#define LCD_D2_PIN                  GPIO_Pin_2
#define LCD_D3_PIN                  GPIO_Pin_3
#define LCD_D4_PIN                  GPIO_Pin_4
#define LCD_D5_PIN                  GPIO_Pin_5
#define LCD_D6_PIN                  GPIO_Pin_7          /* NOTE: not Pin_6   */
#define LCD_D7_PIN                  GPIO_Pin_8

#define LCD_CS1_PORT                GPIOG
#define LCD_CS1_PIN                 GPIO_Pin_11         /* left half-screen  */
#define LCD_CS2_PORT                GPIOG
#define LCD_CS2_PIN                 GPIO_Pin_12         /* right half-screen */
#define LCD_E_PORT                  GPIOG
#define LCD_E_PIN                   GPIO_Pin_13         /* enable/strobe     */
#define LCD_RW_PORT                 GPIOG
#define LCD_RW_PIN                  GPIO_Pin_14
#define LCD_DI_PORT                 GPIOG
#define LCD_DI_PIN                  GPIO_Pin_15         /* RS: data/instruction */

#define LCD_RESET_PORT              GPIOA               /* [OK: lcd.c LCD_RES macro, lcd_initial() */
#define LCD_RESET_PIN               GPIO_Pin_8

/*==============================================================================
 * 8. KEY / BUZZER / RELAY OUT / ISOLATED IN         [OK: key.c bsp_key_gpio_init]
 *============================================================================*/
/* Membrane keys KEY5..KEY9 - GPIOE, IPU (internal pull-up)                   */
/* NOTE: CHKEYIN() in key.c actually reads GPIO_ReadInputData(GPIOE) & 0x01F0,
 * i.e. PE4..PE8 - one bit lower than the PE5..PE9 configured here. This
 * off-by-one exists in the current firmware as-is; flagged, not "fixed". */
#define KEY5_PORT                   GPIOE
#define KEY5_PIN                    GPIO_Pin_5
#define KEY6_PORT                   GPIOE
#define KEY6_PIN                    GPIO_Pin_6
#define KEY7_PORT                   GPIOE
#define KEY7_PIN                    GPIO_Pin_7
#define KEY8_PORT                   GPIOE
#define KEY8_PIN                    GPIO_Pin_8
#define KEY9_PORT                   GPIOE
#define KEY9_PIN                    GPIO_Pin_9

#define BUZZER_PORT                 GPIOC               /* Out_PP */
#define BUZZER_PIN                  GPIO_Pin_12

/* Relay outputs OUTPUT1..4 - GPIOD Pin11/12/13 + GPIOB Pin4, all Out_PP      */
/* [TODO] key.c configures these 4 pins as generic outputs but does not label
 * which physical pin is "OUTPUT1" vs "OUTPUT4" - that numbering was not
 * found in adc.c/lcd.c/rtc.c/key.c (main.c's relay helpers drive a separate
 * silo-select relay pair, not this 4-channel bank). Order below is
 * assignment order only - verify against the schematic before relying on it. */
#define OUT1_PORT                   GPIOD
#define OUT1_PIN                    GPIO_Pin_11
#define OUT2_PORT                   GPIOD
#define OUT2_PIN                    GPIO_Pin_12
#define OUT3_PORT                   GPIOD
#define OUT3_PIN                    GPIO_Pin_13
#define OUT4_PORT                   GPIOB
#define OUT4_PIN                    GPIO_Pin_4

/* Isolated inputs INPUT1..4 - GPIOC Pin7/8/9 + GPIOB Pin3, all IN_FLOATING   */
/* [TODO] same caveat as OUT1..4 - exact channel numbering unconfirmed.      */
#define IN1_PORT                    GPIOC
#define IN1_PIN                     GPIO_Pin_7
#define IN2_PORT                    GPIOC
#define IN2_PIN                     GPIO_Pin_8
#define IN3_PORT                    GPIOC
#define IN3_PIN                     GPIO_Pin_9
#define IN4_PORT                    GPIOB
#define IN4_PIN                     GPIO_Pin_3

/*==============================================================================
 * 9. MODEM CONTROL (CDMA/LTE)                  [OK: buzzer.c, key.c, main.c]
 *============================================================================*/
/* CORRECTION: there is only ONE modem-related GPIO pin in the whole firmware.
 * Cdma_Power_On()/Cdma_Power_Off() (buzzer.c) and the equivalently-named
 * Relay_4_On()/Relay_4_Off() (buzzer.c, same pin, legacy alt name) both
 * drive GPIOD Pin_2 - matching key.c's "CDMA POWER ON/OFF" init block.
 * main.c's modem_power_reset() does NOT use a separate reset pin: it sends
 * the AT command "AT$LGTRESET", then power-cycles via Cdma_Power_Off() +
 * Cdma_Power_On() (same MODEM_PWR pin). No MODEM_RST / MODEM_ONOFF GPIO
 * exists in source - those fields are removed rather than left as guesses. */
#define MODEM_PWR_PORT              GPIOD
#define MODEM_PWR_PIN               GPIO_Pin_2

/*==============================================================================
 * 10. MISC
 *============================================================================*/
/* [PARTIAL] key.c configures GPIOD Pin0 as IN_FLOATING, commented "POWER CHECK INPUT" (power check input, romanized) - plausibly this BAT_CHECK signal, but it is
 * a plain digital IN_FLOATING pin, not GPIO_Mode_AIN (analog). CLAUDE.md 3.1
 * describes BAT_CHECK as monitoring the 7.4V battery, which normally implies
 * an ADC channel. Verify against the schematic whether this is really the
 * analog battery-voltage input or a separate digital "power good" signal. */
#define BAT_CHECK_PORT              GPIOD
#define BAT_CHECK_PIN               GPIO_Pin_0

/* Option switches OPTION_D0..D6 : [TODO] fill when needed                    */

#endif /* __BOARD_GPI8510_H */

/* END OF FILE */
