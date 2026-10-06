

## Hardware


## Pinout

| Pin    | Target       | Note               |
| ------ | ------------ | ------------------ |
| GPIO1  | DF-Player TX |                    |
| GPIO3  | DF-Player RX |                    |
| GPIO5  | Power-LED 2  | Flash/Thunder      |
| GPIO13 | Power-LED 1  | Flash/Thunder      |
| GPIO15 | 5V Fan       |                    |
| GPIO16 | LD2410C RX   | Radar-Sensor       |
| GPIO17 | LD2410C TX   | Radar-Sensor       |
| GPIO18 | Power-LED 4  | Flash/Thunder      |
| GPIO19 | Power-LED 3  | Flash/Thunder      |
| GPIO21 | BH1750 SDA   | Ambient Light      |
| GPIO22 | BH1750 SCL   | Ambient Light      |
| GPIO23 | WS2812 Data  | RGB LED-Strip      |
| GPIO26 | Power-LED 5  | Flash/Thunder      |
| GPIO36 | Red Buzzer   | _(only Input Pin)_ |

> GPIO7-11 are not allowed to be used (SPI bus for flash memory)  
> GPIO34-36+39 are input only pins  
> GPIO1+3 must be used with care and can only be used with disable logging via UART


## Trigger 

```mermaid
flowchart LR
	START(["LD2410C Radar Sensor<br>on / below distance threshold"])
    START -->|"CONDITIONS:<br>- no emergency stop?<br>- illumination <= threshold?<br>- music scene running?"| CONTROLLER
	CONTROLLER["SCENE CONTROLLER<br>- stop current scene<br>- choose scene type<br>- start random scene<br>- set global variables"]
    CONTROLLER --> MUSIC & APPROACH & ACTION & DEPARTURE
	MUSIC["MUSIC dispatcher<br>- selects a random music scene"]
	APPROACH["APPROACH dispatcher<br>- selects a random approach scene"]
	ACTION["ACTION dispatcher<br>- selects a random action scene"]
	DEPARTURE["DEPARTURE dispatcher<br>- selects a random departure scene"]
```
```mermaid
flowchart LR
    END(["LD2410C Radar Sensor<br>off"]) -->
    STOP["STOP script<br>- stop current scene<br>- turn off everything<br>- reset global variables"]
```

