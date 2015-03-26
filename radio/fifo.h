#ifndef FIFO_H_INCLUDED
#define FIFO_H_INCLUDED

#include "asf.h"

#define FIFO_UNIT_LEN 	256
#define FIFO_SLOTS 		16
#define FIFO_SIZE		(FIFO_SLOTS*FIFO_UNIT_LEN)

typedef struct {
	uint8_t data[FIFO_SIZE];
	uint32_t h;
	uint32_t t;
} fifo_t;

//! Initializes a fifo_t structure
/*!
  \param f 		The fifo structure
*/
void fifo_init(fifo_t *f);

//! Checks the contents of the fifo. Returns 1 if empty, 0 if not empty
/*!
  \param f 		The fifo structure
*/
uint8_t fifo_isEmpty(fifo_t *f);

//! Writes one FIFO_UNIT_LEN slot of the fifo
/*!
  \param f 		The fifo structure
  \param buf	The source buffer
  \param len 	The amount of data from buf to write. Note that the write action still uses up a whole FIFO_UNIT_LEN slot of the fifo
*/
void fifo_write(fifo_t *f, uint8_t *buf, uint8_t len);

//! Reads one FIFO_UNIT_LEN slot of the fifo
/*!
  \param f 		The fifo structure
  \param buf 	The destination to write to. It should have size at least FIFO_UNIT_LEN.
*/
void fifo_read(fifo_t *f, uint8_t *buf);


#endif /* FIFO_H_INCLUDED */