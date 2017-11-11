//
// Created by davi on 11/9/17.
//

#ifndef ESTACAO_MAIN_H
#define ESTACAO_MAIN_H

// Led de status está na GPIO 14
#define LED_STATUS 15

#define INTERVALO_UMIDADE 4000
#define INTERVALO_TEMPERATURA 10000
#define INTERVALO_PRESSAO 10000
#define INTERVALO_VENTO 10000

extern volatile int terminate;
extern volatile int sensor_umidade;
extern volatile int sensor_temperatura;
extern volatile int sensor_pressao;
extern volatile int sensor_vento;
extern volatile int conexao_mqtt;

#endif //ESTACAO_MAIN_H
