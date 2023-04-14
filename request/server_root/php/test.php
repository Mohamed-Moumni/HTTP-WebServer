<?php
// Get all environment variables
$envVars = $_SERVER;

// Loop through each environment variable and print its key and value
foreach ($envVars as $key => $value) {
    echo $key . " = " . $value . "<br />";
}
?>
