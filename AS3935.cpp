#include <Wire.h>

#include "AS3935.h"

#define	AS3935_AFE_INDOOR	(0x24)
#define	AS3935_AFE_OUTDOOR	(0x1c)

AS3935::AS3935(void)
{
  read_register = &AS3935::spi_read_register;
  write_register = &AS3935::spi_write_register;
}

AS3935::AS3935(uint8_t id)
{
  _id = id;

  read_register = &AS3935::i2c_read_register;
  write_register = &AS3935::i2c_write_register;
}

uint8_t
AS3935::i2c_read_register(uint8_t reg)
{
  Wire.beginTransmission(_id);
  Wire.write(reg);
  Wire.endTransmission(false);

  Wire.requestFrom(_id, 1, true);

  return (Wire.read());
}

void
AS3935::i2c_write_register(uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(_id);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission(true);
}

uint8_t
AS3935::spi_read_register(uint8_t reg)
{
  /* FIXME */
}

void
AS3935::spi_write_register(uint8_t reg, uint8_t value)
{
  /* FIXME */
}

void
AS3935::reset(void)
{
  (this->*write_register)(0x3c, 0x96);
}

void
AS3935::calibrate(int8_t tune_cap)
{
  uint8_t data = (this->*read_register)(0x08);

  (this->*write_register)(0x08, (data & 0xf0) | tune_cap);

  delay(2);

  data = (this->*read_register)(0x08);

  (this->*write_register)(0x3d, 0x96);
  (this->*write_register)(0x08, data | 0x20);

  delay(2);

  (this->*write_register)(0x08, data & 0xdf);
}

uint8_t
AS3935::interrupt(void)
{
  return ((this->*read_register)(0x03) & 0x0f);
}

uint8_t
AS3935::distance(void)
{
  return ((this->*read_register)(0x07) & 0x3f);
}

void
AS3935::indoors(bool indoors)
{
  uint8_t data = (this->*read_register)(0x00);

  (this->*write_register)(0x00, (data & 0xc1) | indoors ? AS3935_AFE_INDOOR : AS3935_AFE_OUTDOOR);
}

bool
AS3935::indoors(void)
{
  return (((this->*read_register)(0x00) & 0x3e) == AS3935_AFE_INDOOR);
}

void
AS3935::noise_floor(uint8_t level)
{
  uint8_t data = (this->*read_register)(0x01);

  (this->*write_register)(0x01, (data & 0x8f) | ((level & 0x07) << 4));
}

uint8_t
AS3935::noise_floor(void)
{
  return (((this->*read_register)(0x01) & 0x70) >> 4);
}

uint8_t
AS3935::raise_noise_floor(void)
{
  uint8_t level = AS3935::noise_floor();

  if (level < 7) {
    AS3935::noise_floor(++level);
  }

  return (level);
}

uint8_t
AS3935::lower_noise_floor(void)
{
  uint8_t level = AS3935::noise_floor();

  if (level > 0) {
    AS3935::noise_floor(--level);
  }

  return (level);
}

void
AS3935::min_strikes(uint8_t strikes)
{
  uint8_t data = (this->*read_register)(0x02);

  (this->*write_register)(0x02, (data & 0xcf) | ((strikes & 0x03) << 4));
}

uint8_t
AS3935::min_strikes(void)
{
  return (((this->*read_register)(0x02) & 0x30) >> 4);
}
