# Painel I²C
Projeto de Redes de Comunicação Automotiva do curso de [Tecnologia em Eletrônica Automotiva](http://fatecsorocaba.edu.br/curso_ea.asp "Eletrônica Fatec") da [Faculdade de Tecnologia de Sorocaba](http://fatecsorocaba.edu.br/ "Fatec Sorocaba").

## Objetivo
Criar uma rede de comunicação entre quatro microcontroladores usando o padrão [I²C](https://www.i2c-bus.org/specification/ "Padrão I²C").
Cada microcontrolador deverá simular uma [ECU](https://en.wikipedia.org/wiki/Engine_control_unit "Wikipedia EN"). Sendo estas ECUs divididas da seguinte forma:
* [Painel](ecu-painel.cpp#L1) - Mostra todas as informações obtidas através da rede.
* [Motor](ecu-motor.cpp#L1) - Simula os seguintes sensores do motor:
  - Rotação
  - Temperatura da água do arrafecimento
  - Temperatura ambiente
  - Temperatura de óleo
  - Interruptor de óleo
* [Câmbio](ecu-cambio.cpp#L1) - Simula a troca de marchas da caixa de câmbio.
* [Traseira](ecu-traseira.cpp#L1) - Simula os seguintes sensores da parte traseira do veículo:
  - Distância traseira
  - Nível do tanque de combustível

O Projeto deverá ser implementado através da plataforma [AUTODESK® TINKERCAD™](https://www.tinkercad.com/ "TINKERCAD"). A imagem abaixo demonstra a visão principal do [painel](https://www.tinkercad.com/things/4LXyvpHGJDU "Projeto no TINKERCAD").
![Imagem principal do painel automotivo](imagens/tinkercad.png?raw=true "Painel principal")

## Grupo
O projeto for divido em um grupo de cinco integrantes:
* Brendon Politori
* Lucas Maciel
* Michel Ribeiro
* Murilo Martins
* [Rafhael Payão](https://github.com/rafaeltmbr/ "GitHub")
