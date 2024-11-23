#pragma once
#include "FirstBootWizard/IWizardPage.hpp"

#include <memory>
#include <vector>

class FirstBootWizard {
public:
  bool draw();
  void addPage(const std::shared_ptr<IWizardPage>& page);

private:
  std::vector<std::shared_ptr<IWizardPage>> m_pages;
  int m_currentPage = 0;
};
