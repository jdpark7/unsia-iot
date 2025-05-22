package id.belajarhub.dht11.controller;

import java.util.List;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import id.belajarhub.dht11.model.SensorData;
import id.belajarhub.dht11.service.SensorDataService;

@Controller
@RequestMapping("/api/sensors")
public class SensorDataController {
    private final SensorDataService service;

    public SensorDataController(SensorDataService service) {
        this.service = service;
    }

    @PostMapping
    @ResponseBody
    public SensorData receiveSensorData(@RequestBody SensorData data) {
        return service.save(data);
    }

    @GetMapping("/{deviceId}")
    public List<SensorData> getSensorData(@PathVariable String deviceId) {
        return service.getByDeviceId(deviceId);
    }
    @GetMapping("/web/{deviceId}")
    public String showData(@PathVariable String deviceId, Model model) {
    List<SensorData> dataList = service.getByDeviceId(deviceId);
    model.addAttribute("deviceId", deviceId);
    model.addAttribute("sensorList", dataList);
    return "sensor";  // sensor.html 렌더링
}
}
