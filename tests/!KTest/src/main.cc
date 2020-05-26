
#include "tbx/application.h"
#include "tbx/autocreatelistener.h"
#include "KTestwnd.h"

using namespace std;

int main()
{
   tbx::Application my_app("<KTest$Dir>");

   KTestWnd window_handler;

   my_app.set_autocreate_listener("Window", &window_handler);

   my_app.run();

   return 0;
}
