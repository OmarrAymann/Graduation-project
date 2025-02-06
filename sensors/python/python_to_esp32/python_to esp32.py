import socket

# ESP32 server IP address and port
SERVER_IP = '192.168.1.2'
SERVER_PORT = 80

# Data to send
data = b'Hello, ESP32!'

# Create a TCP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    # Connect to the server
    sock.connect((SERVER_IP, SERVER_PORT))

    # Send data
    sock.sendall(data)

    # Receive response
    response = sock.recv(1024)
    print("Received:", response.decode())

finally:
    # Close the socket
    sock.close()