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
 * showfullobject.cc
 *
 *  Created on: 28-Nov-2008
 *      Author: alanb
 */

#include "showfullobject.h"
#include "swixcheck.h"
#include <swis.h>

namespace tbx {

/**
 * Show object with full position specified
 *  */
void ShowFullObject::show(const ShowFullSpec &full_spec)
{
	swix_check(_swix(0x44EC3, _INR(0,5),
			0,
			_handle,
			1,
			&(full_spec.visible_area),
			0,
			-1));
}

/**
 * Show object with full position specified
 */
void ShowFullObject::show(const ShowFullSpec &full_spec, const Object &parent)
{
	swix_check(_swix(0x44EC3, _INR(0,5),
			0,
			_handle,
			1,
			&(full_spec.visible_area),
			parent.handle(),
			-1));
}
/**
 * Show object  with full position specified
 */
void ShowFullObject::show(const ShowFullSpec &full_spec, const Component &parent)
{
	swix_check(_swix(0x44EC3, _INR(0,5),
			0,
			_handle,
			1,
			&(full_spec.visible_area),
			parent.handle(),
			parent.id()));
}



/**
 * Show object as a menu with full position specified and with the given parent.
 */
void ShowFullObject::show_as_menu(const ShowFullSpec &full_spec)
{
	swix_check(_swix(0x44EC3, _INR(0,5),
		1,
		_handle,
		1,
		&(full_spec.visible_area),
		0,
		-1));
}


/**
 * Show object as a menu with full position specified and with the given parent.
 */
void ShowFullObject::show_as_menu(const ShowFullSpec &full_spec, const Object &parent)
{
	swix_check(_swix(0x44EC3, _INR(0,5),
		1,
		_handle,
		1,
		&(full_spec.visible_area),
		parent.handle(),
		-1));
}

/**
 * Show object as a menu with full position specified
 * and with the given parent.
 */
void ShowFullObject::show_as_menu(const ShowFullSpec &full_spec, const Component &parent)
{
	swix_check(_swix(0x44EC3, _INR(0,5),
		1,
		_handle,
		1,
		&(full_spec.visible_area),
		parent.handle(),
		parent.id()));
}

/**
 * Show object as a sub menu  with full position specified from
 * the given menu item
 */
void ShowFullObject::show_as_submenu(const ShowFullSpec &full_spec, const MenuItem &parent)
{
	swix_check(_swix(0x44EC3, _INR(0,5),
		2,
		_handle,
		1,
		&(full_spec.visible_area),
		parent.handle(),
		parent.id()));
}

}