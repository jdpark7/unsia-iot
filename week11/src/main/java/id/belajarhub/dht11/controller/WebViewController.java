package id.belajarhub.dht11.controller;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;

import id.belajarhub.dht11.model.SensorData;
import id.belajarhub.dht11.repository.SensorDataRepository;

@Controller
public class WebViewController {

    @Autowired
    private SensorDataRepository repository;

    @GetMapping("/sensor/{deviceId}")
    public String viewSensorData(@PathVariable String deviceId, Model model) {
        List<SensorData> dataList = repository.findByDeviceId(deviceId);
        model.addAttribute("deviceId", deviceId);
        model.addAttribute("dataList", dataList);
        return "sensor"; // sensor.html 템플릿 렌더링
    }
}
