/*
 * tbx RISC OS toolbox library
 *
 * Copyright (C) 2021 Alan Buckley   All Rights Reserved.
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
 
#ifndef TBX_CLIPBOARD_H_ 
#define TBX_CLIPBOARD_H_ 

#include "saver.h"
#include <vector>
#include <map>

#include "gadget.h"

namespace tbx
{

/**
 * Listener for clipboard related events
 */
class ClipboardClaimedListener
{
	public:
	    virtual ~ClipboardClaimedListener() {}
		/**
		 * Called when another application has claimed the caret/selection
		 */
		virtual void clipboard_claimed_caret() {}
		/**
		 * Called when another application has claimed the global clipboard
		 */
		virtual void clipboard_claimed() {}
};

class ClipboardData;

/**
 * Class to handler the global RISC OS clipboard
 */
class Clipboard 
{
   public:
      static Clipboard *instance();
	  ~Clipboard();
	  void clear();
	  
      void copy(int file_type, ClipboardData *data, bool add = false);
	  void copy(const std::string &text, int file_type = 0xFFF);
	  void copy(int file_type, char *bytes, int size, bool copy_bytes = true);
	  void paste(int file_type, Gadget &gadget, int x = 0, int y = 0);
	  void paste(int *file_types, int num_types, Gadget &gadget, int x = 0, int y = 0);
	  void paste(int file_type, Window &window, int x = 0, int y = 0);
	  void paste(int *file_types, int num_types, Window &window, int x = 0, int y = 0);
	  
	  void claim_caret();
	  
	  void add_claimed_listener(ClipboardClaimedListener *listener);
	  void remove_claimed_listener(ClipboardClaimedListener *listener);
	  
      /**
       * Check if global clipboard is currently owned by this application.
       * @returns true if global clipboard is owned.
       */	   
      bool owns_clipboard() const {return _owns_clipboard;}		  
	  ClipboardData *data(int file_type) const;
	  ClipboardData *native_data() const;
	  int native_file_type() const;
	  
	private:
	  Clipboard();
      void claim_clipboard();
	  void fire_claimed(bool clipboard_claimed, bool caret_claimed);
	  void paste(int *file_types, int num_types, Window window, Gadget gadget, int x, int y);
	  
	  // Private class to do all the message handling
	  class MessageHandler
		: WimpUserMessageListener,
		  WimpRecordedMessageListener,		 
		  tbx::SaverFinishedHandler,
		  tbx::SaverSaveCompletedHandler,
		  tbx::SaverSaveToFileHandler,
		  tbx::SaverFillBufferHandler
	  {
		public:
		   MessageHandler();
		   virtual ~MessageHandler();
		   void clear();
		   void paste_ref(int ref) {_paste_ref = ref;}

		   virtual void user_message(WimpMessageEvent &event);
		   virtual void recorded_message(WimpMessageEvent &event, int reply_to);

		   virtual void saver_finished(const tbx::SaverFinishedEvent &finished);
		   virtual void saver_save_completed(tbx::SaverSaveCompletedEvent &event);
		   virtual void saver_save_to_file(tbx::Saver saver, std::string file_name);
		   virtual void saver_fill_buffer(tbx::Saver saver, int size, void *buffer, int already_transmitted);
		private:
		   tbx::Saver *_saver;
	       ClipboardData *_save_data;
	       char *_save_buffer;		  
		   int _paste_ref;
	  } _message_handler;
	  friend class MessageHandler;
	  	  
	private:
	   bool _owns_clipboard;
	   bool _owns_caret;
	   int _native_file_type;
	   std::map<int, ClipboardData *> _data;	   
	   std::vector<ClipboardClaimedListener *> _claimed_listeners;
	   static Clipboard *s_instance;
};

/**
 * Return pointer to the global clipboard
 *
 * Data can be added to the clipboard by using any of the copy() methods
 * To paste data you should have claimed the caret/selection with claim_caret()
 * and call the paste() method.
 */
inline Clipboard *clipboard() {return Clipboard::instance();}

/**
 * Base class for data used on the clipboard.
 *
 * This class is used to provide the data to the clipboard at
 * the time it is actually copied.
 * The data can either be pre-populated or created when a copy
 * of it is requested by calling size or data.
 */
class ClipboardData
{
public:
  virtual ~ClipboardData() {}
  /**
   * The size of the data in bytes.
   * If the data is created on demand in a subclass the size
   * at least must be calculated when this is called.
   */
  virtual int size() = 0;
  /**
   * The data to copy.
   * If the data is created on demand in a subclass, it must
   * be created when this is called.
   */  
  virtual const char *data() = 0;
  
  /**
   * Called when data is about to be copied to another
   * application from via clipboard.
   */
  virtual void copy_started() {}
  /**
   * Called when a copy of the data has finished
   */
  virtual void copy_finished() {}
};

/**
 * Convenience class to create clipboard data containing a string
 */
class ClipboardText : public ClipboardData
{
public:
    ClipboardText(const std::string &text) : _text(text) {}
	virtual ~ClipboardText() {}
	
	int size() {return static_cast<int>(_text.size());}
	const char *data() {return _text.data();}
	  
private:
    std::string _text;
};

/**
 * Convenience class to create clipboard data for an array of bytes
 */
class ClipboardBytes : public ClipboardData
{
   public:      
      ClipboardBytes(char *bytes, int size, bool copy = true);   
	  virtual ~ClipboardBytes() { delete _data;}
	  
	  int size() {return _size;}
	  const char *data() {return _data;}
	private:
	  char *_data;
	  int _size;
};

}

#endif /* TBX_CLIPBOARD_H_ */
