#include<Prjoct2/TextureHolder.hpp>
#include<Prjoct2/JSONHolder.hpp>
#include<Prjoct2/GUIHolder.hpp>
#include<Prjoct2/Utility.hpp>
#include<Prjoct2/App.hpp>
#include<functional>
#include<iostream>

GUIHolder * GUIHolder::s_instance = nullptr;

GUIHolder::GUIHolder()
	: m_gui(App::get().window()) {
	std::string resourcePackName = JSONHolder::get()["settings"]
		["resource pack"];
	std::string resourcePackPath = "data/resources/" + resourcePackName
		+ '/';
	JSONHolder::get().fromFile(resourcePackPath + "conf", false,
		"resources/" + resourcePackName);
	m_theme.load(resourcePackPath + "themes/" + JSONHolder::get()
		["resources/" + resourcePackName]["theme"].get<std::string>()
		+ ".txt");
	tgui::Theme::setDefault(&m_theme);
}

void GUIHolder::initEditorGUI() {
	m_createMenuBar();
	m_createComponentSelector();
	m_createFileNameLabel();
}

void GUIHolder::clear() {
	m_gui.removeAllWidgets();
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

void GUIHolder::m_createMenuBar() {
	auto bar = tgui::MenuBar::create();
	m_gui.add(bar, "menuBar");
	bar->setSize("100%", JSONHolder::get()["settings"]["menuBar"]
		["height"].get<int>());
	bar->addMenu("File");
	bar->addMenuItem("New");
	bar->addMenuItem("Open");
	bar->addMenuItem("Save");
	std::function<void(const std::vector<sf::String> &)> signal
		= std::bind(&GUIHolder::m_menuBarSignal, this, std::placeholders::_1);
	bar->connect("menuItemClicked", signal);
}

void GUIHolder::m_createComponentSelector() {
	tgui::ScrollablePanel::Ptr panel = tgui::ScrollablePanel::create();
	m_gui.add(panel, "componentPanel");
	panel->setSize(0, "100% - menuBar.size");
	panel->setPosition("0", "menuBar.bottom");
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
				"100% - menuBar.size");
		} else {
			m_gui.get<tgui::Button>("componentPanelToggleButton")
				->setText(">");
			m_gui.get<tgui::ScrollablePanel>("componentPanel")
				->setSize(0, "100% - menuBar.size");
		}
	});
	m_createComponentButtons();
	auto menu = m_gui.get<tgui::MenuBar>("menuBar");
	m_gui.remove(menu);
	m_gui.add(menu, "menuBar");
}

void GUIHolder::m_createComponentButtons() {
	std::string resourcePackName = JSONHolder::get()["settings"]
		["resource pack"];
	for (nlohmann::json & entry : JSONHolder::get()["resources/"
		+ resourcePackName]["components"]) {
		std::string name = entry["name"];
		if (entry.count("texture") == 1) {
			m_addComponentButton(name, entry["texture"]
				.get<std::string>());
		} else {
			m_addComponentButton(name);
		}
	}
}

