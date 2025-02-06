import cv2
import numpy as np
import requests
import time
from ultralytics import YOLO

# ESP32-CAM MJPEG stream URL
url = 'http://192.168.1.83/cam-lo.jpg'

# Desired frame dimensions
frame_width = 320
frame_height = 240

# Desired frames per second
fps = 80
delay = 1 / fps

# Function to fetch frames from MJPEG stream
def get_frame_from_stream(url):
    try:
        resp = requests.get(url, timeout=5)
        img_arr = np.array(bytearray(resp.content), dtype=np.uint8)
        frame = cv2.imdecode(img_arr, -1)
        return frame
    except Exception as e:
        print(f"Failed to fetch frame: {e}")
        return None

# Load the YOLOv8n model
model = YOLO("yolov8n.pt")

while True:
    start_time = time.time()
    
    # Get frame from ESP32-CAM stream
    frame = get_frame_from_stream(url)
    if frame is None:
        print("Failed to capture image")
        break

    # Resize the frame to the desired dimensions
    frame = cv2.resize(frame, (frame_width, frame_height))

    # Run YOLOv8 inference on the frame
    results = model(frame)

    # Filter results to detect persons only (class ID for person in COCO dataset is 0)
    person_detections = []
    for result in results:
        if hasattr(result, 'boxes'):
            for box in result.boxes:
                if int(box.cls) == 0:
                    person_detections.append(box)

    # Plot the filtered results
    if person_detections:
        for det in person_detections:
            bbox = det.xyxy.cpu().numpy()[0]  # Bounding box coordinates
            conf = det.conf.cpu().numpy()[0]  # Confidence score
            label = model.names[int(det.cls.cpu().numpy()[0])]  # Class label
            x1, y1, x2, y2 = map(int, bbox)  # Bounding box coordinates

            # Draw the bounding box and label on the frame
            cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
            cv2.putText(frame, f"{label} {conf:.2f}", (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

    # Display the result frame
    cv2.imshow('ESP32-CAM YOLOv8 Person Detection', frame)

    # Calculate the time taken to process the frame
    elapsed_time = time.time() - start_time
    sleep_time = max(0, delay - elapsed_time)

    # Exit on pressing 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

    # Sleep to control the FPS
    time.sleep(sleep_time)

cv2.destroyAllWindows()
