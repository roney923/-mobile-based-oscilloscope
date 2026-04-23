# ⚡ OsciMobile – Mobile-Based Smart Oscilloscope

🚀 A portable, low-cost, wireless oscilloscope built using **ESP32**, capable of real-time waveform visualization on a web browser and TFT display.

---

## 📌 Overview

**OsciMobile** is a smart IoT-based oscilloscope that measures:

* ⚡ AC Voltage (ZMPT101B)
* 🔋 DC Voltage (Op-Amp Divider)
* 🔌 Current (ACS712)

It processes signals using the ESP32 and streams real-time data wirelessly to a **web dashboard**, eliminating the need for expensive oscilloscopes.

---

## 🧠 Key Features

* 📡 **Wireless Data Transmission** (UDP + WebSocket)
* 📊 **Real-Time Waveform Visualization**
* 📈 **Live Metrics Display**:

  * Voltage (DC)
  * Current
  * Power
  * RMS Voltage
* 🎯 **Adjustable Trigger Level**
* 🔍 **Zoom & Auto Scaling**
* ⏸️ **Freeze / Record Mode**
* 📁 **CSV Export for Data Analysis**
* 📉 **Statistics Tracking** (Min, Max, Peak)
* ⚠️ **Fault Detection System**
* 🌐 **Works on Any Browser (No App Required)**

---

## 🏗️ System Architecture

* **ESP32 (Dual Core)**

  * Core 1 → Signal Acquisition + Display
  * Core 0 → WiFi + Data Transmission

* **Sensors Used**:

  * ZMPT101B → AC Voltage
  * ACS712 → Current
  * LMH6644 Circuit → DC Voltage

* **Display**:

  * TFT (ILI9341)
  * Web Dashboard (Chart.js based UI)

---

## 🔌 Hardware Setup

### Components

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

The dashboard provides:

* Dual waveform (Voltage + Current)
* RMS calculation
* Power calculation
* Trigger line adjustment
* Event logging
* Live/Demo mode auto switching

👉 Built using **HTML + Chart.js + WebSocket** 

---

## 🔄 Data Flow

```
Sensors → ESP32 ADC → Processing → WiFi → Node.js Server → Web Dashboard
```

---

## 📡 Communication

* ESP32 → Server → **UDP (Port 8888)**
* Server → Browser → **WebSocket (Port 8080)** 

---

## 📊 Sample Output

* Voltage (DC)
* Current (A)
* Power (W)
* RMS Voltage

Displayed in real-time with waveform graphs.

---

## 🚀 How to Run

### 1. Upload ESP32 Code

* Open Arduino IDE
* Select ESP32 board
* Upload `.ino` file

### 2. Run Server

```bash
node server.js
```

### 3. Open Dashboard

* Open `index.html` in browser

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
* PCB design upgrade

---

## 💰 Cost

Approximate BOM Cost: **₹2000 – ₹5000**

---

## 👨‍💻 Author

**Rohit Kumar Sahu**
NIT Rourkela
Electrical Engineering

---

## ⭐ Acknowledgment

Developed as part of **Product Development Lab Project (EE3670)**

---

## 📜 License

Open-source for educational use 🚀

---
