import requests

# URL of the local HTTP server
url = 'http://localhost:8081'

# Send GET request
response = requests.get(url)

# Check if the request was successful
if response.status_code == 200:
    # Read and print the response content
    print(response.content.decode('utf-8'))
else:
    print('Failed to get response. Status code:', response.status_code)