#pragma once
#include <optional>
#include <string_view>
#include <vector>

#include "OREMath/Point.hpp"
#include "OREMath/Size.hpp"

#include "Utils/Typename.hpp"

#include <orei18n.hpp>

class Map;
class IMapTool {
public:
  virtual ~IMapTool() = default;

  /**
   *
   * @param layer
   * @param cursorPoint
   * @param mapSize
   * @param tiles
   * @param penData
   * @return
   */
  [[nodiscard]] virtual bool execute(int layer, const Point& cursorPoint, const Size& mapSize, std::vector<std::optional<int>>& tiles, const std::vector<std::array<int, 7>>& penData) const = 0;

  /**
   *
   * @return
   */
  [[nodiscard]] virtual std::string_view identifier() const = 0;

  /**
   *
   * @return
   */
  [[nodiscard]] virtual std::string description() const = 0;

  /**
   *
   * @return
   */
  [[nodiscard]] virtual std::string name() const = 0;

  /**
   *
   * @return
   */
  [[nodiscard]] virtual int width() const = 0;

  /**
   *
   * @param width
   */
  void setWidth(const int width) { setSize(width, height()); }

  /**
   *
   * @return
   */
  [[nodiscard]] virtual int height() const = 0;

  /**
   *
   * @param height
   */
  void setHeight(const int height) { setSize(width(), height); }

  /**
   *
   * @return
   */
  [[nodiscard]] virtual const Size& size() const = 0;

  /**
   *
   * @param size
   */
  virtual void setSize(const Size& size) = 0;

  /**
   *
   * @param width
   * @param height
   */
  void setSize(int width, int height) { setSize({width, height}); }
};

template <class ToolClass>
class ITypedMapTool : public IMapTool {
public:
  static constexpr std::string_view ToolIdentifer() { return type_name<ToolClass>(); }
  [[nodiscard]] constexpr std::string_view identifier() const override { return ToolIdentifer(); }
};

class PenMapTool final : public ITypedMapTool<PenMapTool> {
  /**
   * The Pen map tool is always 1x1 tiles in size, regardless of whether the tiles it edits are AutoTiles or not, so we can hard code it here for convenience
   */
  static constexpr Size skPenSize{1, 1};

public:
  [[nodiscard]] bool execute(int layer, const Point& cursorPoint, const Size& mapSize, std::vector<std::optional<int>>& tiles, const std::vector<std::array<int, 7>>& penData) const override {
    return true;
  }
  [[nodiscard]] int width() const override { return skPenSize.width(); }
  [[nodiscard]] int height() const override { return skPenSize.height(); }
  [[nodiscard]] const Size& size() const override { return skPenSize; }
  void setSize(const Size& size) override {}

  [[nodiscard]] std::string description() const override { return trNOOP("Modifies map tiles on a tile by tile basis, AutoTiles will automatically update their neighboring tiles."); }
  [[nodiscard]] std::string name() const override { return trNOOP("Pen Tool"); }
};

// class ScriptedMapTool final : IMapTool {
// public:
//
// };