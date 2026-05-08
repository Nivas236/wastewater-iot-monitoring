# 🌊 Cloud-Based IoT System for Real-Time Monitoring and Filtration of Industrial Wastewater

![Project Banner](https://img.shields.io/badge/Platform-ESP8266%20%7C%20Arduino-blue?style=for-the-badge)
![IoT](https://img.shields.io/badge/IoT-ThingSpeak-green?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Completed-brightgreen?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)

> **Final Year Project** — B.Tech in Electronics and Communication Engineering  
> Andhra Loyola Institute of Engineering and Technology, Vijayawada (2022–2026)  
> Affiliated to JNTUK, Kakinada

---

## 📌 Project Overview

Industrial wastewater discharge is one of the leading causes of water pollution globally. This project presents a **cloud-based IoT solution** that monitors industrial wastewater quality in real-time using multiple sensors, triggers automated filtration when parameters exceed safe thresholds, and logs all data to the **ThingSpeak cloud dashboard** for remote access and analysis.

The system enables **multi-purpose reuse** of treated water — for irrigation, cooling, or other industrial applications — reducing environmental harm and promoting sustainable water management.

---

## 👥 Team Members

| Name | Roll Number |
|------|-------------|
|  M. Nivas| 22HP1A04A8 |
|SK. Kalesha Vali | 22HP1A0499 |
| CH. R. V. Durga Sai | 22HP1A04B2 |
| N. Sanjeeva Reddy | 22HP1A04B8 |

**Guide:** Mr. G. Vijay Kumar, M.Tech., (Ph.D) — Assistant Professor, Dept. of ECE  
**HOD:** Dr. T. Lakshmi Narayana, M.Tech., Ph.D.

---

## 🎯 Objectives

- Monitor **pH**, **turbidity**, **temperature**, and **TDS** of industrial wastewater in real time
- Trigger **automated filtration** when any parameter crosses a safe threshold
- Send **Wi-Fi alert notifications** when water quality is unsafe
- Display readings on **LCD** and push data to **ThingSpeak cloud**
- Enable **remote caregiver/admin access** via web dashboard

---

## 🏗️ System Architecture

```
Sensors (pH, Turbidity, Temp, TDS)
         │
         ▼
   Arduino Uno  ◄──── Power Supply
         │
         ▼
   ESP8266 Wi-Fi Module
         │
    ┌────┴────┐
    │         │
    ▼         ▼
LCD Display  ThingSpeak Cloud Dashboard
                    │
                    ▼
           Remote Monitoring (Browser/App)
                    │
              ┌─────┴──────┐
              │            │
              ▼            ▼
       Wi-Fi Alerts   Water Pump (Filtration)
```

---

## 🔧 Hardware Requirements

| Component | Purpose |
|-----------|---------|
| Arduino Uno | Main microcontroller |
| ESP8266 Wi-Fi Module | Cloud connectivity |
| pH Sensor | Measures acidity/alkalinity of water |
| Turbidity Sensor | Measures water clarity/suspended particles |
| DS18B20 Probe Temperature Sensor | Measures water temperature |
| TDS Meter | Measures Total Dissolved Solids |
| LCD Display (16×2) | Real-time local display of readings |
| Water Pump Motor | Automated filtration control |
| Buzzer | Local alert when threshold is exceeded |
| Power Supply | 5V/12V regulated supply |

---

## 💻 Software Requirements

| Tool | Purpose |
|------|---------|
| Arduino IDE | Writing and uploading firmware |
| ThingSpeak | Cloud IoT dashboard & data logging |
| MATLAB (ThingSpeak Analytics) | Data visualization and analysis |

---

## 📊 Monitored Parameters & Safe Thresholds

| Parameter | Safe Range | Action if Exceeded |
|-----------|-----------|-------------------|
| pH | 6.5 – 8.5 | Activate filtration + alert |
| Turbidity | < 4 NTU | Activate filtration + alert |
| Temperature | 10°C – 40°C | Alert notification |
| TDS | < 500 mg/L | Activate filtration + alert |

---

## 📁 Project Structure

```
wastewater-iot/
├── src/
│   ├── main.ino              # Main Arduino firmware
│   ├── sensors.ino           # Sensor reading functions
│   ├── thingspeak.ino        # ThingSpeak cloud upload
│   └── filtration.ino        # Pump & buzzer control logic
├── hardware/
│   ├── circuit_diagram.png   # Full circuit diagram
│   └── block_diagram.png     # System block diagram
├── docs/
│   └── project_report.pdf    # Full project report
├── data/
│   └── sample_readings.csv   # Sample sensor data
├── images/
│   ├── setup.jpg             # Hardware setup photos
│   └── thingspeak_dashboard.png  # Cloud dashboard screenshot
└── README.md
```

---

## ⚙️ Setup & Installation

### 1. Clone the Repository
```bash
git clone https://github.com/Nivas236/wastewater-iot-monitoring.git
cd wastewater-iot-monitoring
```

### 2. Install Arduino IDE
Download from: https://www.arduino.cc/en/software

### 3. Install Required Libraries
In Arduino IDE → Tools → Manage Libraries, install:
- `OneWire` — for DS18B20 temperature sensor
- `DallasTemperature` — temperature sensor library
- `LiquidCrystal` — LCD display
- `ESP8266WiFi` — Wi-Fi connectivity
- `ThingSpeak` — ThingSpeak cloud API

### 4. Configure ThingSpeak
1. Create a free account at https://thingspeak.com
2. Create a new **Channel** with 4 fields: pH, Turbidity, Temperature, TDS
3. Copy your **Channel ID** and **Write API Key**
4. Update in `src/thingspeak.ino`:
```cpp
unsigned long myChannelNumber = YOUR_CHANNEL_ID;
const char * myWriteAPIKey = "YOUR_WRITE_API_KEY";
```

### 5. Configure Wi-Fi Credentials
In `src/main.ino`:
```cpp
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
```

### 6. Upload Firmware
- Connect Arduino Uno via USB
- Select Board: **Arduino Uno** and correct COM Port
- Click **Upload**

---

## 📈 Results
## 📹 Demo Video ▶️ Watch the full project working live: [Click here to watch — Live Filtration Demo](https://drive.google.com/file/d/1PdVjUQTq6wb-R-Iy_n0IpBOlFMqUvAyF/view?usp=drivesdk) **Video shows:** - Live sensor readings on LCD (pH, Turbidity, Temp, TDS) - Automatic pump activation when water is unsafe - ThingSpeak dashboard updating in real time
- Real-time sensor data successfully streamed to ThingSpeak every 15 seconds
- Automated pump activation triggered correctly when thresholds were exceeded
- LCD displayed live readings with clear parameter labels
- Remote monitoring validated via ThingSpeak web dashboard
- Wi-Fi alert notifications sent successfully on threshold breach

---

## 🌱 Applications

- Industrial effluent treatment plants
- Agricultural irrigation water reuse
- Municipal wastewater monitoring stations
- Smart city water quality management
- Research and academic IoT demonstrations

---

## 🔮 Future Scope

- Integration with **mobile app** (Android/iOS) for real-time alerts
- Adding **turbidity camera** for visual water quality assessment
- **Machine learning** prediction of water quality trends
- Solar-powered system for remote/rural deployments
- Multi-node sensor network for large industrial plants

---

## 📄 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

---

## 🙏 Acknowledgements

We sincerely thank **Mr. G. Vijay Kumar** for his invaluable guidance, and **Dr. T. Lakshmi Narayana** (HOD, ECE) for constant support. We also thank the management of **ALIET, Vijayawada** for providing excellent laboratory facilities.

---

<p align="center">Made with ❤️ by Team 22HP1A04A8 | ALIET ECE 2022–2026</p>
