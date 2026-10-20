// ESP32-WROOM-32E + 240x320 ST7789 diagnostic sketch.
// Scans SPI modes 0-3 and RGB565/RGB666 pixel formats in sequence.

#include <SPI.h>

constexpr int PIN_MOSI = 23;
constexpr int PIN_SCLK = 18;
constexpr int PIN_CS = 5;
constexpr int PIN_DC = 2;
constexpr int PIN_RST = 15;
constexpr int PIN_BL = 13;

constexpr uint16_t TFT_WIDTH = 240;
constexpr uint16_t TFT_HEIGHT = 320;
const uint32_t SPI_SPEEDS[] = {1000000, 4000000, 10000000, 20000000};
constexpr uint32_t CONFIG_HOLD_MS = 8000;
constexpr uint8_t CONFIG_COUNT = 4 * 4 * 2;

SPIClass tftSPI(FSPI);
uint8_t activeSpiMode = SPI_MODE0;
uint32_t activeSpiHz = SPI_SPEEDS[0];

struct PixelFormat {
  const char *label;
  uint8_t colmod;
  bool rgb666;
};

const PixelFormat pixelFormats[] = {
    {"RGB565", 0x55, false},
    {"RGB666", 0x66, true},
};

constexpr uint8_t FONT[][5] = {
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // A
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // C
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, // W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // X
    {0x07, 0x08, 0x70, 0x08, 0x07}, // Y
    {0x61, 0x51, 0x49, 0x45, 0x43}, // Z
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 9
    {0x00, 0x00, 0x00, 0x00, 0x00}, // space
    {0x08, 0x08, 0x08, 0x08, 0x08}, // -
    {0x00, 0x60, 0x60, 0x00, 0x00}, // :
};

uint8_t fontIndex(char c) {
  if (c >= 'A' && c <= 'Z') return c - 'A';
  if (c >= '0' && c <= '9') return 26 + c - '0';
  if (c == '-') return 37;
  if (c == ':') return 38;
  return 36;
}

void writeCommand(uint8_t command) {
  tftSPI.beginTransaction(SPISettings(activeSpiHz, MSBFIRST, activeSpiMode));
  digitalWrite(PIN_DC, LOW);
  digitalWrite(PIN_CS, LOW);
  tftSPI.transfer(command);
  digitalWrite(PIN_CS, HIGH);
  tftSPI.endTransaction();
}

void writeData(const uint8_t *data, size_t length) {
  tftSPI.beginTransaction(SPISettings(activeSpiHz, MSBFIRST, activeSpiMode));
  digitalWrite(PIN_DC, HIGH);
  digitalWrite(PIN_CS, LOW);
  tftSPI.writeBytes(data, length);
  digitalWrite(PIN_CS, HIGH);
  tftSPI.endTransaction();
}

void writeCommandData(uint8_t command, const uint8_t *data, size_t length) {
  writeCommand(command);
  if (length) writeData(data, length);
}

void setAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  const uint8_t columns[] = {uint8_t(x0 >> 8), uint8_t(x0),
                             uint8_t(x1 >> 8), uint8_t(x1)};
  const uint8_t rows[] = {uint8_t(y0 >> 8), uint8_t(y0),
                          uint8_t(y1 >> 8), uint8_t(y1)};
  writeCommandData(0x2A, columns, sizeof(columns));
  writeCommandData(0x2B, rows, sizeof(rows));
  writeCommand(0x2C);
}

uint8_t pixelBytes[120];

void fillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
              uint16_t color, const PixelFormat &format) {
  if (!width || !height || x >= TFT_WIDTH || y >= TFT_HEIGHT) return;
  if (x + width > TFT_WIDTH) width = TFT_WIDTH - x;
  if (y + height > TFT_HEIGHT) height = TFT_HEIGHT - y;

  const uint8_t red = (color >> 11) & 0x1F;
  const uint8_t green = (color >> 5) & 0x3F;
  const uint8_t blue = color & 0x1F;
  const uint8_t red8 = (red << 3) | (red >> 2);
  const uint8_t green8 = (green << 2) | (green >> 4);
  const uint8_t blue8 = (blue << 3) | (blue >> 2);
  const uint8_t red6 = red8 & 0xFC;
  const uint8_t green6 = green8 & 0xFC;
  const uint8_t blue6 = blue8 & 0xFC;
  const uint8_t red5 = (color >> 8) & 0xF8;
  const uint8_t green6_565 = (color >> 3) & 0xFC;
  const uint8_t blue5 = (color << 3) & 0xF8;

  const uint8_t pixel[] = {format.rgb666 ? red6 : red5,
                           format.rgb666 ? green6 : green6_565,
                           format.rgb666 ? blue6 : blue5};
  const size_t bytesPerPixel = format.rgb666 ? 3 : 2;
  for (size_t i = 0; i < sizeof(pixelBytes); i += bytesPerPixel) {
    pixelBytes[i] = pixel[0];
    pixelBytes[i + 1] = pixel[1];
    if (format.rgb666) pixelBytes[i + 2] = pixel[2];
  }

  setAddressWindow(x, y, x + width - 1, y + height - 1);
  uint32_t remaining = uint32_t(width) * height * bytesPerPixel;
  tftSPI.beginTransaction(SPISettings(activeSpiHz, MSBFIRST, activeSpiMode));
  digitalWrite(PIN_DC, HIGH);
  digitalWrite(PIN_CS, LOW);
  while (remaining) {
    const size_t count = remaining > sizeof(pixelBytes) ? sizeof(pixelBytes) : remaining;
    tftSPI.writeBytes(pixelBytes, count);
    remaining -= count;
  }
  digitalWrite(PIN_CS, HIGH);
  tftSPI.endTransaction();
}

