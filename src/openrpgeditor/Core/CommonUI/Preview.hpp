#pragma once

#include "IDialogController.hpp"

#include <string>
#include <vector>

class Preview : public IDialogController {
public:
  Preview(const std::string_view name, const std::vector<std::string>& values, int selection, int rangeStart = 1);
  std::tuple<bool, bool> draw() override;
};
