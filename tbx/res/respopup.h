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

#ifndef TBX_RES_RESPOPUP_H
#define TBX_RES_RESPOPUP_H

#include "resgadget.h"

namespace tbx {
namespace res {

/**
 * Class for PopUp gadget template
 */
class ResPopUp : public ResGadget
{

public:
	enum {TYPE_ID = 704 };

	ResPopUp(const ResGadget &other) : ResGadget(other)
	{
     check_type(TYPE_ID);
	}

	ResPopUp(const ResPopUp &other) : ResGadget(other)
	{
	}

	virtual ~ResPopUp() {}

	ResPopUp &operator=(const ResGadget &other)
	{
		other.check_type(TYPE_ID);
		ResBase::operator=(other);
		return *this;
	}

	ResPopUp &operator=(const ResPopUp &other)
	{
		ResBase::operator=(other);
		return *this;
	}

	ResPopUp()
	  : ResGadget(704,40)
	{
		init_string(36,0); // menu
	}

	bool generate_about_to_be_shown() const {return flag(0, 1<<0);}
	void generate_about_to_be_shown(bool value) {flag(0,1<<0,value);}

	const char *menu() const {return string(36);}
	void menu(const char *value) {string(36, value);}
	void menu(const std::string &value) {string(36, value);}

};

}
}

#endif // TBX_RES_RESPOPUP_H
