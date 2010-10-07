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

#ifndef TBX_RES_RESOPTIONBUTTON_H
#define TBX_RES_RESOPTIONBUTTON_H

#include "resgadget.h"

namespace tbx {
namespace res {

/**
 * Class for OptionButton gadget template
 */
class ResOptionButton : public ResGadget
{

public:
	enum {TYPE_ID = 192 };

	ResOptionButton(const ResGadget &other) : ResGadget(other)
	{
     check_type(TYPE_ID);
	}

	ResOptionButton(const ResOptionButton &other) : ResGadget(other)
	{
	}

	virtual ~ResOptionButton() {}

	ResOptionButton &operator=(const ResGadget &other)
	{
		other.check_type(TYPE_ID);
		ResBase::operator=(other);
		return *this;
	}

	ResOptionButton &operator=(const ResOptionButton &other)
	{
		ResBase::operator=(other);
		return *this;
	}

	ResOptionButton()
	  : ResGadget(192,48)
	{
		init_message(36,0); // label
	}

	bool generate_state_changed() const {return flag(0, 1<<0);}
	void generate_state_changed(bool value) {flag(0,1<<0,value);}
	bool on() const {return flag(0, 1<<2);}
	void on(bool value) {flag(0,1<<2,value);}

	const char *label() const {return message(36);}
	void label(const char *value, int max_length = -1) {message_with_length(36, value, max_length);}
	void label(const std::string &value, int max_length = -1) {message_with_length(36, value, max_length);}
	int max_label_len() const {return int_value(40);}
	int event_id() const {return int_value(44);}
	void event_id(int value) {int_value(44,value);}

};

}
}

#endif // TBX_RES_RESOPTIONBUTTON_H

