# Haier AC160 ESPHome Component

This is an ESPHome Climate IR component that provides support for Haier ACs using the Haier AC160 protocol.

## Usage
```yaml
# example.yaml

# ---------- Your Board Configuration Here ----------

# add the component to your board
external_components:
  - source:
      type: git
      url: https://github.com/hamdannawaz582/esphome_haierac160
      ref: main
    components: [haierac160]
    
sensor:               # optional
  - platform: dht
    pin: GPIO13
    temperature:
      name: "Bedroom Temperature"
      id: tempsense
    humidity:
      name: "Bedroom Humidity"
      id: humsense
    update_interval: 60s
    
# create a remote transmitter
remote_transmitter:   # mandatory
  pin: GPIO4
  carrier_duty_percent: 50%

# create the climate component for the haierac160 platform
climate:
  - platform: haierac160
    name: "AC"
    sensor: tempsense         # optional
    humidity_sensor: humsense # optional
    visual:
      min_temperature: 16
      max_temperature: 30
      temperature_step: 1
```

## Credits
All Haier codes & the protocol struct are from [crankyoldgit's IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266) library.