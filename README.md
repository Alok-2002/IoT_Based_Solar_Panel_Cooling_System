# 🌞 IoT-Based Solar Panel Cooling System

![Platform](https://img.shields.io/badge/Platform-ESP8266-blue?style=flat-square)
![Sensors](https://img.shields.io/badge/Sensors-DHT11%2C%20DS18B20-orange?style=flat-square)
![Cloud](https://img.shields.io/badge/Cloud-AWS%20IoT%20Core%20%26%20Blynk-green?style=flat-square)
![Language](https://img.shields.io/badge/Language-C%2B%2B-ff69b4?style=flat-square)
![License](https://img.shields.io/badge/License-MIT-brightgreen?style=flat-square)
![Status](https://img.shields.io/badge/Project-Working%20Prototype-success?style=flat-square)

---

## 📌 Project Overview

The **IoT-Based Solar Panel Cooling System** is an innovative smart system aimed at improving the efficiency of solar panels by automatically activating a water pump to cool the panel when the surface temperature exceeds 25°C. This ensures that the solar panel operates closer to its optimal temperature, thus enhancing energy output.

The system uses sensors to monitor:
- Surface temperature of the panel (DS18B20)
- Ambient temperature and humidity (DHT11)
- Voltage and current to calculate output power

Collected data is:
- **Published to AWS IoT Core** using MQTT for cloud logging and future ML analysis
- **Monitored in real-time on Blynk Cloud Dashboard** via mobile or web

---

## 👨‍💻 Project Team

- **Alok Sharma**
- **Sanskriti Yadav**

Developed as part of our **Minor Project** during our college coursework.

---

## 🔧 Hardware Components

| Component                          | Quantity | Description                                |
|-----------------------------------|----------|--------------------------------------------|
| ESP8266                           | 1        | Microcontroller for IoT connectivity        |
| DS18B20 Temperature Sensor        | 1        | Surface temperature sensing                 |
| DHT11 Temperature & Humidity      | 1        | Ambient temperature & humidity              |
| DC Water Pump                     | 1        | For cooling panel                           |
| Relay Module                      | 1        | For switching pump                          |
| Charging Protection Module        | 1        | Battery protection circuit                  |
| SPST Switch                       | 1        | Power control                               |
| Digital Volt/Amp Meter            | 1        | To monitor solar panel output               |
| Power Booster Module              | 1        | Boost battery voltage as needed             |
| Li-ion Battery                    | 1        | Power source                                |

---

## 📈 Working Principle

1. **Read Temperature**:
   - DS18B20 senses panel surface temperature
   - DHT11 measures environmental temperature & humidity

2. **Decision Logic**:
   - If panel temperature > 25°C → pump is activated via relay
   - If panel temperature ≤ 25°C → pump is turned off

3. **Power Monitoring**:
   - Read voltage from analog pin (scaled)
   - Fixed or calculated current used to estimate **power = voltage × current**

4. **Data Logging**:
   - Data sent to **AWS IoT Core (MQTT)** for cloud storage
   - Real-time updates to **Blynk Cloud Dashboard** for visualization

5. **Future Expansion**:
   - Logged data downloadable in CSV format for **machine learning analysis**

---

## 🧠 Technologies Used

- **ESP8266 (NodeMCU)**
- **AWS IoT Core (MQTT Protocol)**
- **Blynk Cloud** (Web + Mobile App)
- **Arduino IDE**
- **Digital Sensors (DS18B20, DHT11)**
- **Relay and Actuator control**

---

## 🖥️ Blynk Dashboard Layout

- **V0** → Panel Temperature
- **V1** → Ambient Temperature
- **V2** → Humidity
- **V3** → Voltage
- **V4** → Current
- **V5** → Power Output
- **V6** → Pump Status (ON/OFF LED)

Visual Example:

![blynk](https://github.com/user-attachments/assets/e6bf0d23-d513-4f86-8e0c-9c7de8de0ead)


---

## 🔐 AWS IoT Integration

1. Setup Thing, Certificates, and Policy
2. Attach policy to certificate
3. Copy and paste your AWS credentials into the Arduino sketch:
   ```cpp
   static const char AWS_ROOT_CA[] PROGMEM = R"EOF(...certificate...)EOF";
   static const char DEVICE_CERT[] PROGMEM = R"KEY(...certificate...)KEY";
   static const char DEVICE_PRIVATE_KEY[] PROGMEM = R"KEY(...privatekey...)KEY";
   ```
4. MQTT Topic Used: `solar/panel/data`

---

## 📂 Data Sample Format (Sent to AWS & Future ML Use)
```json
{
  "panel_temp": 33.5,
  "env_temp": 30.2,
  "humidity": 67.8,
  "pump_status": "ON",
  "voltage": 20.1,
  "current": 1.2,
  "power": 24.12
}
```

---

## 🧪 Testing and Results

- Cooling system effectively reduced panel temperature by ~5-7°C
- Voltage and power readings showed improved performance during cooling
- Pump status reliably toggled based on threshold logic
- Real-time updates on Blynk and data persisted to AWS for post-analysis

---

## 💡 Future Improvements

- Use solar-powered pump and add energy harvesting logic
- Add ML model integration for predictive cooling
- Log panel efficiency gains in different seasons
- Connect to local database or Grafana dashboard

---

## 📜 License

This project is licensed under the [MIT License](LICENSE).

---

## 📞 Contact

Feel free to reach out:

- 📧 Mail : [Alok Sharma](mailto:sharmaalok02gwl@gmail.com)
- 💼 LinkedIn: [Alok Sharma](https://linkedin.com/in/alok-sharma2002)

---

> 🚀 *Built to enhance solar efficiency through smart automation and cloud intelligence.*
