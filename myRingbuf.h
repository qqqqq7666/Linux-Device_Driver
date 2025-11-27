#ifndef MYRINGBUF_LIB_H
#define MYRINGBUF_LIB_H

int open_ringbuf(void);
int close_ringbuf(void);
int enqueue(const char* buf, int len);
int dequeue(char* buf, int len);

#endif
