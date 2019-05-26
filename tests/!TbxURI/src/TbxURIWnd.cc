
#include "TbxURIWnd.h"
#include "tbx/window.h"
#include "tbx/actionbutton.h"
#include "tbx/messagewindow.h"
#include "tbx/stringutils.h"

using namespace tbx;

const ComponentId c_open_button = 2;
const ComponentId c_check_button = 3;
 /**
 * Set up listeners and commands for the window.
 */
void TbxURIWnd::auto_created(std::string template_name, Object object)
{
    Window window(object);
    _uri_field = window.gadget(1);
    _version_field = window.gadget(5);
    _status_field = window.gadget(6);
    ActionButton open = window.gadget(c_open_button);
    ActionButton test = window.gadget(c_check_button);
    open.add_selected_listener(this);
    test.add_selected_listener(this);
    URI::ensure_uri_handler();
    _uri.set_result_handler(this);
    update_version();
}

void TbxURIWnd::button_selected(tbx::ButtonSelectedEvent &event)
{
    _uri.uri(_uri_field.text());
    bool launched = false;

    switch(event.id_block().self_component().id())
    {
        case c_open_button:
           launched = _uri.dispatch();
           break;
        case c_check_button:
           launched = _uri.check();
           break;
    }

    if (launched)
    {
       if (_version.empty()) update_version();
    } else
    {
       show_message("Unable to dispatch the URI");
    }
    update_status();
}

void TbxURIWnd::uri_result(URI &uri, bool claimed)
{
   update_status();
   if (!claimed)
   { 
      show_message("No application claimed processing for the URI");
   }
}

void TbxURIWnd::update_version()
{
   int v = URI::version();
   if (v)
   {
      _version = to_string(v);
      _version.insert(_version.size()-2,".");
      _version_field.text(_version);
   }
}

void TbxURIWnd::update_status()
{
    std::string stat;
    switch(_uri.state())
    {                
       case URI::NotDispatched: stat = "Not dispatched"; break;
       case URI::Dispatched:    stat = "Dispatched"; break;
       case URI::DispatchError: stat = "Dispatch error"; break;
       case URI::WaitingForResult: stat = "Waiting for result"; break;
       case URI::Claimed: stat = "Claimed"; break;
       case URI::NotClaimed: stat = "Not claimed"; break;
    }
    _status_field.text(stat);
}
