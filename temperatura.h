//
// Created by davi on 11/8/17.
//

#ifndef ESTACAO_TEMPERATURA_H
#define ESTACAO_TEMPERATURA_H

#define SENSOR_TEMPERATURA "/sys/bus/w1/devices/28-0416c1d9dfff/w1_slave"

double read_temp(void);

void *thread_temperatura(void *pVoid);

void *temperatura(void *pVoid);

#endif //ESTACAO_TEMPERATURA_H
