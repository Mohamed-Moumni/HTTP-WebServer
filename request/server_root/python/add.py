#!/usr/bin/env python
import os
from urllib.parse import parse_qs

# get the values of num1 and num2 from the query string
query_string = os.environ.get('QUERY_STRING', '')
query_params = parse_qs(query_string)
num1 = int(query_params.get('num1', [''])[0])
num2 = int(query_params.get('num2', [''])[0])

# calculate the sum
sum = num1 + num2

# output the result
print("Content-Type: text/html")
print("")
print("<html><body>")
print("<h1>Sum of %d and %d is %d</h1>" % (num1, num2, sum))
print("</body></html>")