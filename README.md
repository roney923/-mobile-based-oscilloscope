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

**ESP32 (Dual Core FreeRTOS)**

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

| Function | GPIO |
|----------|------|
| AC Voltage | 34 |
| Current | 33 |
| DC Voltage | 32 |
| TFT MOSI | 13 |
| TFT MISO | 12 |
| TFT SCLK | 14 |
| TFT CS | 15 |
| TFT DC | 4 |
| TFT RST | 2 |

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

# 🚀 How to Run

## 📦 Prerequisites

Install the following software:

- Node.js (v18 or later)
- Python 3.x
- Arduino IDE
- ESP32 Board Package for Arduino IDE

Verify installation:

```bash
node -v
npm -v
python --version
```

---

## 📁 Project Structure

```
oscilloscope/
│
├── dashboard/
│   └── oscilloscope_dashboard.html
│
├── server/
│   ├── server.js
│   ├── package.json
│   └── node_modules/
│
├── firmware/
│   └── oscimobile.ino
│
└── docs/
```

---

## 📦 Install Dependencies (One Time)

Open a terminal inside the **server** folder.

```bash
cd server
npm install
```

---

## 🖥️ Demo Mode (Without ESP32)

1. Open `server/server.js`.
2. Enable Demo Mode:

```javascript
const DEMO_MODE = true;
```

(or set `MODE = "DEMO"` depending on your code.)

3. Start the Node.js server:

```bash
cd server
node server.js
```

4. Open another terminal and launch the dashboard:

```bash
cd dashboard
python -m http.server 8000
```

5. Open your browser and visit:

```
http://localhost:8000/oscilloscope_dashboard.html
```

The dashboard will display simulated real-time voltage and current waveforms.

---

## 📡 Live Mode (With ESP32)

1. Open `firmware/oscimobile.ino` using Arduino IDE.
2. Update the Wi-Fi credentials:

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
```

3. Find your computer's IP address:

```bash
ipconfig
```

Update the server IP inside the firmware:

```cpp
IPAddress serverIP(192,168,1,100);
```

Replace it with your computer's IPv4 address.

4. Select **ESP32 Dev Module**, choose the correct COM port, and upload the firmware.

5. Start the Node.js server:

```bash
cd server
node server.js
```

6. Launch the dashboard:

```bash
cd dashboard
python -m http.server 8000
```

7. Open:

```
http://localhost:8000/oscilloscope_dashboard.html
```

The dashboard will now display live measurements received from the ESP32.

> **Note:** The ESP32 and the computer must be connected to the same Wi-Fi network.

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
