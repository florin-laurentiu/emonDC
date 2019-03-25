## **emonDC**

emonDC is a project aiming to develop DC current and voltage measuring tools compatible with openenergymonitor.org project.<br>
The main board in development is emonDCduo, a dual-channel generator/battery monitor, WiFi enabled, aimed at solar/battery systems.

###### Project status: Development units available for sale. Crowd-funding application in development.
  <br/>

![Example System Schematic](/images/emonDC_system.jpg)


### Target use:

- 48V Solar PV systems.
- Battery monitoring.
- Any generator / battery system up to 64.4V.

### Features

Latest spec:

- Two-channel high side **only** DC current and voltage sensing, up to 64.4V or 75V, suitable for a wide range of low-voltage generator/battery systems.
- 50Amps onboard shunt rating or 1000A external shunt rating.
- Waterproof, clear-top plastic case.
- OLED display.
- WiFi Connectivity.
- Low-power mode (10mA draw).
- Accurate 12-bit ADC.

Also:
- It can function as independent unit, self-powered from either the generator or battery, using a buck-regulator module, storing the data to a local MicroSD card, keeping time with on-board RTC and coin-cell.
- Radio transmission of data possible through RFM69Pi module.
- Voltage divisions for reference voltages easily modified by soldering for maximum resolution for given system.
- The I2C port can connect control boards for power management, in development.
- Skipping wireless transmission, a physical cable UART connection is possible, compatible with RS485 pending testing.

### Hardware Options

