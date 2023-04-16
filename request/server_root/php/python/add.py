#!/usr/bin/env python

import cgi

# Read input from query string (GET) or from standard input (POST)
form = cgi.FieldStorage()
if os.environ['REQUEST_METHOD'] == 'GET':
    # Read input from query string
    if 'num1' in form and 'num2' in form:
        num1 = form['num1'].value
        num2 = form['num2'].value
    else:
        # Handle missing input
        print("Content-type: text/html\n")
        print("Error: num1 and num2 must be provided in the query string.")
        exit(1)
elif os.environ['REQUEST_METHOD'] == 'POST':
    # Read input from standard input (POST)
    if 'num1' in form and 'num2' in form:
        num1 = form['num1'].value
        num2 = form['num2'].value
    else:
        # Handle missing input
        print("Content-type: text/html\n")
        print("Error: num1 and num2 must be provided in the request body.")
        exit(1)
else:
    # Handle unsupported request methods
    print("Content-type: text/html\n")
    print("Error: Unsupported request method.")
    exit(1)

# Calculate sum
try:
    num1 = int(num1)
    num2 = int(num2)
    sum = num1 + num2
    # Print the result
    print("Content-type: text/html\n")
    print(f"Sum of {num1} and {num2} is: {sum}")
except ValueError:
    # Handle invalid input
    print("Content-type: text/html\n")
    print("Error: Invalid input. Please provide valid integer values for num1 and num2.")
    exit(1)