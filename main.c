#include <wiringPi.h>
#include <stdlib.h>

#include "umidade.h"
#include "mqtt.h"
#include "debug.h"

int main() {
    TRACE("Configurando wiringPi\n");
    if (wiringPiSetup() == -1) {
        exit(1);
    }

    mqtt();

    TRACE("Inicializando sensores...\n");
    umidade();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (1);
#pragma clang diagnostic pop
}