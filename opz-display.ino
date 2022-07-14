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

#include "opzDisplay.h"

// USB MIDI object
Adafruit_USBD_MIDI usb_midi;

MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

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

void setup(void)
{
  Wire.setSDA(22);
  Wire.setSCL(23);
  Wire.begin();
  u8g2.begin();

  display.set((char *)"Connect OPZ", (char *)"OP-Z", (char *)"Display");

#if defined(ARDUINO_ARCH_MBED) && defined(ARDUINO_ARCH_RP2040)
  // Manual begin() is required on core without built-in support for TinyUSB such as mbed rp2040
  TinyUSB_Device_Init(0);
#endif

  pinMode(LED_BUILTIN, OUTPUT);

  MIDI.begin(MIDI_CHANNEL_OMNI);

  Serial.begin(115200);

  Serial.println("OPZ Display");

  MIDI.setHandleControlChange(handleControlChange);
  MIDI.setHandleSystemExclusive(handleSysEx);

  // wait until device mounted
  while (!TinyUSBDevice.mounted())
    delay(1);

  midiInitSysExOpz();

  byte inArray[4] = {0x7E, 0x7F, 0x06, 0x01};
  MIDI.sendSysEx(4, &inArray[0]);
}

void loop(void)
{
  MIDI.read();
  draw();
}
