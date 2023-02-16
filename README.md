# SistemaVLC
Esse sistema consiste em uma aplicação de Comunicação pela Luz Visível aplicada a sistemas embarcados de baixo custo. Esse é separado em aplicação do emissor e receptor. 
O emissor consiste em um LED, conectado à porta 11 de um Arduino, e um módulo fotodiodo conectado à porta 8 de um segundo Arduino. 
Emissor: Tem a função de criar uma mensagem binária, decodificá-la em Manchester e enviar por meio de variações da porta digital.
Receptor: Tem como função ler a porta digital, decodificar seu valor em Manchester, identificar a mensagem recebida, checar seu valor e imprimi-la. 
