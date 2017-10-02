#ifndef GUI_HOLDER_HPP
#define GUI_HOLDER_HPP

#include<TGUI/TGUI.hpp>

class GUIHolder {
	GUIHolder();
	void m_createComponentSelector();
	void m_addComponentButton(const std::string & name,
		const std::string & textureName = "");
	void m_componentButtonSignal(const std::string & name);

	static GUIHolder * s_instance;
	tgui::Gui m_gui;
	tgui::Theme m_theme;

public:
	static GUIHolder & get();
	tgui::Gui & gui();
};

#endif // GUI_HOLDER_HPP

