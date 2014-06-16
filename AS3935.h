#ifndef	AS3935_H
#define	AS3935_H

#include "Arduino.h"

#define	AS3935_INTERRUPT_NOISE		(0x01)
#define	AS3935_INTERRUPT_DISTURBER	(0x04)
#define	AS3935_INTERRUPT_LIGHTNING	(0x08)

#define	AS3935_LIGHTNING_OVERHEAD	(0x01)
#define	AS3935_LIGHTNING_OUT_OF_RANGE	(0x3f)

#define	AS3935_MIN_LIGHTNING_1		(0x00)
#define	AS3935_MIN_LIGHTNING_5		(0x01)
#define	AS3935_MIN_LIGHTNING_9		(0x02)
#define	AS3935_MIN_LIGHTNING_16		(0x03)

class AS3935
{
  public:
    AS3935(void);
    AS3935(uint8_t);
    void reset(void);
    void calibrate(int8_t);
    uint8_t interrupt(void);
    uint8_t distance(void);
    void indoors(bool);
    bool indoors(void);
    void noise_floor(uint8_t);
    uint8_t noise_floor(void);
    uint8_t raise_noise_floor(void);
    uint8_t lower_noise_floor(void);
    void min_strikes(uint8_t);
    uint8_t min_strikes(void);
  private:
    int _id;
    uint8_t (AS3935::*read_register)(uint8_t);
    void (AS3935::*write_register)(uint8_t, uint8_t);
    uint8_t i2c_read_register(uint8_t);
    void i2c_write_register(uint8_t, uint8_t);
    uint8_t spi_read_register(uint8_t);
    void spi_write_register(uint8_t, uint8_t);
};

#endif
