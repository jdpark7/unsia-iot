<?php
// Database connection configuration
$host = "localhost";           // Database host
$dbname = "unsia_db";          // Name of the user's database
$user = "unsia";               // MariaDB username
$pass = "your_password";       // MariaDB user password

// Try to establish a connection to the MariaDB server
try {
    $pdo = new PDO("mysql:host=$host;dbname=$dbname;charset=utf8", $user, $pass);
} catch (PDOException $e) {
    // If connection fails, print error message and exit
    die("DB Connection failed: " . $e->getMessage());
}

// Handle POST request from IoT device
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $temp = $_POST["temp"] ?? null;        // Read temperature value
    $hum = $_POST["hum"] ?? null;          // Read humidity value
    $studentid = $_POST["studentid"] ?? "unknown"; // Read student ID

    // If both temperature and humidity are received, insert into database
    if ($temp !== null && $hum !== null) {
        $stmt = $pdo->prepare("INSERT INTO sensor_data (device_id, temperature, humidity) VALUES (?, ?, ?)");
        $stmt->execute([$studentid, $temp, $hum]);
        echo "Data inserted successfully.";
    } else {
        echo "Missing data.";
    }
} else {
    echo "Invalid request.";
}
?>