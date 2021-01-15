// Name        : tbxharness.cc
// Author      : Alan Buckley
// Copyright   : This is released under the same COPYRIGHT as TBX
// Description : Minimal tbx application
//

#include "tbx/application.h"
#include "mainwindow.h"
#include "tbx/menu.h"

int main()
{
    // Set up the application loading in the toolbox resources
	 tbx::Application my_app("<TbxHarness$Dir>");

  MainWindow main;
  tbx::Menu menu("Menu");

    // Run the application
	my_app.run();

	return 0;
}
