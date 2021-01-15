#include "mainwindow.h"
#include "eventlog.h"

#include "tbx/optionbutton.h"
#include "tbx/path.h"
#include "tbx/ext/tabs.h"
#include "tbx/sprite.h"
#include "tbx/pointerinfo.h"
#include "tbx/keypressed.h"

#include "tbx/res/reswindow.h"
#include "tbx/ext/res/restabs.h"
#include "tbx/ext/res/restreeview.h"

#include <sstream>

#define COMPONENT_TREEVIEW		0x0
#define EVENTLOG_BUTTON 0x5
#define CREATE_BUTTON 0x6

#define COMPONENT_MOVEPREV		0x10
#define COMPONENT_MOVENEXT		0x11
#define COMPONENT_MOVECHILD		0x12
#define COMPONENT_MOVEPARENT		0x13

#define COMPONENT_ADD 			0x14
#define COMPONENT_ADDCHILD		0x15
#define COMPONENT_GETSTATE		0x16
#define COMPONENT_SETSTATE		0x17
#define COMPONENT_DELETE		0x18
#define COMPONENT_UPDATE		0x19
#define COMPONENT_GETFIRST		0x1a
#define COMPONENT_GETNEXT		0x1b
#define COMPONENT_EXPAND		0x1c
#define COMPONENT_SELECT		0x1d
#define COMPONENT_MAKEVISIBLE		0x1e
#define COMPONENT_GETNODESTATE		0x1f

#define COMPONENT_SETNODETEXT		0x60
#define COMPONENT_SETNODESPRITE		0x61

#define COMPONENT_WRITABLE_TEXT		0x100
#define COMPONENT_WRITABLE_SPRITE	0x101
#define COMPONENT_WRITABLE_EXPANDED	0x102
#define COMPONENT_DISPLAY_NODESTATE	0x103
#define COMPONENT_FLAGS_BASE		0x20
#define COMPONENT_NODE_FLAGS_BASE   0x80


const int NumTreeviewStates = 22;
typedef bool (tbx::ext::TreeView::*GetTreeStateFunc)(void) const;
GetTreeStateFunc GetTreeState[NumTreeviewStates] =
{
  &tbx::ext::TreeView::allow_vscroll,
  &tbx::ext::TreeView::allow_hscroll,
  &tbx::ext::TreeView::auto_update,
  &tbx::ext::TreeView::show_current_node,
  &tbx::ext::TreeView::plot_lines,
  &tbx::ext::TreeView::allow_selection,
  &tbx::ext::TreeView::notify_selection,
  &tbx::ext::TreeView::select_children,
  &tbx::ext::TreeView::allow_expand,
  &tbx::ext::TreeView::notify_expansion,
  &tbx::ext::TreeView::expand_by_default,
  &tbx::ext::TreeView::allow_rename,
  &tbx::ext::TreeView::notify_rename,
  &tbx::ext::TreeView::allow_drags,
  &tbx::ext::TreeView::notify_drag_end,
  &tbx::ext::TreeView::all_events,
  &tbx::ext::TreeView::double_click_expands,
  &tbx::ext::TreeView::flat_mode,
  &tbx::ext::TreeView::text_below_sprite,
  &tbx::ext::TreeView::sort_display,
  &tbx::ext::TreeView::sort_reversed,
  &tbx::ext::TreeView::sort_by_sprite
};

typedef void (tbx::ext::TreeView::*SetTreeStateFunc)(bool);
SetTreeStateFunc SetTreeState[NumTreeviewStates] =
{
  &tbx::ext::TreeView::allow_vscroll,
  &tbx::ext::TreeView::allow_hscroll,
  &tbx::ext::TreeView::auto_update,
  &tbx::ext::TreeView::show_current_node,
  &tbx::ext::TreeView::plot_lines,
  &tbx::ext::TreeView::allow_selection,
  &tbx::ext::TreeView::notify_selection,
  &tbx::ext::TreeView::select_children,
  &tbx::ext::TreeView::allow_expand,
  &tbx::ext::TreeView::notify_expansion,
  &tbx::ext::TreeView::expand_by_default,
  &tbx::ext::TreeView::allow_rename,
  &tbx::ext::TreeView::notify_rename,
  &tbx::ext::TreeView::allow_drags,
  &tbx::ext::TreeView::notify_drag_end,
  &tbx::ext::TreeView::all_events,
  &tbx::ext::TreeView::double_click_expands,
  &tbx::ext::TreeView::flat_mode,
  &tbx::ext::TreeView::text_below_sprite,
  &tbx::ext::TreeView::sort_display,
  &tbx::ext::TreeView::sort_reversed,
  &tbx::ext::TreeView::sort_by_sprite
};

