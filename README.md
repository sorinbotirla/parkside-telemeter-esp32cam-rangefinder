# Building a rangefinder made of Parkside PLEM-50-C3 laser telemeter and ESP32-CAM
A simple rangefinder having the Parkside PLEM-50-C3 laser telemeter, a pair of binoculars, an ILI9341 2.8 inch Display and 2x ESP32-CAM to measure distance exactly on the right spot.


## Pinout

<table border="1" cellpadding="6" cellspacing="0">
  <tr>
    <th>ILI9341 Display</th>
    <th>ESP32-CAM (Display/Camera)</th>
    <th>ESP32-CAM (Telemeter)</th>
    <th>Parkside Telemeter</th>
  </tr>

  <tr><td>VCC</td><td>5V</td><td></td><td></td></tr>

  <tr><td>GND</td><td>GND</td><td>GND</td><td>GND (Pin 1)</td></tr>

  <tr><td>MOSI</td><td>23</td><td></td><td></td></tr>
  <tr><td>MISO</td><td>19</td><td></td><td></td></tr>
  <tr><td>SCK</td><td>18</td><td></td><td></td></tr>
  <tr><td>CS</td><td>17</td><td></td><td></td></tr>
  <tr><td>DC</td><td>16</td><td></td><td></td></tr>
  <tr><td>RST</td><td>5</td><td></td><td></td></tr>
  <tr><td>BL</td><td>15</td><td></td><td></td></tr>

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

  <tr>
    <td></td>
    <td></td>
    <td>15 (POWER)</td>
    <td>PWR (Pin 19)</td>
  </tr>

  <tr>
    <td></td>
    <td></td>
    <td>3.3V</td>
    <td>VBAT (3.0 V)</td>
  </tr>

  <tr>
    <td></td>
    <td></td>
    <td>GND</td>
    <td>GND (Pin 1)</td>
  </tr>
</table>

Due to the lack of gpio pins and not having a gpio extender, 2x esp-cam boards have been used. In this repository are 2 .ino files, cameradisplay.ino is for the esp-cam handling the camera and the display, telemeter.ino is for the esp-cam handling the telemeter data.

More info to be added soon (wiring the telemeter, using the binoculars, 3d models for printing the case and mount, etc)
