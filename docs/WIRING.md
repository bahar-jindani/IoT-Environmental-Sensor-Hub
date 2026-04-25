# Wiring Reference

The wiring matches `wokwi/diagram.json`. The BMP280 breakout used in the simulation is the **6-pin SPI variant** (VCC, GND, SCK, SDI, SDO, CS).

## Pin map

| Sensor | Sensor pin | ESP32 GPIO | Wire color (in diagram) |
|--------|-----------|------------|--------------------------|
| DHT22  | VCC        | 3V3        | Red    |
| DHT22  | GND        | GND        | Black  |
| DHT22  | DATA / SDA | GPIO 15    | Green  |
| BMP280 | VCC        | 3V3        | Red    |
| BMP280 | GND        | GND        | Black  |
| BMP280 | SCK        | GPIO 18    | Green  |
| BMP280 | SDI (MOSI) | GPIO 19    | Green  |
| BMP280 | SDO (MISO) | GPIO 23    | Green  |
| BMP280 | CS         | GPIO  5    | Green  |

> If your physical BMP280 breakout exposes only **VCC / GND / SCL / SDA** then it is in **I²C mode**. Wire `SCL → GPIO22`, `SDA → GPIO21`, and switch to the I²C constructor in the sketch:
>
> ```cpp
> Adafruit_BMP280 bmp;          // I²C
> bmp.begin(0x76);              // or 0x77, depends on board
> ```

## DHT22 notes

- A 10 kΩ pull-up resistor between **DATA** and **VCC** is recommended on real hardware. Wokwi handles this internally.
- Maximum sample rate is **1 Hz** — do not poll faster.

## BMP280 notes

- The Wokwi virtual chip has sliders for temperature, pressure, and altitude — you can move them while the simulation runs to see live MQTT updates on your panel.
- `setSampling()` in the sketch is configured for indoor monitoring (Mode = Normal, T×2, P×16, IIR filter ×16, standby 500 ms).

## Power

Both sensors are 3.3 V parts powered from the ESP32's `3V3` rail. Do **not** connect them to `VIN`/`5V`.
