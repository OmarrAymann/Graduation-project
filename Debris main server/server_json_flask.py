from flask import Flask, request, jsonify

app = Flask(__name__)

clients = {}
data_ = {}

# Define specific functions for different client IDs
def function_for_client_1(data):
    print("Executing function for client 1")
    # Add your specific functionality for client 1 here
    return {"message": "Function for client 1 executed"}

def function_for_client_2(data):
    print("Executing function for client 2")
    # Add your specific functionality for client 2 here
    return {"message": "Function for client 2 executed"}

@app.route('/')
def index():
    return "Welcome to the Flask Server"

@app.route('/connect', methods=['POST'])
def connect():
    client_id = request.json.get('client_id')
    if client_id:
        clients[client_id] = request.remote_addr
        data_[client_id] = []
        print(f'Client {client_id} connected from {request.remote_addr}')
        return jsonify({"message": f"Client {client_id} connected"}), 200
    else:
        return jsonify({"error": "Client ID is required"}), 400

@app.route('/data', methods=['POST'])
def receive_data():
    client_id = request.json.get('client_id')
    data = request.json.get('data')

    if client_id and data:
        print(f'Received data from {client_id}: {data}')
        
        # Since `data` is already a list, we can directly process it
        data_array = data
        data_[client_id].append(data_array)  # Corrected line
        print(f'Parsed data array: {data_array}')

        # Perform specific functions based on the client ID
        if client_id == "ESP32_1":
            response = function_for_client_1(data_array)
        elif client_id == "ESP32_2":
            response = function_for_client_2(data_array)
        else:
            response = {"message": f"Data received from {client_id}"}
        
        return jsonify(response), 200
    else:
        return jsonify({"error": "Client ID and data are required"}), 400

@app.route('/data/esp32_1', methods=['POST'])
def receive_data_esp32_1():
    client_id = request.json.get('client_id')
    data = request.json.get('data')

    if client_id != "ESP32_1":
        return jsonify({"error": "Unauthorized client"}), 403

    if data:
        print(f'Received data from {client_id}: {data}')
        
        # Since `data` is already a list, we can directly process it
        data_array = data
        data_[client_id].append(data_array)
        print(f'Parsed data array: {data_array}')

        # Perform specific function for ESP32_1
        response = function_for_client_1(data_array)
        
        return jsonify(response), 200
    else:
        return jsonify({"error": "Data is required"}), 400

@app.route('/clients', methods=['GET'])
def get_clients():
    return jsonify(list(clients.keys()))

@app.route('/data_show', methods=['GET'])
def get_data():
    return jsonify(data_)

if __name__ == "__main__":
    print("[starting] Flask server is starting....")
    app.run(host='0.0.0.0', port=5000)
