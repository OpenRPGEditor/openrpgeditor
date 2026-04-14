#pragma once

#include <algorithm>
#include <cinttypes>
#include <deque>
#include <string>
#include <string_view>

#include <imgui.h>
#include <imgui_internal.h>

using namespace std::string_view_literals;

class INotification {
public:
  explicit INotification(const std::string_view title);
  virtual ~INotification();

  [[nodiscard]] std::string_view title() const { return m_title; }
  void setTitle(const std::string_view title) { m_title = title; }

  virtual void draw() = 0;

private:
  std::string_view m_title;
};

class SimpleNotification : public INotification {
public:
  SimpleNotification(const std::string_view title, const std::string_view text)
  : INotification(title)
  , m_text(text) {}

  [[nodiscard]] std::string_view text() const { return m_text; }
  void setText(const std::string_view text) { m_text = text; }
  void draw() override;

private:
  std::string m_text;
};

static float calculateProgressWidth(const float initialTime, const float currentTime, const float maxWidth) {
  if (initialTime <= 0.0f) {
    return 0.0f;
  }
  return std::clamp(currentTime / initialTime, 0.0f, 1.0f) * maxWidth;
}

class TimedNotification : public SimpleNotification {
public:
  TimedNotification(const std::string_view title, const std::string_view text, const float timeout)
  : SimpleNotification(title, text)
  , m_initialTime(timeout)
  , m_timeout(timeout) {}

  void draw() override {
    SimpleNotification::draw();
    m_timeout -= ImGui::GetIO().DeltaTime;
    const auto windowSize = ImGui::GetContentRegionMax();
    const auto win = ImGui::GetCurrentWindow();
    const auto dl = win->DrawList;
    if (const auto width = calculateProgressWidth(m_initialTime, m_timeout, windowSize.x); width > 0) {
      dl->AddLine(win->ContentRegionRect.Min + ImVec2{0, windowSize.y}, win->ContentRegionRect.Min + ImVec2{width, windowSize.y}, ImGui::GetColorU32(ImGuiCol_PlotHistogram), 5);
    }
  }

  bool isExpired() const { return m_timeout <= 0.f; }

private:
  float m_initialTime;
  float m_timeout;
};
class ProgressNotification final : public INotification {
public:
  ProgressNotification(const std::string_view initialText, const std::string_view title, const int totalWork)
  : INotification(title)
  , m_totalWork(totalWork)
  , m_text(initialText) {}

  double progress() const { return (static_cast<double>(m_currentWork) / m_totalWork) * 100.0; }

  void addProgress(const uint64_t total) { m_currentWork += total; }
  void decProgress(const uint64_t total) { m_currentWork -= total; }

  void draw() override;

private:
  uint64_t m_totalWork;
  uint64_t m_currentWork = 0;
  std::string m_text;
};

template <int N>
class MultiProgressNotification final : public INotification {
public:
  MultiProgressNotification(const std::string_view title, const std::array<uint64_t, N>& totalWork)
  : INotification(title)
  , m_totalWork(totalWork) {}

  double progress(const int idx) {
    if (idx < 0 || idx >= N) {
      return 0.f;
    }

    return (static_cast<double>(m_currentWork[idx]) / m_totalWork[idx]) * 100.0;
  }

  void addProgress(const int idx, const uint64_t total) {
    if (idx < 0 || idx >= N) {
      return;
    }
    m_currentWork[idx] += total;
  }
  void decProgress(const int idx, const uint64_t total) {
    if (idx < 0 || idx >= N) {
      return;
    }
    m_currentWork[idx] -= total;
  }

  void reset(const int idx, uint64_t totalWork, const std::string_view text) {
    if (idx < 0 || idx >= N) {
      return;
    }
    m_currentWork[idx] = 0;
    m_totalWork[idx] = totalWork;
    m_text[idx] = text;
  }

  void setText(const int idx, const std::string_view text) {
    if (idx < 0 || idx >= N) {
      return;
    }
    m_text[idx] = text;
  }

  [[nodiscard]] std::string_view text(const int idx) const {
    if (idx < 0 || idx >= N) {
      return ""sv;
    }
    return m_text[idx];
  }

  void draw() override {
    for (int i = 0; i < N; ++i) {
      ImGui::ProgressBar(progress(i), ImVec2(-FLT_MIN, 0), m_text[i].c_str());
    }
  }

private:
  std::array<uint64_t, N> m_totalWork;
  std::array<uint64_t, N> m_currentWork;
  std::array<std::string, N> m_text;
};

class NotificationCenter {
public:
  NotificationCenter(const NotificationCenter&) = delete;
  NotificationCenter& operator=(const NotificationCenter&) = delete;
  NotificationCenter(NotificationCenter&&) = delete;
  NotificationCenter& operator=(NotificationCenter&&) = delete;

  static NotificationCenter& instance() {
    static NotificationCenter instance;
    return instance;
  }

  void addNotification(INotification* notification) {
    if (std::ranges::find(m_notifications, notification) == m_notifications.end()) {
      m_notifications.push_front(notification);
    }
  }

  void removeNotification(INotification* notification) {
    const auto iter = std::ranges::remove(m_notifications, notification);
    m_notifications.erase(iter.begin(), iter.end());
  }

  void draw();

private:
  NotificationCenter() = default;

  std::deque<INotification*> m_notifications;
  bool m_isOpen = true;
};
