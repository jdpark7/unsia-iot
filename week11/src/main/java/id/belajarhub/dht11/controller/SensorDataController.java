package id.belajarhub.dht11.controller;

import java.util.List;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import id.belajarhub.dht11.model.SensorData;
import id.belajarhub.dht11.service.SensorDataService;

@RestController
@RequestMapping("/api/sensors")
public class SensorDataController {
    private final SensorDataService service;

    public SensorDataController(SensorDataService service) {
        this.service = service;
    }

    @PostMapping
    public SensorData receiveSensorData(@RequestBody SensorData data) {
        System.out.println("Received Data: " + data);
        return service.save(data);
    }

    @GetMapping("/{deviceId}")
    public List<SensorData> getSensorData(@PathVariable String deviceId) {
        return service.getByDeviceId(deviceId);
    }

}

