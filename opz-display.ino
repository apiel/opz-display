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

// USB MIDI object
Adafruit_USBD_MIDI usb_midi;

MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

#define RENDER_SIZE 100
char render[3][RENDER_SIZE] = {"", "", ""};

unsigned long lastDraw = 0;
void draw()
{
  if (millis() - lastDraw > 100)
  {
    lastDraw = millis();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_7x14_tf);
    u8g2.setFontRefHeightExtendedText();
    u8g2.setDrawColor(1);
    u8g2.setFontPosTop();
    u8g2.setFontDirection(0);
    u8g2.drawStr(0, 0, render[0]);
    u8g2.drawStr(0, 14, render[1]);
    u8g2.drawStr(0, 28, render[2]);
    u8g2.sendBuffer();
  }
}

void setup(void)
{
  Wire.setSDA(22);
  Wire.setSCL(23);
  Wire.begin();
  u8g2.begin();

  strcpy(render[0], "OP-Z screen");
  strcpy(render[1], "v0.0.1");

#if defined(ARDUINO_ARCH_MBED) && defined(ARDUINO_ARCH_RP2040)
  // Manual begin() is required on core without built-in support for TinyUSB such as mbed rp2040
  TinyUSB_Device_Init(0);
#endif

  pinMode(LED_BUILTIN, OUTPUT);

  MIDI.begin(MIDI_CHANNEL_OMNI);

  MIDI.setHandleControlChange(handleControlChange);

  Serial.begin(115200);

  // wait until device mounted
  while (!TinyUSBDevice.mounted())
    delay(1);
}

void loop(void)
{
  MIDI.read();
  draw();
}

void handleControlChange(byte channel, byte cc, byte value)
{
  switch (channel)
  {
  case 1:
    strcpy(render[0], "Kick");
    break;

  case 2:
    strcpy(render[0], "Snares");
    break;

  case 3:
    strcpy(render[0], "Perc");
    break;

  case 4:
    strcpy(render[0], "Sample");
    break;

  case 5:
    strcpy(render[0], "Bass");
    break;

  case 6:
    strcpy(render[0], "Lead");
    break;

  case 7:
    strcpy(render[0], "ARP");
    break;

  case 8:
    strcpy(render[0], "Chord");
    break;

  default:
    break;
  }

  switch (cc)
  {
  case 1:
    strcpy(render[1], "P1");
    break;

  case 2:
    strcpy(render[1], "P2");
    break;

  case 3:
    strcpy(render[1], "Cutoff");
    break;

  case 4:
    strcpy(render[1], "Res");
    break;

  case 5:
    strcpy(render[1], "Attack");
    break;

  case 6:
    strcpy(render[1], "Decay");
    break;

  case 7:
    strcpy(render[1], "Substain");
    break;

  case 8:
    strcpy(render[1], "Release");
    break;

  case 9:
    strcpy(render[1], channel == 7 ? "ARP speed" : "LFO Depth");
    break;

  case 10:
    strcpy(render[1], channel == 7 ? "ARP Pattern" : "LFO Rate");
    break;

  case 11:
    strcpy(render[1], channel == 7 ? "ARP Style" : "LFO Dest");
    break;

  case 12:
    strcpy(render[1], channel == 7 ? "ARP Range" : "LFO Shape");
    break;

  case 13:
    strcpy(render[1], "FX Send 1");
    break;

  case 14:
    strcpy(render[1], "FX Send 2");
    break;

  case 15:
    strcpy(render[1], "Pan");
    break;

  case 16:
    strcpy(render[1], "Level");
    break;

  default:
    break;
  }

  if (cc == 12 && channel != 7)
  {
    if (value < 10)
    {
      strcpy(render[2], "SIN");
    }
    else if (value < 21)
    {
      strcpy(render[2], "TRI");
    }
    else if (value < 32)
    {
      strcpy(render[2], "SSQR");
    }
    else if (value < 43)
    {
      strcpy(render[2], "SAW");
    }
    else if (value < 54)
    {
      strcpy(render[2], "RND");
    }
    else if (value < 64)
    {
      strcpy(render[2], "GYRO");
    }
    else if (value < 75)
    {
      strcpy(render[2], "SIN triggered");
    }
    else if (value < 85)
    {
      strcpy(render[2], "TRI triggered");
    }
    else if (value < 96)
    {
      strcpy(render[2], "SSQR triggered");
    }
    else if (value < 107)
    {
      strcpy(render[2], "SAW triggered");
    }
    else if (value < 118)
    {
      strcpy(render[2], "RND triggered");
    }
    else
    {
      strcpy(render[2], "ONCE triggered");
    }
  }
  else if (cc == 11 && channel != 7)
  {
    if (value < 16)
    {
      strcpy(render[2], "P1");
    }
    else if (value < 32)
    {
      strcpy(render[2], "P2");
    }
    else if (value < 48)
    {
      strcpy(render[2], "CUTOFF");
    }
    else if (value < 64)
    {
      strcpy(render[2], "RES");
    }
    else if (value < 80)
    {
      strcpy(render[2], "ATTACK");
    }
    else if (value < 96)
    {
      strcpy(render[2], "PITCH");
    }
    else if (value < 112)
    {
      strcpy(render[2], "PAN");
    }
    else
    {
      strcpy(render[2], "VOLUME");
    }
  }
  else if (cc == 10 && channel != 7)
  {
    if (value < 8)
    {
      strcpy(render[2], "1/64");
    }
    else if (value < 16)
    {
      strcpy(render[2], "1/32");
    }
    else if (value < 24)
    {
      strcpy(render[2], "1/16");
    }
    else if (value < 32)
    {
      strcpy(render[2], "1/8");
    }
    else if (value < 40)
    {
      strcpy(render[2], "1/4");
    }
    else if (value < 48)
    {
      strcpy(render[2], "1/2");
    }
    else if (value < 56)
    {
      strcpy(render[2], "1/1");
    }
    else if (value < 64)
    {
      strcpy(render[2], "2/1");
    }
    else
    {
      snprintf(render[2], RENDER_SIZE, "%d Hz", value - 64);
    }
  }
  else if (cc == 9 && channel != 7)
  {
    snprintf(render[2], RENDER_SIZE, "%d", value - 64);
  }
  else if (cc == 15)
  {
    if (value == 64)
    {
      strcpy(render[2], "Center");
    }
    else
    {
      snprintf(render[2], RENDER_SIZE, "%d %s", abs(value - 64), value < 64 ? "Left" : "Right");
    }
  }
  else if (cc == 9 && channel == 7)
  {
    if (value == 0)
    {
      strcpy(render[2], "Off");
    }
    else if (value < 16)
    {
      snprintf(render[2], RENDER_SIZE, "%d", (int)(value / 16) + 1);
    }
  }
  else if (cc == 10 && channel == 7)
  {
    if (value < 21)
    {
      strcpy(render[2], "manual");
    }
    else if (value < 43)
    {
      strcpy(render[2], "up");
    }
    else if (value < 64)
    {
      strcpy(render[2], "down");
    }
    else if (value < 85)
    {
      strcpy(render[2], "up/down");
    }
    else if (value < 107)
    {
      strcpy(render[2], "down/up");
    }
    else
    {
      strcpy(render[2], "random");
    }
  }
  else if (cc == 11 && channel == 7)
  {
    snprintf(render[2], RENDER_SIZE, "%d", (int)(value / 21) + 1);
  }
  else
  {
    snprintf(render[2], RENDER_SIZE, "%d", value);
  }
}