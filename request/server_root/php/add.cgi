#!/usr/bin/php-cgi
<?php

// Retrieve the query parameters from the URL
$queryString = getenv('QUERY_STRING');

// Parse the query string to extract the two numbers
parse_str($queryString, $params);

// Get the values of the two numbers from the query parameters
$num1 = $params['num1'] ?? 0; // Default to 0 if not provided
$num2 = $params['num2'] ?? 0; // Default to 0 if not provided

// Calculate the sum of the two numbers
$sum = $num1 + $num2;

// Set the response content type to plain text
header('Content-Type: text/plain');

// Print the sum of the two numbers
echo "The sum of $num1 and $num2 is: $sum";

?>