void drawText(const char *text, uint16_t x, uint16_t y, uint8_t scale,
              uint16_t foreground, uint16_t background,
              const PixelFormat &format) {
  while (*text) {
    const uint8_t *glyph = FONT[fontIndex(*text++)];
    const uint8_t glyphWidth = 5 * scale;
    const uint8_t glyphHeight = 7 * scale;
    const size_t bytesPerPixel = format.rgb666 ? 3 : 2;
    uint8_t glyphPixels[5 * 3 * 7 * 3 * 3];
    size_t offset = 0;
    for (uint8_t row = 0; row < glyphHeight; row++) {
      for (uint8_t col = 0; col < glyphWidth; col++) {
        const uint8_t sourceCol = col / scale;
        const uint8_t sourceRow = row / scale;
        const uint16_t color = (glyph[sourceCol] & (1 << sourceRow))
                                   ? foreground
                                   : background;
        const uint8_t red = (color >> 11) & 0x1F;
        const uint8_t green = (color >> 5) & 0x3F;
        const uint8_t blue = color & 0x1F;
        if (format.rgb666) {
          glyphPixels[offset++] = ((((red << 3) | (red >> 2)) & 0xFC));
          glyphPixels[offset++] = ((((green << 2) | (green >> 4)) & 0xFC));
          glyphPixels[offset++] = ((((blue << 3) | (blue >> 2)) & 0xFC));
        } else {
          glyphPixels[offset++] = color >> 8;
          glyphPixels[offset++] = color;
        }
      }
    }
    setAddressWindow(x, y, x + glyphWidth - 1, y + glyphHeight - 1);
    writeData(glyphPixels, offset);
    x += 6 * scale;
  }
}

void drawConfig(uint8_t spiMode, uint8_t formatIndex, uint8_t speedIndex,
                uint8_t configNumber) {
  activeSpiMode = spiMode;
  activeSpiHz = SPI_SPEEDS[speedIndex];
  const PixelFormat &format = pixelFormats[formatIndex];
  const uint16_t black = 0x0000;
  const uint16_t white = 0xFFFF;

  digitalWrite(PIN_RST, LOW);
  delay(20);
  digitalWrite(PIN_RST, HIGH);
  delay(150);
  writeCommandData(0x01, nullptr, 0);
  delay(150);
  writeCommand(0x11);
  delay(120);
  const uint8_t pixelFormat[] = {format.colmod};
  const uint8_t orientation[] = {0x00};
  writeCommandData(0x3A, pixelFormat, sizeof(pixelFormat));
  writeCommandData(0x36, orientation, sizeof(orientation));
  writeCommand(0x21);
  writeCommand(0x13);
  writeCommand(0x29);
  delay(20);

  fillRect(0, 0, TFT_WIDTH, TFT_HEIGHT, black, format);
  char label[16];
  snprintf(label, sizeof(label), "TEST %d OF %d", configNumber + 1,
           CONFIG_COUNT);
  drawText(label, 12, 24, 3, white, black, format);
  snprintf(label, sizeof(label), "SPI MODE %d", spiMode);
  drawText(label, 12, 74, 3, white, black, format);
  snprintf(label, sizeof(label), "SPI %d MHZ", activeSpiHz / 1000000);
  drawText(label, 12, 124, 3, white, black, format);
  drawText(format.label, 12, 174, 3, white, black, format);

  const uint16_t colors[] = {0xF800, 0x07E0, 0x001F, 0xFFFF};
  const char *names[] = {"RED", "GREEN", "BLUE", "WHITE"};
  for (uint8_t i = 0; i < 4; i++) {
    fillRect(12, 220 + i * 23, 55, 18, colors[i], format);
    drawText(names[i], 82, 222 + i * 23, 2, white, black, format);
  }

  Serial.printf("Config %u/%u: SPI mode %u, %s, %u Hz\n",
                configNumber + 1, CONFIG_COUNT, spiMode, format.label,
                activeSpiHz);
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_CS, OUTPUT);
  pinMode(PIN_DC, OUTPUT);
  pinMode(PIN_RST, OUTPUT);
  pinMode(PIN_BL, OUTPUT);
  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_DC, HIGH);
  digitalWrite(PIN_BL, LOW);

  tftSPI.begin(PIN_SCLK, -1, PIN_MOSI, PIN_CS);

  digitalWrite(PIN_RST, HIGH);
  delay(10);
  digitalWrite(PIN_RST, LOW);
  delay(20);
  digitalWrite(PIN_RST, HIGH);
  delay(150);
  digitalWrite(PIN_BL, HIGH);

  Serial.println("ST7789 SPI/pixel-format scan starting");
}

void loop() {
  static uint8_t configNumber = 0;
  const uint8_t speedIndex = configNumber / 8;
  const uint8_t spiMode = (configNumber % 8) / 2;
  const uint8_t formatIndex = configNumber % 2;
  drawConfig(spiMode, formatIndex, speedIndex, configNumber);
  delay(CONFIG_HOLD_MS);
  configNumber = (configNumber + 1) % CONFIG_COUNT;
}
