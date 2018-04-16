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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "circular_buffer.h"

/*
 *	This function is used to initilaize a circular buffer. 'max_len' elements 
 *	can be stored in this buffer. Remeber to deinit this buffer as internally
 *	memory for the buffer is allocated dynamically which must be freed.
 *
 *	@param	IN	c_buf
 *	a pointer to a buffer which needs to be initialized
 *
 *	@param	IN	max_len
 *	This will decde how many elements can be stored in this buffer
 *
 *	@return
 *	Returns 0 on success else returns -1 with errno set to following value(s)
 *	EINVAL	In case c_buf is NULL
 *	ENOMEM	Not enough memory for buffer
 *
 */
int circular_buffer_init(struct circular_buffer *c_buf, int max_len) {
	/* Validate input */
	if(c_buf == NULL) {
#ifdef DEBUG
		fprintf(stderr, "[%s, ERROR] invlalid c_buf: c_buf cannot be NULL\r\n"
				, __FUNCTION__);
#endif
		errno = EINVAL;
		return -1;
	}
	
	c_buf->buffer = malloc(sizeof(void *) * max_len);
	if(c_buf->buffer == NULL) {
#ifdef DEBUG
		fprintf(stderr, "[%s, ERROR] Out of memory\r\n", __FUNCTION__);
#endif
		errno = ENOMEM;
		return -1;
	}

	c_buf->len = 0;
	c_buf->head = 0;
	c_buf->tail = 0;
	c_buf->maxlen = max_len;

	return 0;
}

/*
 *	This function is used to initilaize a circular buffer. 'max_len' elements 
 *	can be stored in this buffer. Remeber to deinit this buffer as internally
 *	memory for the buffer is allocated dynamically which must be freed.
 *
 *	@param	IN	c_buf
 *	a pointer to a buffer which needs to be initialized
 *
 *	@return
 *	Returns zero on success -1 on error
 *
 */
int circular_buffer_deinit(struct circular_buffer *c_buf) {
	/* Validate input */
	if(c_buf == NULL) {
#ifdef DEBUG
		fprintf(stderr, "[%s, ERROR] invlalid c_buf: c_buf cannot be NULL\r\n"
				, __FUNCTION__);
#endif
		errno = EINVAL;
		return -1;
	}

	/* Free memory allocated to buffer */
	if(c_buf->buffer != NULL) {
		free(c_buf->buffer);
		c_buf->buffer = NULL;
	}

	/* Reset all other data */
	c_buf->len = 0;
	c_buf->head = 0;
	c_buf->tail = 0;
	c_buf->maxlen = 0;

	return 0;
}

/*
 * 	This function will push single data element into the circular buffer
 * 	if buffer is full error will be retured.
 * 	This function will check for NULL pointer for buf, if NULL is passed
 * 	then it will return -1;
 *
 * 	@param	IN	c_buf
 * 	A pointer to a circular buffer to which we wish to add data
 *
 * 	@param	IN	data
 * 	Data we wish to push data
 *
 * 	@return
 * 	Returns 0 on success and -1 on failure
 */
int circular_buffer_push(struct circular_buffer *c_buf, void *data) {
	/* Validate input parameters */
	if(c_buf == NULL) {
#ifdef DEBUG
		fprintf(stderr, "[%s, ERORR] buf cannot be NULL\r\n", __FUNCTION__);
#endif
		errno = EINVAL;
		return -1;
	}

	/* Check if buffer is full */
	if(circular_buffer_is_full(c_buf) == 1) {
#ifdef DEBUG
		fprintf(stderr, "[%s, ERROR] Buffer is full\r\n", __FUNCTION__);
#endif
		return -1;
	}

	/* Increamnet head to the next place only if buffer is not empty*/
	if(c_buf->len > 0 ) {
		c_buf->head = ((c_buf->head + 1) % c_buf->maxlen);
	}

	/* Put data at head */
	c_buf->buffer[c_buf->head]= data;
	c_buf->len++;

	return 0;
}

/*
 * 	This function will pop single data element from the circular buffer
 * 	if buffer is empty error will be retured.
 * 	This function will check for NULL pointer for buf and data, if NULL
 * 	is passed then it will return -1;
 *
 * 	@param	IN	c_buf
 * 	A pointer to a circular buffer from which we wish to pop data
 *
 * 	@param	OUT	data
 * 	Popped data will be copied here.
 *
 * 	@return
 * 	Returns 0 on success and -1 on failure
 */
