package id.belajarhub.dht11.repository;

import id.belajarhub.dht11.model.SensorData;
import org.springframework.data.jpa.repository.JpaRepository;
import java.util.List;

public interface SensorDataRepository extends JpaRepository<SensorData, Long> {
    List<SensorData> findByDeviceId(String deviceId);
}
