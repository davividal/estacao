//
// Created by davi on 11/8/17.
//

#ifndef ESTACAO_VENTO_H
#define ESTACAO_VENTO_H

// GPIO 24 = pino 5
#define PINO 5

// Raio, medido com paquímetro
#define R 0.0155

void conta_pulsos(void);

double velocidade();

void *thread_vento(void *pVoid);

void *vento(void *pVoid);

#endif //ESTACAO_VENTO_H
