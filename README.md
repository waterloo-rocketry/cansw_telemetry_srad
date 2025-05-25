# cansw_telemetry_srad

## Overview
RF transciever operating inside rocket. Will serve telemetry purposes by transmitting all messages recieved over RocketCAN outwards. Will also be able to receive messages from mission control such as commands normally transmitted over the CAN bus; acting as a redundant way to communicate with the rocket while railed. Planned to have two operating modes: 
- Ground mode: a high speed, high power consumption, high range; for while the rocket is on the ground and connected to external power
- Railed mode: low speed, low power consumption, low range; for while the rocket is on the ground and disconnected from external power
- Flight mode: low speed, high power consumption, high range; for while the rocket is in flight
  
** still haven't figured out all the software caveats for these three operating modes

## Hardware Architecture
The board is part of the borader Waterloo Rocketry RocketCAN Bus. Like most avionics on this bus, the central MCU is the PIC18 microcontroller. Following previous rocketry RF architecture, the board supoorts 2 antennas, one COTS other SRAD. The RF transciever (TRX) is the CC1200, which communicates to the PIC18 over SPI. An external power amplifier (PA), GRF5509, has been added in series with the TX pin of the TRX for increased output gain. There are two series switches in between the TRX output and the antennas, one for toggling between RX and TX mode while the other is for selecting an antenna.
![image](https://github.com/user-attachments/assets/bf5134b9-8183-4618-b704-2cc612554a7a)

## RF 
The board operates on the 902-928 MHz band (centre frequency 915 MHz). The output power is aproximatly 33dBm. The device requires a ISED HAM licence (or equivilant) to operate in Canada.

## Register Configurations
Registers can be configured using the SMARTRFTM-STUDIO [Windows only] application (https://www.ti.com/tool/SMARTRFTM-STUDIO). Once configured registers can be exported to a CSV. A python script will generate the C++ SPI transfer functions that can be flashed to the PIC18. When exporting register settings, select the `TrxEB RF Settings Value Line` format, which should generate the `registerSetting_t` used in the `CC1200.c` file, copy over the new settings and build the code.
