#include "cursesprototype.h"

#include <algorithm>
#include <memory>

#include <boost/locale/encoding_utf.hpp>

#include <cwidget/generic/util/ssprintf.h>
#include <cwidget/generic/util/transcode.h>
#include <cwidget/style.h>
#include <cwidget/toplevel.h>
#include <cwidget/widgets/button.h>
#include <cwidget/widgets/center.h>
#include <cwidget/widgets/editline.h>
#include <cwidget/widgets/frame.h>
#include <cwidget/widgets/label.h>
#include <cwidget/widgets/menu.h>
#include <cwidget/widgets/menubar.h>
#include <cwidget/widgets/multiplex.h>
#include <cwidget/widgets/stacked.h>
#include <cwidget/widgets/subtree.h>
#include <cwidget/widgets/table.h>
#include <cwidget/widgets/tree.h>
#include <cwidget/widgets/treeitem.h>

#include <sigc++/bind.h>

#include <gamekeeper/client/autowire.h>
#include <gamekeeper/client/game.h>
#include <gamekeeper/client/store.h>
#include <gamekeeper/client/storecontroller.h>

namespace client = gamekeeper::client;
namespace cw = cwidget;
namespace cww = cwidget::widgets;

using boost::locale::conv::utf_to_utf;

GAMECLIENTUI_CLASS(CursesPrototype)

static cww::stacked_ref rootView;
static cww::tree_ref gameTree;
static cww::tree_ref storeTree;
static cww::multiplex_ref tabbedContent;

static std::vector<std::unique_ptr<client::Game>> games;
static std::vector<std::unique_ptr<client::Store>> stores;

static void popup(const cww::widget_ref & w, bool all = true)
{
	rootView->add_widget(w);
	if(all)
	{
		w->show_all();
	}
	else
	{
		w->show();

	}
}

template <class Model>
class ModelTreeItem : virtual public cww::treeitem
{
protected:
	const std::unique_ptr<Model> & model;
	const std::wstring name;

public:
	ModelTreeItem(const std::unique_ptr<Model> & _model)
	:	model(_model),
		name(utf_to_utf<wchar_t>(this->model->getName())){}

	const std::unique_ptr<Model> & getModel()
	{
		return this->model;
	}

	virtual const wchar_t * tag() override
	{
		return this->name.c_str();
	}

	virtual const wchar_t * label() override
	{
		return this->tag();
	}

	virtual void paint(cww::tree *win, int y, bool hierarchical, const cw::style &st) override
	{
		this->cww::treeitem::paint(win, y, hierarchical, this->name);
	}
};

static void doLogin(cww::editline_ref login, cww::editline_ref cancel, const std::unique_ptr<client::Store> & store, cww::widget_ref widget)
{
	//if successfull fill game list
	if(store->login(utf_to_utf<char>(login->get_text()), utf_to_utf<char>(cancel->get_text())))
	{
		for(auto & game : store->getAllGames())
		{
			games.push_back(std::move(game));
		}
	}
	widget->destroy();

}

class StoreModelTreeItem : public ModelTreeItem<client::Store>
{
public:
	StoreModelTreeItem(const std::unique_ptr<client::Store> & model)
	:	ModelTreeItem<client::Store>(model){}

	virtual void paint(cww::tree *win, int y, bool hierarchical, const cw::style &st) override
	{
		this->cww::treeitem::paint(win, y, hierarchical, this->name + L" [" + (this->getModel()->isLoggedIn() ? L"Logged In]" : L"Not Logged In]"));
	}

