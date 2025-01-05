#include "Database/Map.hpp"
#include "Database/Event.hpp"
#include "Database/EventPage.hpp"
#include <fstream>

void to_json(nlohmann::ordered_json& json, const Map& map) {
  json = {
      {"autoplayBgm", map.m_autoplayBgm},
      {"autoplayBgs", map.m_autoplayBgs},
      {"battleback1Name", map.m_battleback1Name},
      {"battleback2Name", map.m_battleback2Name},
      {"bgm", map.m_bgm},
      {"bgs", map.m_bgs},
      {"disableDashing", map.m_disableDashing},
      {"displayName", map.m_displayName},
      {"encounterList", map.m_encounterList},
      {"encounterStep", map.m_encounterStep},
      {"height", map.m_height},
      {"note", map.m_note},
      {"parallaxLoopX", map.m_parallaxLoopX},
      {"parallaxLoopY", map.m_parallaxLoopY},
      {"parallaxName", map.m_parallaxName},
      {"parallaxShow", map.m_parallaxShow},
      {"parallaxSx", map.m_parallaxSx},
      {"parallaxSy", map.m_parallaxSy},
      {"scrollType", map.m_scrollType},
      {"specifyBattleback", map.m_specifyBattleback},
      {"tilesetId", map.m_tilesetId},
      {"width", map.m_width},
      {"data", map.m_data},
      {"events", map.m_events},
  };
}
void from_json(const nlohmann::ordered_json& json, Map& map) {
  map.m_autoplayBgm = json.value("autoplayBgm", map.m_autoplayBgm);
  map.m_autoplayBgs = json.value("autoplayBgs", map.m_autoplayBgs);
  map.m_battleback1Name = json.value("battleback1Name", map.m_battleback1Name);
  map.m_battleback2Name = json.value("battleback2Name", map.m_battleback2Name);
  map.m_bgm = json.value("bgm", map.m_bgm);
  map.m_bgs = json.value("bgs", map.m_bgs);
  map.m_disableDashing = json.value("disableDashing", map.m_disableDashing);
  map.m_displayName = json.value("displayName", map.m_displayName);
  map.m_encounterList = json.value("encounterList", map.m_encounterList);
  map.m_encounterStep = json.value("encounterStep", map.m_encounterStep);
  map.m_height = json.value("height", map.m_height);
  map.m_note = json.value("note", map.m_note);
  map.m_parallaxLoopX = json.value("parallaxLoopX", map.m_parallaxLoopX);
  map.m_parallaxLoopY = json.value("parallaxLoopY", map.m_parallaxLoopY);
  map.m_parallaxName = json.value("parallaxName", map.m_parallaxName);
  map.m_parallaxShow = json.value("parallaxShow", map.m_parallaxShow);
  map.m_parallaxSx = json.value("parallaxSx", map.m_parallaxSx);
  map.m_parallaxSy = json.value("parallaxSy", map.m_parallaxSy);
  map.m_scrollType = json.value("scrollType", map.m_scrollType);
  map.m_specifyBattleback = json.value("specifyBattleback", map.m_specifyBattleback);
  map.m_tilesetId = json.value("tilesetId", map.m_tilesetId);
  map.m_width = json.value("width", map.m_width);
  map.m_data = json.value("data", map.m_data);
  map.m_events = json.value("events", map.m_events);
}

void to_json(nlohmann::ordered_json& json, const Map::Encounter& encounter) {
  json = {
      {"regionSet", encounter.regionSet},
      {"troopId", encounter.troopId},
      {"weight", encounter.weight},
  };
}
void from_json(const nlohmann::ordered_json& json, Map::Encounter& encounter) {
  encounter.regionSet = json.value("regionSet", encounter.regionSet);
  encounter.troopId = json.value("troopId", encounter.troopId);
  encounter.weight = json.value("weight", encounter.weight);
}

void Map::resize(const int newWidth, const int newHeight) {
  static constexpr int MaxLayers = 6;
  std::vector<std::optional<int>> newData;
  newData.resize(MaxLayers * newWidth * newHeight);
  std::ranges::fill(newData.begin(), newData.end(), 0);

  for (int y = 0; y < newHeight; ++y) {
    for (int x = 0; x < newWidth; ++x) {
      if (x < m_width && y < m_height) {
        for (int z = 0; z < MaxLayers; ++z) {
          const int si = (z * m_height + y) * m_width + x;
          const int di = (z * newHeight + y) * newWidth + x;
          newData[di] = m_data[si];
        }
      }
    }
  }

  std::ranges::for_each(m_events, [&](auto& event) {
    if (event->x() >= newWidth || event->y() >= newHeight) {
      event.reset();
    }
  });

  m_width = newWidth;
  m_height = newHeight;
  m_data = std::move(newData);
}

[[nodiscard]] std::vector<Event*> Map::getSorted() {
  std::vector<Event*> ret;
  for (auto& e : m_events) {
    if (e) {
      ret.push_back(&e.value());
    }
  }
  std::ranges::sort(ret, [](const Event* a, const Event* b) {
    const int aZ = static_cast<int>(a->page(0)->priorityType) * 2 + 1;
    const int bZ = static_cast<int>(b->page(0)->priorityType) * 2 + 1;
    if (aZ != bZ) {
      return aZ < bZ;
    }

    if (a->y() != b->y()) {
      return a->y() < b->y();
    }
    return a->renderer()->spriteId() < b->renderer()->spriteId();
  });
  return ret;
}

[[nodiscard]] std::vector<Event*> Map::getRenderSorted() {
  std::vector<Event*> ret;
  for (auto& e : m_events) {
    if (e) {
      ret.push_back(&e.value());
    }
  }
  std::ranges::sort(ret, [](const Event* a, const Event* b) {
    if (a->renderer()->screenZ() != b->renderer()->screenZ()) {
      return a->renderer()->screenZ() < b->renderer()->screenZ();
    }
    if (a->renderer()->y() != b->renderer()->y()) {
      return a->renderer()->y() < b->renderer()->y();
    }
    return a->renderer()->spriteId() < b->renderer()->spriteId();
  });
  return ret;
}

std::vector<Event*> Map::eventsAtNoThrough(const int x, const int y) {
  std::vector<Event*> ret;
  std::ranges::for_each(m_events, [&ret, &x, &y](auto& ev) {
    if (ev && ev->x() == x && ev->y() == y && !ev->renderer()->isThrough()) {
      ret.push_back(&ev.value());
    }
  });
  return ret;
}

std::vector<Event*> Map::eventsAtRenderPosNoThrough(const int x, const int y) {
  std::vector<Event*> ret;
  std::ranges::for_each(m_events, [&ret, &x, &y](auto& ev) {
    if (ev && ev->renderer()->x() == x && ev->renderer()->y() == y && !ev->renderer()->isThrough()) {
      ret.push_back(&ev.value());
    }
  });
  return ret;
}