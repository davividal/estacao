//
// Created by davi on 11/7/17.
//

#ifndef ESTACAO_HUMIDADE_H
#define ESTACAO_HUMIDADE_H

#include <wiringPi.h>
#include <pthread.h>

#define MAX_TIMINGS    85
#define DHT_PIN        3    /* GPIO-22 */

extern int data[5];

//void setup_dht();

void read_dht_data();

void *thread_dht(void *pVoid);

void humidade(void);

#endif //ESTACAO_HUMIDADE_H
