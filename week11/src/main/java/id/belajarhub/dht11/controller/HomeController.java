package id.belajarhub.dht11.controller;

import id.belajarhub.dht11.service.HomeService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;

@Controller
public class HomeController {

    private final HomeService homeService;

    @Autowired
    public HomeController(HomeService homeService) {
        this.homeService = homeService;
    }

    @GetMapping("/")
    public String home(Model model) {
        String message = homeService.getWelcomeMessage();
        model.addAttribute("message", message);
        return "home";  // templates/home.html 로 렌더링
    }
}
