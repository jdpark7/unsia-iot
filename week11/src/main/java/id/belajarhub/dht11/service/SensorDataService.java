package id.belajarhub.dht11.service;

import java.util.List;

import org.springframework.data.domain.PageRequest;
import org.springframework.data.domain.Pageable;
import org.springframework.data.domain.Sort;
import org.springframework.stereotype.Service;

import id.belajarhub.dht11.model.SensorData;
import id.belajarhub.dht11.repository.SensorDataRepository;

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
       Pageable limit = PageRequest.of(0, 20, Sort.by(Sort.Direction.DESC, "timestamp"));
       return repository.findByDeviceIdOrderByTimestampDesc(deviceId, limit);
    }
}