emonDCduo as designed here has shunt, termination, power-supply and other module options.<br>
[See this quick demo video of the options](https://vimeo.com/319164161) and listed below in detail.

##### The Shunt
Onboard @ 50Amps max  ||  External @ 1000Amps max.<br>
Onboard shunts should be selected to not dissipate more than 50% of their rated value continuously [shunt power calculator](https://github.com/danbates2/emonDC/tree/master/calculators).

##### Terminations for Onboard Shunts
- 10.16mm Pitch Terminal Block Connector, such as [this](https://uk.farnell.com/2668653).
- 7.62mm Pitch Terminal Block Connector, such as [this](https://uk.farnell.com/2579818).
- 5mm ring terminal bolted termination. Ring terminal outer-diameter not exceeding 11.5mm.

##### Terminations for External Shunts
- Any of the above, 7.62mm Pitch Terminal Block recommended for simplicity.

##### Power Supply Options
- 5.08mm or 5.00mm Pitch terminal block for 5V direct input. I could not enable a USB connection for the safety reasons in floating-ground systems.
- Buck regulator module mounting, made compatible with [LM2596 modules on ebay](https://github.com/danbates2/emonDC/blob/master/images/LM2596_dc-dc_buck_module.jpg), or my own modules sold for a bit more. The buck module is jumper connected to one of the current sense channels. Note the external shunt option and buck module combination has not been tested, but should work fine theoretically.

##### Module Options
**I2C modules:**
- OLED Display cycles through real-time and daily data, including network information if connected.
- RTC based on PCF8523, with coin cell backup.
- Any I2C control module for power management.
<br>

And..
- RFM69Pi radio module, typically 433MHz.

<br>

#### Other Customisation
<details>
<summary>Reference voltage and line voltage divider resistors.</summary>
<br>
![Reference voltage and line voltage divider resistors.](/images/v3.6_overlay_resistors.jpg)
<details>

<details>
<summary>RFM69Pi headers</summary>
<br>
![RFM69Pi headers](/images/v3.6_overlay_rfm.jpg)
<details>

<details>
<summary>Power Select header, for bringing power into an on-board buck regulator.</summary>
<br>
![Power Select header, for bringing power into an on-board buck regulator.](/images/v3.6_overlay_pwr.jpg)
<details>

<details>
<summary>I2C port for the OLED or other expansions.</summary>
<br>
![I2C port for the OLED or other expansions.](/images/v3.6_overlay_i2c.jpg)
<details>

<details>
<summary>GND connections, for the power-supply earth and shunt-lead shielding, if used.</summary>
<br>
![GND connections, for the power-supply earth and shunt-lead shielding, if used.](/images/v3.6_overlay_gnd.jpg)

<details>

<details>
<summary>5V input location</summary>
<br>
![5V input location](/images/v3.6_overlay_5v.jpg)
<details>

<details>
<summary>Shunt monitor reference select headers.</summary>
<br>
![Shunt monitor reference select headers.](/images/v3.6_overlay_refsel.jpg)
<details>


### Measurement of direct current (DC).

This is a project description but also a collection of ideas and findings.
The diversity of approaches could be illustrated by these two websites:
- https://www.coolcomponents.co.uk/attopilot-voltage-and-current-sense-breakout-180a.html
- http://www.victronenergy.com/upload/documents/Datasheet-Battery-monitoring-EN.pdf

### Safety

Firstly, isolation! The protection of users and connected devices from floating ground voltages in certain applications is very important. The approach at present is to make use of suitable physical isolation and warning labels. Bright warning labels stating ‘disconnect DC system before connecting UART’ or ‘ensure common ground between DC system and other devices to be connected’ or something similar, pending advice.
<br/>

Secondly, short circuit protection needs to exist at different levels of the hardware design to avoid inadvertent damage to the unit, by mal-assembly on part of the manufacturer (PCB and assembly) or assembler, or the user upon installation. This can be achieved with adding fuses and buffering certain inputs with resistors. The layout of the components factors into the safety significantly!
<br/>

Thirdly, transient and reverse voltage protection. The TVS diodes specified seem to do a good job. I've taken a sparker from a cigarette lighter and shoved a few kV into the inputs while it was running. It worked just fine, after a reset.
<br/>

The unit is defined as a Protected Extra-Low Voltage (PELV) device, and should be, if all goes well, certified for up to 75VDC by European and 110VDC by International regulations.


### Approach
Hall-effect DC monitoring ICs are simple and provide a degree of electrical isolation, however, they are inflexible and costly. This will change in time as the technology develops.
<br/>

Shunt monitoring potentially provides greater flexibility, accuracy and cost-effectiveness.

1. The range of requirements in DC monitoring applications require a flexible approach because of:<br/>
a. Unidirectional vs bidirectional measurement needs.<br/>
b. Amperage ranges and associated cable cross sectional area. Current carriers in DC systems can range from 4mm<sup>2</sup> to over 25mm<sup>2</sup>. The target measurement range has significant physical design implications.<br/>
c. Different cable dimensions require terminations suitable to their size and application (screw terminals, bolt ring terminals, soldered connections, etc.) there is no single solution.<br/>
d. Whole battery systems can be monitored with one shunt or individual cells of the system can require a multi-cell monitoring unit.<br/>
e. There are also high humidity, marine and automotive applications to consider in the future.<br/>

2. When I consider cable sizes and suitable cable terminations, the design challenge of making a suitable unit for a wide current range makes it difficult to see any one PCB realistically and cost-effectively meeting ALL requirements. Unlike in many AC requirements where a different CT and burden resistor can be selected, DC demands a more targeted approach due to the different physical dimensions of suitable terminations.
This possibly leads to the solution of designing several different boards according to the different requirements.
I'm designed emonDCduo to be an alrounder as much as possible, with the option of external shunts for amperages up to 1000A.

The approach selected for first production has been dubbed emonDCduo, onboard shunts and pheonix connectors for external shunts ensure flexibility.

3. Another option might be to have a two-part system where Tx functions are separated from shunt monitoring functions in two separate PCBs, connected via RJ11 or ribbon cable. This could lend itself to multi-source monitoring effectively. Also, galvanic isolation would become easier. Also, the standalone Tx PCB may be compatible with other emon projects in the future.

4. Given the nature of solar, wind and various DC applications there a requirement for a buck regulating supply, probably isolated. There needs to be a regulated and smooth output for powering the shunt monitor and other ICs.

5. Failsafe monitoring... Fuses between the shunt and monitor to protect from short circuit conditions.

6. High-side vs. low-side sensing… This shunt monitor needs to be capable of handling both applications, which means using a shunt monitor IC capable of high-side measurement. The device should ideally handle a wide common-mode voltage range, say up to 80V, and protection be in place for voltage over the common-mode voltage limit of the chip.

### Project Aims

Create as flexible as possible unit. Aiming primarily at domestic solar, wind and battery applications.
Compare hall-effect type to shunt monitor type DC measuring approaches, particularly in relation to safety, accuracy and flexibility.
Integrate wireless Tx function using RFM69pi and ESP8266.
Explore potential for AC monitoring applications.
To ascertain for certain the viability or necessity of a two part system. Such as Part A: Tx PCB and Part B: DC monitoring shield.
Investigate a calibration routine.
Look at case design.
Look at the energy efficiency of different manufacturing approaches.
Make a unit!


Questions…
What’s the relationship to different amperages, cables sizes and suitable cable terminations at the device?
Would a particular amperage range require soldered connection? Do specific applications require soldered connections?
Could the PCB safely handle the required amps? What’s the current carrying capacity of header pins and PCB track?
Can PCB track be used as the shunt?
How does heat dissipate through the PCB and how does this effect the value being monitored? Is a temperature sensor required for factoring in temperature drift?
How much responsibility does the installer have in installing correct cable terminations for this board? What are the divisions of responsibility?

Several spinoff boards have derived of this project, in various stages of completion, the latest being the ACS770 or 772 series hall-effect current sensor breakout board. Also, an isolated FTDI adaptor, emonDCmini targeted at single channel DC monitoring with WiFi capabilities, and an emonDC shield for Arduino pattern.


### Licence
- Hardware licensed according to the TAPR Open Hardware Licence v1.0. Documentation free to use and change.
https://www.tapr.org/TAPR_Open_Hardware_License_v1.0.txt
- Software licensed licensed to GNU General Public License v3. Free to use and change.
https://www.gnu.org/licenses/gpl.html
