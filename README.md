# D.E.B.R.I.S: Detection and Exploration Bot for Rescue, Investigation, and Survival

## Overview
D.E.B.R.I.S is an advanced, multi-sensor robotic platform designed to assist in rescue, investigation, and survival scenarios. By combining thermal imaging, environmental sensors, and artificial intelligence, the system addresses critical challenges in locating and rescuing survivors in disaster-stricken areas. The project is divided into three main parts:

1. **Embedded Systems**: Integration and control of sensors and hardware.
2. **Artificial Intelligence (AI)**: Detection and classification of human presence using cameras and thermal imaging.
3. **Web Development**: Real-time data visualization and management through a web interface.

This system is engineered for deployment in complex environments, enhancing rescue efficiency and reducing risks for responders.

---

## Features
- **Advanced Sensing Technologies:** Equipped with cameras, thermal sensors (AMG8833), DHT11, MQ2 gas sensor, microwave Doppler radar, and GPS.
- **Thermal Imaging:** Captures and processes 8x8 thermal data, upscaled to 64x64 for human classification.
- **Real-Time Data Transmission:** Transmits live data to a web server for visualization.
- **Human Detection Models:** Implements YOLOv8 for object detection and a CNN for thermal image classification.
- **User-Friendly Interface:** Displays live video, thermal images, and sensor readings on a responsive web dashboard.
- **Mobile Platform:** Features high-torque motors and large wheels for navigation across uneven terrain.

---

## System Architecture
1. **Embedded Systems:**
   - Integration of multiple sensors including AMG8833, DHT11, MQ2 gas sensor, microwave Doppler radar, and GPS.
   - Real-time data acquisition and transmission via ESP32-CAM.

2. **Artificial Intelligence:**
   - **Thermal Image Classification:** Uses a custom CNN for detecting human presence from AMG8833 data.
   - **Object Detection:** Employs YOLOv8 for real-time human detection using the ESP32-CAM.

3. **Web Development:**
   - Displays live camera feeds, thermal images, and environmental sensor readings.
   - Provides real-time location tracking through GPS.

---

## Hardware Components
- **AMG8833 Thermal Camera:** Detects infrared radiation for human presence detection.
- **ESP32-CAM:** Streams live video and integrates with YOLOv8 for real-time object detection.
- **DHT11 Sensor:** Measures temperature and humidity.
- **MQ2 Gas Sensor:** Detects combustible gases and smoke.
- **Microwave Doppler Radar (RCWL-0516):** Detects motion through frequency shifts.
- **NEO-6 GPS Module:** Tracks the robot’s location.
- **High-Torque Motors and Wheels:** Ensures mobility over rough terrain.
- **12V Lithium-ion Battery:** Provides reliable power with reverse polarity protection and filtration circuits.

---

## Software Stack
- **Programming Languages:** Python, C++
- **Deep Learning Frameworks:** PyTorch, TensorFlow
- **Object Detection:** YOLOv8 by Ultralytics
- **Web Frameworks:** Flask, HTML, CSS, JavaScript
- **Tools:** Visual Studio, ESP-IDF, Postman

---

## My Role
As the AI developer, my contributions included:
- Designing and training a **CNN-based thermal classification model** to identify human presence from AMG8833 sensor data.
- Implementing **YOLOv8** for real-time object detection using the ESP32-CAM.
- Integrating AI models with the embedded system for seamless processing and real-time detection.
- Collaborating with the web development team to ensure proper visualization of detection results.

---

## Installation and Setup
### 1. Prerequisites
- Python 3.8+
- PyTorch
- Ultralytics YOLOv8
- ESP-IDF for ESP32 development
- Flask, OpenCV, and related libraries

### 2. Clone the Repository
```bash
$ git clone https://github.com/username/D.E.B.R.I.S
$ cd D.E.B.R.I.S
```

### 3. Install Dependencies
```bash
$ pip install -r requirements.txt
```

### 4. Configure Hardware
- Connect sensors and the AMG8833 thermal camera to the ESP32-CAM.
- Flash the ESP32-CAM with the provided firmware.

### 5. Run the Application
```bash
$ python main.py
```

---

## Results
- **Enhanced Rescue Efficiency:** Successfully detected humans under low-visibility conditions using thermal imaging and real-time video feeds.
- **Comprehensive Data Visualization:** Displayed sensor readings, thermal data, and location tracking on a responsive web interface.
- **AI Integration:** Achieved high accuracy in human detection using deep learning models.

---

## Future Work
- Enable autonomous navigation using advanced path-planning algorithms.
- Expand training datasets to improve AI model accuracy.
- Incorporate additional sensors for hazardous environment detection (e.g., gas sensors for toxic gases).
- Develop mobile applications for easier access to live data.

---

## Contributors
- **Omar** (AI Developer)
- Mohamed Ghozy, Mario Mohsen, Ayman, Maya Tarek, Mohamed Abdelwhab, Maryam Mohamed, Yousef Hany, Walaa Saad Abdelfatah

---

## Acknowledgments
This project was supervised by **Professor Abeer** and supported by resources from **Mansoura University**.

---

## License
This project is licensed under the [MIT License](LICENSE).

---

Feel free to modify or expand on this template to include more specific details about your project!
