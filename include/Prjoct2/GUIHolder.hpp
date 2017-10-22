#ifndef GUI_HOLDER_HPP
#define GUI_HOLDER_HPP

#include<TGUI/TGUI.hpp>

class GUIHolder {
	GUIHolder();
	void m_createMenuBar();
	void m_createComponentSelector();
	void m_createComponentButtons();
	void m_addComponentButton(const std::string & name,
		const std::string & textureName = "");
	void m_createComponentButtonTextures(const std::string & name);
	void m_componentButtonSignal(const std::string & name);
	void m_contextMenuSignal(const std::string & selectedItem);
	void m_menuBarSignal(const std::vector<sf::String> & data);
	void m_createDialogWindow();
	void m_createFileNameLabel();
	void m_createComponentPropertyEditor();
	bool m_checkSelectedComponentProperties();
	void m_componentPropertyEditorSignal(sf::String str, std::string prop);

	static GUIHolder * s_instance;
	tgui::Gui m_gui;
	tgui::Theme m_theme;

public:
	static GUIHolder & get();
	tgui::Gui & gui();
	void createContextMenu(int x, int y);
	void removeContextMenu();
	void createOpenFileDialogWindow();
	void createSaveFileDialogWindow();
	void closeDialogWindow();
	void closeComponentPropertyEditor();
	void updateFileName(const std::string & name = "");
};

#endif // GUI_HOLDER_HPP