const int NumNodeStates = 9;
typedef bool (tbx::ext::TreeViewCurrentNode::*GetTreeNodeStateFunc)(void) const;
GetTreeNodeStateFunc GetTreeNodeState[NumTreeviewStates] =
{
	&tbx::ext::TreeViewCurrentNode::has_next_sibling,
	&tbx::ext::TreeViewCurrentNode::has_prev_sibling,
	&tbx::ext::TreeViewCurrentNode::has_child,
	&tbx::ext::TreeViewCurrentNode::parent_is_root,
	&tbx::ext::TreeViewCurrentNode::has_sprite,
	&tbx::ext::TreeViewCurrentNode::has_expanded_sprite,
	&tbx::ext::TreeViewCurrentNode::has_text,
	&tbx::ext::TreeViewCurrentNode::expanded,
	&tbx::ext::TreeViewCurrentNode::selected
};

typedef void (tbx::ext::res::ResTreeView::*SetResTreeStateFunc)(bool);
SetResTreeStateFunc SetResTreeState[NumTreeviewStates] =
{
  &tbx::ext::res::ResTreeView::allow_vscroll,
  &tbx::ext::res::ResTreeView::allow_hscroll,
  &tbx::ext::res::ResTreeView::auto_update,
  &tbx::ext::res::ResTreeView::show_current_node,
  &tbx::ext::res::ResTreeView::plot_lines,
  &tbx::ext::res::ResTreeView::allow_selection,
  &tbx::ext::res::ResTreeView::notify_selection,
  &tbx::ext::res::ResTreeView::select_children,
  &tbx::ext::res::ResTreeView::allow_expand,
  &tbx::ext::res::ResTreeView::notify_expansion,
  &tbx::ext::res::ResTreeView::expand_by_default,
  &tbx::ext::res::ResTreeView::allow_rename,
  &tbx::ext::res::ResTreeView::notify_rename,
  &tbx::ext::res::ResTreeView::allow_drags,
  &tbx::ext::res::ResTreeView::notify_drag_end,
  &tbx::ext::res::ResTreeView::all_events,
  &tbx::ext::res::ResTreeView::double_click_expands,
  &tbx::ext::res::ResTreeView::flat_mode,
  &tbx::ext::res::ResTreeView::text_below_sprite,
  &tbx::ext::res::ResTreeView::sort_display,
  &tbx::ext::res::ResTreeView::sort_reversed,
  &tbx::ext::res::ResTreeView::sort_by_sprite
};


#include <iostream>

MainWindow::MainWindow()
  : _event_log(0)
{
  tbx::Window window("MainWindow");

  try
  {
	tbx::ActionButton(window.gadget(EVENTLOG_BUTTON)).add_selected_listener(this);
	tbx::ActionButton(window.gadget(CREATE_BUTTON)).add_selected_listener(this);
    _tabs = window.gadget(1);
    _pane0 = _tabs.window_from_tab(0);
    _treeview = _pane0.gadget(COMPONENT_TREEVIEW);
    _current_node = _treeview.current_node();
    _node_text = _pane0.gadget(COMPONENT_WRITABLE_TEXT);
    _node_sprite = _pane0.gadget(COMPONENT_WRITABLE_SPRITE);
    _node_expanded_sprite = _pane0.gadget(COMPONENT_WRITABLE_EXPANDED);

	_expand_node = _pane0.gadget(0x41);
    _recurse_node = _pane0.gadget(0x42);
    _select = _pane0.gadget(0x43);
    _unique = _pane0.gadget(0x44);

    for (int comp_id = COMPONENT_MOVEPREV; comp_id <= COMPONENT_GETNODESTATE; ++comp_id)
    {
      tbx::ActionButton(_pane0.gadget(comp_id)).add_selected_listener(this);
    }
	tbx::ActionButton(_pane0.gadget(COMPONENT_SETNODETEXT)).add_selected_listener(this);
	tbx::ActionButton(_pane0.gadget(COMPONENT_SETNODESPRITE)).add_selected_listener(this);

	tbx::Window tree_window = _treeview.underlying_window();
	tree_window.add_loader(this);

  } catch(std::exception &e)
  {
    std::cout << "Exception " << e.what() << std::endl;
  }
    window.show();
    get_treeview_state();
}

MainWindow::~MainWindow()
{
	delete _event_log;
}

