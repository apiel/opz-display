#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_72X40_ER_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE); // EastRising 0.42" OLED

#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

#define LOG(...) Serial.printf(__VA_ARGS__)

// USB MIDI object
Adafruit_USBD_MIDI usb_midi;

MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

// Must be before opzDisplay.h
#define MIDI_INTERFACE_H
void sendSysEx(uint8_t *inArray, uint16_t len)
{
    MIDI.sendSysEx(len, inArray, false);
}

// For the moment RP2040 is too slow to handle sysex without to impact OP-Z performance.
// Let's see if ESP32 could handle it better...
#define USE_SYSEX false

#include "opzDisplay.h"

unsigned long lastDraw = 0;
void draw()
{
  if (millis() - lastDraw > 100)
  {
    lastDraw = millis();
    u8g2.clearBuffer();
    u8g2.setFontRefHeightExtendedText();
    u8g2.setDrawColor(1);
    u8g2.setFontPosTop();
    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_5x8_tf);
    u8g2.drawStr(0, 0, display.line[0]);
    u8g2.setFont(u8g2_font_7x14_tf);
    u8g2.drawStr(0, 10, display.line[1]);
    u8g2.drawStr(0, 24, display.line[2]);
    u8g2.sendBuffer();
  }
}

unsigned long lastHeartBeat = 0;
void heartBeat()
{
  if (millis() - lastHeartBeat > 1000)
  {
    lastHeartBeat = millis();
    sendHeartBeat();
  }
}

void handleSysExIno(byte *array, unsigned size)
{
  // display.set(0, "sysex");
  // sprintf(display.line[1], "size: %d", size);
  // if (size > 1)
  // {
  //   sprintf(display.line[2], "%02X %02X", array[0], array[1]);
  // } else {
  //   sprintf(display.line[2], "%02X", array[0]);
  // }

  handleSysEx((uint8_t *)array + 1, (uint16_t)size - 2);
}

void setup(void)
{
  Wire.setSDA(22);
  Wire.setSCL(23);
  Wire.begin();
  u8g2.begin();

  display.set((char *)"Connect OPZ", (char *)"OP-Z v0.1", (char *)"Display");

#if defined(ARDUINO_ARCH_MBED) && defined(ARDUINO_ARCH_RP2040)
  // Manual begin() is required on core without built-in support for TinyUSB such as mbed rp2040
  TinyUSB_Device_Init(0);
#endif

  pinMode(LED_BUILTIN, OUTPUT);

  MIDI.begin(MIDI_CHANNEL_OMNI);

  Serial.begin(115200);

  Serial.println("OPZ Display");

  MIDI.setHandleControlChange(handleControlChange);

  // MIDI.setHandleSystemExclusive((void (*)(byte * array, unsigned size))handleSysEx);
  MIDI.setHandleSystemExclusive(handleSysExIno);

  // wait until device mounted
  while (!TinyUSBDevice.mounted())
    delay(1);

  midiInitSysExOpz();
}

void loop(void)
{
  MIDI.read();
  draw();
  heartBeat();
}
