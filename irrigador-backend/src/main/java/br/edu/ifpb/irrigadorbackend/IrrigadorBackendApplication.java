package br.edu.ifpb.irrigadorbackend;

import br.edu.ifpb.irrigadorbackend.gateway.MqttGateway;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.builder.SpringApplicationBuilder;
import org.springframework.context.ConfigurableApplicationContext;
import org.springframework.integration.annotation.IntegrationComponentScan;

@SpringBootApplication
@IntegrationComponentScan
public class IrrigadorBackendApplication {

	public static void main(String[] args) {
		SpringApplication.run(IrrigadorBackendApplication.class, args);
//		ConfigurableApplicationContext context =
//				new SpringApplicationBuilder(IrrigadorBackendApplication.class)
//						.run(args);
//		MqttGateway gateway = context.getBean(MqttGateway.class);
//		gateway.sendToMqtt("IRRIGAMENTO:S;UMIDADE:60");
	}
}
