import socket

def start_client(host='127.0.0.1', port=12345):
    # Create a TCP/IP socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Connect to the server
    client_socket.connect((host, port))
    print(f'Connected to {host}:{port}')

    try:
        # Send a message to the server
        message = 'Hello, Server!'
        client_socket.sendall(message.encode('utf-8'))
        print('Sent:', message)

        # Receive the response from the server
        response = client_socket.recv(1024)
        print('Received:', response.decode('utf-8'))

    except Exception as e:
        print('Error:', e)
    
    finally:
        # Close the client socket
        client_socket.close()

# Example usage
if __name__ == '__main__':
    start_client()
