#include <Arduino.h>
#include <SPI.h>
#include <driver/gpio.h>
#include <initializer_list>

constexpr int PIN_MOSI = 23;
constexpr int PIN_SCLK = 18;
constexpr int PIN_CS = 5;
constexpr int PIN_DC = 2;
constexpr int PIN_RST = 15;
constexpr int PIN_BL = 13;
constexpr uint16_t PANEL_WIDTH = 240;
constexpr uint16_t PANEL_HEIGHT = 320;
constexpr uint32_t SPI_HZ = 1000000;

struct TestColor {
  const char *name;
  uint8_t rgb[3];
};

constexpr TestColor COLORS[] = {
    {"RED", {0xFC, 0, 0}},
    {"GREEN", {0, 0xFC, 0}},
    {"BLUE", {0, 0, 0xFC}},
    {"WHITE", {0xFC, 0xFC, 0xFC}},
    {"BLACK", {0, 0, 0}},
};
constexpr size_t COLOR_COUNT = sizeof(COLORS) / sizeof(COLORS[0]);

void writeCommand(uint8_t command, std::initializer_list<uint8_t> data = {});

void writeCommand(uint8_t command, std::initializer_list<uint8_t> data) {
  SPI.beginTransaction(SPISettings(SPI_HZ, MSBFIRST, SPI_MODE0));
  digitalWrite(PIN_DC, LOW);
  digitalWrite(PIN_CS, LOW);
  SPI.transfer(command);
  if (data.size()) {
    digitalWrite(PIN_DC, HIGH);
    SPI.writeBytes(data.begin(), data.size());
  }
  digitalWrite(PIN_CS, HIGH);
  SPI.endTransaction();
}

void initPanel() {
  // T200H7-H12-01 vendor Arduino reference, OLED_init(), portrait RGB666.
  digitalWrite(PIN_CS, LOW);
  digitalWrite(PIN_RST, LOW);
  delay(1000);
  digitalWrite(PIN_RST, HIGH);
  digitalWrite(PIN_CS, HIGH);
  delay(1000);

  writeCommand(0x11);
  delay(120);
  writeCommand(0x2A, {0x00, 0x00, 0x00, 0xEF});
  writeCommand(0x2B, {0x00, 0x28, 0x01, 0x17});
  writeCommand(0xB2, {0x0C, 0x0C, 0x00, 0x33, 0x33});
  writeCommand(0x20);
  writeCommand(0xB7, {0x56});
  writeCommand(0xBB, {0x18});
  writeCommand(0xC0, {0x2C});
  writeCommand(0xC2, {0x01});
  writeCommand(0xC3, {0x1F});
  writeCommand(0xC4, {0x20});
  writeCommand(0xC6, {0x0F});
  writeCommand(0xD0, {0xA6, 0xA1});
  const std::initializer_list<uint8_t> gamma = {
      0xD0, 0x0D, 0x14, 0x0B, 0x0B, 0x07, 0x3A,
      0x44, 0x50, 0x08, 0x13, 0x13, 0x2D, 0x32};
  writeCommand(0xE0, gamma);
  writeCommand(0xE1, gamma);
  writeCommand(0x36, {0x00});
  writeCommand(0x3A, {0x66});
  writeCommand(0xE7, {0x00});
  writeCommand(0x21);
  writeCommand(0x29);
  delay(20);
}

void fillColumns(uint16_t first, uint16_t width, const TestColor &color) {
  const uint16_t last = first + width - 1;
  // The vendor's full-screen fill addresses rows 0 through 319.
  writeCommand(0x2A, {uint8_t(first >> 8), uint8_t(first),
                     uint8_t(last >> 8), uint8_t(last)});
  writeCommand(0x2B, {0x00, 0x00, uint8_t((PANEL_HEIGHT - 1) >> 8),
                     uint8_t(PANEL_HEIGHT - 1)});
  writeCommand(0x2C);

  uint8_t pixels[120];
  for (size_t i = 0; i < sizeof(pixels); i += sizeof(color.rgb)) {
    memcpy(pixels + i, color.rgb, sizeof(color.rgb));
  }
  uint32_t remaining = uint32_t(width) * PANEL_HEIGHT * sizeof(color.rgb);
  SPI.beginTransaction(SPISettings(SPI_HZ, MSBFIRST, SPI_MODE0));
  digitalWrite(PIN_DC, HIGH);
  digitalWrite(PIN_CS, LOW);
  while (remaining > 0) {
    const size_t chunk = min<size_t>(remaining, sizeof(pixels));
    SPI.writeBytes(pixels, chunk);
    remaining -= chunk;
  }
  digitalWrite(PIN_CS, HIGH);
  SPI.endTransaction();
}

void setup() {
  // Keep the two active-high relay outputs low during the display test.
  gpio_set_level(GPIO_NUM_32, 0);
  gpio_set_level(GPIO_NUM_33, 0);
  gpio_set_direction(GPIO_NUM_32, GPIO_MODE_OUTPUT);
  gpio_set_direction(GPIO_NUM_33, GPIO_MODE_OUTPUT);

  Serial.begin(115200);
  pinMode(PIN_CS, OUTPUT);
  pinMode(PIN_DC, OUTPUT);
  pinMode(PIN_RST, OUTPUT);
  pinMode(PIN_BL, OUTPUT);
  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_BL, LOW);
  SPI.begin(PIN_SCLK, -1, PIN_MOSI, PIN_CS);
  initPanel();
  Serial.println("T200H7-H12-01 INIT SENT: 240x320 RGB666 SPI MODE0 1000000 Hz");
}

void loop() {
  fillColumns(0, PANEL_WIDTH, COLORS[0]);
  digitalWrite(PIN_BL, HIGH);
  Serial.println("BACKLIGHT ON");
  for (size_t i = 0; i < COLOR_COUNT; ++i) {
    if (i > 0) {
      fillColumns(0, PANEL_WIDTH, COLORS[i]);
    }
    Serial.printf("FRAME %s\n", COLORS[i].name);
    delay(3000);
  }

  constexpr uint16_t BAR_WIDTH = PANEL_WIDTH / COLOR_COUNT;
  for (size_t i = 0; i < COLOR_COUNT; ++i) {
    fillColumns(i * BAR_WIDTH, BAR_WIDTH, COLORS[i]);
  }
  Serial.println("FRAME BARS RED GREEN BLUE WHITE BLACK");
  delay(6000);
  digitalWrite(PIN_BL, LOW);
  Serial.println("BACKLIGHT OFF");
  delay(2000);
  digitalWrite(PIN_BL, HIGH);
  Serial.println("BACKLIGHT ON: BARS");
  delay(3000);
}
