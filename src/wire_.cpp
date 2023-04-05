
#include "wire_.h"
#include <wiringPiI2C.h>

void wire_::set_fd(int fd)
{
    _fd = fd;
}

int wire_::read(int ctx, uint8_t reg, uint8_t *data, uint16_t len)
{
    int32_t ret;
    // ret = ctx->read_reg(ctx->handle, reg, data, len);
    ret = wiringPiI2CReadBlock(ctx, (int)reg, data, (int)len);

    return ret;
}

int wire_::write(int ctx, uint8_t reg, uint8_t *data, uint16_t len)
{
    int32_t ret;
    // ret = ctx->write_reg(ctx->handle, reg, data, len);
    ret = wiringPiI2CWriteBlock(ctx, (int)reg, data, (int)len);

    return ret;
}