void MainWindow::button_selected(tbx::ButtonSelectedEvent &event)
{
  std::cout << "Button pressed " << event.id_block().self_component().id() << std::endl;
  switch(event.id_block().self_component().id())
  {
	  case EVENTLOG_BUTTON:
	    show_log();
	  break;
	  case CREATE_BUTTON:
	     test_create();
		 break;
  case COMPONENT_MOVEPREV:
  _current_node.move_prev(_treeview.sort_display()); break;
  break;
case COMPONENT_MOVENEXT:
  _current_node.move_next(_treeview.sort_display()); break;
 break;
 case COMPONENT_MOVECHILD:
 _current_node.move_child(_treeview.sort_display());
 break;
 case COMPONENT_MOVEPARENT:
   _current_node.move_parent();
    break;
     case COMPONENT_ADD:
      _current_node.add_sibling(_node_text.text());
	  _current_node.sprite(1, _node_sprite.text(), _node_expanded_sprite.text());
      break;
    case COMPONENT_ADDCHILD:
      _current_node.add_child(_node_text.text());
	  _current_node.sprite(1, _node_sprite.text(), _node_expanded_sprite.text());
     break;

    case COMPONENT_SETNODETEXT:
	  _current_node.text(_node_text.text());
      break;

    case COMPONENT_SETNODESPRITE:
	  _current_node.sprite(1, _node_sprite.text(), _node_expanded_sprite.text());
      break;

     case COMPONENT_GETSTATE:
	     get_treeview_state();
	 break;
     case COMPONENT_SETSTATE:
	     set_treeview_state();
	 break;
     case COMPONENT_DELETE:
	 _current_node.erase();
	 break;
     case COMPONENT_UPDATE:
       _treeview.update_display();
       break;
     case COMPONENT_GETFIRST:
	   _current_node.move_first_selected();
	   get_node_details();
	 break;
     case COMPONENT_GETNEXT:
	 _current_node.move_next_selected();
	 get_node_details();
	 break;
     case COMPONENT_EXPAND:
	  _current_node.expand(_expand_node.on(), _recurse_node.on());
	 break;
     case COMPONENT_SELECT:
	  _current_node.select(_select.on(), !_unique.on());
	 break;
     case COMPONENT_MAKEVISIBLE:
	   _current_node.make_visible();
	 break;
     case COMPONENT_GETNODESTATE:
		get_node_details();
	 break;
  }
}

void MainWindow::show_log()
{
	if (!_event_log) _event_log = new EventLog(_treeview, _tabs);
	else _event_log->show();
}

void MainWindow::get_treeview_state()
{
	for (int i = 0; i < NumTreeviewStates; ++i)
	{
		tbx::OptionButton(_pane0.gadget(COMPONENT_FLAGS_BASE+i)).on((_treeview.*GetTreeState[i])());
	}
}

void MainWindow::set_treeview_state()
{
	for (int i = 0; i < NumTreeviewStates; ++i)
	{
		(_treeview.*SetTreeState[i])(tbx::OptionButton(_pane0.gadget(COMPONENT_FLAGS_BASE+i)).on());
	}
}

void MainWindow::get_node_details()
{
	_node_text.text(_current_node.text());
	_node_sprite.text(_current_node.sprite_name());
	_node_expanded_sprite.text(_current_node.expanded_sprite_name());
	for (int i = 0; i < NumNodeStates; ++i)
	{
		tbx::OptionButton(_pane0.gadget(COMPONENT_NODE_FLAGS_BASE+i)).on((_current_node.*GetTreeNodeState[i])());
	}
}

 bool MainWindow::load_file(tbx::LoadEvent &event)
 {
	 if (tbx::key_pressed(tbx::INTERNAL_KEY_ALT))
	 {
		 report_node_under_ptr();
		 return true;
	 }

	 if (event.file_type() < tbx::FILE_TYPE_DIRECTORY)
	 {
		 return true;
     }
	_treeview.clear();

	/* check the "small icons" option	*/
	bool small = tbx::OptionButton(_pane0.gadget(0x40)).on();

	/* if Shift pressed, just add contents of the directory as a single	*/
	/* level in the tree, otherwise add everything recursively.		*/

	if (tbx::key_pressed(tbx::INTERNAL_KEY_SHIFT))
	{
		scan_directory(event.file_name(), small, false, false);
	} else
	{
		scan_directory(event.file_name(), small, true, true);
	}

	_treeview.update_display();

	return true;
 }

