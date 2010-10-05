/*
 * tbx RISC OS toolbox library
 *
 * Copyright (C) 2010 Alan Buckley   All Rights Reserved.
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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
 * adjuster.cc
 *
 *  Created on: 6-Jul-2010
 *      Author: alanb
 */

#include "adjuster.h"

namespace tbx
{

static void adjuster_clicked_router(IdBlock &id_block, PollBlock &data, Listener *listener)
{
	AdjusterClickedEvent ev(id_block, data);
	static_cast<AdjusterClickedListener *>(listener)->adjuster_clicked(ev);
}

/**
 * Add listener for
 */
void Adjuster::add_clicked_listener(AdjusterClickedListener *listener)
{
	add_listener(0x8288c , listener, adjuster_clicked_router);
}

/**
 * Remove listener for
 */
void Adjuster::remove_clicked_listener(AdjusterClickedListener *listener)
{
	remove_listener(0x8288c, listener);
}

}
