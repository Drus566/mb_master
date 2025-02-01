#include <iostream>

// #ifdef __cplusplus
// extern "C" {
//     #include "modbus.h"
// }
// #endif
#include "modbus.h"

int main(void) {
    modbus_t *ctx = modbus_new_rtu("/dev/pts/17", 115200, 'N', 8, 1);
    if (ctx == NULL) {
        fprintf(stderr, "Unable to allocate libmodbus context\n");
        return -1;
    }

    // Установка адреса ведомого устройства
    modbus_set_slave(ctx, 1);

    // Подключение к устройству
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    // Чтение регистров
    uint16_t tab_reg[32]; // Массив для хранения прочитанных регистров
    int rc = modbus_read_registers(ctx, 0, 10, tab_reg); // Чтение 10 регистров с адреса 0
    if (rc == -1) {
        fprintf(stderr, "Read failed: %s\n", modbus_strerror(errno));
    } else {
        printf("Read %d registers:\n", rc);
        for (int i = 0; i < rc; i++) {
            printf("Register %d: %d\n", i, tab_reg[i]);
        }
    }

    // Запись в регистры
    uint16_t new_values[5] = {1, 2, 3, 4, 5};
    rc = modbus_write_registers(ctx, 0, 5, new_values); // Запись 5 регистров с адреса 0
    if (rc == -1) {
        fprintf(stderr, "Write failed: %s\n", modbus_strerror(errno));
    }

    // Закрытие соединения и освобождение ресурсов
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}