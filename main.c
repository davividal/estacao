#include <wiringPi.h>
#include <stdlib.h>

#include "umidade.h"
#include "mqtt.h"
#include "debug.h"
#include "temperatura.h"
#include "pressao.h"
#include "vento.h"

int main() {
    pthread_t s1, s2, s3, s4;
    s1 = s2 = s3 = s4 = 0;

    TRACE("Configurando wiringPi\n");
    if (wiringPiSetup() == -1) {
        exit(1);
    }

    mqtt();

    TRACE("Inicializando sensores...\n");

    pthread_create(&s1, NULL, umidade, NULL);
    pthread_create(&s2, NULL, temperatura, NULL);
    pthread_create(&s3, NULL, pressao, NULL);
    pthread_create(&s4, NULL, vento, NULL);

    pthread_join(s1, NULL);
    pthread_join(s2, NULL);
    pthread_join(s3, NULL);
    pthread_join(s4, NULL);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (1);
#pragma clang diagnostic pop
}