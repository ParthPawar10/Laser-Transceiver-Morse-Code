#  Laser Transceiver Morse Code System

A communication system using **Arduino**, **laser modules**, and **photoresistors** to send and decode Morse code messages via light signals. 
This project bridges old-school Morse code with modern embedded systems for wireless optical communication.

##  Overview

This project demonstrates how to transmit text using Morse code over a laser beam and decode it using an Arduino-based receiver. It's ideal for educational, hobbyist, and backup communication applications where low power and high security are priorities.

## Features

- Arduino-based transmitter and receiver
- Real-time encoding/decoding of Morse code
- Adaptive light thresholding for noise resistance
- Interrupt-driven data capture for higher accuracy
- Visual debugging via serial monitor

##  System Architecture

1. **Transmitter**:
   - Arduino with a laser module
   - Encodes characters into Morse code
   - Blinks laser on/off to send dot/dash signals

2. **Receiver**:
   - Arduino with a photoresistor (LDR)
   - Decodes laser blinks into Morse symbols
   - Reconstructs readable characters using a binary tree

##  Hardware Required

- 2× Arduino Uno/Nano
- 1× KY-008 Laser Module
- 1× LDR (Photoresistor)
- 1× 1KΩ Resistor
- Breadboard, wires, USB cables

## Software Components

- **Encoder Sketch** (`encoder/encoder.ino`): Converts string to Morse and flashes laser
- **Decoder Sketch** (`decoder/decoder.ino`): Detects light signals and decodes Morse
- **Logic Processor**: Adjusts for ambient light changes dynamically
- **Interrupt Service Routine**: Captures signal transitions with millisecond precision
- **Morse Tree Decoder**: Translates signals into characters using a binary tree

##  File Structure

| Folder         | Description |
|----------------|-------------|
| `encoder/`     | Code for transmitting Morse code |
| `decoder/`     | Code for receiving and decoding Morse |
| `diagrams/`    | System flowcharts and hardware schematics |
| `docs/`        | Research paper and documentation |
| `images/`      | Prototype images and visuals |

##  Performance

- 📡 **Range**: ~300 meters (line of sight)
- 💬 **Speed**: ~80 WPM (Words Per Minute)
- 🔐 **Security**: Laser beam minimizes signal leakage
- ⚡ **Power**: Ultra-low power usage

##  Use Cases

- Disaster response communications
- Learning Morse code with real hardware
- Low-cost remote sensing & signaling
- Hobbyist or student electronics projects

##  Limitations

- Performance depends on environmental light conditions
- Line-of-sight required for signal transmission
- Basic version lacks encryption or robust error correction

## Future Improvements

- Range extension via beam focusing or relays
- Secure transmission using encryption
- Mobile app interface for control and display
- AI-based error correction and adaptive encoding

## Documentation

Find the full research write-up in reaserch paper, including methodology, analysis, and references.

## Authors

- Parth Pawar     
- Manish  


B.Tech CSE, MIT — Semester IV


