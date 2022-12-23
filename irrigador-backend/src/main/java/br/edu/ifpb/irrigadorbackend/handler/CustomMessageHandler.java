package br.edu.ifpb.irrigadorbackend.handler;

import br.edu.ifpb.irrigadorbackend.entity.Status;
import br.edu.ifpb.irrigadorbackend.service.StatusService;
import lombok.RequiredArgsConstructor;
import org.springframework.messaging.Message;
import org.springframework.messaging.MessageHandler;
import org.springframework.messaging.MessagingException;
import org.springframework.stereotype.Component;

import java.sql.Timestamp;

@Component
@RequiredArgsConstructor
public class CustomMessageHandler implements MessageHandler {

    private final StatusService statusService;

    @Override
    public void handleMessage(Message<?> message) throws MessagingException {
        System.out.println(message.getPayload());
        Timestamp timestamp = new Timestamp(System.currentTimeMillis());
        if (message.getPayload().toString().startsWith("IRRIGAMENTO")) {
            String[] payload = message.getPayload().toString().split(";");
            String[] umidityStatus = payload[0].split(":");
            String[] umidity = payload[1].split(":");
            Status status = new Status();
            status.setUmidity(umidity[1]);
            status.setTimestamp(timestamp);
            switch (umidityStatus[1]) {
                case "S" :
                    status.setStatus("Iniciado processo de irrigamento!");
                    break;
                case "F":
                    status.setStatus("Finalizado processo de irrigamento!");
                    break;
                case "I" :
                    status.setStatus("Falta de água no reservatório!");
                    break;
            }
            statusService.add(status);
        }
    }
}
