#pragma once
#include "tbx/ext/treeview.h"
#include "tbx/ext/treeviewlisteners.h"
#include "tbx/ext/tabs.h"
#include "tbx/ext/tabsselectionlistener.h"
#include "tbx/view/listview.h"
#include "tbx/view/itemrenderer.h"

class EventLog :
	public tbx::ext::TreeViewNodeSelectedListener,
	public tbx::ext::TreeViewNodeExpandedListener,
	public tbx::ext::TreeViewNodeDraggedListener,
	public tbx::ext::TreeViewScrollListener,
	public tbx::ext::TabsSelectionListener
{
public:
	EventLog(tbx::ext::TreeView &treeview, tbx::ext::Tabs &tabs);
	virtual ~EventLog();
	void show();

	void add_message(const std::string &msg);

	virtual void treeview_node_selected(const tbx::ext::TreeViewNodeSelectedEvent &event);
	virtual void treeview_node_expanded(const tbx::ext::TreeViewNodeExpandedEvent &event);
	virtual void treeview_node_dragged(const tbx::ext::TreeViewNodeDraggedEvent &event);
	virtual void treeview_scroll(const tbx::ext::TreeViewScrollEvent &event);

	virtual void tabs_selection(const tbx::ext::TabsSelectionEvent &event);


private:
   tbx::ext::TreeView _treeview;
   std::vector<std::string> _lines;
   tbx::view::IndexItemViewValue< std::string, std::vector<std::string> > _item_value;
   tbx::view::WimpFontItemRenderer _line_renderer;
   tbx::view::ListView _view;
};