# ⚡ OsciMobile – Mobile-Based Smart Oscilloscope

🚀 A portable, low-cost, wireless oscilloscope built using **ESP32**, capable of real-time waveform visualization on a web browser and TFT display.

---

## 📸 Project Preview

### 🔌 Circuit Diagram
<p align="center">
  <img src="https://raw.githubusercontent.com/roney923/-mobile-based-oscilloscope/main/oscilloscope/docs/circuit.jpg" width="650"/>
</p>

### 🖥️ Web Dashboard Output
<p align="center">
  <img src="https://raw.githubusercontent.com/roney923/-mobile-based-oscilloscope/main/oscilloscope/docs/display.jpeg" width="650"/>
</p>

### 🛠️ Hardware Prototype
<p align="center">
  <img src="https://raw.githubusercontent.com/roney923/-mobile-based-oscilloscope/main/oscilloscope/docs/product.jpeg" width="650"/>
</p>
---

## 📌 Overview

**OsciMobile** is a smart IoT-based oscilloscope that measures:

* ⚡ AC Voltage (ZMPT101B)
* 🔋 DC Voltage (LMH6644-based divider)
* 🔌 Current (ACS712)

It processes signals using the ESP32 and streams real-time data wirelessly to a **web dashboard**, eliminating the need for expensive oscilloscopes.

---

## 🧠 Key Features

* 📡 Wireless Data Transmission (UDP + WebSocket)
* 📊 Real-Time Waveform Visualization
* 📈 Live Metrics:

  * Voltage (DC)
  * Current (A)
  * Power (W)
  * RMS Voltage
* 🎯 Adjustable Trigger Level
* 🔍 Zoom & Auto Scaling
* ⏸️ Freeze / Record Mode
* 📁 CSV Export Support
* 📉 Statistics Tracking (Min, Max, Peak)
* ⚠️ Fault Detection System
* 🌐 Works on Any Browser (No App Required)

---

## 🏗️ System Architecture

```text
Sensors → ESP32 ADC → Processing → WiFi → Node.js Server → Web Dashboard
```

* **ESP32 (Dual Core FreeRTOS)**

  * Core 1 → Data Acquisition + TFT Display
  * Core 0 → WiFi Communication

---

## 🔌 Hardware Components

* ESP32 DevKit V1
* ZMPT101B AC Voltage Sensor
* ACS712 Current Sensor
* LMH6644 Op-Amp Circuit
* ILI9341 TFT Display
* LM2596 Buck Converter
* SMPS Power Supply

---

## ⚙️ Pin Configuration

| Function   | GPIO |
| ---------- | ---- |
| AC Voltage | 34   |
| Current    | 33   |
| DC Voltage | 32   |
| TFT MOSI   | 13   |
| TFT MISO   | 12   |
| TFT SCLK   | 14   |
| TFT CS     | 15   |
| TFT DC     | 4    |
| TFT RST    | 2    |

---

## 🌐 Web Dashboard

* Dual waveform (Voltage + Current)
* Real-time plotting using Chart.js
* Trigger line adjustment
* Event logging & fault alerts
* Auto LIVE / DEMO mode switching

---

## 📡 Communication

* ESP32 → Server → **UDP (Port 8888)**
* Server → Browser → **WebSocket (Port 8080)**

---

## 🚀 How to Run

### 1️⃣ Upload ESP32 Code

* Open Arduino IDE
* Select ESP32 board
* Upload firmware (`oscimobile.ino`)

### 2️⃣ Run Server

```bash
node server.js
```

### 3️⃣ Open Dashboard

* Open `oscilloscope_dashboard.html` in browser

---

## 🎯 Applications

* 📚 Educational Labs
* 🔧 Electrical Diagnostics
* 🌍 IoT Energy Monitoring
* 🧪 Signal Analysis

---

## 💡 Future Improvements

* Multi-channel oscilloscope
* Mobile app integration
* Cloud data logging
* Higher sampling rate
* Custom PCB design

---

## 💰 Cost

Approximate cost: **₹2000 – ₹5000**

---

## 👨‍💻 Author

**Rohit Kumar Sahu**
NIT Rourkela – Electrical Engineering

---

## ⭐ Acknowledgment

Developed as part of **Product Development Lab (EE3670)**

---

## 📜 License

Open-source for educational use 🚀
