#include <Arduino.h>
#include <driver/gpio.h>

constexpr gpio_num_t RELAY_1 = GPIO_NUM_32;
constexpr gpio_num_t RELAY_2 = GPIO_NUM_33;
constexpr unsigned long HOLD_MS = 5000;

bool reportLevels(const char *phase, int expected32, int expected33) {
  delay(20);
  const int level32 = gpio_get_level(RELAY_1);
  const int level33 = gpio_get_level(RELAY_2);
  const bool matches = level32 == expected32 && level33 == expected33;
  Serial.printf("%s GPIO32=%d GPIO33=%d %s\n", phase, level32, level33,
                matches ? "PASS" : "FAIL");
  return matches;
}

void testRelay(gpio_num_t pin) {
  const int pinNumber = static_cast<int>(pin);
  Serial.printf("BEGIN GPIO%d\n", pinNumber);
  if (!reportLevels("BEFORE", 0, 0)) {
    Serial.printf("END GPIO%d FAIL\n", pinNumber);
    return;
  }

  ESP_ERROR_CHECK(gpio_set_level(pin, 1));
  const int expected32 = pin == RELAY_1;
  const int expected33 = pin == RELAY_2;
  const bool on = reportLevels("ON", expected32, expected33);
  bool held = false;
  if (on) {
    delay(HOLD_MS - 40);
    held = reportLevels("HELD", expected32, expected33);
  }

  ESP_ERROR_CHECK(gpio_set_level(pin, 0));
  const bool off = reportLevels("OFF", 0, 0);
  Serial.printf("END GPIO%d %s\n", pinNumber, on && held && off ? "PASS" : "FAIL");
}

void setup() {
  // Set the output latches low before enabling either output driver.
  ESP_ERROR_CHECK(gpio_set_level(RELAY_1, 0));
  ESP_ERROR_CHECK(gpio_set_level(RELAY_2, 0));
  gpio_config_t relayPins = {};
  relayPins.pin_bit_mask = (1ULL << RELAY_1) | (1ULL << RELAY_2);
  relayPins.mode = GPIO_MODE_INPUT_OUTPUT;  // Enable digital pad readback.
  ESP_ERROR_CHECK(gpio_config(&relayPins));

  Serial.begin(115200);
  Serial.println("RELAY TEST READY: 1=GPIO32 2=GPIO33 ?=STATUS");
  reportLevels("IDLE", 0, 0);
}

void loop() {
  switch (Serial.read()) {
    case '1':
      testRelay(RELAY_1);
      break;
    case '2':
      testRelay(RELAY_2);
      break;
    case '?':
      reportLevels("IDLE", 0, 0);
      break;
  }
  delay(10);
}
