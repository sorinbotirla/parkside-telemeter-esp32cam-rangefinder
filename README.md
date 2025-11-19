# Building a rangefinder made of Parkside PLEM-50-C3 laser telemeter and ESP32-CAM
<a href="https://www.youtube.com/shorts/rriwwwzv8iQ"><img src="https://raw.githubusercontent.com/sorinbotirla/parkside-telemeter-esp32cam-rangefinder/refs/heads/main/images/esp32rangefinderparkside.jpg" /></a>
<br />
A simple rangefinder having the Parkside PLEM-50-C3 laser telemeter, a pair of binoculars, an ILI9341 2.8 inch Display and 2x ESP32-CAM to measure distance exactly on the right spot.

## Merits & recognition
Many thanks to <a href="https://github.com/pavel-rybnicek">Pavel Rybnicek</a> for publishing a little documentation about the sensor wiring. More about that on his repository at <a href="https://github.com/pavel-rybnicek/esphome-parkside-plem-50-c3
">https://github.com/pavel-rybnicek/esphome-parkside-plem-50-c3</a>

## Bill of Materials (BOM)
<ol>
  <li>1x Parkside PLEM-50-C3 laser telemeter (you will cut some holes in the case but it will be also be still usable as standalone)</li>
  <li>1x side micro switch (to turn on/off the beeping speaker</li>
  <li>1x side micro switch for putting both ESP32-cam into download mode</li>
  <li>2x 6PIN dupont connectors for uploading the code wkth a USB serial interface on each ESP32-cam</li>
  <li>(OPTIONAL) a single tap button, for external measure triggering</li>
  <li>(OPTIONAL) 1x 2PIN JST connector (male and female) - to connect the telemeter and the external trigger button</li>
  <li>1x 4PIN JST connector (male and female) - to connect the esp32-cam and the telemeter</li>
  <li>2x ESP32-CAM boards with 1x camera attached</li>
  <li>1x ESP32-CAM MB or an USB TTL-UART Adapter to upload the code (CP2101, CH340, FTDI FT232 etc)</li>
  <li>Micro USB to USB-A for uploading the code (or USB-C if your esp-cam has it)</li>
  <li>1x 2.8 inch ILI9341 display (touch not needed)</li>
  <li>very thin wire to solder everything</li>
  <li>soldering iron, solder wire and flux</li>
  <li>A pair of binoculars</li>
  <li>3D printer and PLA filament or duct tape to attach everything together</li>
  <li>Arduino IDE and ESP32 Library and json installed</li>
  <li>TP4056 battery charging module</li>
  <li>3.7V to 5V Step up booster</li>
  <li>Li-Ion rechargeable battery</li>
  <li>2x kOhm resistors (each one on SDA/SCL in series)</li>
  <li>100uF electrolytic capacitor (between gnd and 5v rail</li>
  <li>2x 100nF ceramic capacitor (one between 5v and gnd rail, one between the esp32 measure button pins)</li>
</ol>


## Pinout

<table border="1" cellpadding="6" cellspacing="0">
<thead>
<tr>
<th>ILI9341 Display</th>
<th>ESP32-CAM (Display/Camera)</th>
<th>ESP32-CAM (Telemeter)</th>
<th>Parkside Telemeter</th>
</tr>
</thead>

<tbody>
<tr>
<td>VCC</td>
<td>5V</td>
<td>&nbsp;</td>
<td>&nbsp;</td>
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
<td>&nbsp;</td>
<td>&nbsp;</td>
</tr>
<tr>
<td>MISO</td>
<td>19</td>
<td>&nbsp;</td>
<td>&nbsp;</td>
</tr>
<tr>
<td>SCK</td>
<td>18</td>
<td>&nbsp;</td>
<td>&nbsp;</td>
</tr>
<tr>
<td>CS</td>
<td>17</td>
<td>&nbsp;</td>
<td>&nbsp;</td>
</tr>
<tr>
<td>DC</td>
<td>16</td>
<td>&nbsp;</td>
<td>&nbsp;</td>
</tr>
<tr>
<td>RST</td>
<td>5</td>
<td>&nbsp;</td>
<td>&nbsp;</td>
</tr>
<tr>
<td>BL</td>
<td>15</td>
<td>&nbsp;</td>
<td>&nbsp;</td>
</tr>

<tr>
<td>&nbsp;</td>
<td>13 (RX2)</td>
<td>16 (TX2)</td>
<td>&nbsp;</td>
</tr>

<tr>
<td>&nbsp;</td>
<td>&nbsp;</td>
<td>13 (SDA)</td>
<td>SDA (Pin 10)</td>
</tr>

<tr>
<td>&nbsp;</td>
<td>&nbsp;</td>
<td>2 (SCL)</td>
<td>SCL (Pin 11)</td>
</tr>

<tr>
<td>&nbsp;</td>
<td>&nbsp;</td>
<td>14 (MEASURE)</td>
<td>KEY (Pin 18)</td>
</tr>
</tbody>

</table>

## Code Setup
Due to the lack of gpio pins and not having a gpio extender, 2x esp-cam boards have been used. In this repository are 2 .ino files, cameradisplay.ino is for the esp-cam handling the camera and the display, telemeter.ino is for the esp-cam handling the telemeter data.

## Wiring Setup
You need to disassemble the Parkside Telemeter. Inside you will find the display, the keyboard module and the laser sensor.<br />
Laser sensor has 2 traces you need to solder with thin wire.<br />
<br />
<img width="100%" src="https://raw.githubusercontent.com/sorinbotirla/parkside-telemeter-esp32cam-rangefinder/refs/heads/main/images/gnd-33-sda-scl.jpg" />
<br />
<br />
These traces will be used for I2C communication between the sensor and the esp32cam. They are used as SDA/SCL pins. <br />
You will also need 3.3V and GND from this sensor. You can see the 3.3V pin on the left side, marked on the pcb with the 3.3V symbol. GND pin is on the opposite end.
<br />
<br />
Use a 4PIN JST Connector to get the pins out (I Used a 6PIN for debugging too). You can use epoxy, bi-component glue or anything else you have
for securing the wires in place. I used Mitre-Apel bi-component glue. I know it's messy, but the wires won't go anywhere else.
<table>
  <td>
    <img width="100%" src="https://raw.githubusercontent.com/sorinbotirla/parkside-telemeter-esp32cam-rangefinder/refs/heads/main/images/20251103_020630.jpg" />
  <td>
    <img width="100%" src="https://raw.githubusercontent.com/sorinbotirla/parkside-telemeter-esp32cam-rangefinder/refs/heads/main/images/20251103_020720.jpg" />
  </td>
</table>
Colors used for marking the wires:

```php
3.3v - red
GND - black
SDA - white
SCL - blue 
White/transparent - Keyboard (tested but left unused)
Red/black - power (tested but left unused)
```
<br />
<br />
I have used a perfboard and 2 ESP32 CAM talking to eachother via software serial. I needed more GPIO pins but I had to solder everything on the chip pins instead. Still, camera pins and sdcard pins were left usable.
<br />
Before soldering the esp32 cam to the perfboard, solder the ILI9341 display directly to the esp32 chip pins, using thin wire, bypassing the board GPIO pins.
<br />
<br />
Also, you need to solder connectors for a CP2101/FTDI FT232/CH340 usb serial interface because esp32 cam doesn't have a usb port (it has a MB with usb but it won't be used in this porject). Still, you will use the connector to upload the code with the USB serial interface.
<br />
<br />
Pinout for the ESP32-cam serial connector (use one for each esp32 cam:
<br />
<table>
  <thead>
    <tr>
      <th>
        ESP32
      </th>
      <th>
        USB Serial Interface
      </th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>
        5V
      </td>
      <td>
        5V
      </td>
    </tr>
    <tr>
      <td>
        GND
      </td>
      <td>
        GND
      </td>
    </tr>
    <tr>
      <td>
        UDT
      </td>
      <td>
        RX
      </td>
    </tr>
    <tr>
      <td>
        UDR
      </td>
      <td>
        TX
      </td>
    </tr>
  </tbody>
</table>
<br />
<br />
<table>
  <td>
    <img width="100%" src="https://raw.githubusercontent.com/sorinbotirla/parkside-telemeter-esp32cam-rangefinder/refs/heads/main/images/20251116_204115.jpg" />
  <td>
    <img width="100%" src="https://raw.githubusercontent.com/sorinbotirla/parkside-telemeter-esp32cam-rangefinder/refs/heads/main/images/20251116_204118.jpg" />
  </td>
</table>
<br />
<br />
I have added a TP4056 and a step up converter for powering up using a 3.7V Li-Ion Battery. It outputs 5V 1A, enough for both esp32-cam, and the display. <br />
I also added a switch for turning it on and off, a switch for putting both esp32-cam in download mode by shorting GPIO0 to GND. GPIO0 is connected between each esp32-cam. Also they share GND. <br />
<br />
<table>
  <td>
    <img width="100%" src="https://raw.githubusercontent.com/sorinbotirla/parkside-telemeter-esp32cam-rangefinder/refs/heads/main/images/20251116_204052.jpg" />
  <td>
    <img width="100%" src="https://raw.githubusercontent.com/sorinbotirla/parkside-telemeter-esp32cam-rangefinder/refs/heads/main/images/20251116_204106.jpg" />
  </td>
</table>
<br />
<br />
I added the ILI9341 display to the first ESP32-cam (the one with the camera). And I checked everything works as intended.
<br />
<br />
<table>
  <td>
    <img width="100%" src="https://raw.githubusercontent.com/sorinbotirla/parkside-telemeter-esp32cam-rangefinder/refs/heads/main/images/20251116_204439.jpg" />
  <td>
    <img width="100%" src="https://raw.githubusercontent.com/sorinbotirla/parkside-telemeter-esp32cam-rangefinder/refs/heads/main/images/20251117_014612.jpg" />
  </td>
</table>
<br />
<br />
In the telemeter.ino code you will also see a button being used to trigger measurements from the esp32. You can add it or rely on the telemeter's measure button.<br />

## First prototype
<table>
  <td>
    <img width="100%" src="https://raw.githubusercontent.com/sorinbotirla/parkside-telemeter-esp32cam-rangefinder/refs/heads/main/images/20251119_194756.jpg" />
  <td>
    <img width="100%" src="https://raw.githubusercontent.com/sorinbotirla/parkside-telemeter-esp32cam-rangefinder/refs/heads/main/images/20251119_194658.jpg" />
  </td>
</table>
<table>
  <td>
    <img width="100%" src="https://raw.githubusercontent.com/sorinbotirla/parkside-telemeter-esp32cam-rangefinder/refs/heads/main/images/20251119_194611.jpg" />
  <td>
    <img width="100%" src="https://raw.githubusercontent.com/sorinbotirla/parkside-telemeter-esp32cam-rangefinder/refs/heads/main/images/20251119_194619.jpg" />
  </td>
</table>
<br />
<br />
Have fun :)
