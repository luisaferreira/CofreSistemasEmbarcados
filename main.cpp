/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

//Lucas Gabriel Silva Santos
//Maria Luisa de Jesus Silva Ferreira

#include "mbed.h"
#include <chrono>
#include <iostream>
#include <string>

using namespace std::chrono;
// Blinking rate in milliseconds
#define BLINKING_RATE 250ms

// Declaração de contador que será utilizado para verificar a quantidade de
int contador = 0;

// Declaração dos pino digitais que serão utilizados por leds para representação
// dos dígitos pressionados.
BusOut leds(PA_8, PB_5, PB_6, PB_10, PB_3, PA_10);
// Declaração do pino digital que será utilizado por led para representar que a
// senha inserida está correta.
DigitalOut ledCorreto(PB_3);
/* Declaração do pino digital que será utilizado por led para representar que a
 * senha inserida está incorreta.*/
DigitalOut ledIncorreto(PA_10);

/* Declaração de timer que será utilizado para verificar o intervalo de tempo em
 * que os botões foram acionados, não aceitando o acionamento de botões com um
 * intervalo menor que 20 milissegundos. */
Timer tClique;

/* Declaração de timer que será utilizado para contagem dos 5 segundos para
 * piscar os leds de senha correta e incorreta.*/
Timer t5Segundos;

// Declaração da senha do cofre.
int senha = 5389;

/* Declaração da string que será preenchida quando o usuário pressionar algum
 * botão númerico.*/
string tentativaSenha = "";

/*Declaração de variável que irá guardar o valor de tempo, em milissegundos, da
 * última vez em que algum botão foi pressionado.*/
long long timeBefore = 0;

void AdicionarCaractere(int numero) {
  /*Capturando o valor atual do timer de clique para futura comparação.*/
  unsigned long long timeNow =
      duration_cast<milliseconds>(tClique.elapsed_time()).count();

  /*Verificando se o intervalo de tempo entre entre um pressionamento de botão e
   * outro foi menor ou igual a 20 milissegundos*/
  if (timeNow - timeBefore >= 20) {
    /*Se o intervalo for maior que 20 milissegundos, irá ser verificado o valor
     * do contador e se ele for menor que 4 significa que o usuário ainda não
     * digitou os 4 dígitos da senha.*/
    if (contador < 4) {
      /*Se os 4 dígitos ainda não foram digitados, irá se atribuir o valor do
       * último botão pressionado a string de representação da tentativa de
       * senha.*/
      tentativaSenha += numero;
      /*E o contador irá ser incrementado, representando o tamanho da string
       * tentativaSenha*/
      contador++;
      /*Cada if a seguir irá verificar o valor do contador para saber quantos
       * leds serão acionados a depender do valor do contador*/
      if (contador == 1) {
        leds = 0x20; // Será acionado 1 led.
      } else if (contador == 2) {
        leds = 0x30; // Serão acionados 2 leds.
      } else if (contador == 3) {
        leds = 0x38; // Serão acionados 3 leds.
      } else if (contador == 4) {
        leds = 0x3C; // Serão acionados 4 leds.
      }
    }
    /*Caso o contador tenha valor igual a 4, significa que todos os dígitos
     * foram preenchidos e o programa não irá fazer nada.*/
  }
  /*Caso o intervalo de tempo seja maior que 20 milissegundos, o programa não
   * irá fazer nada.*/

  /*A variável global timeBefore recebe o valor de timeNow, que guarda o valor
   * de tempo, em milissegundos, da última vez em que algum botão foi
   * pressionado.*/
  timeBefore = timeNow;
}

/*Como a função rise não aceita parâmetros, cada uma das seguintes funções foram
 * criadas para conseguir passar o valor de determinado botão.*/
void AdicionarCaractere0() { AdicionarCaractere(0); }
void AdicionarCaractere1() { AdicionarCaractere(1); }
void AdicionarCaractere2() { AdicionarCaractere(2); }
void AdicionarCaractere3() { AdicionarCaractere(3); }
void AdicionarCaractere4() { AdicionarCaractere(4); }
void AdicionarCaractere5() { AdicionarCaractere(5); }
void AdicionarCaractere6() { AdicionarCaractere(6); }
void AdicionarCaractere7() { AdicionarCaractere(7); }
void AdicionarCaractere8() { AdicionarCaractere(8); }
void AdicionarCaractere9() { AdicionarCaractere(9); }

void PiscarLedPor5Segundos(DigitalOut led) {
  /*Ínicio do timer que faz a contagem dos 5 segundos em que o led deverá
   * piscar.*/
  t5Segundos.start();

  /*Capturando o valor do timer que marca os 5 segundos para futura
   * comparação.*/
  unsigned long long duracaoTimer5Segundos =
      duration_cast<seconds>(t5Segundos.elapsed_time()).count();

  /*Verificando se o valor do timer que marca os 5 segundos é menor que 5
   * segundos*/
  while (duracaoTimer5Segundos <= 5) {
    /*Se o tempo de duração do timer for menor que 5 segundos, é alterado o
     * valor do led para a sua negação.*/
    led = !led;
    /*Colocando o programa para "dormir" por 250ms para o intervalo entre as
     * trocas de nível lógico do led ser maior.*/
    ThisThread::sleep_for(BLINKING_RATE);
    /*Capturando o valor do timer que marca os 5 segundos para comparar na
     * verificação do while.*/
    duracaoTimer5Segundos =
        duration_cast<seconds>(t5Segundos.elapsed_time()).count();
  }
  /*Assim que o timer tiver um valor maior que 5 segundos o loop será
   * encerrado.*/

  /*Parando o timer.*/
  t5Segundos.stop();
  /*Resetando o valor do timer para 0.*/
  t5Segundos.reset();
  /*Garantindo que o led terá como nível lógico 0 ao final da função para que
   * fique desligado.*/
  led = 0;
}

