<?php
// Database connection using PDO
$dsn = 'mysql:host=localhost;dbname=your-database;charset=utf8mb4';
$username = 'your_username';
$password = 'your_password';

try {
    $pdo = new PDO($dsn, $username, $password);
    // Set error mode to exception for better error handling
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
} catch (PDOException $e) {
    die("Database connection failed: " . $e->getMessage());
}

// Fetch the latest 20 records
$stmt = $pdo->query("SELECT * FROM iot_data ORDER BY timestamp DESC LIMIT 20");
$data = $stmt->fetchAll(PDO::FETCH_ASSOC);

// Prepare data for Chart.js
$timestamps = array_column($data, 'timestamp');
$temperatures = array_column($data, 'temperature');
$humidities = array_column($data, 'humidity');
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>IoT Sensor Data</title>
    <!-- Bootstrap 5 CSS -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">
    <!-- Chart.js -->
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
</head>
<body>
<div class="container my-5">
    <h1 class="mb-4 text-center">IoT Sensor Data Overview</h1>

    <!-- Chart Container -->
    <div class="card mb-4">
        <div class="card-body">
            <canvas id="sensorChart" height="100"></canvas>
        </div>
    </div>

    <!-- Data Table -->
    <div class="card">
        <div class="card-header">
            <strong>Recent Sensor Readings</strong>
        </div>
        <div class="card-body p-0">
            <table class="table table-striped mb-0">
                <thead class="table-light">
                    <tr>
                        <th scope="col">Timestamp</th>
                        <th scope="col">Temperature (°C)</th>
                        <th scope="col">Humidity (%)</th>
                    </tr>
                </thead>
                <tbody>
                    <?php foreach ($data as $row): ?>
                        <tr>
                            <td><?= htmlspecialchars($row['timestamp']) ?></td>
                            <td><?= htmlspecialchars($row['temperature']) ?></td>
                            <td><?= htmlspecialchars($row['humidity']) ?></td>
                        </tr>
                    <?php endforeach; ?>
                </tbody>
            </table>
        </div>
    </div>
</div>

<!-- Chart.js Script -->
<script>
    const ctx = document.getElementById('sensorChart').getContext('2d');
    const sensorChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: <?= json_encode(array_reverse($timestamps)) ?>,
            datasets: [
                {
                    label: 'Temperature (°C)',
                    data: <?= json_encode(array_reverse($temperatures)) ?>,
                    borderColor: 'rgba(255, 99, 132, 1)',
                    backgroundColor: 'rgba(255, 99, 132, 0.2)',
                    tension: 0.4,
                    fill: true
                },
                {
                    label: 'Humidity (%)',
                    data: <?= json_encode(array_reverse($humidities)) ?>,
                    borderColor: 'rgba(54, 162, 235, 1)',
                    backgroundColor: 'rgba(54, 162, 235, 0.2)',
                    tension: 0.4,
                    fill: true
                }
            ]
        },
        options: {
            responsive: true,
            scales: {
                x: {
                    title: {
                        display: true,
                        text: 'Timestamp'
                    }
                },
                y: {
                    beginAtZero: true,
                    title: {
                        display: true,
                        text: 'Value'
                    }
                }
            },
            plugins: {
                legend: {
                    position: 'top'
                },
                tooltip: {
                    mode: 'index',
                    intersect: false
                }
            },
            interaction: {
                mode: 'nearest',
                axis: 'x',
                intersect: false
            }
        }
    });
</script>
</body>
</html>
