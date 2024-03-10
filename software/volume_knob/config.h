// ===================================================================================
// User configurations
// ===================================================================================

#pragma once

// Pin definitions
#define PIN_ENC_A           PA0       // pin connected to rotary encoder A
#define PIN_ENC_B           PA1       // pin connected to rotary encoder B
#define PIN_ENC_SW          PA4       // pin connected to rotary encoder switch
#define PIN_NEO             PA7       // pin connected to NeoPixels (do not change)

// NeoPixel configuration
#define NEO_GRB                       // type of pixel: NEO_GRB or NEO_RGB
#define NEO_COUNT           12        // total number of pixels in the string
#define NEO_BRIGHT          0         // brightness of pixels (0..2)

// Rotary encoder configuration
#define ENC_DETENTS         20        // number of detents per revolution

// MCU supply voltage
#define USB_VDD             1         // 0: 3.3V, 1: 5V

// USB device descriptor
#define USB_VENDOR_ID       0x1189    // VID
#define USB_PRODUCT_ID      0x8890    // PID
#define USB_DEVICE_VERSION  0x0100    // v1.0 (BCD-format)
#define USB_LANGUAGE        0x0409    // US English

// USB configuration descriptor
#define USB_MAX_POWER_mA    50        // max power in mA 

// USB descriptor strings
#define MANUF_STR           "wagiminator"
#define PROD_STR            "CH32X033-TinyKnob"
#define SERIAL_STR          "CH32X035HID"
#define INTERF_STR          "HID-Media"
