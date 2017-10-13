#include<Prjoct2/JSONHolder.hpp>
#include<Prjoct2/History.hpp>
#include<nlohmann/json.hpp>
#include<stdexcept>
#include<iostream>
#include<iomanip>

History * History::s_instance = nullptr;

History::History()
	: m_modificationStarted() {
}

History & History::get() {
	if (s_instance == nullptr) {
		s_instance = new History();
	}
	return *(s_instance);
}

void History::beginModification() {
	if (m_modificationStarted) {
		throw std::logic_error("[HISTORY] Modification is already \
started!");
	}
	m_beforeModification = JSONHolder::get()["components"];
	m_modificationStarted = true;
}

void History::abortModification() {
	if (!m_modificationStarted) {
		throw std::logic_error("[HISTORY] Modification is not started, \
cannot abort!");
	}
	m_modificationStarted = false;
}

void History::endModification() {
	if (!m_modificationStarted) {
		throw std::logic_error("[HISTORY] Modification is not started, \
cannot end!");
	}
	nlohmann::json afterModification = JSONHolder::get()["components"];
	HistoryEntryPtr entry(new HistoryEntry);
	entry->patch = nlohmann::json::diff(m_beforeModification,
		afterModification);
	entry->reversePatch = nlohmann::json::diff(afterModification,
		m_beforeModification);
	m_entryList.push_front(std::move(entry));
	m_modificationStarted = false;
}

void History::undo() {
	if (m_entryList.empty()) {
		return;
	}
	m_entryList.front()->undo();
	m_entryList.pop_front();
}

#include<Prjoct2/Debug.hpp>

void History::HistoryEntry::undo() {
	nlohmann::json components = JSONHolder::get()["components"];
	JSONHolder::get()["components"] = components.patch(reversePatch);
}

