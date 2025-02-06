import socket
HOST = '0.0.0.0'  # Replace with the ESP32's IP address
PORT = 12345  # Choose a port number
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    conn, addr = s.accept()
    with conn:
        print('Connected by', addr)
        while True:
            data = conn.recv(1024)  # Assuming array size is less than 1024 bytes
            if not data:
                break
            print(list(data))