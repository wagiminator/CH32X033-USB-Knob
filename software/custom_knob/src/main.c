// ===================================================================================
// Project:   USB Rotary Encoder for CH32X033 - Custom Control
// Version:   v1.0
// Year:      2024
// Author:    Stefan Wagner
// Github:    https://github.com/wagiminator
// EasyEDA:   https://easyeda.com/wagiminator
// License:   http://creativecommons.org/licenses/by-sa/3.0/
// ===================================================================================
//
// Description:
// ------------
// Customize your USB Rotary Encoder Knob.
//
// References:
// -----------
// - WCH Nanjing Qinheng Microelectronics: http://wch.cn
//
// Compilation Instructions:
// -------------------------
// - Make sure GCC toolchain (gcc-riscv64-unknown-elf, newlib) and Python3 with chprog
//   are installed. In addition, Linux requires access rights to the USB bootloader.
// - Press the BOOT button on the MCU board and keep it pressed while connecting it
//   via USB to your PC.
// - Run 'make flash'.
//
// Operating Instructions:
// -----------------------
// - Costomize your rotary encoder knob in the macros.h file.
// - Connect the board via USB to your PC. It should be detected as a HID composite
//   device with keyboard, mouse and game controller interface.
// - Turn the rotary encoder or press the encoder switch.
// - To enter bootloader hold down the rotary encoder switch while connecting the 
//   device to USB. The NeoPixels will light up white as long as the device is in 
//   bootloader mode (about 10 seconds).

// ===================================================================================
// Libraries, Definitions and Macros
// ===================================================================================
#include <config.h>                               // user configurations
#include <system.h>                               // system functions
#include <gpio.h>                                 // GPIO functions
#include <neo_spi.h>                              // NeoPixel functions
#include <usb_composite.h>                        // USB HID composite functions
#include <macros.h>                               // user defined macros

// ===================================================================================
// NeoPixels Ring Hue Cycle Function
// ===================================================================================
uint8_t hue = 0;                                          // hue cycle value

// Write NeoPixels hue ring with current value
void NEO_set(void) {
  uint8_t i, j;
  j = hue;
  for(i=0; i<NEO_COUNT; i++) {
    NEO_writeHue(i, j, 0);
    j += 192 / NEO_COUNT;
    if(j >= 192) j -= 192;
  }
  NEO_update();
}

// Turn NeoPixels hue ring counter-clockwise
void NEO_turn_CCW(void) {
  hue += 192 / 20;
  if(hue >= 192) hue -= 192;
  NEO_set();
}

// Turn NeoPixels hue ring clockwise
void NEO_turn_CW(void) {
  hue -= 192 / 20;
  if(hue >= 192) hue -= 64;
  NEO_set();
}

// ===================================================================================
// Main Function
// ===================================================================================
int main(void) {
  // Variables
  uint8_t isSwitchPressed = 0;                            // state of rotary encoder switch

  // Setup
  PIN_input_PU(PIN_ENC_A);                                // set encoder pins to input pullup
  PIN_input_PU(PIN_ENC_B);
  PIN_input_PU(PIN_ENC_SW);
  NEO_init();                                             // init NeoPixels

  // Enter bootloader if encoder switch is pressed
  if(!PIN_read(PIN_ENC_SW)) {                             // encoder switch pressed?
    for(uint8_t i=33; i; i--) NEO_sendByte(63);           // light up all pixels
    BOOT_now();                                           // enter bootloader
  }

  // Init USB mouse
  HID_init();                                             // init USB HID composite device
  NEO_set();                                              // set initial NeoPixels hue ring

  // Loop
  while(1) {
    if(!PIN_read(PIN_ENC_A)) {                            // encoder turned ?
      if(PIN_read(PIN_ENC_B)) {                           // clockwise?
        ENC_CW_ACTION();                                  // -> take proper action
        NEO_turn_CW();                                    // -> turn hue ring clockwise
        DLY_ms(5);                                        // -> debounce
        ENC_CW_RELEASED();                                // -> quit action
      }
      else {                                              // counter-clockwise?
        ENC_CCW_ACTION();                                 // -> take proper action
        NEO_turn_CCW();                                   // -> turn hue ring counter-clockwise
        DLY_ms(5);                                        // -> debounce
        ENC_CCW_RELEASED();                               // -> quit action
      }
      while(!PIN_read(PIN_ENC_A));                        // wait until next detent
    } 
    else {
      if(!isSwitchPressed && !PIN_read(PIN_ENC_SW)) {     // switch previously pressed?
        ENC_SW_PRESSED();                                 // -> take proper action
        isSwitchPressed = 1;                              // -> update switch state
      }
      else if(isSwitchPressed && PIN_read(PIN_ENC_SW)) {  // switch previously released?
        ENC_SW_RELEASED();                                // -> take proper action
        isSwitchPressed = 0;                              // -> update switch state
      }
    }
    DLY_ms(1);                                            // slow down a little
  }
}
