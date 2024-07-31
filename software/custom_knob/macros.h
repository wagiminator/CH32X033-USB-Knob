// ===================================================================================
// Macro Functions which associate Actions with Events (Customize your USB Knob here!)
// ===================================================================================
//
// The list of available USB HID functions can be found in src/usb_composite.h

// Rotary encoder example -> volume control knob
// ---------------------------------------------

// Define action(s) if encoder was rotated clockwise
static inline void ENC_CW_ACTION() {
  CON_press(CON_VOL_UP);                                  // press VOLUME UP key
}

// Define action(s) after encoder was rotated clockwise
static inline void ENC_CW_RELEASED() {
  CON_release();                                          // release VOLUME UP KEY
}

// Define action(s) if encoder was rotated counter-clockwise
static inline void ENC_CCW_ACTION() {
  CON_press(CON_VOL_DOWN);                                // press VOLUME DOWN key
}

// Define action(s) after encoder was rotated counter-clockwise
static inline void ENC_CCW_RELEASED() {
  CON_release();                                          // release VOLUME DOWN KEY
}

// Define action(s) if encoder switch was pressed
static inline void ENC_SW_PRESSED() {
  CON_press(CON_VOL_MUTE);                                // press VOLUME MUTE key
}

// Define action(s) if encoder switch was released
static inline void ENC_SW_RELEASED() {
  CON_release();                                          // release VOLUME MUTE key
}
