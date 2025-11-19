# Building a rangefinder made of Parkside PLEM-50-C3 laser telemeter and ESP32-CAM
<a href="https://www.youtube.com/shorts/rriwwwzv8iQ"><img src="https://raw.githubusercontent.com/sorinbotirla/parkside-telemeter-esp32cam-rangefinder/refs/heads/main/images/esp32rangefinderparkside.jpg" /></a>
A simple rangefinder having the Parkside PLEM-50-C3 laser telemeter, a pair of binoculars, an ILI9341 2.8 inch Display and 2x ESP32-CAM to measure distance exactly on the right spot.

## Bill of Materials (BOM)
<ol>
  <li>1x Parkside PLEM-50-C3 laser telemeter (you will cut some holes in the case but it will be also be still usable as standalone)</li>
  <li>1x side micro switch (to turn on/off the beeping speaker</li>
  <li>(OPTIONAL) a single tap button, for external measure triggering</li>
  <li>(OPTIONAL) 1x 2PIN JST connector (male and female) - to connect the telemeter and the external trigger button</li>
  <li>1x 6PIN JST connector (male and female) - to connect the esp32-cam and the telemeter</li>
  <li>2x ESP32-CAM boards with 1x camera attached</li>
  <li>1x ESP32-CAM MB or an USB TTL-UART Adapter to upload the code (CP2101, CH340, FTDI FT232 etc)</li>
  <li>Micro USB to USB-A for uploading the code (or USB-C if your esp-cam has it)</li>
  <li>1x 2.8 inch ILI9341 display (touch not needed)</li>
  <li>very thin wire to solder everything</li>
  <li>solder iron, solder wire and flux</li>
  <li>A pair of binoculars</li>
  <li>3D printer and PLA filament or duct tape to attach everything together</li>
  <li>Arduino IDE and ESP32 Library and json installed</li>
</ol>


## Pinout

<table border="1" cellpadding="6" cellspacing="0">
    <tr>
        <th>ILI9341 Display</th>
        <th>ESP32-CAM (Display/Camera)</th>
        <th>ESP32-CAM (Telemeter)</th>
        <th>Parkside Telemeter</th>
    </tr>

    <tr>
        <td>VCC</td>
        <td>5V</td>
        <td></td>
        <td></td>
    </tr>

    <tr>
        <td>GND</td>
        <td>GND</td>
        <td>GND</td>
        <td>GND (Pin 1)</td>
    </tr>

    <tr>
        <td>MOSI</td>
        <td>23</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>MISO</td>
        <td>19</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>SCK</td>
        <td>18</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>CS</td>
        <td>17</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>DC</td>
        <td>16</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>RST</td>
        <td>5</td>
        <td></td>
        <td></td>
    </tr>
    <tr>
        <td>BL</td>
        <td>15</td>
        <td></td>
        <td></td>
    </tr>

    <tr>
        <td></td>
        <td>13 (RX2)</td>
        <td>16 (TX2)</td>
        <td></td>
    </tr>

    <tr>
        <td></td>
        <td></td>
        <td>13 (SDA)</td>
        <td>SDA (Pin 10)</td>
    </tr>

    <tr>
        <td></td>
        <td></td>
        <td>2 (SCL)</td>
        <td>SCL (Pin 11)</td>
    </tr>

    <tr>
        <td></td>
        <td></td>
        <td>14 (MEASURE)</td>
        <td>KEY (Pin 18)</td>
    </tr>
</table>

## Code Setup
Due to the lack of gpio pins and not having a gpio extender, 2x esp-cam boards have been used. In this repository are 2 .ino files, cameradisplay.ino is for the esp-cam handling the camera and the display, telemeter.ino is for the esp-cam handling the telemeter data.

## Wiring Setup
You need to disassemble the Parkside Telemeter. Inside you will find the display, the keyboard module and the laser sensor.<br />
Laser sensor has 2 traces you need to solder with thin wire.<br />
<img width="100%" src="https://raw.githubusercontent.com/sorinbotirla/parkside-telemeter-esp32cam-rangefinder/refs/heads/main/images/gnd-33-sda-scl.jpg" />
<br />
These traces will be used for I2C communication between the sensor and the esp32cam. They are used as SDA/SCL pins. <br />
You will also need 3.3V and GND from this sensor. You can see the 3.3V pin on the left side, marked on the pcb with the 3.3V symbol. GND pin is on the opposite end.

Pinout of the FFC (left to right):

```
VCC (3.3V)
VCC (3.3V)
VCC (3.3V)
GND
GND
GND
I2C SDA
I2C SCL
display backlight
Tx (not used)
Rx (not used)
Signal for buzzer
Keyboard
Power button - when pressed, set to 0.33V. Set to 0V works, too.
Reference voltage for the keyboard?
Keyboard (at reference 3.38V):
0V Measure
1.12V Functíons (volumes, Pythagoras...)
2.67V Beep on/off
3.02V Reference button
1.52V Continuous measurement
2.56V Memory
2.77V Unit
2.35V Clear
1.69V +/-
```
