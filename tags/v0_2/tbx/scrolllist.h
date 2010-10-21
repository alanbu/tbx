/*
 * tbx RISC OS toolbox library
 *
 * Copyright (C) 2009 Alan Buckley   All Rights Reserved.
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

#ifndef TBX_SCROLLLIST_H_
#define TBX_SCROLLLIST_H_

#include "gadget.h"
#include "sprite.h"
#include "listener.h"
#include "eventinfo.h"

namespace tbx
{

class ScrollListSelectionListener;

/**
 * Scrolling list of items with an optional sprite
 */
class ScrollList: public tbx::Gadget
{
public:
	enum {TOOLBOX_CLASS = 0x401a}; //!< Toolbox class for this gadget.

	ScrollList() {} //!< Construct an uninitialised ScrollList range.
	/**
	 * Destroy a scroll list gadget.
	 *
	 * This does not delete the underlying toolbox gadget.
	 */
	~ScrollList() {}

	/**
	 * Construct a scroll list from another scroll list.
	 *
	 * Both ScrollList will refer to the same underlying toolbox gadget.
	 */
	ScrollList(const ScrollList &other) : Gadget(other) {}

	/**
	 * Construct a scroll list from another gadget.
	 *
	 * The ScrollList and the Gadget will refer to the same
	 * underlying toolbox gadget.
	 *
	 * @throws GadgetClassError if the underlying toolbox gadget
	 *         isn't a scroll list
	 */
	ScrollList(const Gadget &other) : Gadget(other)	{check_toolbox_class(ScrollList::TOOLBOX_CLASS);}

	/**
	 * Construct a scroll list from a gadget.
	 *
	 * The ScrollList and the Component will refer to the same
	 * underlying toolbox component.
	 */
	ScrollList(const Component &other) : Gadget(other) {Window check(other.handle()); check_toolbox_class(ScrollList::TOOLBOX_CLASS);}

	/**
	 * Assign a scroll list to refer to the same underlying toolbox gadget
	 * as another.
	 */
	ScrollList &operator=(const ScrollList &other) {_handle = other._handle; _id = other._id; return *this;}

	/**
	 * Assign a scroll list to refer to the same underlying toolbox gadget
	 * as an existing Gadget.
	 * @throws GadgetClassError if the underlying toolbox gadget
	 *         isn't a scroll list
	 */
	ScrollList &operator=(const Gadget &other) {_handle = other.handle(); _id = other.id(); check_toolbox_class(TOOLBOX_CLASS); return *this;}

	/**
	 * Assign a scroll list to refer to the same underlying toolbox component
	 * as an existing Gadget.
	 * @throws ObjectClassError if the component is not in a window.
	 * @throws GadgetClassError if the underlying toolbox gadget
	 *         isn't a scroll list
	 */
	ScrollList &operator=(const Component &other) {Window check(other.handle()); _handle = other.handle(); _id = other.id(); check_toolbox_class(TOOLBOX_CLASS); return *this;}

	/**
	 * Check if this scroll list refers to the same underlying toolbox gadget
	 * as another gadget.
	 * @returns true if they refer to the same underlying toolbox gadget.
	 */
	bool operator==(const Gadget &other) const {return (_handle == other.handle() && _id == other.id());}

	/**
	 * Check if this scroll list refers to the same underlying toolbox gadget
	 * as another gadget.
	 * @returns true if they do not refer to the same underlying toolbox gadget.
	 */
	bool operator!=(const Gadget &other) const {return (_handle != other.handle() || _id != other.id());}

	/**
	 * Get the state
	 */
	int state() const {return int_property(0x401A);}

	/**
	 * Set the state
	 */
	void state(int value) {int_property(0x401B, value);}

	/**
	 * Get Allow multiple selections
	 */
	bool allow_multiple_selections() const {return flag_property(0x401A, 1);}

	/**
	 * Set allow multiple selections
	 */
	void allow_multiple_selections(bool value) {flag_property(0x401A, 1, value);}

	/**
	 * Add a text item to the scroll list
	 */
	void add_item(const std::string &text, int index = -1);

	/**
	 * Add an item with a sprite from the wimp sprite pool
	 */
	void add_item(const std::string &text, const WimpSprite &sprite, int index = -1);

	/**
	 * Add an item with a use sprite
	 */
	void add_item(const std::string &text, const UserSprite &sprite, int index = -1);

	/**
	 * Delete an item
	 */
	void delete_item(int index);

	/**
	 * Delete a range of items
	 */
	void delete_items(int start, int end);

	/**
	 * Delete all the items
	 */
	void clear();

	void select_unique_item(int index);
	void select_item(int index);
	void deselect_item(int index);
	int first_selected() const;
	int next_selected(int from) const;

	void make_visible(int index);
	void item_text(int index, const std::string &text);
	std::string item_text(int index) const;

	int count_items() const;

	void font(const std::string &name, int width, int height);
	void system_font(int width, int height);

	void set_colour(WimpColour foreground, WimpColour background);
	void set_colour(Colour foreground, Colour background);
	void get_colour(Colour &foreground, Colour &background) const;

	void add_selection_listener(ScrollListSelectionListener *listener);
	void remove_selection_listener(ScrollListSelectionListener *listener);
};

/**
 * Item has been selected event
 */
class ScrollListSelectionEvent : public EventInfo
{
public:
	ScrollListSelectionEvent(IdBlock &id_block, PollBlock &data) :
		 EventInfo(id_block, data) {}

    virtual ~ScrollListSelectionEvent() {};

    /**
     * Index of item event is for
     */
    int index() const {return _data.word[5];}

    /**
     * true if index was selected
     */
    bool selected() const {return (_data.word[4] & 1) !=0;}

    /**
     * true if index was double-clicked
     */
    bool double_click() const {return (_data.word[4] & 2) != 0;}
};

/**
 * Listener for selections on a scroll list
 */
class ScrollListSelectionListener : public Listener
{
public:
	virtual ~ScrollListSelectionListener() {}

	virtual void scrolllist_selection(const ScrollListSelectionEvent &event) = 0;
};


}

#endif /* SCROLLLIST_H_ */