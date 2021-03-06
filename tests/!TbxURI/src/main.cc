
#include "tbx/application.h"
#include "tbx/autocreatelistener.h"
#include "TbxURIwnd.h"

using namespace std;

int main()
{
   tbx::Application my_app("<TbxURI$Dir>");

   TbxURIWnd window_handler;

   my_app.set_autocreate_listener("Window", &window_handler);

   my_app.run();

   return 0;
}
