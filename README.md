# PROJETE2405

- ***[Read this in English](https://github.com/11-pog/PROJETE2405ESP/blob/main/README_EN.md)***

O Codigo do meu projeto para a feira de ciências da escola ETEFMC, chamada PROJETE.

A feira já passou, então não há motivo para eu continuar mexendo nessa bagunça de código (mas provavelmente vou acabar mexendo mesmo assim).

O projeto consistia em um **alimentador automático para ovelhas**, com algumas funcionalidades: Um gráfico que exibia a **umidade** e outro o **nível de ração** que restava no alimentador. Além disso, era possível programar três horários diferentes para o alimentador liberar a quantidade certa de ração para as ovelhas. Tudo isso foi possível com o uso [Deste Site](https://github.com/JuanCB1/site-com-contador), desenvolvido por [outro integrante da equipe](https://github.com/JuanCB1), e que eu integrei ao microcontrolador utilizando o protocolo **MQTT**.

(No site tbm tem uma parte de visão, eu só n falei disso aqui pq n tem nada a ver com o ESP e tbm não fui eu q fiz).

## Algumas notas

- O código foi feito em **C++** e utiliza o **ESP32**.
- Eu usei o PlatformIO no VsCode.
- A configuração de quantidade de ração **não controla** a quantidade exata que o alimentador dispensa. Ele simplesmente gira por um tempo específico, sem medir precisamente a quantidade de ração liberada.

## Algumas Caracteristicas

- Foi utilizado um sensor *DHT22* para medir a umidade no armazém de ração e um sensor ultrassônico *Hc-sr04* para monitorar o nível de ração.
- O motor para o alimentador foi um *Micro Motor DC 12V 300rpm com Caixa De Redução N20*
- **SE EU NÃO ME ENGANO**, foi usado um *Esp32 ESP-WROOM-32*.
- Os pinos foram:

  - 5 Para o Trigger, e 18 para o Echo do Ultrasônico.
  - 27 Para informação do DHT22.
  - 22 Para o motor.
  - Esses pinos são editaveis em `ProjectSettings.cpp/.h`, e em `SerialUtils.h`, respectivamente.

## O Fim

![NEON GENESIS PROJETE 2K24](https://github.com/11-pog/PROJETE2405ESP/blob/main/THE%20END%20OF%20PROJETE.png?raw=true)

Em 2025 tem PROJETE denovo XD.
