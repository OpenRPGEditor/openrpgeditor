#pragma once
#include "Editor/Graphics/Texture.hpp"

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
   * @param layer Current layer to modify, -1 means all layers
   * @param penData
   * @return Tiles to modify in map,
   */
  [[nodiscard]] virtual std::vector<int> execute(int layer, const std::vector<std::array<int, 4>>& penData) const = 0;
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
   * Optional icon
   * @return \sa Texture The icon to display in the tools menu
   */
  [[nodiscard]] virtual Texture icon() const { return {}; }
  virtual std::string fontIcon() const { return {}; }

  virtual void resetDefaults() = 0;

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

  virtual void onLeftClickStart() {};
  virtual void onLeftClickDrag(int deltaX, int deltaY) {}
  virtual void onLeftClickExit() {}

  virtual void onRightClickStart() {};
  virtual void onRightClickDrag(int deltaX, int deltaY) {}
  virtual void onRightClickExit() {}

  void setPalettePenSize(const Size& size) { m_palettePenSize = size; }

protected:
  Size m_palettePenSize;
};

template <class ToolClass>
class ITypedMapTool : public IMapTool {
public:
  static constexpr std::string_view ToolIdentifier() { return type_name<ToolClass>(); }
  [[nodiscard]] constexpr std::string_view identifier() const override { return ToolIdentifier(); }
};

// class ScriptedMapTool final : IMapTool {
// public:
//
// };