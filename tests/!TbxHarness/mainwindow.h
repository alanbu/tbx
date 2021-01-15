#pragma once

#include "tbx/window.h"
#include "tbx/actionbutton.h"
#include "tbx/buttonselectedlistener.h"
#include "tbx/writablefield.h"
#include "tbx/optionbutton.h"
#include "tbx/loader.h"
#include "tbx/ext/treeview.h"
#include "tbx/ext/tabs.h"

class EventLog;

class MainWindow :
	tbx::ButtonSelectedListener,
	tbx::Loader
{
  public:
  MainWindow();
  ~MainWindow();

  virtual void button_selected(tbx::ButtonSelectedEvent &event);
  virtual bool load_file(tbx::LoadEvent &event);

  void get_treeview_state();
  void set_treeview_state();
  void get_node_details();

  int scan_directory(const std::string &path, bool small, bool add_this, bool recurse);
  void test_create();
  void report_node_under_ptr();
  void show_log();

  private:
  tbx::ext::TreeView _treeview;
  tbx::ext::TreeViewCurrentNode _current_node;
  tbx::ext::Tabs _tabs;
  tbx::Window _pane0;
  tbx::WritableField _node_text;
  tbx::WritableField _node_sprite;
  tbx::WritableField _node_expanded_sprite;
  tbx::OptionButton _expand_node;
  tbx::OptionButton _recurse_node;
  tbx::OptionButton _select;
  tbx::OptionButton _unique;
  EventLog *_event_log;
};
