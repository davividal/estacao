//
// Created by davi on 11/7/17.
//

#ifndef ESTACAO_UMIDADE_H
#define ESTACAO_UMIDADE_H

#include <wiringPi.h>
#include <pthread.h>

#define MAX_TIMINGS    85
#define DHT_PIN        3    /* GPIO-22 */

extern int data[5];

double read_dht_data();

void *thread_dht(void *pVoid);

void *umidade(void *pVoid);

#endif //ESTACAO_UMIDADE_H
