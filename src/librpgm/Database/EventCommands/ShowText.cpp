#include "ShowText.hpp"

#include "Database/Database.hpp"

NextTextCommand::NextTextCommand(const std::optional<int>& _indent, const nlohmann::json& parameters)
: IEventCommand(_indent, parameters) {
  parameters[0].get_to(text);
}

void NextTextCommand::serializeParameters(nlohmann::json& out) const { out.push_back(text); }

ShowTextCommand::ShowTextCommand(const std::optional<int>& _indent, const nlohmann::json& parameters)
: IEventCommand(_indent, parameters) {
  parameters[0].get_to(faceImage);
  parameters[1].get_to(faceIndex);
  parameters[2].get_to(background);
  parameters[3].get_to(position);
}

void ShowTextCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(faceImage);
  out.push_back(faceIndex);
  out.push_back(background);
  out.push_back(position);
}

std::string ShowTextCommand::stringRep(const Database& db) const {
  const auto name = UndectorateEnumName(code());
  std::string ret = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + name + colon.data() +
                    ColorFormatter::popColor() + ColorFormatter::getColor(Color::Gray) +
                    db.imageText(faceImage, faceIndex) + ", " + DecodeEnumName(background) + ", " +
                    DecodeEnumName(position) + ColorFormatter::popColor();

  for (const auto& t : text) {
    if (!ret.empty()) {
      ret += "\n";
    }
    ret += indentText(indent) + symbol(t->code()) + ColorFormatter::getColorCode(t->code()) +
           std::string(name.length(), ' ') + colon.data() + t->text + ColorFormatter::popColor();
  }
  return ret;
}