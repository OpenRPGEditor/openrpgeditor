#pragma once
#include <iostream>
#include <utility>
#include <vector>

template <typename T>
class TrackedVector {
public:
  using value_type = T;
  using size_type = std::size_t;

  using iterator = typename std::vector<T>::iterator;
  using const_iterator = typename std::vector<T>::const_iterator;
  using reference = typename std::vector<T>::reference;
  using const_reference = typename std::vector<T>::const_reference;

  TrackedVector() = default;
  TrackedVector(std::initializer_list<T> list)
  : m_data(list) {}
  TrackedVector(const_iterator first, const_iterator last)
  : m_data(first, last) {}

  bool is_dirty() const { return m_modified; }
  void clear_dirty() { m_modified = false; }

  template <typename... Args>
  reference emplace_back(Args&&... args) {
    mark_dirty();
    return m_data.emplace_back(std::forward<Args>(args)...);
  }

  reference front() {
    mark_dirty();
    return m_data.front();
  }

  const_reference front() const { return m_data.front(); }

  reference back() {
    mark_dirty();
    return m_data.back();
  }

  const_reference back() const { return m_data.back(); }

  reference at(size_type index) {
    mark_dirty();
    return m_data.at(index);
  }

  const_reference at(size_type index) const { return m_data.at(index); }

  // Non-const iterators allow modifications, so they must flag dirty immediately
  iterator begin() {
    mark_dirty();
    return m_data.begin();
  }
  iterator end() { return m_data.end(); }

  // Const iterators guarantee no modifications
  const_iterator begin() const { return m_data.begin(); }
  const_iterator end() const { return m_data.end(); }
  const_iterator cbegin() const { return m_data.cbegin(); }
  const_iterator cend() const { return m_data.cend(); }

  void push_back(const T& value) {
    mark_dirty();
    m_data.push_back(value);
  }
  void push_back(T&& value) {
    mark_dirty();
    m_data.push_back(std::move(value));
  }

  iterator insert(iterator position, const T& value) {
    mark_dirty();
    return m_data.insert(position, value);
  }

  template <typename InputIterator>
  iterator insert(const_iterator position, InputIterator first, InputIterator last) {
    mark_dirty();
    return m_data.insert(position, first, last);
  }

  iterator erase(iterator position) {
    mark_dirty();
    return m_data.erase(position);
  }

  iterator erase(const_iterator first, const_iterator last) {
    mark_dirty();
    return m_data.erase(first, last);
  }

  T& operator[](size_type index) {
    mark_dirty();
    return m_data[index];
  }
  const T& operator[](size_type index) const { return m_data[index]; }
  size_type size() const { return m_data.size(); }
  bool empty() const { return m_data.empty(); }

  void clear() {
    if (empty()) {
      return;
    }
    mark_dirty();
    m_data.clear();
  }

  void reserve(size_type capacity) {
    mark_dirty();
    m_data.reserve(capacity);
  }

  void resize(size_type capacity) {
    mark_dirty();
    m_data.resize(capacity);
  }

  void resize(size_type capacity, const value_type& value) {
    mark_dirty();
    m_data.resize(capacity, value);
  }

  bool operator==(const TrackedVector& rhs) const { return m_data == rhs.m_data; }
  bool operator!=(const TrackedVector& rhs) const { return m_data != rhs.m_data; }

private:
  std::vector<T> m_data;
  bool m_modified = false;

  void mark_dirty() { m_modified = true; }
};