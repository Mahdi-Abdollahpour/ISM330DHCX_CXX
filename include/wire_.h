#ifndef __wire_pp__
#define __wire_pp__

#include <stdint.h>
// #include <cstdint>


class wire_
{
	public:
		// wire_();
		void set_fd(int fd);
		int read(int ctx, uint8_t reg,uint8_t *data, uint16_t len);
		int write(int ctx, uint8_t reg,uint8_t *data, uint16_t len);
		private:
		int _fd{-1};
};







#endif // __wire_pp__