#pragma once

#include <string>

using namespace std::string_view_literals;

class IWizardPage {
public:
  virtual ~IWizardPage() = default;
  virtual void draw() = 0;
  [[nodiscard]] virtual constexpr std::string_view pageTitle() const = 0;
  [[nodiscard]] virtual bool allRequiredFieldsAreSet() const { return true; }
};