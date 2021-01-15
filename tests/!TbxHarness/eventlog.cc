#include "eventlog.h"
#include <sstream>
#include <iostream>

EventLog::EventLog(tbx::ext::TreeView &treeview, tbx::ext::Tabs &tabs) :
    _treeview(treeview),
	_item_value(_lines),
	_line_renderer(&_item_value),
	_view(tbx::Window("EventLog"), &_line_renderer)
{
	treeview.add_node_selected_listener(this);
	treeview.add_node_expanded_listener(this);
	treeview.add_node_dragged_listener(this);
	treeview.add_scroll_listener(this);	
	tabs.add_selection_listener(this);
	
}

EventLog::~EventLog()
{
}

void EventLog::show()
{
_view.window().show();
}


void EventLog::add_message(const std::string &msg)
{
	_lines.push_back(msg);
	_view.inserted(_lines.size()-1, 1);
	tbx::WindowInfo winfo;
	_view.window().get_info(winfo);
	int visible_height = winfo.visible_area().bounds().height();
	int diff_to_bottom = winfo.visible_area().scroll().y - winfo.work_area().min.y;
	if (diff_to_bottom > visible_height)
	{
		_view.window().scroll(winfo.visible_area().scroll().x, winfo.work_area().min.y + visible_height);
	}
}
	
void EventLog::treeview_node_selected(const tbx::ext::TreeViewNodeSelectedEvent &event)
{
	tbx::ext::TreeViewCurrentNode node = _treeview.current_node(event.node());
	std::ostringstream ss;
	ss << "Node selected: object: " << event.id_block().self_object().handle()
	   << " component: " << event.id_block().self_component().id()
	   << " select flag: " << event.selected()
	   << " multiple flag: " << event.multiple()
	   << " node id: " << event.node() << " node text: " << node.text();
	add_message(ss.str());
}

void EventLog::treeview_node_expanded(const tbx::ext::TreeViewNodeExpandedEvent &event)
{
	tbx::ext::TreeViewCurrentNode node = _treeview.current_node(event.node());
	std::ostringstream ss;
	ss << "Node expanded: object: " << event.id_block().self_object().handle()
	   << " component: " << event.id_block().self_component().id()
	   << " expand flag: " << event.expanded()
	   << " node id: " << event.node() << " node text: " << node.text();
	add_message(ss.str());
}

void EventLog::treeview_node_dragged(const tbx::ext::TreeViewNodeDraggedEvent &event)
{
	tbx::ext::TreeViewCurrentNode node = _treeview.current_node(event.node());
	std::ostringstream ss;
	ss << "Node dragged: object: " << event.id_block().self_object().handle()
	   << " component: " << event.id_block().self_component().id()
	   << " node id: " << event.node() << " node text: " << node.text()
	   << " destination: ";
   if (event.outside())
   {
		ss << "outside";
	} else
	{
	   if (event.destination_node() != 0)
	   {
		node.move_to(event.destination_node());
		ss << event.destination_node() << " " << node.text();
	} else
	{ 
	   ss << "unknown";
	}
	}
	
	ss << " mouse button: ";
	if (event.select()) ss << "SELECT";
	else if (event.adjust()) ss << "ADJUST";
	else ss << "?";
		   
	add_message(ss.str());
}

void EventLog::treeview_scroll(const tbx::ext::TreeViewScrollEvent &event)
{
	std::ostringstream ss;
	ss << "Tree scrolled: object: " << event.id_block().self_object().handle()
	   << " component: " << event.id_block().self_component().id();
	
	add_message(ss.str());
}


void EventLog::tabs_selection(const tbx::ext::TabsSelectionEvent &event)
{
	std::ostringstream ss;
	ss << "Tabs selecton: object: " << event.id_block().self_object().handle()
	   << " component: " << event.id_block().self_component().id()
	   << " old index: " << event.previous()
	   << " new index: " << event.selected();
	
	add_message(ss.str());
}
