#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>

#include "humidade.h"
#include "mqtt.h"

int main() {
    printf("Configurando wiringPi\n");
    if (wiringPiSetup() == -1) {
        exit(1);
    }

    mqtt();

    printf("Inicializando sensores...\n");
    humidade();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (1);
#pragma clang diagnostic pop
}