
/*
 * filter.h
 * Kit Shaw
 * 2019.10.5
 */

#ifndef __FILTER_H_
#define __FILTER_H_

#define FILTER_TOTAL_TIME		14400000             //先定为4000小时

void filter_init(void);
unsigned long read_filter_time(void);
void filter_task(void);
void reset_filter_time(void);
void write_filter_time(void);

#endif           //__FILTER_H_