	virtual bool dispatch_key(const cw::config::key & k, cww::tree * owner) override
	{
		if(k.ch == L'a')
		{
			cww::table_ref t = cww::table::create();

			cww::button_ref ok = cww::button::create("Ok");
			t->add_widget_opts(cww::label::create("l: login/logout"), 0, 0, 1, 1, cww::table::EXPAND | cww::table::SHRINK, cww::table::EXPAND);
			t->add_widget_opts(ok, 1, 0, 1, 1, cww::table::EXPAND | cww::table::SHRINK, cww::table::EXPAND);

			t->show_all();
			t->focus_widget(ok);

			cww::frame_ref frame = cww::frame::create(t);
			frame->set_bg_style(cw::style_attrs_flip(A_REVERSE));

			cww::center_ref c = cww::center::create(frame);
			ok->pressed.connect(sigc::mem_fun(c.unsafe_get_ref(), &cww::widget::destroy));

			popup(c);
			return true;
		}
		else if(k.ch == L'l')
		{
			const auto & store = this->getModel();
			if(store->isLoggedIn())
			{
				store->logout();
			}
			else
			{
				cww::table_ref t = cww::table::create();
				cww::editline_ref name = cww::editline::create(32, "Username: ");
				cww::editline_ref password = cww::editline::create(32, "Password: ");

				cww::button_ref login = cww::button::create("Login");
				cww::button_ref cancel = cww::button::create("Cancel");

				t->add_widget_opts(name, 0, 0, 1, 2, cww::table::EXPAND | cww::table::SHRINK, cww::table::EXPAND);
				t->add_widget_opts(password, 1, 0, 1, 2, cww::table::EXPAND | cww::table::SHRINK, cww::table::EXPAND);
				t->add_widget_opts(login, 2, 0, 1, 1, cww::table::EXPAND | cww::table::SHRINK, cww::table::EXPAND);
				t->add_widget_opts(cancel, 2, 1, 1, 1, cww::table::EXPAND | cww::table::SHRINK, cww::table::EXPAND);

				cww::frame_ref frame = cww::frame::create(t);
				frame->set_bg_style(cw::style_attrs_flip(A_REVERSE));

				cww::center_ref c = cww::center::create(frame);
				login->pressed.connect(sigc::bind<cww::editline_ref, cww::editline_ref, const std::unique_ptr<client::Store> &, cww::widget_ref>
					(sigc::ptr_fun(&doLogin), name, password, store, c));
				cancel->pressed.connect(sigc::mem_fun(c.unsafe_get_ref(), &cww::widget::destroy));

				popup(c);
			}
			return true;
		}
		return false;
	}
};

template <class Model>
class ModelSubTree : public cww::subtree<ModelTreeItem<Model>, cww::tag_sort_policy>
{
private:
	const std::wstring category;
public:
	ModelSubTree(const std::string & _category)
	:	cww::subtree<ModelTreeItem<Model>, cww::tag_sort_policy>(true),
		category(utf_to_utf<wchar_t>(_category)){}

	virtual void paint(cwidget::widgets::tree *win, int y, bool hierarchical, const cwidget::style &st) override
	{
		this->cww::treeitem::paint(win, y, hierarchical, this->label());
	}

	virtual const wchar_t * label() override
	{
		return this->category.c_str();
	}

	virtual const wchar_t * tag() override
	{
		return this->label();
	}
};

static ModelSubTree<client::Store> * storeTreeRoot;
static ModelSubTree<client::Game> * gameTreeRoot;

static void
refillGameList()
{
	static std::vector<std::string> addedGames;

	for(const auto & game : games)
	{
		if(std::find(addedGames.begin(), addedGames.end(), game->getName()) != addedGames.end())
		{
			continue;
		}
		addedGames.push_back(game->getName());
		gameTreeRoot->add_child(new ModelTreeItem<client::Game>(game));
	}
	gameTreeRoot->sort();
}

static void
refillStoreList()
{
	static std::vector<std::string> addedStores;

	for(const auto & store : stores)
	{
		if(std::find(addedStores.begin(), addedStores.end(), store->getName()) != addedStores.end())
		{
			continue;
		}
		addedStores.push_back(store->getName());
		storeTreeRoot->add_child(new StoreModelTreeItem(store));
	}
}

static void
closeCurrentview()
{
	cww::widget_ref curWidget = tabbedContent->visible_widget();
	if(!curWidget.valid())
	{
		return;
	}
	curWidget->hide();
}

static void
displayGameList()
{
	if(!gameTree.valid())
	{
		gameTreeRoot = new ModelSubTree<client::Game>("All");
		gameTree = cww::tree::create(gameTreeRoot, true);
		tabbedContent->add_widget(gameTree, L"Game List");
	}

	refillGameList();
	gameTree->show();
}

