package id.belajarhub.dht11.service;

import org.springframework.stereotype.Service;

@Service
public class HomeService {
    public String getWelcomeMessage() {
        return "Welcome to UNSIA IoT Dashboard!";
    }
}
