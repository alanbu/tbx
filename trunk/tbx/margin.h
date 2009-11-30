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


#ifndef TBX_MARGIN_H_
#define TBX_MARGIN_H_

namespace tbx
{

class Margin
{
public:
	Margin() : left(0), top(0), right(0), bottom(0) {}
	Margin(int all) : left(all), top(all), right(all), bottom(all) {}
	Margin(int x, int y)  : left(x), top(y), right(x), bottom(y) {}
	Margin(int l, int t, int r, int b) : left(l), top(t), right(r), bottom(b) {}

	Margin(const Margin &other) : left(other.left), top(other.top),
								  right(other.right), bottom(other.bottom) {}

	int left;
	int top;
	int right;
	int bottom;

	Margin &operator=(const Margin &other) {left=other.left; top=other.top; right=other.right; bottom=other.bottom; return *this;}
	bool operator==(const Margin &other) const {return (left==other.left) && (top==other.top) && (right==other.right) && (bottom==other.bottom);}
	bool operator!=(const Margin &other) const {return (left!=other.left) || (top!=other.top) || (right!=other.right) || (bottom!=other.bottom);}
};

}

#endif /* MARGIN_H_ */