static void
displayStoreList()
{
	if(!storeTree.valid())
	{
		storeTreeRoot = new ModelSubTree<client::Store>("All");
		storeTree = cww::tree::create(storeTreeRoot, true);
		tabbedContent->add_widget(storeTree, L"Store List");
	}

	refillStoreList();
	storeTree->show();
}

static cww::menu_info fileMenuInfo[] =
{
	cww::menu_info(cww::menu_info::MENU_ITEM, "^Quit", "QuitProgram", "Exit the program", sigc::ptr_fun(cwidget::toplevel::exitmain)),
	cww::menu_info(cww::menu_info::MENU_ITEM, "^Close", "CloseView", "Close the current view", sigc::ptr_fun(closeCurrentview)),
	cww::menu_info::MENU_END
};

static cww::menu_info viewsMenuInfo[] =
{
	cww::menu_info(cww::menu_info::MENU_ITEM, "^Games", "Show Game List", "Show Game List", sigc::ptr_fun(displayGameList)),
	cww::menu_info(cww::menu_info::MENU_ITEM, "^Stores", "Show Store List", "Show Store List", sigc::ptr_fun(displayStoreList)),
	cww::menu_info::MENU_END
};

CursesPrototype::CursesPrototype(gamekeeper::core::Logger& _logger)
:	logger(_logger){}

void
CursesPrototype::init(const ConfigMap & cm)
{
	cw::toplevel::init();
	rootView = cww::stacked::create();
	rootView->show();

	cww::menubar_ref menubar = cww::menubar::create(true);
	rootView->add_widget(menubar);
	menubar->show();

	menubar->append_item(cw::util::transcode("File"), cww::menu::create(0, 0, 0, fileMenuInfo));
	menubar->append_item(cw::util::transcode("Views"), cww::menu::create(0, 0, 0, viewsMenuInfo));

	cww::table_ref mainTable = cww::table::create();
	menubar->set_subwidget(mainTable);
	mainTable->show();

	cw::config::global_bindings.set("Quit", cw::config::parse_key(L"q"));
	cw::config::global_bindings.set("Close", cw::config::parse_key(L"c"));
	cw::config::global_bindings.set("Games", cw::config::parse_key(L"g"));
	cw::config::global_bindings.set("Stores", cw::config::parse_key(L"s"));

	rootView->connect_key_post("Quit", &cw::config::global_bindings, sigc::ptr_fun(cwidget::toplevel::exitmain));
	rootView->connect_key_post("Close", &cw::config::global_bindings, sigc::ptr_fun(closeCurrentview));
	rootView->connect_key_post("Games", &cw::config::global_bindings, sigc::ptr_fun(displayGameList));
	rootView->connect_key_post("Stores", &cw::config::global_bindings, sigc::ptr_fun(displayStoreList));

	std::wstring quitKey = cw::config::global_bindings.readable_keyname("Quit");
	std::wstring gamesKey = cw::config::global_bindings.readable_keyname("Games");
	std::wstring storesKey = cw::config::global_bindings.readable_keyname("Stores");
	std::wstring helptext = cw::util::swsprintf(L"%ls: Quit | %ls: Games | %ls: Stores | a: open action menu of list items", quitKey.c_str(), gamesKey.c_str(), storesKey.c_str());
	cww::label_ref helpLabel = cww::label::create(helptext, cw::get_style("Header"));
	mainTable->add_widget_opts(helpLabel, 0, 0, 1, 1, cww::table::FILL, 0);

	tabbedContent = cww::multiplex::create(true);
	mainTable->add_widget(tabbedContent, 2, 0, 1, 1);
	tabbedContent->show();

	cw::toplevel::settoplevel(rootView);
}

void
CursesPrototype::onShutdown()
{
	cw::toplevel::shutdown();
}

void
CursesPrototype::startEventLoop()
{
	// preload all stores
	client::Autowire<client::StoreController> sc;
	stores = sc->getAll();
	cw::toplevel::mainloop();
}

bool
CursesPrototype::isPortable()
{
	return false;
}
