int led = 11;
int temp = 40;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  pinMode(led, OUTPUT);
}

void loop() {

  unsigned char mensagem[5];//mensagem simples com apenas 1 informação
  bool dig_out = false;

  //cabeçalho: 0xB5 (posição 0 do vetor)(1011 0101)
  mensagem[0] = 0xB5;

  //ID da velocidade = 0xA1 (posição 1 do vetor) (1010 0001)
  mensagem[1] = 0xA1;

  //conteúdo da mensagem = 120km/h (0111 1000)
  mensagem[2] = 0x78;

  //calculo do checksumA
  mensagem[3] = mensagem[1] + mensagem[2]; //checksumA - 0001 1001
  mensagem[4] = mensagem[1] + mensagem[2] + mensagem[3]; //checksumB - 0011 0010

  Serial.println("ChecksumA");
  Serial.println(mensagem[3]);
  Serial.println("ChecksumB");
  Serial.println(mensagem[4]);

  for (int i = 0; i < 5; i++)
  {
    dig_out = 0x80 & mensagem[i];
    if (dig_out == true) {
      digitalWrite(led, HIGH);
      Serial.print("1");
      delay(temp);
      digitalWrite(led, LOW);
      Serial.print("0");
      delay(temp);
    }
    else {
      digitalWrite(led, LOW);
      Serial.print("0");
      delay(temp);
      digitalWrite(led, HIGH);
      Serial.print("1");
      delay(temp);
    }

    dig_out = 0x40 & mensagem[i];
    if (dig_out == true) {
      digitalWrite(led, HIGH);
      Serial.print("1");
      delay(temp);
      digitalWrite(led, LOW);
      Serial.print("0");
      delay(temp);
    }
    else {
      digitalWrite(led, LOW);
      Serial.print("0");
      delay(temp);
      digitalWrite(led, HIGH);
      Serial.print("1");
      delay(temp);
    }

    dig_out = 0x20 & mensagem[i];
    if (dig_out == true) {
      digitalWrite(led, HIGH);
      Serial.print("1");
      delay(temp);
      digitalWrite(led, LOW);
      Serial.print("0");
      delay(temp);
    }
    else {
      digitalWrite(led, LOW);
      Serial.print("0");
      delay(temp);
      digitalWrite(led, HIGH);
      Serial.print("1");
      delay(temp);
    }

    dig_out = 0x10 & mensagem[i];
    if (dig_out == true) {
      digitalWrite(led, HIGH);
      Serial.print("1");
      delay(temp);
      digitalWrite(led, LOW);
      Serial.print("0");
      delay(temp);
    }
    else {
      digitalWrite(led, LOW);
      Serial.print("0");
      delay(temp);
      digitalWrite(led, HIGH);
      Serial.print("1");
      delay(temp);
    }

    dig_out = 0x08 & mensagem[i];
    if (dig_out == true) {
      digitalWrite(led, HIGH);
      Serial.print("1");
      delay(temp);
      digitalWrite(led, LOW);
      Serial.print("0");
      delay(temp);
    }
    else {
      digitalWrite(led, LOW);
      Serial.print("0");
      delay(temp);
      digitalWrite(led, HIGH);
      Serial.print("1");
      delay(temp);
    }
    dig_out = 0x04 & mensagem[i];
    if (dig_out == true) {
      digitalWrite(led, HIGH);
      Serial.print("1");
      delay(temp);
      digitalWrite(led, LOW);
      Serial.print("0");
      delay(temp);
    }
    else {
      digitalWrite(led, LOW);
      Serial.print("0");
      delay(temp);
      digitalWrite(led, HIGH);
      Serial.print("1");
      delay(temp);
    }

    dig_out = 0x02 & mensagem[i];
    if (dig_out == true) {
      digitalWrite(led, HIGH);
      Serial.print("1");
      delay(temp);
      digitalWrite(led, LOW);
      Serial.print("0");
      delay(temp);
    }
    else {
      digitalWrite(led, LOW);
      Serial.print("0");
      delay(temp);
      digitalWrite(led, HIGH);
      Serial.print("1");
      delay(temp);
    }

    dig_out = 0x01 & mensagem[i];
    if (dig_out == true) {
      digitalWrite(led, HIGH);
      Serial.print("1");
      delay(temp);
      digitalWrite(led, LOW);
      Serial.print("0");
      delay(temp);
    }
    else {
      digitalWrite(led, LOW);
      Serial.print("0");
      delay(temp);
      digitalWrite(led, HIGH);
      Serial.print("1");
      delay(temp);
    }

  }
  Serial.println("Fim");
}



