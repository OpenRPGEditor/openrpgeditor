#include "Database/Weapon.hpp"

void to_json(nlohmann::ordered_json& j, const Weapon& w) {
  j = {
      {"id", w.m_id},
      {"animationId", w.m_animationId},
      {"description", w.m_description},
      {"etypeId", w.m_etypeId},
      {"traits", w.m_traits},
      {"iconIndex", w.m_iconIndex},
      {"name", w.m_name},
      {"note", w.m_note},
      {"params", w.m_params},
      {"price", w.m_price},
      {"wtypeId", w.m_wtypeId},
  };
}

void from_json(const nlohmann::ordered_json& j, Weapon& w) {
  w.m_id = j.value("id", w.m_id);
  w.m_animationId = j.value("animationId", w.m_animationId);
  w.m_description = j.value("description", w.m_description);
  w.m_etypeId = j.value("etypeId", w.m_etypeId);
  w.m_traits = j.value("traits", w.m_traits);
  w.m_iconIndex = j.value("iconIndex", w.m_iconIndex);
  w.m_name = j.value("name", w.m_name);
  w.m_note = j.value("note", w.m_note);
  w.m_params = j.value("params", w.m_params);
  w.m_price = j.value("price", w.m_price);
  w.m_wtypeId = j.value("wtypeId", w.m_wtypeId);
}