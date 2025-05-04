<?php
// Database connection configuration
$host = "localhost";
$dbname = "unsia_db";
$user = "unsia";
$pass = "your_password";

// Try to connect to the database
try {
    $pdo = new PDO("mysql:host=$host;dbname=$dbname;charset=utf8", $user, $pass);
} catch (PDOException $e) {
    die("DB Connection failed: " . $e->getMessage());
}

// Query the latest 50 records from the sensor_data table
$stmt = $pdo->query("SELECT * FROM sensor_data ORDER BY timestamp DESC LIMIT 50");

// Output as HTML table
echo "<h2>Sensor Data (Latest 50)</h2>";
echo "<table border='1'><tr><th>ID</th><th>API Key</th><th>Temperature</th><th>Humidity</th><th>Timestamp</th></tr>";
while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
    echo "<tr>";
    echo "<td>{$row['id']}</td>";
    echo "<td>{$row['apikey']}</td>";
    echo "<td>{$row['temperature']}</td>";
    echo "<td>{$row['humidity']}</td>";
    echo "<td>{$row['timestamp']}</td>";
    echo "</tr>";
}
echo "</table>";
?>