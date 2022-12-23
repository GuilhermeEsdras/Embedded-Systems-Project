package br.edu.ifpb.irrigadorbackend.config;

import br.edu.ifpb.irrigadorbackend.handler.CustomMessageHandler;
import lombok.RequiredArgsConstructor;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.integration.annotation.ServiceActivator;
import org.springframework.integration.channel.DirectChannel;
import org.springframework.integration.core.MessageProducer;
import org.springframework.integration.mqtt.core.DefaultMqttPahoClientFactory;
import org.springframework.integration.mqtt.core.MqttPahoClientFactory;
import org.springframework.integration.mqtt.inbound.MqttPahoMessageDrivenChannelAdapter;
import org.springframework.integration.mqtt.outbound.MqttPahoMessageHandler;
import org.springframework.integration.mqtt.support.DefaultPahoMessageConverter;
import org.springframework.messaging.MessageChannel;
import org.springframework.messaging.MessageHandler;

@Configuration
@RequiredArgsConstructor
public class MqttConfig {

    private final CustomMessageHandler customMessageHandler;

    @Bean
    public MessageChannel mqttInputChannel() {
        return new DirectChannel();
    }

    @Bean
    public MessageChannel mqttOutboundChannel() {
        return new DirectChannel();
    }

    @Bean
    public MqttPahoClientFactory mqttPahoClientFactory() {
        DefaultMqttPahoClientFactory clientFactory = new DefaultMqttPahoClientFactory();
        MqttConnectOptions connectOptions = new MqttConnectOptions();
        connectOptions.setServerURIs(new String[]{"tcp://192.168.0.16:1883"});
        connectOptions.setUserName("guest");
        connectOptions.setPassword("guest".toCharArray());
        clientFactory.setConnectionOptions(connectOptions);
        return clientFactory;
    }

    @Bean
    public MessageProducer inbound() {
        MqttPahoMessageDrivenChannelAdapter channelAdapter =
                new MqttPahoMessageDrivenChannelAdapter(
                        "tcp://guest:guest@192.168.0.16:1883",
                        "irrigador-backend-client",
                        "/topic/status", "/topic/umidade");
        channelAdapter.setCompletionTimeout(5000);
        channelAdapter.setConverter(new DefaultPahoMessageConverter());
        channelAdapter.setQos(0);
        channelAdapter.setOutputChannel(mqttInputChannel());
        return channelAdapter;
    }

    @Bean
    @ServiceActivator(inputChannel = "mqttInputChannel")
    public MessageHandler handler() {
        return customMessageHandler;
    }

    @Bean
    @ServiceActivator(inputChannel = "mqttOutboundChannel")
    public MessageHandler mqttOutbound() {
        MqttPahoMessageHandler messageHandler =
                new MqttPahoMessageHandler("irrigador-backend-client-manager", mqttPahoClientFactory());
        messageHandler.setAsync(true);
        messageHandler.setDefaultTopic("/topic/umidade");
        return messageHandler;
    }
}
