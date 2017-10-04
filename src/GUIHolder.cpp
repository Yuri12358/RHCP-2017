#include<Prjoct2/TextureHolder.hpp>
#include<Prjoct2/JSONHolder.hpp>
#include<Prjoct2/GUIHolder.hpp>
#include<Prjoct2/App.hpp>
#include<functional>
#include<iostream>

GUIHolder * GUIHolder::s_instance = nullptr;

GUIHolder::GUIHolder()
	: m_gui(App::get().window()) {
	std::string defaultThemePath = JSONHolder::get()["settings"]
		["default theme"];
	m_theme.load(defaultThemePath);
	tgui::Theme::setDefault(&m_theme);
	m_createComponentSelector();
	m_addComponentButton("lamp");
	m_addComponentButton("battery");
	m_addComponentButton("resistor");
	m_addComponentButton("switch");
	m_addComponentButton("transistor");
	m_addComponentButton("rheostat");
	m_addComponentButton("bell");
	m_addComponentButton("ampermeter");
	m_addComponentButton("voltmeter");
	m_addComponentButton("diode");
	m_addComponentButton("electromagnet");
	m_addComponentButton("fuse");
}

GUIHolder & GUIHolder::get() {
	if (s_instance == nullptr) {
		s_instance = new GUIHolder();
	}
	return *s_instance;
}

tgui::Gui & GUIHolder::gui() {
	return m_gui;
}

void GUIHolder::m_createComponentSelector() {
	tgui::ScrollablePanel::Ptr panel = tgui::ScrollablePanel::create();
	m_gui.add(panel, "componentPanel");
	panel->setSize(0, "100%");
	tgui::Button::Ptr toggleButton = tgui::Button::create(">");
	m_gui.add(toggleButton, "componentPanelToggleButton");
	toggleButton->setSize(25, 25);
	toggleButton->setPosition(tgui::bindRight(panel), 100);
	toggleButton->connect("pressed", [&](std::string text) {
		if (text == ">") {
			m_gui.get<tgui::Button>("componentPanelToggleButton")
				->setText("<");
			int buttonSize = JSONHolder::get()["settings"]
				["componentPanel"]["button"]["size"];
			int spacing = JSONHolder::get()["settings"]
				["componentPanel"]["spacing"];
			m_gui.get<tgui::ScrollablePanel>("componentPanel")
				->setSize(2 * spacing + buttonSize + 20,
				"100%");
		} else {
			m_gui.get<tgui::Button>("componentPanelToggleButton")
				->setText(">");
			m_gui.get<tgui::ScrollablePanel>("componentPanel")
				->setSize(0, "100%");
		}
	});
}

void GUIHolder::m_addComponentButton(const std::string & name,
	const std::string & requestedTextureName) {
	std::string textureName;
	if (requestedTextureName == "") {
		textureName = name;
	} else {
		textureName = requestedTextureName;
	}
	if (JSONHolder::get()["components/" + name].is_null()) {
		JSONHolder::get().fromFile("components/" + name);
	}
	int buttonSize = JSONHolder::get()["settings"]["componentPanel"]
		["button"]["size"];
	int buttonBorders = JSONHolder::get()["settings"]["componentPanel"]
		["button"]["borders"];
	int spacing = JSONHolder::get()["settings"]["componentPanel"]
		["spacing"];
	auto button = tgui::Button::create();
	button->setSize(buttonSize, buttonSize);
	button->getRenderer()->setTexture(TextureHolder::get()[textureName]);
	button->getRenderer()->setBorders(tgui::Borders(buttonBorders));
	auto panel = m_gui.get<tgui::ScrollablePanel>("componentPanel");
	int index = panel->getWidgets().size();
	button->setPosition(spacing, buttonSize * index + spacing 
		* (index + 1));
	std::function<void(const std::string &)> signal = std::bind(&GUIHolder
		::m_componentButtonSignal, this, name);
	button->connect("pressed", signal);
	panel->add(button);
	panel->setContentSize(sf::Vector2f(spacing * 2 + buttonSize,
		buttonSize * (index + 1) + spacing * (index + 2)));
}

void GUIHolder::m_componentButtonSignal(const std::string & name) {
	JSONHolder::get()["current"] = JSONHolder::get()["components/" + name];
}

void GUIHolder::createContextMenu(int x, int y) {
	auto list = tgui::ListBox::create();
	list->setPosition(x, y);
	list->addItem("Delete");
	list->setSize(100, list->getItemHeight() * list->getItemCount() + 2);
	std::function<void(const std::string &)> signal = std::bind(&GUIHolder
		::m_contextMenuSignal, this, std::placeholders::_1);
	list->connect("itemSelected", signal);
	m_gui.add(list, "contextMenu");
}

void GUIHolder::m_contextMenuSignal(const std::string & item) {
	if (item == "") {
		return;
	}
	removeContextMenu();
	if (item == "Delete") {
		App::get().deleteSelectedComponent();
	}
}

void GUIHolder::removeContextMenu() {
	m_gui.remove(m_gui.get("contextMenu"));
}

