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
        String[] payload = message.getPayload().toString().split(";");
        String[] umidityStatus = payload[0].split(":");
        if (umidityStatus[1].equals("S")) {
            if (payload.length == 2) {
                String[] umidity = payload[1].split(":");
                Status status = new Status();
                status.setUmidity(umidity[1]);
                status.setStatus("Irrigamento feito com sucesso");
                Timestamp timestamp = new Timestamp(System.currentTimeMillis());
                status.setTimestamp(timestamp);
                statusService.add(status);
            }
        }
    }
}
