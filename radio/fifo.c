#include "fifo.h"
#include <string.h>

void fifo_init(fifo_t *f) {
	f->h = 0;
	f->t = 0;
	f->stored = 0;
}

uint8_t fifo_isEmpty(fifo_t *f) {
	return (f->stored == 0);
}

uint8_t fifo_isFull(fifo_t *f) {
	return (f->stored > 0 && f->t == f->h);
}

void fifo_write(fifo_t *f, uint8_t *buf, uint8_t len) {
	memcpy(&f->data[f->h], buf, len);
	f->h += FIFO_UNIT_LEN;
	if (f->h == FIFO_SIZE) f->h = 0;
	f->stored++;
}

uint8_t * fifo_read(fifo_t *f) {
	uint8_t *ret = &f->data[f->t];
	fifo_skip(f);
	return ret;
}

uint8_t fifo_peek(fifo_t *f, uint16_t offset) {
	return f->data[(f->t)*FIFO_UNIT_LEN + offset];
}

void fifo_skip(fifo_t *f) {
	f->t += FIFO_UNIT_LEN;
	if (f->t == FIFO_SIZE) f->t = 0;
	f->stored--;
}
