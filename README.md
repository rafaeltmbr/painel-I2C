# Painel I²C
Projeto de Redes de Comunicação Automotiva do curso de [Tecnologia em Eletrônica Automotiva](http://fatecsorocaba.edu.br/curso_ea.asp "Eletrônica Fatec") na [Faculdade de Tecnologia de Sorocaba](http://fatecsorocaba.edu.br/ "Fatec Sorocaba").

## Objetivo
Criar uma rede de comunicação entre quatro microcontroladores usando o padrão [I²C](https://www.i2c-bus.org/specification/ "Padrão I²C").
Cada microcontrolador deverá simular uma [ECU](https://en.wikipedia.org/wiki/Engine_control_unit "Wikipedia EN"). Sendo estas ECUs divididas da seguinte maneira:
* [Painel](ecu-painel/main.cpp) - Mostra todas as informações obtidas através dos sensores.
* [Motor](ecu-motor/main.cpp) - Simula os sensores do motor (e.g. temperatura d'água, rotação, pressão de óleo e etc.).
* [Câmbio](ecu-cambio/main.cpp) - Simula a troca de marchas da caixa de câmbio.
* [Traseira](ecu-traseira/main.cpp) - Simula o sensor de distância da traseira e a nível da bóia de combustível.

O Projeto deverá ser implementado através da plataforma [AUTODESK® TINKERCAD™](https://www.tinkercad.com/). A imagem abaixo demonstra a visão principal do [painel](https://www.tinkercad.com/things/4LXyvpHGJDU "Projeto no TINKERCAD").
![Imagem principal do painel automotivo](imagens/painel-I2C.png?raw=true "Painel principal")
