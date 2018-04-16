/****************************************************************************/
/*																			*
 *	circular_buffer.c - Circular buffer library for C. 						*
 *	Copyright (c) 2018 Akshay Arun Dandekar. 								*
 *																			*
 ****************************************************************************
 *																			*
 *	This program is free software: you can redistribute it and/or modify	*
 *	it under the terms of the GNU Lesser General Public License as 			*
 *	published by the Free Software Foundation, either version 3 of the 		*
 *	License, or (at your option) any later version. 						*
 *	This program is distributed in the hope that it will be useful, 		*
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of			*
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the			*
 *	GNU General Public License for more details. 							*
 *	You should have received a copy of the GNU General Public License		*
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>. 	*
 *																			*/
/****************************************************************************/

#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

#include <stdint.h>

struct circular_buffer {
	void **buffer;
	int head;
	int tail;
	int len;
	int maxlen;
};

int circular_buffer_init(struct circular_buffer *c_buf, int max_len);

int circular_buffer_deinit(struct circular_buffer *c_buf);

int circular_buffer_push(struct circular_buffer *c_buf, void *data);

int circular_buffer_pop(struct circular_buffer *c_buf, void **data);

int circular_buffer_empty(struct circular_buffer *c_buf);

int circular_buffer_is_empty(struct circular_buffer *c_buf);

int circular_buffer_is_full(struct circular_buffer *c_buf);

int circular_buffer_get_data(struct circular_buffer *c_buf, void **data_buf
		, int len, int offset);

int circular_buffer_set_data(struct circular_buffer *c_buf, void **data_buf
		, int len, int offset);

int circular_buffer_peek(struct circular_buffer *c_buf, void **data_buf
		, int len, int offset, int offset_cb);

#endif /* _CIRCULAR_BUFFER_H_ */
