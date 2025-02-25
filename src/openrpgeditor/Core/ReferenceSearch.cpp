#include "Core/ReferenceSearch.hpp"

#include "MainWindow.hpp"
ReferenceSearch::ReferenceSearch(MainWindow* parent) : m_parent(parent) {}
void ReferenceSearch::findAllReferences(int targetId, SearchType type) {
  m_results.clear();
  for (auto& mapInfo : m_parent->database().mapInfos.mapInfos()) {
    if (mapInfo->map()) {
      for (auto& event : mapInfo->map()->events()) {
        if (event) {
          for (auto& ev : event.value().isConditionalReference(targetId, type)) {
            if (ev.second) {
              m_results.emplace_back(mapInfo->id(), event.value(), ev.first);
            }
          }
        }
      }
    }
  }
  searchAllListsByTarget(targetId, type);
  searchAllCommonByTarget(targetId, type);
}
void ReferenceSearch::findAllReferences(std::string text, SearchType type) {
  for (auto& result : m_results) {
    auto refEvent = result.getEvent();
    for (auto& page : refEvent.pages()) {
      int pageIndex{0};
      for (auto& cmd : page.list()) {
        if (cmd->hasStringReference(text, type)) {
          m_list.emplace_back(result.getMapId(), result.getEvent(), cmd, pageIndex);
        }
        pageIndex++;
      }
    }
  }
  searchAllListsByText(text, type);
  searchAllCommonByText(text, type);
}
void ReferenceSearch::searchAllListsByTarget(int targetId, SearchType type) {
  for (auto& mapInfo : m_parent->database().mapInfos.mapInfos()) {
    if (mapInfo->map()) {
      for (auto& event : mapInfo->map()->events()) {
        if (event) {
          for (auto& page : event.value().pages()) {
            int pageIndex{0};
            for (auto& cmd : page.list()) {
              if (cmd->hasReference(targetId, type)) {
                m_list.emplace_back(mapInfo->id(), event.value(), cmd, pageIndex);
              }
              pageIndex++;
            }
          }
        }
      }
    }
  }
}
void ReferenceSearch::searchAllListsByText(std::string text, SearchType type) {
  for (auto& mapInfo : m_parent->database().mapInfos.mapInfos()) {
    if (mapInfo->map()) {
      for (auto& event : mapInfo->map()->events()) {
        if (event) {
          for (auto& page : event.value().pages()) {
            int pageIndex{0};
            for (auto& cmd : page.list()) {
              if (cmd->hasStringReference(text, type)) {
                m_list.emplace_back(mapInfo->id(), event.value(), cmd, pageIndex);
              }
              pageIndex++;
            }
          }
        }
      }
    }
  }
}
void ReferenceSearch::searchAllCommonByTarget(int targetId, SearchType type) {
  m_common.clear();
  int index{0};
  for (auto& common : m_parent->database().commonEvents.events()) {
    if (common.has_value()) {
      for (auto& cmd : common.value().commands()) {
        if (cmd->hasReference(targetId, type)) {
          m_common.emplace_back(&common, index);
        }
        index++;
      }
    }
  }
}
void ReferenceSearch::searchAllCommonByText(std::string text, SearchType type) {
  m_common.clear();
  int index{0};
  for (auto& common : m_parent->database().commonEvents.events()) {
    if (common.has_value()) {
      bool resultFound{false};
      for (auto& cmd : common.value().commands()) {
        if (cmd->hasStringReference(text, type)) {
          m_common.emplace_back(&common, index);
        }
        index++;
      }
    }
  }
}
