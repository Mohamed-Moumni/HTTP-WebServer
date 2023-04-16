<?php
// Read input from query string (GET) or from standard input (POST)
if ($_SERVER['REQUEST_METHOD'] === 'GET') {
    // Read input from query string
    if (isset($_GET['num1']) && isset($_GET['num2'])) {
        $num1 = $_GET['num1'];
        $num2 = $_GET['num2'];
    } else {
        // Handle missing input
        die("Error: num1 and num2 must be provided in the query string.");
    }
} elseif ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Read input from standard input (POST)
    $input = file_get_contents('php://input');
    $inputArr = explode('&', $input);
    if (count($inputArr) === 2) {
        $num1 = explode('=', $inputArr[0])[1];
        $num2 = explode('=', $inputArr[1])[1];
    } else {
        // Handle missing input
        die("Error: num1 and num2 must be provided in the request body.");
    }
} else {
    // Handle unsupported request methods
    die("Error: Unsupported request method.");
}

// Calculate sum
$sum = intval($num1) + intval($num2);

// Print the result
echo "Sum of $num1 and $num2 is: $sum";
// while(1)
// {

// }
?>
