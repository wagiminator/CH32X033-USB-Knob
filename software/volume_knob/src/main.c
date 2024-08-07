// ===================================================================================
// Project:   USB Rotary Encoder for CH32X033 - Volume Control
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
// Controls PC volume via rotary encoder.
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
// - Connect the board via USB to your PC. It should be detected as a HID device.
// - Turn the rotary encoder to increase/decrease volume.
// - Press rotary encoder button to mute/unmute.
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
#include <usb_consumer.h>                         // USB consumer multimedia keyboard

// ===================================================================================
// NeoPixels Ring Hue Cycle Function
// ===================================================================================
uint8_t hue = 0;                                          // hue cycle value

// Write NeoPixels hue ring with current value
void NEO_set(void) {
  uint8_t i, j;
  j = hue;
  for(i=0; i<NEO_COUNT; i++) {
    NEO_writeHue(i, j, NEO_BRIGHT);
    j += 192 / NEO_COUNT;
    if(j >= 192) j -= 192;
  }
  NEO_update();
}

// Turn NeoPixels hue ring counter-clockwise
void NEO_turn_CCW(void) {
  hue += 192 / ENC_DETENTS;
  if(hue >= 192) hue -= 192;
  NEO_set();
}

// Turn NeoPixels hue ring clockwise
void NEO_turn_CW(void) {
  hue -= 192 / ENC_DETENTS;
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

  // Init USB keyboard
  CON_init();                                             // init USB HID consumer keyboard
  NEO_set();                                              // set initial NeoPixels hue ring

  // Loop
  while(1) {
    if(!PIN_read(PIN_ENC_A)) {                            // encoder turned ?
      if(PIN_read(PIN_ENC_B)) {                           // clockwise?
        CON_press(CON_VOL_UP);                            // -> press volume up
        NEO_turn_CW();                                    // -> turn hue ring clockwise
      }
      else {                                              // counter-clockwise?
        CON_press(CON_VOL_DOWN);                          // -> press volume down
        NEO_turn_CCW();                                   // -> turn hue ring counter-clockwise
      }
      DLY_ms(10);                                         // debounce
      CON_release();                                      // release key
      while(!PIN_read(PIN_ENC_A));                        // wait until next detent
    } 
    else {
      if(!isSwitchPressed && !PIN_read(PIN_ENC_SW)) {     // switch previously pressed?
        CON_press(CON_VOL_MUTE);                          // press volume mute key
        isSwitchPressed = 1;
      }
      else if(isSwitchPressed && PIN_read(PIN_ENC_SW)) {  // switch previously released?
        CON_release();                                    // release volume mute key
        isSwitchPressed = 0;                              // update switch state
      }
    }
    DLY_ms(1);                                            // slow down a little
  }
}
