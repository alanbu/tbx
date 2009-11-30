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

/*
 * saveas.h
 *
 *  Created on: 08-Apr-2009
 *      Author: alanb
 */

#ifndef TBX_SAVEAS_H_
#define TBX_SAVEAS_H_

#include "showfullobject.h"
#include "listener.h"
#include "window.h"

namespace tbx {

// Listener classes used by SaveAs
class AboutToBeShownListener;
class SaveAsDialogueCompletedListener;
class SaveAsSaveToFileHandler;
class SaveAsFillBufferHandler;
class SaveAsSaveCompletedListener;

/**
 * Class to show the RISC OS SaveAs dialog box and generate
 * events to allow the application to do the Save.
 *
 * The SaveAs object has three types:
 *   Type 1: SaveAs does all the saving from a client provided buffer
 *   Type 2: SaveAs fires events so the client can save the data to a file
 *   Type 3: SaveAs supports type 2 but also fires events to allow a RAM Transfer
 */
class SaveAs: public tbx::ShowFullObject
{
public:
	enum { TOOLBOX_CLASS = 0x82bc0 };

	SaveAs() {};
	SaveAs(const SaveAs &other) : ShowFullObject(other._handle) {}
	SaveAs(const Object &other) : ShowFullObject(other)	{check_toolbox_class(SaveAs::TOOLBOX_CLASS);}
	SaveAs(const std::string &template_name) : ShowFullObject(template_name)	{check_toolbox_class(SaveAs::TOOLBOX_CLASS);}

	SaveAs &operator=(const SaveAs &other) {_handle = other.handle(); return *this;}
	SaveAs &operator=(const Object &other) {_handle = other.handle(); check_toolbox_class(TOOLBOX_CLASS); return *this;}
	bool operator==(const Object &other) const {return (_handle == other.handle());}
	bool operator!=(const Object &other) const {return (_handle != other.handle());}


	// Properties
	/**
	 * Return the underlying toolbox window used to implement the SaveAs object
	 */
	Window window() const {return Window((ObjectId)int_property(0));}

	/**
	 * Set the title for the SaveAs object
	 */
	void title(std::string value) {string_property(1, value);}

	/**
	 * Get the title for the SaveAs object
	 */
	std::string title() const {return string_property(2);}

	/**
	 * Set the file name for the save as object
	 */
	void filename(std::string value) {string_property(3, value);}

	/**
	 * Get the filename for the SaveAs object
	 */
	std::string filename() const {return string_property(4);}

	/**
	 * Set the file type for the SaveAs object
	 */
	void filetype(int value) {int_property(5, value);}

	/**
	 * Get the file type from the SaveAs object
	 */
	int filetype() const {return int_property(6);}

	/**
	 * Set the file size for the SaveAs object
	 */
	void filesize(int value) {int_property(7, value);}

	/**
	 * Get the file size from the SaveAs object
	 */
	int filesize() const {return int_property(8);}

	/**
	 * Set if a selection is available
	 */
	void selection_available(bool available) { bool_property(9, available);}

	void set_data_address(void *data, int size, void *selection  = 0, int selection_size = 0);
	void buffer_filled(void *buffer, int size);
	void file_save_completed(bool successful, std::string filename);

	void add_about_to_be_shown_listener(AboutToBeShownListener *listener);
	void remove_about_to_be_shown_listener(AboutToBeShownListener *listener);
	void add_dialogue_completed_listener(SaveAsDialogueCompletedListener *listener);
	void remove_dialogue_completed_listener(SaveAsDialogueCompletedListener *listener);
	void add_save_completed_listener(SaveAsSaveCompletedListener *listener);
	void remove_save_completed_listener(SaveAsSaveCompletedListener *listener);

	void set_save_to_file_handler(SaveAsSaveToFileHandler *handler);
	void set_fill_buffer_handler(SaveAsFillBufferHandler *handler);

};

/**
 * Listener for when the save as dialogue box has been closed
 */
class SaveAsDialogueCompletedListener : public Listener
{
public:
	/**
	 * Called when dialog has be closed
	 * @param save_done true if the dialogue box was closed after a successful save.
	 * This value will also be false if a successful save by click on OK with adjust
	 * and then cancelling the dialogue box.
	 */
	virtual void saveas_dialogue_completed(SaveAs save_as, bool save_done) = 0;
};

/**
 * Event for SaveAsSaveCompletedListener
 */

class SaveAsSaveCompletedEvent
{
private:
	SaveAs _saveas;
	int _flags;
	int _message_id;
	std::string _filename;

public:
	SaveAsSaveCompletedEvent(SaveAs saveas, int flags, int message, const char *filename) :
		_saveas(saveas), _flags(flags), _message_id(message), _filename(filename)
	{}

	/**
	 * Return save as dialogue this event was on
	 */
	SaveAs saveas() {return _saveas;}

	/**
	 * Check if save was of the selection
	 */
	bool selection_saved() const {return (_flags & 1) !=0;}

	/**
	 * Check if the save was to a safe location (e.g. a filing system)
	 */
	bool safe() const {return (_flags & 2) != 0;}

	/**
	 * Wimp message of original datasave message or 0 if
	 * save wasn't by a drag
	 */
	int message_id() const {return _message_id;}

	/**
	 * Full filename of file location if save was safe
	 */
	const std::string &filename() const {return _filename;}
};

/**
 * Listener for when a save has been completed.
 */
class SaveAsSaveCompletedListener : public Listener
{
public:
	/**
	 * Called when a save has been successful
	 */
	virtual void saveas_save_completed(SaveAsSaveCompletedEvent &event) = 0;
};

/**
 * Handler for SaveAsSaveToFile message.
 *
 * Called for Type 2 and 3 saves only
 */
class SaveAsSaveToFileHandler : public Listener
{
public:
	/**
	 * Request to save the file to the specified location.
	 *
	 * Once the file is saved or the attempt to save failed.
	 * saveas.file_save_completed should be called.
	 *
	 * @param saveas SaveAs object for save
	 * @param selection save the selection only
	 * @param filename filename to save to
	 */
	virtual void saveas_save_to_file(SaveAs saveas, bool selection, std::string filename) = 0;
};

/**
 * Handler for SaveAsFillBuffer message
 *
 * Called for Type 3 saves only to get the next set of data to transfer
 */
class SaveAsFillBufferHandler : public Listener
{
public:
	/**
	 * Called when the buffer need to be refilled for a RAM transfer (type 3) save.
	 *
	 * If buffer is 0 the buffer for the RAM transfer should be allocated or
	 * maintain the address in memory of the data to be transferred.
	 *
	 * Either way saveas.buffer_filled should be called once the buffer has
	 * the correct data.
	 *
	 * The transfer will stop when the size of data transferred is less than
	 * a complete buffer.
	 *
	 * @param saveas SaveAs the transfer is occurring on.
	 * @param selection saving selection only
	 * @param size of buffer for transfer in bytes
	 * @param buffer for transfer
	 * @param already_transmitted number of bytes already transmitted
	 */
	virtual void saveas_fill_buffer(SaveAs saveas, bool selection, int size, void *buffer, int already_transmitted) = 0;
};

}

#endif /* TBX_SAVEAS_H_ */
