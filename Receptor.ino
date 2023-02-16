int pos_fim_vteste = 0;
bool entrou = false;
int i_dec = 0;
int achou_cabecalho = 0;
unsigned long tempo1 = 0;
unsigned long tempo2 = 0;
unsigned long tempo_total = 0;
int contagem_bits = 0;

//função para detectar a transição de borda entre os dois bits centrais do vetor de leituras
unsigned char manchester(unsigned char *msg)
{
  int uns = 0; //contador de número de uns recebidos
  int zeros = 0; //contador do número de zeros recebidos
  bool subida = false; //detectada uma transição de subida
  bool descida = false; //detectada uma transição de descida
  int i = 0; //indice da posição do vetor mensagem
  static int erro_manch_zeros = 0;
  static int erro_manch_uns = 0;
  for (i = 0; i < 8; ++i)
  {
    //se o valor do vetor msg na posição i for igual a 1, incrementa o contador de uns
    if (msg[i] == 1)
    {
      ++uns;
    }
    //se o valor do vetor msg na posição i for igual a 0, incrementa o contador de zeros
    else if (msg[i] == 0)
    {
      ++zeros;
    }
    //se i está na posição 4
    if (i == 4)
    {
      //e o valor de msg na posição 3 é diferente da posição 4
      if (msg[i - 1] != msg[i])
      {
        //e se foram detectados 4 zeros até então
        if (zeros == 4)
        {
          subida = true;//detectou-se uma transição de subida
          if(erro_manch_zeros == 1){
            Serial.println("Erro! Número de 0s ou 1s incoerente com a codificação Manchester");
            erro_manch_zeros = 0;
          }
        }
        //ou se foram detectados 4 uns
        else if (uns == 4)
        {
          descida = true; //detectou-se uma transição de descida
          if(erro_manch_uns == 1){
            Serial.println("Erro! Número de 0s ou 1s incoerente com a codificação Manchester");
            erro_manch_uns = 0;
          }
        }
        else
        {
          return 2; //se não foram detectados 4 zeros ou 4 uns após 4 leituras da porta digital, existe um erro no receptor
          Serial.println("Erro! Número de 0s ou 1s incoerente com a codificação Manchester");
        }
      }
    }
    //se o contador i estiver na última posição do vetor (7)
    if (i == 7)
    {
      //e já foram detectados 4 zeros e 4 uns, como previsto pelo código Manchester
      if (zeros == 4 && uns == 4)
      {
        //se já foi detectada uma transição de subida
        if (subida == true)
        {
          return 1;// retorna o bit 1 (lembrando que a detecção é inversa devido ao módulo fotodiodo)
        }
        //se já foi detectada uma transição de descida
        else if (descida == true)
        {
          return 0; //retorna o bit 0
        }
        else if(zeros>4){
         ++erro_manch_zeros;
        }
        else if(uns>4){
          ++erro_manch_uns;
        }
      }
    }
  }
  Serial.println("Erro! Número de 0s ou 1s incoerente com a codificação Manchester");
  return 2;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//função para transformar um byte em caracter
unsigned char byteparachar(unsigned char *vetor) {
  unsigned char teste; //variável para guardar o valor do caracter
  bool cabecalho; //variável para guardar o retorno da função idcabecalho
  //utiliza-se a técnica de máscara de bit para selecionar a posição do bit na formação
  //do caracter e desloca-se cada posição do byte
  teste = 0x80 & (vetor[0] << 7);
  teste = (0x40 & (vetor[1] << 6)) | teste;
  teste = (0x20 & (vetor[2] << 5)) | teste;
  teste = (0x10 & (vetor[3] << 4)) | teste;
  teste = (0x08 & (vetor[4] << 3)) | teste;
  teste = (0x04 & (vetor[5] << 2)) | teste;
  teste = (0x02 & (vetor[6] << 1)) | teste;
  teste = (0x01 & vetor[7]) | teste;
  return teste;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//


//função para inserir retorno da função Manchester em um vetor circular para teste de cabecalho
int insercaoparateste(unsigned char valor, int &pos_fim)
{
  static bool cheio = false; //variável booleana que guarda true quando o primeiro vetor está cheio
  static int pos_ini = 0;
  static unsigned char vetor[8] = {0, 0, 0, 0, 0, 0, 0, 0}; //vetor circular
  static unsigned char vet[8] = {0, 0, 0, 0, 0, 0, 0, 0}; //vetor circular auxiliar
  unsigned char caractere = 0;
  int retorno = 0;

  if (!cheio) {
    vetor[pos_fim] = valor; // vetor na posição final recebe o valor passado como parâmetro na função
    if (pos_fim - pos_ini == 8) {
      caractere = byteparachar(vetor);
      if (caractere == 0xb5) {
        cheio = false;
        pos_ini = 0;
        pos_fim = 0;
        Serial.println("Cabecalho encontrado!");
        retorno = 1;
      }
    }
    else {
      cheio = true; //primeiro vetor cheio
      retorno = 0;
    }
  }
  //vetor circular: descarta a leitura mais antiga e insere uma nova na posição final,
  //formando um novo byte a cada novo valor.
  else {
    if (pos_fim > 7) {
      pos_fim = 0;
    }

    vetor[pos_fim] = valor;
    ++pos_ini;

    if (pos_ini > 7) {
      pos_ini = 0;
    }

    int k = pos_ini;
    int m = 0;
    while (true) {
      vet[m] = vetor[k];
      ++k;
      ++m;
      if (k >= 8) {
        k = 0;
      }
      if (k == pos_fim) {
        vet[m] = vetor[k];
        break;
      }
    }
    //a cada byte formado, chama-se a função byteparachar para conversão dos valores
    caractere = byteparachar(vet);

    if (caractere == 0xb5) {
      cheio = false;
      pos_ini = 0;
      pos_fim = 0;
      Serial.println("Cabecalho encontrado");
      retorno = 1;
    }
    else {
      ++pos_fim;
      cheio = true;
      retorno = 0;
    }
  }
  return retorno;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//cálculo do checksum A
unsigned char checksuma(unsigned char idmensagem, unsigned char dado) {
  unsigned char checksum_a = idmensagem + dado;
  return checksum_a;
}

//cálculo do checksum B
unsigned char checksumb(unsigned char idmensagem, unsigned char dado, unsigned char checksum_a) {
  unsigned char checksum_b = idmensagem + dado + checksum_a;
  return checksum_b;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//decodificação e impressão da mensagem
bool decodificacao(unsigned char valor_decod) {
  static unsigned char id_msg[] = {0, 0, 0, 0, 0, 0, 0, 0};
  static unsigned char msg_decod[] = {0, 0, 0, 0, 0, 0, 0, 0};
  static unsigned char checksum_a[] = {0, 0, 0, 0, 0, 0, 0, 0};
  static unsigned char checksum_b[] = {0, 0, 0, 0, 0, 0, 0, 0};

  static int i_decod = 0;
  static int pos_decod = 0;

  bool msg_complete = false;
  if (i_decod < 8) {
    id_msg[i_decod] = valor_decod;
  }
  else if (i_decod < 16) {
    pos_decod = i_decod - 8;
    msg_decod[pos_decod] = valor_decod;
  }
  else if (i_decod < 24) {
    pos_decod = i_decod - 16;
    checksum_a[pos_decod] = valor_decod;
  }
  else {
    pos_decod = i_decod - 24;
    checksum_b[pos_decod] = valor_decod;
  
    if (i_decod == 31)
    {
      msg_complete = true;
    }
  }
  ++i_decod;
 
  if (msg_complete)
  {
      
    unsigned char idmensagem_char = byteparachar(id_msg);
    Serial.print("ID da mensagem: ");
    Serial.println(idmensagem_char);
    
    unsigned char dado_char = byteparachar(msg_decod);
    Serial.print("Dado: ");
    Serial.println(dado_char);
    
    unsigned char checksum_a_char = byteparachar(checksum_a);
    Serial.print("Checksum A: ");
    Serial.println(checksum_a_char);
    
    unsigned char checksum_b_char = byteparachar(checksum_b);
    Serial.print("Checksum B: ");
    Serial.println(checksum_b_char);
    
    unsigned char checksuma_calculado = 0;
    checksuma_calculado = checksuma(idmensagem_char, dado_char);
    Serial.print("Checksum A Calculado: ");
    Serial.println(checksuma_calculado);
    
    unsigned char checksumb_calculado = 0;
    checksumb_calculado = checksumb(idmensagem_char, dado_char, checksuma_calculado);
    Serial.print("Checksum B Calculado: ");
    Serial.println(checksumb_calculado);
    
    //se o checksum calculado é igual ao checksum recebido, a mensagem está correta
    if ((checksuma_calculado == checksum_a_char) && (checksumb_calculado == checksum_b_char)) {
      Serial.println("Mensagem recebida com sucesso");
      i_decod = 0;
      achou_cabecalho = 0;
      for(int i = 0; i<8; ++i){
        id_msg[i] = 0;
        msg_decod[i] = 0;
        checksum_a[i] = 0;
        checksum_b[i] = 0;
        
      }
      msg_complete = false;
    }
    else {
      Serial.println("Mensagem incorreta");
      i_decod = 0;
      msg_complete = false;
      achou_cabecalho = 0;
      return false; 
    }
  }
  
  return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//MAIN
int i_ini = 0; //
int i_fim = 0;
bool buffer_cheio = false;
unsigned char mensagem[8]; //vetor circular de leituras
unsigned char msg[8]; //vetor circular auxiliar
int cont = 0;
unsigned char manch = 0; //variável para guardar o retorno da função Manchester

bool msg_ok = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);

}

void loop() {
  // put your main code here, to run repeatedly:

  unsigned char valor = digitalRead(8);//leituras da porta digital do Arduino
  if(contagem_bits == 0){
      tempo1 = millis();
      Serial.print("Tempo 1: ");
      Serial.println(tempo1);
      ++contagem_bits;
  }
  else if(contagem_bits == 1){
    tempo2 = millis();
    Serial.print("Tempo 2: ");
    Serial.println(tempo2);
    tempo_total = tempo2-tempo1;
    Serial.print("Delay mili");
    Serial.println(tempo_total);
    ++contagem_bits;
    float taxa = 1/(tempo_total*0.001);
    Serial.print("Taxa: ");
    Serial.println(taxa);
  }
  //mesmo processo da função insercaoparateste
  if (!buffer_cheio) {
    
    mensagem[i_fim] = valor;
    ++i_fim;
    if (i_fim - i_ini == 8) {
      manch = manchester(mensagem);
      //testa se o retorno da função Manchester é uma transição válida (0 ou 1)
      if (manch == 0 || manch == 1) {
        if (achou_cabecalho == 1) {
          decodificacao(manch);
        }
        else
        {
          achou_cabecalho = insercaoparateste(manch, pos_fim_vteste);
        }

        buffer_cheio = false;
        i_ini = 0;
        i_fim = 0;
      }
      else
      {
        buffer_cheio = true;
      }
    }
  }
  else
  {
    
    if (i_fim > 7)
    {
      i_fim = 0;
    }

    mensagem[i_fim] = valor;
    ++i_ini;

    if (i_ini > 7) {
      i_ini = 0;
    }

    int i = i_ini;
    int j = 0;
    while (true) {
      msg[j] = mensagem[i];
      ++i;
      ++j;
      if (i >= 8) {
        i = 0;
      }
      if (i == i_fim) {
        msg[j] = mensagem[i];
        break;
      }
    }
    manch = manchester(msg);
    if (manch == 0 || manch == 1) {
      if (achou_cabecalho == 1) {
        decodificacao(manch);
      }
      else
      {
        achou_cabecalho = insercaoparateste(manch, pos_fim_vteste);
      }

      buffer_cheio = false;
      i_ini = 0;
      i_fim = 0;

    }
    else {
      ++i_fim;
      buffer_cheio = true;
    }
  }
  delay(4.856); //velocidade das leituras (4x mais rápido que o envio dos bits, menos a compensação do atraso do receptor de 144 microsegundos)
}
