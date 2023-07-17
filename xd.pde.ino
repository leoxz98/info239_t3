#include <VirtualWire.h>
#include <stdint.h>

void coutText(char context,int valOne,char text, char valTwo){
  Serial.print(context);
  Serial.print(valOne);
  Serial.print(text);
  Serial.println(valTwo);
}

void llenarMatriz(char matriz[15][8]) {
  for (int i = 0; i < 15; i++) {
    for (int j = 0; j < 8; j++) {
      matriz[i][j] = 'x';
    }
  }
}

bool verificarFilaCompleta(const char matriz[15][8], int fila) {
  for (int j = 0; j < 8; j++) {
    if (matriz[fila][j] == 'x') {
      return false; // Hay al menos una "x", la fila no está completa
    }
  }
  return true; // No hay ninguna "x", la fila está completa
}

char obtenerSegundoDigito(int numero) {
  int segundoDigito = numero % 10; // Obtener el segundo dígito utilizando el operador módulo (%)
  return char(segundoDigito + '0'); // Convertir el segundo dígito a char sumando el valor ASCII de '0'
}

int charToInt(char c) {
  if (c == 'g' || c == 'G') {
    return 0;
  } else if (c == 'r' || c == 'R') {
    return 1;
  } else if (c == 'u' || c == 'U') {
    return 2;
  } else if (c == 'p' || c == 'P') {
    return 3;
  } else if (c == 'o' || c == 'O') {
    return 4;
  } else if (c == '_') {
    return 5;
  } 
  else {
    return -1; 
  }
}

void imprimirMatriz(char matriz[15][8]) {
  Serial.println("Matriz:");

  for (int i = 0; i < 15; i++) {
    Serial.print("Fila ");

    if (i == 0) {
      Serial.print("BT 0");
    } else {
      Serial.print("GR ");
      Serial.print(i);
    }

    Serial.print(": ");

    for (int j = 0; j < 8; j++) {
      if (matriz[i][j] != '\0') {
        Serial.print(char(matriz[i][j]));
      } else {
        Serial.print("x");
      }
      Serial.print(" ");
    }
    Serial.println();
  }
}

// definicion de variables 
//const uint8_t nosotros = 6;
const uint8_t origen = 6; // Nuestro grupo (6)
const uint8_t destino = 0; // Destino (0)
const uint8_t objetivo = 0; // objetivo

const uint8_t mensaje[8] = {71, 114, 117, 112, 111, 95, 48, 54}; // "Grupo_06" en formato uint8_t

char matriz[15][8];  //almacenamiento de lo que se recibe en colisiones

uint8_t msjSend[3]; // arreglo de lo que enviamos

int exito = 0; // si es 8 se considera exito al enviar todos los paquetes sin colision

void setup() {
  Serial.begin(9600);
  vw_set_ptt_inverted(true);
  vw_setup(2000);
  vw_set_rx_pin(2);
  vw_set_tx_pin(3);
  vw_rx_start();
  llenarMatriz(matriz);
}

void loop() {

  msjSend[0] = origen;
  msjSend[1] = destino;

  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) {
      
      uint8_t ori = buf[0];
      uint8_t desti = buf[1];
      uint8_t msj = buf[2];
      int indexOri = int(ori);

      coutText("Colision con grupo: ",indexOri," Msj: ",char(msj));

      // verificar si msj es letra o numero
      if( charToInt(char(msj)) == -1){
        Serial.println("entro a guardar nums!!");
        int asave = int(msj);
        Serial.println("aassave: ");
        Serial.println(asave);
        if (asave == 48 && matriz[indexOri][6] == 'x' ){
          matriz[indexOri][6] = '0';
        }
        else if(asave == 48 && matriz[indexOri][6] == '0'){
          matriz[indexOri][7] = '0';
        }
        else if( asave == 49 && matriz[indexOri][6]!='x'){
          matriz[indexOri][6] = '1';
        }
        else if(asave == 49 && matriz[indexOri][6]=='1'){
          matriz[indexOri][7] = '1';
        }
        else{
          if (matriz[indexOri][7]=='x'){
            matriz[indexOri][7] = char(msj);
          }
        }
      }
      else{
        int indiceLetra = charToInt(char(msj));
        matriz[indexOri][indiceLetra] = char(msj);
      }
  }

  else{
    int i = 0;
    while(i < 8) {

      if (vw_get_message(buf, &buflen)) {   // Detección de colisiones
      //guardar en matriz y esperar tiempo random
        uint8_t ori = buf[0];
        uint8_t desti = buf[1];
        uint8_t msj = buf[2];
        int indexOri = int(ori);

        coutText("Colision con grupo: ",indexOri," Msj: ",char(msj));

        // verificar si msj es letra o numero
        if( charToInt(char(msj)) == -1){
          Serial.println("entro a guardar nums!!");
          int asave = int(msj);
          Serial.println("aassave: ");
          Serial.println(asave);

        if (asave == 48 && matriz[indexOri][6] == 'x' ){
          matriz[indexOri][6] = '0';
        }
        else if(asave == 48 && matriz[indexOri][6] == '0'){
          matriz[indexOri][7] = '0';
        }
        else if( asave == 49 && matriz[indexOri][6]=='x'){
          matriz[indexOri][6] = '1';
        }
        else if(asave == 49 && matriz[indexOri][6]=='1'){
          matriz[indexOri][7] = '1';
        }
        else{
          //matriz[indexOri][7] = char(msj);
          if (matriz[indexOri][7]=='x'){
            matriz[indexOri][7] = char(msj);
          }
        }
      }
      else{
        int indiceLetra = charToInt(char(msj));
        matriz[indexOri][indiceLetra] = char(msj);
      }

      unsigned int tiempoAleatorio = random(1000, 2000); // Genera tiempo aleatorio de espera
      delay(tiempoAleatorio); // Espera durante el tiempo aleatorio generado
        
      imprimirMatriz(matriz);       
        
      } 
      else{
        msjSend[2] = mensaje[i]; // Cambiar el último término de mensaje[i]
        vw_send((uint8_t *)msjSend, 24);
        vw_wait_tx(); 
        unsigned int tiempoAleatorio = random(1000, 2000); // Genera tiempo aleatorio de espera
        delay(tiempoAleatorio); // Espera durante el tiempo aleatorio generado
        
        Serial.println("mensaje enviado:");
        Serial.print(char(mensaje[i]));

        i+= 1;
      }
    }
  }
}

  

  

    

    

    