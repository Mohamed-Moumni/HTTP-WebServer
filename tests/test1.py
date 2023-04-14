import threading
import time
import requests

N = 1000  # Number of clients to launch
URL = "http://localhost:8081/file.txt"  # URL of the web server

def test_server():
    """Test the server by sending a POST request and checking the response."""
    data = {"key": "value"}  # Data to send in the request
    try:
        response = requests.post(URL, data=data)
        if response.status_code == 200:
            print(f"Connection successful (status code: {response.status_code})")
        else:
            print(f"Connection failed (status code: {response.status_code})")
    except requests.exceptions.ConnectionError:
        print("Connection failed (connection error)")

if __name__ == "__main__":
    # Launch N client threads
    threads = []
    for i in range(N):
        thread = threading.Thread(target=test_server)
        thread.start()
        threads.append(thread)

    # Wait for all threads to finish
    for thread in threads:
        thread.join()

    print("All clients have finished.")
