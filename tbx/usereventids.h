/*
 * tbx RISC OS toolbox library
 *
 * Copyright (C) 2008 Alan Buckley   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * usereventids.h
 *
 *  Created on: 10-Sep-2008
 *      Author: alanb
 */

#ifndef USEREVENTIDS_H_
#define USEREVENTIDS_H_

namespace tbx
{
	// IDs available for user events unique to the program
	const int FIRST_USER_EID = 1;
	const int LAST_USER_EID = 0x4FFF;

	// IDs available to share between multiple applications
	const int FIRST_SHARE_EID = 0x5000;
	const int LAST_SHARE_EID = 0x5FFF;

	// IDs range for dynamically allocated ids
	const int FIRST_DYNAMIC_EID = 0x6000;
	const int LAST_DYNAMIC_EID = 0x6FFF;

	// IDs for the ATB library
	const int FIRST_TBX_EID = 0x7000;
	const int LAST_TBX_EID = 0x7FFF;
}

#endif /* USEREVENTIDS_H_ */
