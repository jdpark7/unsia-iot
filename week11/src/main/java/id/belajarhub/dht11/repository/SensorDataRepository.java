package id.belajarhub.dht11.repository;

import java.util.List;

import org.springframework.data.domain.Pageable;
import org.springframework.data.jpa.repository.JpaRepository;

import id.belajarhub.dht11.model.SensorData;

public interface SensorDataRepository extends JpaRepository<SensorData, Long> {
    List<SensorData> findByDeviceId(String deviceId);
    List<SensorData> findByDeviceIdOrderByTimestampDesc(String deviceId, Pageable pageable);
}
