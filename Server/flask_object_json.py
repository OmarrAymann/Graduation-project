from flask import Flask, request, jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

clients = {}
data_ = {}

@app.route('/')
def index():
    return "Welcome to the Flask Server"

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

@app.route('/clients', methods=['GET'])
def get_clients():
    return jsonify(list(clients.keys()))

@app.route('/data_show', methods=['GET'])
def get_data():
    return jsonify(data_)

if __name__ == '__main__':
    print("[starting] Flask server is starting....")
    app.run(host='0.0.0.0', port=5000)
