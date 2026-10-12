# FSM Traffic Light Controller

A non-blocking finite state machine (FSM) traffic light with a pedestrian
crossing button, written in object-oriented C++ for Arduino and runnable
instantly in the [Wokwi](https://wokwi.com) simulator.

**Live demo:** open `dashboard.html` to test the state machine in your browser.

## Features
- `enum class State` + a `TrafficLight` class with private state
- Fully non-blocking: timing uses `millis()`, never `delay()`
- Debounced pedestrian button (`INPUT_PULLUP`)
- Pedestrian request shortens GREEN (never below a 2 s minimum)

## State transitions
| From   | To     | Condition                                  |
|--------|--------|--------------------------------------------|
| RED    | GREEN  | 5 s elapsed                                |
| GREEN  | YELLOW | 6 s elapsed, or 2 s elapsed if ped request |
| YELLOW | WALK   | 2 s elapsed and ped request queued         |
| YELLOW | RED    | 2 s elapsed and no ped request             |
| WALK   | RED    | 4 s elapsed                                |

```
RED -> GREEN -> YELLOW -> RED
                   \-> WALK -> RED   (if button pressed)
```

## Components (simulated)
| Part              | Pin |
|-------------------|-----|
| Arduino Uno       | -   |
| Red LED + 220 ohm | 13  |
| Yellow LED + 220 ohm | 12 |
| Green LED + 220 ohm | 11 |
| Blue "walk" LED + 220 ohm | 8 |
| Push button       | 2 (to GND) |

## Run it
1. Go to wokwi.com, create a new Arduino Uno project.
2. Paste `traffic_light.ino` and `diagram.json`.
3. Press Play, then click the button to request a crossing.

To get a one-click link for your resume, save the project on Wokwi and
share its URL.

## Files
- `traffic_light.ino` - firmware
- `diagram.json` - Wokwi circuit
- `dashboard.html` - interactive FSM simulator (same logic as the firmware)
