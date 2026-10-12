/*
 * FSM Traffic Light Controller with Pedestrian Crossing
 * Board: Arduino Uno (works on Wokwi)
 * Style: OOP, enum class, non-blocking (millis) state machine
 */
#include <Arduino.h>

enum class State : uint8_t { RED, GREEN, YELLOW, WALK };

class TrafficLight {
public:
  TrafficLight(uint8_t red, uint8_t yellow, uint8_t green,
               uint8_t walk, uint8_t button)
    : pinRed(red), pinYellow(yellow), pinGreen(green),
      pinWalk(walk), pinButton(button) {}

  void begin() {
    pinMode(pinRed, OUTPUT);
    pinMode(pinYellow, OUTPUT);
    pinMode(pinGreen, OUTPUT);
    pinMode(pinWalk, OUTPUT);
    pinMode(pinButton, INPUT_PULLUP);
    enter(State::RED);
  }

  void update() {
    pollButton();
    const uint32_t elapsed = millis() - stateStart;

    switch (state) {
      case State::RED:
        if (elapsed >= RED_MS) enter(State::GREEN);
        break;
      case State::GREEN:
        // A pedestrian request shortens GREEN, but never below MIN_GREEN_MS
        if (elapsed >= (pedRequest ? MIN_GREEN_MS : GREEN_MS)) enter(State::YELLOW);
        break;
      case State::YELLOW:
        if (elapsed >= YELLOW_MS) enter(pedRequest ? State::WALK : State::RED);
        break;
      case State::WALK:
        if (elapsed >= WALK_MS) enter(State::RED);
        break;
    }
  }

private:
  // Timing (ms)
  static constexpr uint32_t RED_MS = 5000;
  static constexpr uint32_t GREEN_MS = 6000;
  static constexpr uint32_t MIN_GREEN_MS = 2000;
  static constexpr uint32_t YELLOW_MS = 2000;
  static constexpr uint32_t WALK_MS = 4000;
  static constexpr uint32_t DEBOUNCE_MS = 50;

  const uint8_t pinRed, pinYellow, pinGreen, pinWalk, pinButton;
  State state = State::RED;
  uint32_t stateStart = 0;
  bool pedRequest = false;
  bool lastButton = HIGH;
  uint32_t lastEdge = 0;

  void pollButton() {
    const bool now = digitalRead(pinButton);
    if (now != lastButton && millis() - lastEdge > DEBOUNCE_MS) {
      lastEdge = millis();
      lastButton = now;
      if (now == LOW && state != State::WALK) {
        pedRequest = true;
        Serial.println(F("Pedestrian request queued"));
      }
    }
  }

  void enter(State next) {
    state = next;
    stateStart = millis();
    if (next == State::WALK) pedRequest = false;
    if (next == State::RED && !pedRequest) { /* normal cycle */ }

    digitalWrite(pinRed,    next == State::RED || next == State::WALK);
    digitalWrite(pinYellow, next == State::YELLOW);
    digitalWrite(pinGreen,  next == State::GREEN);
    digitalWrite(pinWalk,   next == State::WALK);

    Serial.print(F("State -> "));
    Serial.println(name(next));
  }

  static const __FlashStringHelper* name(State s) {
    switch (s) {
      case State::RED:    return F("RED");
      case State::GREEN:  return F("GREEN");
      case State::YELLOW: return F("YELLOW");
      case State::WALK:   return F("WALK");
    }
    return F("?");
  }
};

TrafficLight light(13, 12, 11, 8, 2);  // red, yellow, green, walk LED, button

void setup() {
  Serial.begin(115200);
  light.begin();
}

void loop() {
  light.update();  // non-blocking: no delay() anywhere
}
