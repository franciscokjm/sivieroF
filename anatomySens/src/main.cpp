//Programa desenvolvido para automação de sons descritivos de peças anatômicas.
//MCU: Espressif ESP WROOM 32
//IDE: Visual Studio Code () e Platofórmio 
//Desenvolvedor: Kelliton J M francisco
//Responsável: Fabio Siviero
//Laboratório de de Biologia do Desenvolvimento de Insetos
//Departamento de Biologia Celular e do Desenvolvimento
//Instituto de Ciências Biológicas - USP           

#include <Arduino.h>

const int sens = 4;

void setup(){
    Serial.begin(115200);
    pinMode(sens, INPUT);

}

void loop(){

    Serial.print(digitalRead(sens));
    delay(1000);
}