
#ifndef _KTestWND_H_
#define _KTestWND_H_

#include "tbx/autocreatelistener.h"
#include "tbx/writablefield.h"
#include "tbx/displayfield.h"
#include "tbx/numberrange.h"
#include "tbx/radiobutton.h"
#include "tbx/stringset.h"
#include "tbx/textarea.h"
#include "tbx/button.h"
#include "tbx/KeyListener.h"
#include "tbx/MouseClickListener.h"

#include <map>

/**
 * Class to set up listeners an variables when the single
 * window is shown.
 */
class KTestWnd
: public tbx::AutoCreateListener,
public tbx::KeyListener,
public tbx::MouseClickListener,
public tbx::RadioButtonStateChangedListener
{
public:
virtual void auto_created(std::string template_name, tbx::Object object);
virtual void key(tbx::KeyEvent &key);
virtual void mouse_click(tbx::MouseClickEvent &mouse);
virtual void radiobutton_state_changed(const tbx::RadioButtonStateChangedEvent &state_changed);

private:
void add_gadget_capture();
void remove_gadget_capture();
void add_window_capture();
void remove_window_capture();

void add_gadget_mouse();
void remove_gadget_mouse();
void add_window_mouse();
void remove_window_mouse();
void add_all_mouse();
void remove_all_mouse();

void add_gadget_names(tbx::ComponentId id, const std::string &name);


private:
tbx::Window _window;
tbx::WritableField _writable;
tbx::WritableField _writable2;
tbx::NumberRange _number;
tbx::StringSet _string_set;
tbx::TextArea _text_area;
tbx::DisplayField _gadget_name;
tbx::DisplayField _key_pressed;
tbx::DisplayField _mouse_name;
tbx::DisplayField _mouse_x;
tbx::DisplayField _mouse_y;
tbx::DisplayField _mouse_button;
tbx::Button _button;

std::map<tbx::ComponentId, std::string> _gadget_names;
};

#endif
