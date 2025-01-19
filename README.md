# D.E.B.R.I.S (Detection and Exploration Bot for Rescue, Investigation, and Survival) :
- Combines advanced sensing technologies and real-time data transmission
- Data transmitted to a web interface on a connected server
- Equipped with cameras, proximity sensors, and environmental sensors

![debris](https://github.com/user-attachments/assets/338aee4c-91c0-4a79-ab6a-26c0b2f0f920)

## My Role
As the AI developer, my contributions included:
- Designing and training a **CNN-based thermal classification model** to identify human presence from AMG8833 sensor data.
- Implementing **YOLOv8** for real-time object detection using the ESP32-CAM.
- Integrating AI models into the embedded system to enable efficient processing and real-time detection.
- Collaborating with the web development team to ensure proper visualization of detection results.
---

### Key Highlights:
- **Advanced AI Integration:** Real-time human detection using cameras and thermal imaging.
- **Multi-Sensor Capabilities:** Utilizes environmental sensors like DHT11, MQ2, microwave Doppler radar, and GPS for comprehensive situational awareness.
- **User-Interface:** Displays live video, sensor readings, and location tracking on a responsive web dashboard.
- **Robust Mobility:** High-torque motors and wheels for tough land to enable navigation through debris.
---
## Features
- **Thermal Imaging:** Detects human presence in low-visibility conditions with AMG 8833 thermal camera.
- **Real-Time Detection:** Employs YOLOv8 for live video-based object detection and a CNN for thermal image classification.
- **Environmental Monitoring:** Tracks temperature, humidity, motion, and gas levels using advanced sensors.
- **Web Dashboard:** Provides a clear, real-time interface for monitoring data and live camera feeds.
---
### AI Models Workflow

1. **Thermal Image Processing:**
   - Captures thermal images to make our own dataset contain humans and No humans.
   - Processes data through a CNN for human classification.
   - Customized colormap range between (25 - 35)


![download (4)](https://github.com/user-attachments/assets/ef84cff3-da25-4184-a5c6-cef738832b95)

2. **Object Detection:**
   - Detects humans in real-time using YOLOv8 with live video on our server.

![Screenshot (163)](https://github.com/user-attachments/assets/441181c6-8f5e-4c75-9702-ed80b1a5fb3d)

### Web Interface
- **Live Dashboard:**

 ![Picture8](https://github.com/user-attachments/assets/3bc60e52-b0c1-4015-a18a-9a941aba4447)
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
- **High-Torque Motors and Wheels:** Ensures mobility over rough terrain.
- **12V Lithium-ion Battery:** Provides reliable power with reverse polarity protection and filtration circuits.
---
## Results
- **Enhanced Rescue Efficiency:** Successfully detected humans under low-visibility conditions using thermal imaging and real-time video.
  
- **Data Visualization:** Displayed sensor readings, thermal data, and location tracking on a responsive web server.

- **AI Integration:** Achieved high accuracy in human detection using deep learning models.

![Picture7](https://github.com/user-attachments/assets/3e1cf78b-1b9d-41e9-b0b7-8790966d7f40)

---

## Acknowledgments
This project was supervised by **Professor Abeer twakol** and supported by resources from **Mansoura University**.

---
## Team Members 

![Screenshot (164)](https://github.com/user-attachments/assets/15284fd0-c897-4356-8873-915d6e724b61)


