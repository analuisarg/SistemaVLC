# SistemaVLC
Esse sistema consiste em uma aplicação de Comunicação pela Luz Visível aplicada a sistemas embarcados de baixo custo. Ele é separado em aplicação do emissor e receptor. 

Para reprodução dos testes realizados é necessário um tubo escuro. Ele consiste em um ambiente totalmente isento de incidência de luzes externas, onde pode-se posicionar o emissor e o receptor de forma que seja possível a variação da distância entre ambos. 

Emissor: Tem a função de criar uma mensagem binária, decodificá-la em Manchester e enviar por meio de variações da porta digital. Para o circuito de emissão deste trabalho foi utilizado um LED de 3 mm de luz branca, onde o anodo foi ligado na porta digital 11 do primeiro Arduino UNO passando por um resistor de 1 KΩ e o catodo na porta GND.

Receptor: Tem como função ler a porta digital, decodificar seu valor em Manchester, identificar a mensagem recebida, checar seu valor e imprimi-la. Para o circuito de recepção deste projeto, foi utilizado um módulo de fotodiodo para prototipagem em conjunto com o segundo Arduino UNO. No Arduino, a porta digital 8 foi ligada ao terminal de saída digital do módulo, bem como sua alimentação de 5 V.
O módulo fotodiodo está disponível para compra e seu circuito também é disponibilizado para reprodução no artigo "Visible Light Communication Applied to Low Cost Embedded Systems". 

Testes: Para realização dos testes é necessário o upload de cada um dos códigos disponibilizados em seus respectivos Arduinos UNO e a conexão das plataformas com os circuitos eletrônicos apresentados acima. Dessa forma, será possível a variação da distância entre emissor e receptor e pode-se observar a mensagem transmitida e os resultados dos cálculos de checksum para a validação dos dados. 
