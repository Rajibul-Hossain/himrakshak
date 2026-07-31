# HIMRAKSHAK

**Modular High-Altitude Soldier Monitoring and Communication System**

## Overview
Himrakshak is an offline, infrastructure-independent soldier monitoring, tracking, and communication platform designed for extreme high-altitude environments (e.g., Siachen Glacier, LAC). It solves the critical problem of soldier location loss during avalanches, whiteouts, and communication-denied operations. 

Operating entirely independently of cellular, satellite, and internet infrastructure, Himrakshak utilizes a self-healing LoRa mesh network, edge-computed biometric authentication, and an autonomous drone relay fallback to ensure continuous data transmission of GPS coordinates, vital signs, and encrypted communications.

---

## Key Features
*   **Infrastructure-Independent Mesh Network:** Built on LoRa (SX1262), enabling node-to-node relay without central towers.
*   **Real-Time Tracking & Vitals:** Continuous monitoring of GPS (historical and live), SpO2, heart rate, barometric altitude (avalanche detection), and ambient gas levels (CO/O2).
*   **Autonomous Drone Relay:** Automated Pixhawk-based MAVLink drone deployment triggered by mesh signal degradation to act as an airborne relay station.
*   **Military-Grade Security:** AES-256 encrypted hex payloads with local key storage and NFC-based glove-integrated biometric locks.
*   **Extreme Climate Engineering:** Powered by Li-SOCl2 batteries and solar harvesting to operate continuously in temperatures dropping below −50°C.
*   **Air-Gapped Command Dashboard:** Local Raspberry Pi-based server rendering an offline, translucent, high-visibility UI for commanding officers.

---

## System Architecture

### 1. The Soldier Unit (Edge Node)
A modular, compact wearable device carried by deployed personnel.
*   **Microcontroller:** ESP32
*   **Transceiver:** SX1262 LoRa Module
*   **Sensors:** NEO-M8N (GPS), MAX30102 (Pulse/SpO2), BMP388 (Barometric Altimeter), MQ-7/MQ-8 (Gas)
*   **Power:** Li-SOCl2 primary backup + Integrated Solar Panel

### 2. The Base Unit (Command Server)
An offline, localized server operated by the duty official at the forward post.
*   **Hardware:** Raspberry Pi 4 Model B + LoRa Receiver Node
*   **Backend:** Node.js, SQLite (for historical trajectory logging)
*   **Frontend:** Vanilla JavaScript, HTML, CSS (Custom fluid, glassmorphism UI for high-contrast visibility)
*   **Mapping:** Pre-cached offline local map tiles

### 3. Autonomous Drone Relay (Airborne Node)
A custom-built, high-altitude UAV that bridges signal gaps.
*   **Flight Controller:** Pixhawk (ArduPilot)
*   **Companion Computer:** Raspberry Pi Zero W + LoRa Module
*   **Autonomy:** Python script utilizing DroneKit/MAVLink for RSSI-triggered auto-launch and loiter.

---

## Repository Structure

```text
himrakshak/
├── firmware/
│   ├── soldier_node/       # ESP32 C++ firmware (Sensors, AES encryption, LoRa Mesh)
│   └── base_receiver/      # ESP32 C++ firmware (Serial bridge to Raspberry Pi)
├── server/
│   ├── backend/            # Node.js server, Decryption logic, SQLite DB setup
│   └── frontend/           # HTML/CSS/JS dashboard, offline map assets
├── drone_autonomy/
│   └── relay_logic.py      # MAVLink/DroneKit Python scripts for RSSI monitoring
├── hardware_schematics/    # PCB designs and 3D printable enclosures (STL)
└── docs/                   # Detailed protocol specifications and deployment manuals