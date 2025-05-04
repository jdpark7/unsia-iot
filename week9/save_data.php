<?php
// Database connection configuration
$host = "localhost";           // Database host
$dbname = "your_login_id_db";          // Name of the user's database
$user = "your_login_id";               // MariaDB username
$pass = "your_login_password";       // MariaDB user password

// Try to establish a connection to the MariaDB server
try {
    $pdo = new PDO("mysql:host=$host;dbname=$dbname;charset=utf8", $user, $pass);
} catch (PDOException $e) {
    // If connection fails, print error message and exit
    die("DB Connection failed: " . $e->getMessage());
}

// Handle POST request from IoT device
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $temp = $_POST["temp"] ?? null;               // Read temperature value
    $hum = $_POST["hum"] ?? null;                 // Read humidity value
    $apikey = $_POST["apikey"] ?? "unknown";      // Read API key
    $device_id = $_POST["device_id"] ?? "unknown";// Read device ID

    // If both temperature and humidity are received, insert into database
    if ($temp !== null && $hum !== null) {
        $stmt = $pdo->prepare("INSERT INTO sensor_data (apikey, device_id, temperature, humidity) VALUES (?, ?, ?, ?)");
        $stmt->execute([$apikey, $device_id, $temp, $hum]);
        $countStmt = $pdo->query("SELECT COUNT(*) FROM sensor_data");
        $totalRows = $countStmt->fetchColumn();
        if ($totalRows > 100) {
            $toDelete = $totalRows - 100;
            $deleteStmt = $pdo->prepare("DELETE FROM sensor_data ORDER BY id ASC LIMIT ?");
            $deleteStmt->bindValue(1, $toDelete, PDO::PARAM_INT);
            $deleteStmt->execute();
        }
        echo "Data inserted successfully.";
    } else {
        echo "Missing data.";
    }
} else {
    echo "Invalid request.";
}
?>
