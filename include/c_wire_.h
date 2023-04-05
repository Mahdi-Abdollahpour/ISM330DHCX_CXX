
#ifndef __C_WIRE_H__
#define __C_WIRE_H__

#ifdef __cplusplus
extern "C" {
#endif
// #include <cstdint>
#include <stdint.h>
// #include <sys/types.h>

//   #if defined __UINT32_MAX__ or UINT32_MAX
//   #include <inttypes.h>
//   #else
//   typedef unsigned char uint8_t;
//   typedef unsigned short uint16_t;
//   //typedef unsigned long uint32_t;
//   //typedef unsigned long long uint64_t;
//   #endif


struct c_wire;
typedef struct c_wire c_wire_t;

c_wire_t *c_wire_create();
void c_wire_destroy(c_wire_t *m);

int c_wire_read(c_wire_t *m, int ctx, uint8_t reg,uint8_t *data, uint16_t len);
int c_wire_write(c_wire_t *m, int ctx, uint8_t reg,uint8_t *data, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* __C_WIRE_H__ */





