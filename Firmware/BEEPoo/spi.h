#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

void SpiDisable(void);
void SpiEnable(uint8_t fast);
uint8_t SpiSend(uint8_t data);

#ifdef __cplusplus
}
#endif

#endif
