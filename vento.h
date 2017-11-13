//
// Created by davi on 11/8/17.
//

#ifndef ESTACAO_VENTO_H
#define ESTACAO_VENTO_H

// GPIO 24 = pino 5
#define PINO_ANEMOMETRO 5

// Diametro em metros, medido com paquímetro e convertido com calculadora
#define DIAMETRO 0.0321

// Raio: (DIAMETRO (mm)/100 (mm/m)) / 2
#define R (DIAMETRO/2)

void conta_pulsos(void);

double velocidade();

void *thread_vento(void *pVoid);

void *vento(void *pVoid);

#endif //ESTACAO_VENTO_H
