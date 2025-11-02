#pragma once

#include <orei18n.hpp>
#include <string>

using namespace std::string_view_literals;

class IWizardPage {
public:
  virtual ~IWizardPage() = default;
  virtual void draw() = 0;
  [[nodiscard]] virtual const std::string pageTitle() const = 0;
  [[nodiscard]] virtual bool allRequiredFieldsAreSet() const { return true; }
};