void Ok() {
  /*Capturando o valor atual do timer de clique para futura comparação.*/
  unsigned long long timeNow =
      duration_cast<milliseconds>(tClique.elapsed_time()).count();

  /*Verificando se o intervalo de tempo entre entre um pressionamento de botão e
   * outro foi menor ou igual a 20 milissegundos*/
  if (timeNow - timeBefore >= 20) {
    /*Realizando a conversão da string tentativaSenha para int.*/
    int tentativaSenhaInt = std::stoi(tentativaSenha);

    /*Comparando a senha real com a senha que o usuário forneceu.*/
    if (senha == tentativaSenhaInt) {
      /*Caso ambas as senhas sejam iguais, irá chamar a função
       * PiscarLedPor5Segundos passando como parâmetro o led que indica que a
       * senha está correta.*/
      PiscarLedPor5Segundos(ledCorreto);
    } else {
      /*Caso as senhas sejam diferentes, irá chamar a função
       * PiscarLedPor5Segundos passando como parâmetro o led que indica que a
       * senha está incorreta.*/
      PiscarLedPor5Segundos(ledIncorreto);
    }
  }

  /*Aqui o programa será reiniciado ao seu estado inicial, portanto todas as
   * variáveis, com exceção do timer de clique e a variável timeBefore, serão
   * setadas a seu valor original.*/
  contador = 0;
  tentativaSenha = "";
  leds = 0x00;

  /*A variável global timeBefore recebe o valor de timeNow, que guarda o valor
   * de tempo, em milissegundos, da última vez em que algum botão foi
   * pressionado.*/
  timeBefore = timeNow;
}

void Limpar() {
    /*Capturando o valor atual do timer de clique para futura comparação.*/
  unsigned long long timeNow =
      duration_cast<milliseconds>(tClique.elapsed_time()).count();

/*Verificando se o intervalo de tempo entre entre um pressionamento de botão e
   * outro foi menor ou igual a 20 milissegundos.*/
  if (timeNow - timeBefore >= 20) {
      /*Verificando se a string tentativaSenha não é uma string vazia.*/
    if (!tentativaSenha.empty()) {
        /*Verificando o valor do contador para desligamento do led que representa o dígito que foi apagado.*/
      if (contador == 1) {
        leds = 0x00; // Será desligado todos os leds.
      } else if (contador == 2) {
        leds = 0x20; //Será ligado apenas 1 led.
      } else if (contador == 3) {
        leds = 0x30; //Serão ligados 2 leds.
      } else if (contador == 4) {
        leds = 0x38; //Serão ligados 3 leds.
      }

/*A variável tentativaSenha irá receber como seu novo valor uma substring de si mesma que vai da posição 0 até o penúltimo caractere.*/
      tentativaSenha = tentativaSenha.substr(0, tentativaSenha.size() - 1);
      /*O valor do contador será decrementado para corresponder com o novo número de caracteres de tentativaSenha.*/
      contador--;
    }
  }

  /*A variável global timeBefore recebe o valor de timeNow, que guarda o valor
   * de tempo, em milissegundos, da última vez em que algum botão foi
   * pressionado.*/
  timeBefore = timeNow;
}

int main() {
  /*Ínicio do timer que faz a contagem do intervalo de tempo decorrido entre o
   * acionamento dos botões.*/
  tClique.start();

  // Declaração dos botões que serão utilizados.
  InterruptIn btn0(PB_8);      // Representando o número 0.
  InterruptIn btn1(PB_9);      // Representando o número 1.
  InterruptIn btn2(PA_5);      // Representando o número 2.
  InterruptIn btn3(PA_6);      // Representando o número 3.
  InterruptIn btn4(PA_7);      // Representando o número 4.
  InterruptIn btn5(PC_7);      // Representando o número 5.
  InterruptIn btn6(PA_9);      // Representando o número 6.
  InterruptIn btn7(PB_4);      // Representando o número 7.
  InterruptIn btn8(PA_2);      // Representando o número 8.
  InterruptIn btn9(PA_3);      // Representando o número 9.
  InterruptIn btnLimpar(PB_2); // Representando o "Limpar".
  InterruptIn btnOk(PC_4);     // Representando o "Ok".

  /*Atribuindo o nível lógico 0 aos leds para que comecem desligados.*/
  leds = 0x00;
  ledCorreto = 0;
  ledIncorreto = 0;

/*Quando o botão "subir" será chamada uma função que será correspondente com o papel que esse botão desempenha no programa.*/
  btn0.rise(&AdicionarCaractere0);
  btn1.rise(&AdicionarCaractere1);
  btn2.rise(&AdicionarCaractere2);
  btn3.rise(&AdicionarCaractere3);
  btn4.rise(&AdicionarCaractere4);
  btn5.rise(&AdicionarCaractere5);
  btn6.rise(&AdicionarCaractere6);
  btn7.rise(&AdicionarCaractere7);
  btn8.rise(&AdicionarCaractere8);
  btn9.rise(&AdicionarCaractere9);
  btnOk.rise(&Ok);
  btnLimpar.rise(&Limpar);

  while (true) {
    ThisThread::sleep_for(BLINKING_RATE);
  }
}
