
#ifndef _TbxURIWND_H_
#define _TbxURIWND_H_

#include "tbx/autocreatelistener.h"
#include "tbx/writablefield.h"
#include "tbx/displayfield.h"
#include "tbx/buttonselectedlistener.h"
#include "tbx/uri.h"


/**
 * Class to set up listeners an variables when the single
 * window is shown.
 */
class TbxURIWnd 
   : public tbx::AutoCreateListener,
   public tbx::ButtonSelectedListener,
   public tbx::URIResultHandler
{
public:
    TbxURIWnd() : _uri("") {}
    virtual void auto_created(std::string template_name, tbx::Object object);
    virtual void button_selected(tbx::ButtonSelectedEvent &event);
    virtual void uri_result(tbx::URI &uri, bool claimed);
    void update_version();
    void update_status();

private:
    tbx::URI _uri;
    std::string _version;
    tbx::WritableField _uri_field;
    tbx::DisplayField _version_field;
    tbx::DisplayField _status_field;
};

#endif