void MainWindow::report_node_under_ptr()
{
	tbx::PointerInfo pi;
	tbx::ext::TreeNodeId node_id = _treeview.find_node(pi.mouse_x(), pi.mouse_y());
	std::string text;
	if (node_id)
	{
		_current_node.move_to(node_id);
		text = _current_node.text();
	} else
	{
		text = "[no node]";
	}
	show_log();
	std::ostringstream ss;
	ss << "Drag ended on node id " << std::hex << node_id << " : " << text;
	_event_log->add_message(ss.str());
}

 int MainWindow::scan_directory(const std::string &path, bool small, bool add_this, bool recurse)
 {
	tbx::Path the_path(path);
	std::string spr, exspr;
	int count = 0;
	bool child = true;

	if (add_this)
	{
		std::string leaf = the_path.leaf_name();
		spr = (small) ? "small_dir" : "directory";
		exspr = small ? "small_diro" : "directoryo";
		_current_node.add_sibling(leaf);
		_current_node.sprite(1, spr, exspr);
	}

    for(tbx::PathInfo::Iterator it = tbx::PathInfo::begin(the_path); it != tbx::PathInfo::end(); ++it)
	{
		++count;
		if (it->file_type() < 0x1000)
		{
		   std::ostringstream ss;
		   ss << (small ? "small" : "file") << "_" << std::hex << it->file_type();
		   spr = ss.str();
		   exspr.clear();
		} else
		{
			spr = (small) ? "small_dir" : "directory";
			exspr = small ? "small_diro" : "directoryo";
		}

		if (child) _current_node.add_child(it->name());
		else _current_node.add_sibling(it->name());
		child = false;
		_current_node.sprite(1, spr, exspr);

		if (recurse && it->file_type() >= 0x1000)
		{
		    if (scan_directory(the_path.child(it->name()), small, false, recurse) > 0)
			{
				_current_node.move_parent();
			}
		}
	}

	return count;
 }


void MainWindow::test_create()
{
	tbx::res::ResWindow res_window("CreateTest");
	res_window.title_buflen(42);
	res_window.title_text("Test creating of tabs/treeview from code");
	res_window.visible_xmin(32);
	res_window.visible_ymin(440);
	res_window.visible_xmax(860);
	res_window.visible_ymax(1264);
	tbx::ext::res::ResTabs res_tabs;
	res_tabs.xmin(32);
	res_tabs.ymin(-800);
	res_tabs.xmax(800);
	res_tabs.ymax(-24);

	res_tabs.nested_windows("PaneTest");
	res_window.add_gadget(res_tabs);
	tbx::Window window(res_window);

	tbx::Window pane = tbx::ext::Tabs(window.gadget(0)).window_from_tab(0);
	tbx::ext::res::ResTreeView res_treeview;
	res_treeview.xmin(16);
	res_treeview.ymin(-708);
	res_treeview.xmax(740);
	res_treeview.ymax(-16);
	for (int j = 0; j < NumTreeviewStates; ++j)
	{
		(res_treeview.*SetResTreeState[j])((_treeview.*GetTreeState[j])());
	}
	pane.add_gadget(res_treeview);

	tbx::ext::TreeView new_treeview(pane.gadget(0));
	tbx::ext::TreeViewCurrentNode new_node(new_treeview);

	while (!_current_node.parent_is_root()) _current_node.move_parent();
	_current_node.move_parent();
	tbx::ext::TreeNodeId root_id = _current_node.node_id();
	//std::cout << "root id " << root_id << std::endl;
	do
	{
		if (_current_node.has_child())
		{
			_current_node.move_child();
			// std::cout << "Add child " << _current_node.text() << " ";
			new_node.add_child(_current_node.text());
			new_node.sprite(1, _current_node.sprite_name(), _current_node.expanded_sprite_name());
		} else
		{
 			while (!_current_node.has_next_sibling() && !_current_node.parent_is_root())
			{
				_current_node.move_parent();
				new_node.move_parent();
				// std::cout << "  up level to " << _current_node.node_id() << std::endl;
			}
			if (_current_node.has_next_sibling())
			{
				_current_node.move_next();
				//std::cout << "Add sibling " << _current_node.text() << " ";
				new_node.add_sibling(_current_node.text());
				new_node.sprite(1, _current_node.sprite_name(), _current_node.expanded_sprite_name());
			} else
			{
				// std::cout << "move to parent ";
				_current_node.move_parent();
			}
		}
		// std::cout << "current id " << _current_node.node_id() << std::endl;
	} while (_current_node.node_id() != root_id);

	new_treeview.update_display();
	window.show();
}
