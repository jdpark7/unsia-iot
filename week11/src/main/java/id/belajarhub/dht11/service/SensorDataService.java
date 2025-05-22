package id.belajarhub.dht11.service;

import id.belajarhub.dht11.model.SensorData;
import id.belajarhub.dht11.repository.SensorDataRepository;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class SensorDataService {
    private final SensorDataRepository repository;

    public SensorDataService(SensorDataRepository repository) {
        this.repository = repository;
    }

    public SensorData save(SensorData data) {
        return repository.save(data);
    }

    public List<SensorData> getByDeviceId(String deviceId) {
        return repository.findByDeviceId(deviceId);
    }
}
