# MQTT Interface

## Broker

| Setting     | Value              |
|-------------|--------------------|
| Host        | `broker.emqx.io`   |
| Port        | `1883`             |
| Auth        | None (public test broker) |

> EMQX's public broker is shared. For production use your own broker (Mosquitto, EMQX Cloud, HiveMQ, AWS IoT Core, etc.) with TLS + credentials.

## Published topics

The sketch publishes one **plain-text float** value per topic, every 5 seconds. No JSON — most MQTT panel apps subscribe to one topic per widget.

| Topic                           | Unit  | Source  |
|---------------------------------|-------|---------|
| `esp32/dht22/temperature`       | °C    | DHT22   |
| `esp32/dht22/humidity`          | %RH   | DHT22   |
| `esp32/bmp280/temperature`      | °C    | BMP280  |
| `esp32/bmp280/pressure`         | hPa   | BMP280  |
| `esp32/bmp280/altitude`         | m     | BMP280  |

Example payloads (raw text):
```
24.70
58.30
24.95
1012.40
8.10
```

## Configuring your panel app

Add one widget per topic, e.g.:

| Widget               | Topic                       | Format |
|----------------------|------------------------------|--------|
| Temperature gauge    | `esp32/dht22/temperature`   | float  |
| Humidity gauge       | `esp32/dht22/humidity`      | float  |
| Pressure gauge       | `esp32/bmp280/pressure`     | float  |
| Altitude readout     | `esp32/bmp280/altitude`     | float  |
| BMP temperature      | `esp32/bmp280/temperature`  | float  |

Or subscribe to the wildcard `esp32/#` to see everything.

## Verify from a workstation

```bash
mosquitto_sub -h broker.emqx.io -p 1883 -t 'esp32/#' -v
```
