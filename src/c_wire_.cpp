


#include <stdlib.h>
#include "c_wire_.h"
#include "wire_.h"

struct c_wire {
	void *obj;
};

c_wire_t *c_wire_create()
{
	c_wire_t *m;
	wire_ *obj;

	m      = (typeof(m))malloc(sizeof(*m));
	obj    = new wire_();
	m->obj = obj;

	return m;
}

void c_wire_destroy(c_wire_t *m)
{
	if (m == NULL)
		return;
	delete static_cast<wire_ *>(m->obj);
	free(m);
}

int c_wire_read(c_wire_t *m, int ctx, uint8_t reg,uint8_t *data, uint16_t len)
{
	wire_ *obj;

	if (m == NULL)
		return -1;

	obj = static_cast<wire_ *>(m->obj);
	obj->read( ctx, reg, data, len);
	return 0;
}
int c_wire_write(c_wire_t *m, int ctx, uint8_t reg,uint8_t *data, uint16_t len)
{
	wire_ *obj;

	if (m == NULL)
		return -1;

	obj = static_cast<wire_ *>(m->obj);
	obj->write( ctx, reg, data, len);
	return 0;
}
// int mather_val(mather_t *m)
// {
// 	CPPMather *obj;

// 	if (m == NULL)
// 		return 0;

// 	obj = static_cast<CPPMather *>(m->obj);
// 	return obj->val();
// }

