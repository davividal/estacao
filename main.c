#include <wiringPi.h>
#include <stdlib.h>

#include "umidade.h"
#include "mqtt.h"
#include "debug.h"

int main() {
    TRACE((1, "Configurando wiringPi\n"));
    if (wiringPiSetup() == -1) {
        exit(1);
    }

    mqtt();

    TRACE((1, "Inicializando sensores...\n"));
    umidade();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (1);
#pragma clang diagnostic pop
}