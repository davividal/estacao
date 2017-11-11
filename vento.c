//
// Created by davi on 11/8/17.
//

#include <time.h>
#include <pthread.h>
#include <math.h>
#include <wiringPi.h>

#include "vento.h"
#include "mqtt.h"
#include "debug.h"
#include "config.h"

struct timespec inicio_volta;
static int voltas = 0;
static double tempo_voltas[10];
volatile double frequency;

void conta_pulsos(void) {
    double tempo_volta = 0;
    struct timespec fim_volta;
    int i;
    double sum = 0;

    if (inicio_volta.tv_sec != 0) {
        clock_gettime(CLOCK_MONOTONIC, &fim_volta);
        tempo_volta = (fim_volta.tv_sec - inicio_volta.tv_sec) * 1000000 + (fim_volta.tv_nsec - inicio_volta.tv_nsec) / 1000;
        tempo_volta /= 1000000;
        TRACE("fim_volta: [%lf], [%lf], tempo_volta: [%lf]\n", (double) fim_volta.tv_sec, (double) fim_volta.tv_nsec, tempo_volta);

        if (fabs(tempo_volta) > 10e-7) {
            tempo_voltas[voltas++] = tempo_volta;

            if (voltas == 9) {
                voltas = 0;
                for (i = 0; i < 10; i++) {
                    sum += tempo_voltas[i];
                }
                tempo_volta = sum / 10;
            }

            frequency = 1 / tempo_volta;
            TRACE("t = [%lf]s;  f = [%f] Hz; [%f] rpm\n", tempo_volta, frequency, 60 * frequency);
        }
    }
    TRACE("voltas = %d\n", voltas);
    clock_gettime(CLOCK_MONOTONIC, &inicio_volta);
}

double velocidade() {
    double v = 0;

    v = 2 * frequency * R * M_PI * 3.6;

    return v;
}

void *thread_vento(void *pVoid) {
    double v;

    pinMode(PINO_ANEMOMETRO, INPUT);
    wiringPiISR(PINO_ANEMOMETRO, INT_EDGE_FALLING, &conta_pulsos);

    sensor_vento = 1;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (1) {
        v = velocidade();
        voltas = 0;
        mqtt_pub_double("vento", v);
        printf("Velocidade: %.1f km/h (%.1f m/s)\n", v, v/3.6);
        delay(INTERVALO_VENTO);
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