int circular_buffer_pop(struct circular_buffer *c_buf, void **data) {
	/* Validate input parameters */
	if(c_buf == NULL) {
#ifdef DEBUG
		fprintf(stderr, "[%s, ERORR] buf cannot be NULL\r\n", __FUNCTION__);
#endif
		errno = EINVAL;
		return -1;
	}

	if(data == NULL) {
#ifdef DEBUG
		fprintf(stderr, "[%s, ERORR] data cannot be NULL\r\n", __FUNCTION__);
#endif
		errno = EINVAL;
		return -1;
	}

	if(circular_buffer_is_empty(c_buf) == 1) {
#ifdef DEBUG
		fprintf(stderr, "[%s, ERROR] Buffer is empty\r\n", __FUNCTION__);
#endif
		errno = EINVAL;
		return -1;
	}

	/* Get data at tail and store into data */
	*data = c_buf->buffer[c_buf->tail];

	/* Increament tail to next position if len > 0 */
	if(c_buf->len > 1) {
		c_buf->tail = ((c_buf->tail + 1) % c_buf->maxlen);
	}

	/* Decreament buffer data count */
	c_buf->len--;

	return 0;
}

/*
 * 	This function will empty the given buffer
 * 	This function will check for NULL pointer for buf, if NULL
 * 	is passed then it will return -1;
 *
 * 	@param	IN	c_buf
 * 	A pointer to a circular buffer from which we wish to pop data
 *
 * 	@return
 * 	Returns 0 on success and -1 on failure
 */
int circular_buffer_empty(struct circular_buffer *c_buf) {
	/* Validate input parameters */
	if(c_buf == NULL) {
#ifdef DEBUG
		fprintf(stderr, "[%s, ERORR] buf cannot be NULL\r\n", __FUNCTION__);
#endif
		errno = EINVAL;
		return -1;
	}

	/* Reset position data */
	c_buf->head = c_buf->tail = 0;
	c_buf->len = 0;

	return 0;
}

/*
 * 	This function will check if the buffer is empty or not
 * 	This function will check for NULL pointer for buf, if NULL
 * 	is passed then it will return -1;
 *
 * 	@param	IN	buf
 * 	A pointer to a circular buffer from which we wish to pop data
 *
 * 	@return
 * 	Returns 1 when buffer is empty
 * 	Returns 0 when buffer is not empty
 * 	Returns -1 on failure
 */
int circular_buffer_is_empty(struct circular_buffer *c_buf) {
	/* Validate input parameters */
	if(c_buf == NULL) {
#ifdef DEBUG
		fprintf(stderr, "[%s, ERORR] buf cannot be NULL\r\n", __FUNCTION__);
#endif
		errno = EINVAL;
		return -1;
	}

	/* Check current len of buffer */
	if(c_buf->len == 0) {
		return 1;
	}

	return 0;
}

/*
 * 	This function will check if the buffer is full or not
 * 	This function will check for NULL pointer for buf, if NULL
 * 	is passed then it will return -1;
 *
 * 	@param	IN	c_buf
 * 	A pointer to a circular buffer from which we wish to pop data
 *
 * 	@return	
 * 	Returns 1 when buffer is full
 * 	Returns 0 when buffer is not full
 * 	Returns -1 on failure
 */
int circular_buffer_is_full(struct circular_buffer *c_buf) {
	/* Validte input parameters */
	if(c_buf == NULL) {
#ifdef DEBUG
		fprintf(stderr, "[%s, ERORR] buf cannot be NULL\r\n", __FUNCTION__);
#endif
		errno = EINVAL;
		return -1;
	}

	/* Check current len of buffer */
	if(c_buf->len == (c_buf->maxlen)) {
		return 1;
	}

	return 0;
}

/*
 *	This function is used to get data block from the circular buffer
 *	This function will pop "len" number of data elecments  or the total 
 *	number of elements (whichever is less) from circular buffer and paste 
 *	it to the buffer "data_buf". You can provide offset inside buffer
 *
 *	NOTE: This function will first set all members('len') of the input 
 *	buffer to 0.
 *
 *	@param	IN	c_buf
 *	Circular buffer to use
 *	
 *	@param	OUT	data_buf
 *	Data buffer to which data should be copied to
 *
 *	@param	IN	len
 *	Length of the data_buf
 *
 *	@param	IN	offset
 *	Offset inside data_buf
 *
 * 	@returns
 * 	Count of the data elements read from circular buffer. 
 * 	In case of any errors -1 is returned.
 */
int circular_buffer_get_data(struct circular_buffer *c_buf, void **data_buf
		, int len, int offset) {
	int count;

	/* Validate input parameters */
	if(c_buf == NULL) {
#ifdef DEBUG
		fprintf(stderr, "[%s, ERORR] buf cannot be NULL\r\n", __FUNCTION__);
#endif
		errno = EINVAL;
		return -1;
	}

	if(data_buf == NULL) {
#ifdef DEBUG
		fprintf(stderr, "[%s, ERORR] data_buf cannot be NULL\r\n"
				, __FUNCTION__);
#endif
		errno = EINVAL;
		return -1;
	}

	if(len <= 0) {
#ifdef DEBUG
		fprintf(stderr, "[%s, ERROR] Invalid length of data_buf specified\r\n"
				, __FUNCTION__);
#endif
		errno = EINVAL;
		return -1;
	}

	/* Clear output buffer */
	memset(data_buf, 0, len);

	/* Get data from circular buffer */
	count = 0;
	while(c_buf->len > 0 && (count + offset) < len) {
		/* Get data at tail and store into data */
		data_buf[offset + count++] = c_buf->buffer[c_buf->tail];

		/* Increament tail to next position if len > 0 */
		if(c_buf->len > 1) {
			c_buf->tail = ((c_buf->tail + 1) % c_buf->maxlen);
		}
		c_buf->len--;
	}

	/* Return total number of data bytes read */
	return count;
}

