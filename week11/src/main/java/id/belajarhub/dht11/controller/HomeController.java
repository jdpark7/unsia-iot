package id.belajarhub.dht11.controller;

import java.util.List;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;

import id.belajarhub.dht11.model.SensorData;
import id.belajarhub.dht11.service.SensorDataService;

@Controller
public class HomeController {
    private final SensorDataService service;

    public HomeController(SensorDataService service) {
        this.service = service;
    }

    @GetMapping("/")
    public String home(Model model) {
        String message = "Welcome to UNSIA";
        model.addAttribute("message", message);
        return "home";  // templates/home.html 로 렌더링
    }
    @GetMapping("/{deviceId}")
    public String showData(@PathVariable String deviceId, Model model) {
    List<SensorData> dataList = service.getByDeviceId(deviceId);
    model.addAttribute("deviceId", deviceId);
    model.addAttribute("dataList", dataList);
    return "sensorchart";  // sensor.html 렌더링
}
}
