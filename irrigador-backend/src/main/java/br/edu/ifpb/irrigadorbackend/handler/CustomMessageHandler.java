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
        Timestamp timestamp = new Timestamp(System.currentTimeMillis());
        if (message.getPayload().toString().startsWith("IRRIGAMENTO")) {
            String[] payload = message.getPayload().toString().split(";");
            String[] umidityStatus = payload[0].split(":");
            Status status = new Status();
            switch (umidityStatus[1]) {
                case "S" :
                    String[] umidity = payload[1].split(":");
                    status.setUmidity(umidity[1]);
                    status.setStatus("Irrigamento feito com sucesso!");
                    break;
                case "N":
                    status.setUmidity(null);
                    status.setStatus("Falta de água! Reabastecer recipiente.");
                    break;
                case "I" :
                    String[] umidity1 = payload[2].split(":");
                    status.setUmidity(umidity1[1]);
                    status.setStatus("Irrigamento incompleto! Reabastecer recipiente.");
                    break;
            }
            status.setTimestamp(timestamp);
            statusService.add(status);
        } else {
            System.out.println(message.getPayload());
        }
    }
}
