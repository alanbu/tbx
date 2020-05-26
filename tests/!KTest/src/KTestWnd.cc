
#include "KTestWnd.h"
#include "tbx/window.h"
#include "tbx/stringutils.h"
#include <sstream>
#include <iostream>

using namespace tbx;


/**
 * Set up listeners and commands for the window.
 */
void KTestWnd::auto_created(std::string template_name, Object object)
{
  _window = Window(object);
  _gadget_names[tbx::NULL_ComponentId] = "(none)";
  _writable = _window.gadget(1);    add_gadget_names(1, "Writable");
  _writable2 = _window.gadget(8);   add_gadget_names(8, "Writable2");
  _number = _window.gadget(9);
  add_gadget_names(9, "Number");
  add_gadget_names(_number.numeric_gadget().id(), "Number (number)");
  _string_set = _window.gadget(12); add_gadget_names(12, "StringSet");
  add_gadget_names(_string_set.alphanumeric_gadget().id(), "StringSet (alpha)");
  _text_area = _window.gadget(13);  add_gadget_names(13, "TextArea");
  _button = _window.gadget(31); add_gadget_names(31, "Button");

  tbx::RadioButton gadget_capture = _window.gadget(16);
  tbx::RadioButton window_capture = _window.gadget(17);
  gadget_capture.add_state_changed_listener(this);
  window_capture.add_state_changed_listener(this);

  tbx::RadioButton gadget_mouse= _window.gadget(19);
  tbx::RadioButton window_mouse= _window.gadget(20);
  tbx::RadioButton all_mouse = _window.gadget(21);
  gadget_mouse.add_state_changed_listener(this);
  window_mouse.add_state_changed_listener(this);
  all_mouse.add_state_changed_listener(this);

  _gadget_name = _window.gadget(4);
  _key_pressed = _window.gadget(6);

  _mouse_name = _window.gadget(24);
  _mouse_x = _window.gadget(26);
  _mouse_y = _window.gadget(28);
  _mouse_button = _window.gadget(30);

  add_gadget_capture();
  add_gadget_mouse();
}

void KTestWnd::add_gadget_names(tbx::ComponentId id, const std::string &name)
{
  _gadget_names[id] = name;
}


void KTestWnd::key(tbx::KeyEvent &key)
{
  _gadget_name.text(_gadget_names[key.gadget().id()]);

  std::ostringstream ss;
  ss << key.key();
  if (key.key() >=32 && key.key() <= 256)
  {
    ss << "(" << char(key.key()) << ")";
  }
  _key_pressed.text(ss.str());
}

void KTestWnd::mouse_click(tbx::MouseClickEvent &mouse)
{
  tbx::ComponentId id =mouse.id_block().self_component().id();
  if (_gadget_names.count(id) == 0)
     _gadget_names[id] = "Component " + tbx::to_string(id);
  _mouse_name.text(_gadget_names[id]);
  _mouse_x.text(tbx::to_string(mouse.x()));
  _mouse_y.text(tbx::to_string(mouse.y()));
  _mouse_button.text(tbx::to_string(mouse.button()));

}

void KTestWnd::radiobutton_state_changed(const tbx::RadioButtonStateChangedEvent &state_changed)
{
  if (state_changed.on())
  {
    switch(state_changed.id_block().self_component().id())
    {
    case 16:
      remove_window_capture();
      add_gadget_capture();
      break;
    case 17:
      remove_gadget_capture();
      add_window_capture();
      break;
    case 19:
      add_gadget_mouse();
      remove_window_mouse();
      remove_all_mouse();
      break;
    case 20:
      add_window_mouse();
      remove_gadget_mouse();
      remove_all_mouse();
      break;
    case 21:
      add_all_mouse();
      remove_window_mouse();
      remove_gadget_mouse();
      break;
    }
  }
}

void KTestWnd::add_gadget_capture()
{
  _writable.add_key_listener(this);
  _writable2.add_key_listener(this);
}

void KTestWnd::remove_gadget_capture()
{
  _writable.remove_key_listener(this);
  _writable2.remove_key_listener(this);
}

void KTestWnd::add_window_capture()
{
  _window.add_all_key_listener(this);
}
void KTestWnd::remove_window_capture()
{
  _window.remove_all_key_listener(this);
}

void KTestWnd::add_gadget_mouse()
{
   _button.add_mouse_click_listener(this);
}
void KTestWnd::remove_gadget_mouse()
{
  _button.remove_mouse_click_listener(this);
}
void KTestWnd::add_window_mouse()
{
  _window.add_mouse_click_listener(this);
}

void KTestWnd::remove_window_mouse()
{
  _window.remove_mouse_click_listener(this);
}
void KTestWnd::add_all_mouse()
{
  _window.add_all_mouse_click_listener(this);
}
void KTestWnd::remove_all_mouse()
{
  _window.remove_all_mouse_click_listener(this);
}
