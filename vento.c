//
// Created by davi on 11/8/17.
//

#include <pthread.h>
#include <math.h>
#include <wiringPi.h>
#include "vento.h"
#include "mqtt.h"
#include "debug.h"

static time_t inicio_volta;
static double tempo_volta;
static int voltas;
static double frequency;

void conta_pulsos(void) {
    if (inicio_volta != 0) {
        tempo_volta -= tempo_volta / ++voltas;
        tempo_volta += difftime(time(NULL), inicio_volta) / voltas;

        frequency = 1/tempo_volta;
        printf("t = [%f]s;  f = [%f] Hz; [%f] rpm\n", tempo_volta, frequency, 60*frequency);
    }
    inicio_volta = time(NULL);
}

double velocidade() {
    double v = 0;

    v = 2 * frequency * R * M_PI * 3.6;

    return v;
}

void *thread_vento(void *pVoid) {
    double v;

    pinMode(PINO, INPUT);
    wiringPiISR(PINO, INT_EDGE_FALLING, &conta_pulsos);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (1) {
        v = velocidade();
        voltas = inicio_volta = 0;
        tempo_volta = 0;
        mqtt_pub_double("vento", v);
        printf("Velocidade: %.1f km/h (%.1f m/s)\n", v, v/3.6);
        delay(6000);
    }
#pragma clang diagnostic pop
}

void *vento(void *pVoid) {
    pthread_t t1 = 0;

    TRACE("Criando thread de velocidade do vento\n");
    pthread_create(&t1, NULL, thread_vento, NULL);
    TRACE("Iniciando thread de velocidade do vento\n");
    pthread_join(t1, NULL);

    return NULL;
}