/*
 *	This function is used to set data block from the circular buffer
 *	This function will pop "len" number of data elecments  or the total 
 *	number of elements (whichever is less) from circular buffer and paste 
 *	it to the buffer "data_buf". You can provide offset inside buffer
 *
 *	@param	IN	c_buf
 *	Circular buffer to use
 *
 *	@param	IN	data_buf
 *	Data buffer to which data should be copied to
 *	
 *	@param	IN	len
 *	Length of the data_buf
 *
 *	@param	IN	offset
 *	Offset inside data_buf
 *
 * 	@returns
 * 	Count of the data elements read from circular buffer. 
 * 	In case of any errors -1 is returned.
 *
 */
int circular_buffer_set_data(struct circular_buffer *c_buf, void **data_buf
		, int len, int offset) {
	int count;

	/* Validate input parameters */
	if(c_buf == NULL) {
#ifdef DEBUG
		fprintf(stderr, "[%s, ERORR] buf cannot be NULL\r\n", __FUNCTION__);
#endif
		errno = EINVAL;
		return -1;
	}

	if(data_buf == NULL) {
#ifdef DEBUG
		fprintf(stderr, "[%s, ERORR] data_buf cannot be NULL\r\n"
				, __FUNCTION__);
#endif
		errno = EINVAL;
		return -1;
	}

	if(len < 0) {
#ifdef DEBUG
		fprintf(stderr, "[%s, ERROR] Invalid length of data_buf specified\r\n"
				, __FUNCTION__);
#endif
		errno = EINVAL;
		return -1;
	}

	/* Add data into circular buffer */
	count = 0;
	while((c_buf->len < c_buf->maxlen) && ((count + offset) < len)) {
		/* Increamnet head to the next place only if buffer is not empty*/
		if(c_buf->len > 0 ) {
			c_buf->head = ((c_buf->head + 1) % c_buf->maxlen);
		}
	
		/* Put data at head */
		c_buf->buffer[c_buf->head]= data_buf[offset + count++];
		c_buf->len++;
	}

	/* Return total number of bytes copied to the circular buffer */
	return count;
}

/*
 *	This function is used to peek into data block from the circular buffer
 *	This function will peek "len" number of data elecments  or the total 
 *	number of elements (whichever is less) from circular buffer and paste 
 *	it to the buffer "data_buf". You can provide offset inside buffer
 *	This will not pop elements from the circular bffer.
 *
 *	@param	IN	c_buf
 *	Circular buffer to use
 *
 *	@param	OUT	data_buf
 *	Data buffer to which data should be copied to
 *
 *	@param	IN	len
 *	Length of the data_buf
 *
 *	@param	IN	offset
 *	Offset inside data_buf
 *
 *	@param	IN	offset_cb
 *	Offset inside the circular buffer
 *
 * 	@returns
 * 	Count of the data elements peeked from circular buffer. 
 * 	In case of any errors -1 is returned.
 *
 */
int circular_buffer_peek(struct circular_buffer *c_buf, void **data_buf
		, int len, int offset, int offset_cb) {
	int count;
	int i;

	/* Validate input parameters */
	if(c_buf == NULL) {
#ifdef DEBUG
		fprintf(stderr, "[%s, ERORR] buf cannot be NULL\r\n", __FUNCTION__);
#endif
		errno = EINVAL;
		return -1;
	}

	if(data_buf == NULL) {
#ifdef DEBUG
		fprintf(stderr, "[%s, ERORR] data_buf cannot be NULL\r\n"
				, __FUNCTION__);
#endif
		errno = EINVAL;
		return -1;
	}

	if(len <= 0) {
#ifdef DEBUG
		fprintf(stderr, "[%s, ERROR] Invalid length of data_buf specified\r\n"
				, __FUNCTION__);
#endif
		errno = EINVAL;
		return -1;
	}

	/* Clear output buffer */
	memset(data_buf, 0, len);

	/* Get data from circular buffer */
	count = offset;
	i = ((c_buf->tail + offset) % c_buf->maxlen);
	while((count < c_buf->len) && ((count + offset) < len)) {
		/* Get data at tail and store into data */
		data_buf[offset + count++] = c_buf->buffer[i];

		/* Increament tail to next position if len > 0 */
		if(c_buf->len > 1) {
			i = ((i + 1) % c_buf->maxlen);
		}
	}

	/* Return total number of data bytes read */
	return count;
}

