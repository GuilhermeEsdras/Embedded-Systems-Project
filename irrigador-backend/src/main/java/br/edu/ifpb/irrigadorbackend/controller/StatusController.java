package br.edu.ifpb.irrigadorbackend.controller;

import br.edu.ifpb.irrigadorbackend.dto.MoistureDTO;
import br.edu.ifpb.irrigadorbackend.entity.Status;
import br.edu.ifpb.irrigadorbackend.gateway.MqttGateway;
import br.edu.ifpb.irrigadorbackend.service.StatusService;
import lombok.RequiredArgsConstructor;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/status")
@RequiredArgsConstructor
public class StatusController {

    private final StatusService statusService;

    private final MqttGateway mqttGateway;

    @GetMapping("/page")
    @CrossOrigin(origins = "https://7ac0-200-129-71-126.sa.ngrok.io")
    public Page<Status> findPage(Pageable pageable) {
        return statusService.findPage(pageable);
    }

    @PostMapping("/moisture")
    @ResponseStatus(HttpStatus.NO_CONTENT)
    public void setMoisture(@RequestBody MoistureDTO dto) {
        mqttGateway.sendToMqtt(dto.getMoisture());
    }
}
