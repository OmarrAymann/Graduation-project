from flask import Flask, request, jsonify, render_template, send_file
from flask_cors import CORS
import matplotlib.pyplot as plt
import numpy as np
import io

app = Flask(__name__)
CORS(app)

clients = {}
data_ = {}
thermal_data_ = {}
threshold_temp = None  # Global threshold variable

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/data/<client_id>', methods=['POST'])
def receive_data(client_id):
    data = request.get_json()  # Use get_json to get the full JSON data

    if client_id and data:
        print(f'Received data from {client_id}: {data}')

        if client_id not in data_:
            data_[client_id] = []
        data_[client_id].append(data)
        print(f'Parsed data: {data}')

        # Add client_id to clients dictionary
        clients[client_id] = True

        response = {"message": f"Data received from {client_id}"}
        return jsonify(response), 200
    else:
        return jsonify({"error": "Client ID and Data are required"}), 400

@app.route('/thermal_data/<client_id>', methods=['POST'])
def receive_thermal_data(client_id):
    data = request.get_json()  # Use get_json to get the full JSON data

    if client_id and data:
        print(f'Received thermal data from {client_id}: {data}')

        if client_id not in thermal_data_:
            thermal_data_[client_id] = []
        thermal_data_[client_id].append(data)
        print(f'Parsed thermal data: {data}')

        # Add client_id to clients dictionary
        clients[client_id] = True

        response = {"message": f"Thermal data received from {client_id}"}
        return jsonify(response), 200
    else:
        return jsonify({"error": "Client ID and Data are required"}), 400

@app.route('/clients', methods=['GET'])
def get_clients():
    return jsonify(list(clients.keys()))

@app.route('/data_show', methods=['GET'])
def get_data():
    return jsonify(data_)

@app.route('/thermal_data_show', methods=['GET'])
def get_thermal_data():
    return jsonify(thermal_data_)

@app.route('/update_threshold', methods=['POST'])
def update_threshold():
    global threshold_temp
    data = request.get_json()
    threshold_temp = data.get('threshold')
    return jsonify({"message": "Threshold updated", "threshold": threshold_temp}), 200

@app.route('/thermal_image/<client_id>', methods=['GET'])
def generate_thermal_image(client_id):
    global threshold_temp

    if client_id not in thermal_data_ or len(thermal_data_[client_id]) == 0:
        return jsonify({"error": "No thermal data available for this client"}), 400

    # Use the latest thermal data
    thermal_data = thermal_data_[client_id][-1]['pixels']
    thermal_matrix = np.array(thermal_data).reshape((8, 8))  # Adjust based on actual sensor resolution

    # Apply threshold if specified
    if threshold_temp is not None:
        thermal_matrix = np.where(thermal_matrix >= threshold_temp, thermal_matrix, np.nan)

    # Create heatmap
    plt.figure(figsize=(8, 8))
    plt.imshow(thermal_matrix, cmap='hot', interpolation='nearest')
    plt.colorbar()
    plt.title(f'Thermal Image for {client_id}')

    # Save the image to a BytesIO object
    img_io = io.BytesIO()
    plt.savefig(img_io, format='png')
    img_io.seek(0)
    plt.close()

    return send_file(img_io, mimetype='image/png')

if __name__ == '__main__':
    print("[starting] Flask server is starting....")
    app.run(host='0.0.0.0', port=5000)
