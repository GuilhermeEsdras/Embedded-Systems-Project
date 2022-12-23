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
    public MqttConnectOptions mqttConnectOptions() {
        MqttConnectOptions connectOptions = new MqttConnectOptions();
        connectOptions.setServerURIs(new String[]{"tcp://localhost:1883"});
        connectOptions.setUserName("guest");
        connectOptions.setPassword("guest".toCharArray());
        connectOptions.setCleanSession(true);
        connectOptions.setConnectionTimeout(30);
        connectOptions.setKeepAliveInterval(60);
        connectOptions.setAutomaticReconnect(true);
        return connectOptions;
    }

    @Bean
    public MqttPahoClientFactory mqttPahoClientFactory() {
        DefaultMqttPahoClientFactory clientFactory = new DefaultMqttPahoClientFactory();
        clientFactory.setConnectionOptions(mqttConnectOptions());
        return clientFactory;
    }

    @Bean
    public MessageProducer inbound() {
        MqttPahoMessageDrivenChannelAdapter channelAdapter =
                new MqttPahoMessageDrivenChannelAdapter(
                        "irrigador-backend-client", mqttPahoClientFactory(), "/topic/status", "/topic/umidade");
        channelAdapter.setCompletionTimeout(1000);
        channelAdapter.setConverter(new DefaultPahoMessageConverter());
        channelAdapter.setQos(1);
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
                new MqttPahoMessageHandler("irrigador-backend-manager-client", mqttPahoClientFactory());
        messageHandler.setAsync(true);
        messageHandler.setDefaultTopic("/topic/umidade");
        return messageHandler;
    }
}
