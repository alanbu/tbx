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
 * resexcept.h
 *
 *  Created on: 5 Oct 2010
 *      Author: alanb
 */

#ifndef TBX_RES_RESEXCEPT_H_
#define TBX_RES_RESEXCEPT_H_

#include <stdexcept>
#include "../handles.h"
#include "../stringutils.h"

namespace tbx
{
namespace res
{

/**
 * Exception thrown when a ResObject is assigned to a subclass
 * with the wrong class id.
 */
class ResObjectClassMismatch : public std::invalid_argument
{
public:
	ResObjectClassMismatch(std::string object, int class_id, int check_id)
	  : std::invalid_argument("Object " + object
			  + ", class id " + tbx::to_string(class_id)
			  + " does not match assigned class " + tbx::to_string(check_id))
	{
	}
};

/**
 * Exception thrown when a ResObject is assigned to a subclass
 * with the wrong class id.
 */
class ResGadgetTypeMismatch : public std::invalid_argument
{
public:
	ResGadgetTypeMismatch(int type_id, int check_id)
	  : std::invalid_argument("Gadget with type id " + tbx::to_string(type_id)
			  + " does not match assigned type id " + tbx::to_string(check_id))
	{
	}
};

/**
 * Exception thrown when an object  can't be found in a ResEditor or ResFile
 */
class ResObjectNotFound : public std::invalid_argument
{
public:
	ResObjectNotFound(std::string name)
	  : std::invalid_argument("Resource '" + name
			  + "' not found")
	{
	}
};

/**
 * Exception thrown when the ResEditor already contains an object
 * with a given name
 */
class ResObjectExists : public std::invalid_argument
{
public:
	ResObjectExists(std::string name)
	  : std::invalid_argument("Resource '" + name
			  + "' already exists")
	{
	}
};


/**
 * Exception thrown when an item can't be found in a menu
 */
class ResMenuItemNotFound : public std::invalid_argument
{
public:
	ResMenuItemNotFound(std::string menu, tbx::ComponentId component_id)
	  : std::invalid_argument("Menu " + menu
			  + " does not contain menu item " + tbx::to_string(component_id))
	{
	}
};

/**
 * Exception thrown when a menu resource already contains an item
 * with a component id
 */
class ResMenuItemExists : public std::invalid_argument
{
public:
	ResMenuItemExists(std::string menu, tbx::ComponentId component_id)
	  : std::invalid_argument("Menu " + menu
			  + " already contains menu item " + tbx::to_string(component_id))
	{
	}
};

/**
 * Exception thrown when a gadget can't be found in a window
 */
class ResGadgetNotFound : public std::invalid_argument
{
public:
	ResGadgetNotFound(std::string window, tbx::ComponentId component_id)
	  : std::invalid_argument("Window " + window
			  + " does not contain gadget " + tbx::to_string(component_id))
	{
	}
};

/**
 * Exception thrown when a window resource already contains a gadget
 * with a component id
 */
class ResGadgetExists : public std::invalid_argument
{
public:
	ResGadgetExists(std::string window, tbx::ComponentId component_id)
	  : std::invalid_argument("Window " + window
			  + " already contains gadget " + tbx::to_string(component_id))
	{
	}
};

/**
 * Exception thrown when a shortcut key can't be found in a window
 */
class ResShortcutNotFound : public std::invalid_argument
{
public:
	ResShortcutNotFound(std::string window, tbx::ComponentId component_id)
	  : std::invalid_argument("Window " + window
			  + " does not contain gadget " + tbx::to_string(component_id))
	{
	}
};

/**
 * Exception thrown when a window resource already contains a shortcut
 * with a key code
 */
class ResShortcutExists : public std::invalid_argument
{
public:
	ResShortcutExists(std::string window, tbx::ComponentId component_id)
	  : std::invalid_argument("Window " + window
			  + " already contains gadget " + tbx::to_string(component_id))
	{
	}
};

}
}


#endif /* TBX_RES_RESEXCEPT_H_ */
