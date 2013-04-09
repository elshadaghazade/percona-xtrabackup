/******************************************************
XtraBackup: hot backup tool for InnoDB
(c) 2009-2012 Percona Ireland Ltd.
Originally Created 3/3/2009 Yasufumi Kinoshita
Written by Alexey Kopytov, Aleksandr Kuzminsky, Stewart Smith, Vadim Tkachenko,
Yasufumi Kinoshita, Ignacio Nin and Baron Schwartz.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

*******************************************************/

/* Source file cursor interface */

#ifndef FIL_CUR_H
#define FIL_CUR_H

#include <my_dir.h>
#include "innodb_int.h"

typedef struct {
	os_file_t	file;		/*!< source file handle */
	char		path[FN_REFLEN];/*!< normalized file path */
	MY_STAT		statinfo;	/*!< information about the file */
	ulint		zip_size;	/*!< compressed page size in bytes or 0
					for uncompressed pages */
	ulint		page_size;	/*!< = zip_size for compressed pages or
					UNIV_PAGE_SIZE for uncompressed ones */
	ulint		page_size_shift;/*!< bit shift corresponding to
					page_size */
	ib_int64_t	offset;		/*!< current file offset in bytes */
	my_bool		is_system;	/*!< TRUE for system tablespace, FALSE
					otherwise */
	byte*		orig_buf;	/*!< read buffer */
	byte*		buf;		/*!< aligned pointer for orig_buf */
	ulint		buf_size;	/*!< buffer size in bytes */
	ulint		buf_read;	/*!< number of read bytes in buffer
					after the last cursor read */
	ulint		buf_npages;	/*!< number of pages in buffer after the
					last cursor read */
	ib_int64_t	buf_offset;	/*!< file offset of the first page in
					buffer */
	ulint		buf_page_no;	/*!< number of the first page in
					buffer */
	uint		thread_n;	/*!< thread number for diagnostics */
	ulint		space_id;	/*!< ID of tablespace */
	ulint		space_size;	/*!< space size in pages */
} xb_fil_cur_t;

typedef enum {
	XB_FIL_CUR_SUCCESS,
	XB_FIL_CUR_SKIP,
	XB_FIL_CUR_ERROR,
	XB_FIL_CUR_EOF
} xb_fil_cur_result_t;

/************************************************************************
Open a source file cursor.

@return XB_FIL_CUR_SUCCESS on success, XB_FIL_CUR_SKIP if the source file must
be skipped and XB_FIL_CUR_ERROR on error. */
xb_fil_cur_result_t
xb_fil_cur_open(
/*============*/
	xb_fil_cur_t*	cursor,		/*!< out: source file cursor */
	fil_node_t*	node,		/*!< in: source tablespace node */
	uint		thread_n);	/*!< thread number for diagnostics */

/************************************************************************
Reads and verifies the next block of pages from the source
file. Positions the cursor after the last read non-corrupted page.

@return XB_FIL_CUR_SUCCESS if some have been read successfully, XB_FIL_CUR_EOF
if there are no more pages to read and XB_FIL_CUR_ERROR on error. */
xb_fil_cur_result_t
xb_fil_cur_read(
/*============*/
	xb_fil_cur_t*	cursor);	/*!< in/out: source file cursor */

/************************************************************************
Close the source file cursor opened with xb_fil_cur_open(). */
void
xb_fil_cur_close(
/*=============*/
	xb_fil_cur_t *cursor);	/*!< in/out: source file cursor */

#endif