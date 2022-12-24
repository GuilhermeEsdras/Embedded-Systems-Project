# Sistema de Irrigação Automática 💧🪴

Este é o projeto final da disciplina de Sistemas Embarcadas 2022.2.
Apresentação YouTube: https://youtu.be/aDN4pBqXoVA

## Sumário

- [Sistema de Irrigação Automática 💧🪴](#sistema-de-irrigação-automática-)
  - [Sumário](#sumário)
  - [❕ Descrição](#-descrição)
  - [🔌 Componentes](#-componentes)
    - [Microcontrolador](#microcontrolador)
    - [Sensor de Umidade do Solo Higrômetro](#sensor-de-umidade-do-solo-higrômetro)
    - [Mini Bomba de Água](#mini-bomba-de-água)
    - [Outros componentes](#outros-componentes)
  - [🧩 Diagrama de Blocos](#-diagrama-de-blocos)
  - [🔄 Fluxograma do Firmware](#-fluxograma-do-firmware)
  - [🖼️ Preview do Front End](#️-preview-do-front-end)
  - [👣 Imagens das Etapas de Desenvolvimento](#-imagens-das-etapas-de-desenvolvimento)
  - [👥 Participantes](#-participantes)

---

## ❕ Descrição

Um sistema de irrigação automático para plantas.

## 🔌 Componentes

### Microcontrolador

<center>
  <img src="Assets/Images/esp32-wroom-32d.jpg" alt="Sensor de Umidade do Solo" width="300"/>
</center>

Em nosso projeto vamos utilizar o ESP-32, modelo WROOM-32D ([datasheet](Docs/Datasheets/esp32-wroom-32_datasheet_en.pdf)), principal microcontrolador utilizado na disciplina e o coração do nosso sistema.

Ele nada mais é do que uma placa de desenvolvimento de código aberto que utiliza o chip ESP32.

Possui Wi-Fi e Bluetooth nativo, dessa forma, essa placa é ideal para projetos de automação residencial e internet das coisas.

### Sensor de Umidade do Solo Higrômetro

<center>
  <img src="Assets/Images/sensor-umidade-solo.png" alt="Sensor de Umidade do Solo" width="300"/>
</center>

Este Sensor de Umidade do Solo Higrômetro foi feito para detectar as variações de umidade no solo, sendo que quando o solo está seco a saída do sensor fica em estado alto (HIGH), e quando úmido em estado baixo (LOW) sua tensão de operação é de 3,3 a 5V.

O mesmo consiste em 2 partes: uma sonda que entra em contato com o solo, e um pequeno módulo contendo um chip comparador LM393 ([datasheet](Docs/Datasheets/Datasheet_LM393.pdf)), que vai ler os dados que vêm do sensor e enviá-los para o microcontrolador, no nosso caso, o ESP32.

### Mini Bomba de Água

A Mini Bomba de Água RS385 opera com tensão entre 9V a 15V e permite elevação máxima de até 3 metros e altura de aspiração de até 2 metros.

### Outros componentes

-

## 🧩 Diagrama de Blocos

=======

![Diagrama de Blocos](Assets/Diagrams/diagrama-de-bloco.svg)

## 🔄 Fluxograma do Firmware

![Fluxograma](Assets/Diagrams/fluxograma.svg)

## 🖼️ Preview do Front End

<center>
  <img src="Assets/Images/frontend-ss.png" alt="Front End Screenshot" />
</center>

## 👣 Imagens das Etapas de Desenvolvimento

<center>
  <img src="Assets/Images/foto1.jpg" alt="Desenvolvimento 1" width="600"/>
  <img src="Assets/Images/foto2.jpg" alt="Desenvolvimento 2" width="600"/>
  <img src="Assets/Images/foto3.jpg" alt="Desenvolvimento 3" width="600"/>
</center>

## 👥 Participantes

- [Ayrton Dantas](https://github.com/AYRtonMeD)
- [Guilherme Esdras](https://github.com/GuilhermeEsdras)
- [José Henrique](https://github.com/Joshaby)