void GUIHolder::m_addComponentButton(const std::string & name,
	const std::string & requestedTextureName) {
	if (JSONHolder::get()["components/" + name].is_null()) {
		JSONHolder::get().fromFile("components/" + name);
	}
	std::string textureName;
	if (requestedTextureName != "") {
		textureName = requestedTextureName;
	} else {
		textureName = JSONHolder::get()["components/" + name]
		["texture"];
	}
	int buttonSize = JSONHolder::get()["settings"]["componentPanel"]
		["button"]["size"];
	int spacing = JSONHolder::get()["settings"]["componentPanel"]
		["spacing"];
	auto button = tgui::Button::create();
	button->setSize(buttonSize, buttonSize);
	m_createComponentButtonTextures(textureName);
	button->getRenderer()->setTexture(TextureHolder::get()["shelf/"
		+ textureName]);
	button->getRenderer()->setTextureHover(TextureHolder::get()[
		"shelf/opened/" + textureName]);
	button->getRenderer()->setBorders(tgui::Borders());
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

void GUIHolder::m_createComponentButtonTextures(const std::string & name) {
	int buttonSize = JSONHolder::get()["settings"]["componentPanel"]
		["button"]["size"];
	sf::RenderTexture renderTexture;
	renderTexture.create(buttonSize, buttonSize);
	sf::RectangleShape shelfRect(sf::Vector2f(buttonSize, buttonSize));
	shelfRect.setTexture(&TextureHolder::get()["shelf"], true);
	renderTexture.draw(shelfRect);
	int picSize = JSONHolder::get()["settings"]["componentPanel"]["button"]
		["picture size"];
	sf::RectangleShape picRect(sf::Vector2f(picSize, picSize));
	picRect.setFillColor(sf::Color(255, 255, 255, 128));
	picRect.setPosition((buttonSize - picSize) / 2, buttonSize - picSize
		- 10);
	renderTexture.draw(picRect);
	picRect.setFillColor(sf::Color::White);
	picRect.setTexture(&TextureHolder::get()[name], true);
	renderTexture.draw(picRect);
	renderTexture.display();
	TextureHolder::get().set("shelf/" + name, renderTexture.getTexture());
	shelfRect.setTexture(&TextureHolder::get()["shelf/" + name], true);
	shelfRect.move(0, 10);
	renderTexture.clear(sf::Color(128, 128, 128));
	renderTexture.draw(shelfRect);
	renderTexture.display();
	TextureHolder::get().set("shelf/opened/" + name, renderTexture
		.getTexture());
}

void GUIHolder::m_componentButtonSignal(const std::string & name) {
	JSONHolder::get()["current"] = JSONHolder::get()["components/" + name];
	JSONHolder::get()["next component type"] = name;
	App::get().editor().cancelMovingComponent();
}

void GUIHolder::createContextMenu(int x, int y) {
	auto list = tgui::ListBox::create();
	list->setPosition(x, y);
	list->addItem("Edit");
	list->addItem("Move");
	list->addItem("Rotate");
	list->addItem("Delete");
	auto borders = list->getRenderer()->getBorders();
	auto padding = list->getRenderer()->getPadding();
	list->setSize(100, list->getItemHeight() * list->getItemCount()
		+ borders.getBottom() + borders.getTop()
		+ padding.getBottom() + padding.getTop());
	std::function<void(const std::string &)> signal = std::bind(&GUIHolder
		::m_contextMenuSignal, this, std::placeholders::_1);
	list->connect("itemSelected", signal);
	m_gui.add(list, "contextMenu");
}

void GUIHolder::m_contextMenuSignal(const std::string & item) {
	if (item == "") {
		return;
	}
	if (item == "Edit") {
		m_createComponentPropertyEditor();
	} else if (item == "Move") {
		App::get().editor().moveSelectedComponent();
	} else if (item == "Rotate") {
		App::get().editor().rotateSelectedComponent();
	} else if (item == "Delete") {
		App::get().editor().deleteSelectedComponent();
	}
	removeContextMenu();
}

void GUIHolder::m_createComponentPropertyEditor() {
	if (!JSONHolder::get()["editing component ID"].is_null()) {
		closeComponentPropertyEditor();
	}
	JSONHolder::get()["editing component ID"]
		= JSONHolder::get()["selected component ID"];
	auto panel = tgui::Panel::create();
	m_gui.add(panel, "componentPropertyEditor");
	panel->setSize(JSONHolder::get()["settings"]["componentPropertyEditor"]
		["width"].get<int>(), "100% - menuBar.height");
	panel->setPosition("100% - width", "menuBar.bottom");
	auto & settings = JSONHolder::get()["settings"]
		["componentPropertyEditor"];
	int spacing = settings["spacing"];
	int entryHeight = settings["entry"]["height"];
	auto typeLabel = tgui::Label::create("Type: " + JSONHolder::get()
		["components"][JSONHolder::get()["editing component ID"]
		.get<std::string>()]["type"].get<std::string>());
	panel->add(typeLabel);
	typeLabel->setPosition(spacing, spacing);
	typeLabel->setSize(tgui::bindWidth(panel) - 2 * spacing, entryHeight);
	typeLabel->setVerticalAlignment(typeLabel->VerticalAlignment::Center);
	if (!m_checkSelectedComponentProperties()) {
		auto noPropsLabel = tgui::Label::create("No properties");
		panel->add(noPropsLabel);
		noPropsLabel->setSize(tgui::bindSize(typeLabel));
		noPropsLabel->setPosition(spacing, tgui::bindBottom(typeLabel)
			+ spacing);
		noPropsLabel->setVerticalAlignment(noPropsLabel
			->VerticalAlignment::Center);
	}
	auto toggleButton = tgui::Button::create(">");
	m_gui.add(toggleButton, "componentPropertyEditorToggleButton");
	toggleButton->setSize("componentPanelToggleButton.size");
	toggleButton->setPosition("componentPropertyEditor.left - width",
		"componentPanelToggleButton.top");
	std::function<void()> signal =  std::bind(&GUIHolder
		::closeComponentPropertyEditor, this);
	toggleButton->connect("pressed", signal);
}

void GUIHolder::closeComponentPropertyEditor() {
	m_gui.remove(m_gui.get("componentPropertyEditorToggleButton"));
	m_gui.remove(m_gui.get("componentPropertyEditor"));
	JSONHolder::get()["editing component ID"] = nlohmann::json();
}

bool GUIHolder::m_checkSelectedComponentProperties() {
	bool propertiesFound = false;
	auto panel = m_gui.get<tgui::Panel>("componentPropertyEditor");
	auto & component = JSONHolder::get()["components"]
		[JSONHolder::get()["editing component ID"]
		.get<std::string>()];
	if (component.count("properties") > 0) {
		auto & settings = JSONHolder::get()["settings"]
			["componentPropertyEditor"];
		int spacing = settings["spacing"];
		int entryHeight = settings["entry"]["height"];
		std::map<std::string, nlohmann::json> propMap
			= component["properties"];
		for (auto it = propMap.begin(); it != propMap.end(); it++) {
			propertiesFound = true;
			std::string propName = it->first;
			nlohmann::json & prop = it->second;
			auto label = tgui::Label::create(capitalize(propName)
				+ ":");
			tgui::Widget::Ptr lastWidget;
			lastWidget = panel->getWidgets().back();
			panel->add(label);
			label->setPosition(spacing, spacing
				+ tgui::bindBottom(lastWidget));
			label->setSize(tgui::bindWidth(panel) - spacing * 2,
				entryHeight);
			label->setVerticalAlignment(
				tgui::Label::VerticalAlignment::Center);
			auto editBox = tgui::EditBox::create();
			panel->add(editBox);
			editBox->setSize(tgui::bindSize(label));
			editBox->setPosition(spacing, tgui::bindBottom(label)
				+ spacing);
			editBox->setDefaultText("0");
			editBox->setText(toString(prop.get<float>()));
			editBox->setInputValidator(tgui::EditBox
				::Validator::Float);
			std::function<void(sf::String)> signal = std
				::bind(&GUIHolder
				::m_componentPropertyEditorSignal, this,
				std::placeholders::_1, propName);
			editBox->connect("textChanged", signal);
			editBox->getRenderer()->setCaretWidth(1);
		}
	}
	return propertiesFound;
}

void GUIHolder::m_componentPropertyEditorSignal(sf::String text,
	std::string propName) {
	auto & component = JSONHolder::get()["components"][JSONHolder::get()
		["editing component ID"].get<std::string>()];
	try {
		component["properties"][propName] = std::stof(text
			.toAnsiString());
	} catch (std::invalid_argument e) {
	}
}

void GUIHolder::removeContextMenu() {
	m_gui.remove(m_gui.get("contextMenu"));
	JSONHolder::get()["selected component ID"] = nlohmann::json();
}

void GUIHolder::m_menuBarSignal(const std::vector<sf::String> & data) {
	if (data[0] == "File") {
		if (data[1] == "New") {
			App::get().editor().createNewCircuit();
		} else if (data[1] == "Open") {
			App::get().editor().openCircuit();
		} else if (data[1] == "Save") {
			App::get().editor().saveCircuit();
		}
	}
}

void GUIHolder::m_createDialogWindow() {
	auto dialogWindow = tgui::MessageBox::create();
	m_gui.add(dialogWindow, "dialogWindow");
	dialogWindow->setSize(400, 100);
	dialogWindow->setTitleButtons(tgui::MessageBox::Close);
	auto container = tgui::HorizontalLayout::create();
	dialogWindow->add(container, "container");
	int spacing = 8;
	container->setSize(tgui::bindWidth(dialogWindow) - 2 * spacing, 25);
	container->setPosition(spacing, tgui::bindHeight(dialogWindow)
		- tgui::bindHeight(container) - spacing);
	auto editBox = tgui::EditBox::create();
	container->add(editBox, "editBox");
	container->setRatio(editBox, 2);
	editBox->setInputValidator("[a-zA-Z0-9_-]*");
	auto button = tgui::Button::create("OK");
	container->add(button, "button");
	dialogWindow->setPosition(100, 200);
	dialogWindow->connect("closed", [this] {
		closeDialogWindow();
		App::get().editor().unlockUI();
	});
}

void GUIHolder::createOpenFileDialogWindow() {
	m_createDialogWindow();
	auto dialogWindow = m_gui.get<tgui::MessageBox>("dialogWindow");
	dialogWindow->setTitle("Open file");
	dialogWindow->setText("Enter circuit name:");
	auto container = dialogWindow->get<tgui::HorizontalLayout>("container");
	auto button = container->get<tgui::Button>("button");
	auto editBox = container->get<tgui::EditBox>("editBox");
	button->connect("pressed", [editBox] {
		App::get().editor().openCircuit(editBox->getText());
	});
}

void GUIHolder::createSaveFileDialogWindow() {
	m_createDialogWindow();
	auto dialogWindow = m_gui.get<tgui::MessageBox>("dialogWindow");
	dialogWindow->setTitle("Save file");
	dialogWindow->setText("Enter circuit name:");
	auto container = dialogWindow->get<tgui::HorizontalLayout>("container");
	auto button = container->get<tgui::Button>("button");
	auto editBox = container->get<tgui::EditBox>("editBox");
	button->connect("pressed", [editBox] {
		App::get().editor().saveCircuit(editBox->getText());
	});
}

void GUIHolder::closeDialogWindow() {
	m_gui.remove(m_gui.get("dialogWindow"));
}

void GUIHolder::updateFileName(const std::string & name) {
	if (name == "") {
		m_gui.get<tgui::Label>("fileName")->setText("~Untitled");
	} else {
		m_gui.get<tgui::Label>("fileName")->setText(name);
	}
}

void GUIHolder::m_createFileNameLabel() {
	auto label = tgui::Label::create();
	m_gui.add(label, "fileName");
	label->setSize(300, "menuBar.height");
	label->setPosition("&.w - w - 5", 0);
	label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Right);
	label->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	updateFileName();
}

