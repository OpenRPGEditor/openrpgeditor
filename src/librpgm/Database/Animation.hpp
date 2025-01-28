#pragma once

#include "Database/Audio.hpp"
#include "Database/Globals.hpp"
#include "IModifiable.hpp"

#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

class Animation final : public IModifiable {
  friend class AnimationsSerializer;
  friend void to_json(nlohmann::ordered_json& to, const Animation& animation);
  friend void from_json(const nlohmann::ordered_json& from, Animation& animation);

public:
  class Color final : public IModifiable {
    friend void to_json(nlohmann::ordered_json& to, const Color& color);
    friend void from_json(const nlohmann::ordered_json& from, Color& color);

  public:
    Color() = default;
    Color(const Color& other);
    Color& operator=(const Color& other);
    Color(Color&& other) noexcept;
    Color& operator=(Color&& other) noexcept;

    /**
     *
     * @return
     */
    int r() const;

    /**
     *
     * @param r
     */
    void setR(int r);

    /**
     *
     * @return
     */
    int g() const;

    /**
     *
     * @param g
     */
    void setG(int g);

    /**
     *
     * @return
     */
    int b() const;

    /**
     *
     * @param b
     */
    void setB(int b);

    /**
     *
     * @return
     */
    int intensity() const;

    /**
     *
     * @param intensity
     */
    void setIntensity(int intensity);

    // SIGNALS
    rpgmutils::signal<void(Color*, int)>& rModified();
    rpgmutils::signal<void(Color*, int)>& gModified();
    rpgmutils::signal<void(Color*, int)>& bModified();
    rpgmutils::signal<void(Color*, int)>& intensityModified();

    void restoreOriginal() override;
    void acceptChanges() override;
    nlohmann::ordered_json serializeOldValues() const override;

  private:
    int m_r{0};
    int m_g{0};
    int m_b{0};
    int m_intensity{0};

    std::optional<int> m_oldr;
    std::optional<int> m_oldg;
    std::optional<int> m_oldb;
    std::optional<int> m_oldintensity;

    std::optional<rpgmutils::signal<void(Color*, int)>> m_rModified;
    std::optional<rpgmutils::signal<void(Color*, int)>> m_gModified;
    std::optional<rpgmutils::signal<void(Color*, int)>> m_bModified;
    std::optional<rpgmutils::signal<void(Color*, int)>> m_intensityModified;
  };

  class FramePart final : public IModifiable {
    friend void to_json(nlohmann::ordered_json& to, const FramePart& frame);
    friend void from_json(const nlohmann::ordered_json& from, FramePart& frame);

  public:
    FramePart() = default;
    FramePart(const FramePart& other);
    FramePart& operator=(const FramePart& other);
    FramePart(FramePart&& other) noexcept;
    FramePart& operator=(FramePart&& other) noexcept;

    /**
     *
     * @return
     */
    int pattern() const;

    /**
     *
     * @param pattern
     */
    void setPattern(int pattern);

    /**
     *
     * @return
     */
    int x() const;

    /**
     *
     * @param x
     */
    void setX(int x);

    /**
     *
     * @return
     */
    int y() const;

    /**
     *
     * @param y
     */
    void setY(int y);

    /**
     *
     * @return
     */
    int scale() const;

    /**
     *
     * @param scale
     */
    void setScale(int scale);

    /**
     *
     * @return
     */
    int rotation() const;

    /**
     *
     * @param rotation
     */
    void setRotation(int rotation);

    /**
     *
     * @return
     */
    Mirror mirror() const;

    /**
     *
     * @param mirror
     */
    void setMirror(Mirror mirror);

    /**
     *
     * @return
     */
    int opacity() const;

    /**
     *
     * @param opacity
     */
    void setOpacity(int opacity);

    /**
     *
     * @return
     */
    Blend blend() const;

    /**
     *
     * @param blend
     */
    void setBlend(Blend blend);

    /**
     *
     * @return
     */
    bool showTimeLine() const;
    void setShowTimeLine(bool show);

    /* SIGNALS */
    rpgmutils::signal<void(FramePart*, int)>& patternModified();
    rpgmutils::signal<void(FramePart*, int)>& xModified();
    rpgmutils::signal<void(FramePart*, int)>& yModified();
    rpgmutils::signal<void(FramePart*, int)>& scaleModified();
    rpgmutils::signal<void(FramePart*, int)>& rotationModified();
    rpgmutils::signal<void(FramePart*, Mirror)>& mirrorModified();
    rpgmutils::signal<void(FramePart*, int)>& opacityModified();
    rpgmutils::signal<void(FramePart*, Blend)>& blendModified();

    void restoreOriginal() override;
    void acceptChanges() override;
    nlohmann::ordered_json serializeOldValues() const override;

  private:
    int m_pattern;
    int m_x;
    int m_y;
    int m_scale;
    int m_rotation;
    Mirror m_mirror;
    int m_opacity;
    Blend m_blend;
    bool m_show{false};

    std::optional<int> m_oldpattern;
    std::optional<int> m_oldx;
    std::optional<int> m_oldy;
    std::optional<int> m_oldscale;
    std::optional<int> m_oldrotation;
    std::optional<Mirror> m_oldmirror;
    std::optional<int> m_oldopacity;
    std::optional<Blend> m_oldblend;

    std::optional<rpgmutils::signal<void(FramePart*, int)>> m_patternModified;
    std::optional<rpgmutils::signal<void(FramePart*, int)>> m_xModified;
    std::optional<rpgmutils::signal<void(FramePart*, int)>> m_yModified;
    std::optional<rpgmutils::signal<void(FramePart*, int)>> m_scaleModified;
    std::optional<rpgmutils::signal<void(FramePart*, int)>> m_rotationModified;
    std::optional<rpgmutils::signal<void(FramePart*, Mirror)>> m_mirrorModified;
    std::optional<rpgmutils::signal<void(FramePart*, int)>> m_opacityModified;
    std::optional<rpgmutils::signal<void(FramePart*, Blend)>> m_blendModified;
  };

  class Timing final : public IModifiable {
    friend void to_json(nlohmann::ordered_json& to, const Timing& timing);
    friend void from_json(const nlohmann::ordered_json& from, Timing& timing);

  public:
    Timing() = default;
    Timing(const Timing& other);
    Timing& operator=(const Timing& other);
    Timing(Timing&& other) noexcept;
    Timing& operator=(Timing&& other) noexcept;

    Color& flashColor();
    const Color& flashColor() const;
    void setFlashColor(const Color& color);

    int flashDuration() const;
    void setFlashDuration(int duration);

    FlashScope flashScope() const;
    void setFlashScope(FlashScope scope);

    int frame() const;
    void setFrame(int frame);

    Audio* se();
    const Audio* se() const;
    void setSe(const Audio& se);

    bool showTimeLine() const;
    void setTimeLine(bool show);

    // SIGNALS
    rpgmutils::signal<void(Timing*, const Color&)>& flashColorModified();
    rpgmutils::signal<void(Timing*, int)>& flashDurationModified();
    rpgmutils::signal<void(Timing*, FlashScope)>& flashScopeModified();
    rpgmutils::signal<void(Timing*, int)>& frameModified();
    rpgmutils::signal<void(Timing*, const Audio&)>& seModified();

    void restoreOriginal() override;
    void acceptChanges() override;
    nlohmann::ordered_json serializeOldValues() const override;

    bool isModified() const override { return IModifiable::isModified() | (se() && se()->isModified()); }

  private:
    Color m_flashColor;
    int m_flashDuration{};
    FlashScope m_flashScope{};
    int m_frame{};
    std::optional<Audio> m_se{};
    bool m_show{false};

    std::optional<Color> m_oldflashColor;
    std::optional<int> m_oldflashDuration;
    std::optional<FlashScope> m_oldflashScope;
    std::optional<int> m_oldframe;
    std::optional<Audio> m_oldse;

    std::optional<rpgmutils::signal<void(Timing*, const Color&)>> m_flashColorModified;
    std::optional<rpgmutils::signal<void(Timing*, int)>> m_flashDurationModified;
    std::optional<rpgmutils::signal<void(Timing*, FlashScope)>> m_flashScopeModified;
    std::optional<rpgmutils::signal<void(Timing*, int)>> m_frameModified;
    std::optional<rpgmutils::signal<void(Timing*, const Audio&)>> m_seModified;
  };

  Animation() = default;
  Animation(const Animation& other);
  Animation& operator=(const Animation& other);
  Animation(Animation&& other) noexcept;
  Animation& operator=(Animation&& other) noexcept;

  /**
   *
   * @return
   */
  int id() const;

  /**
   *
   * @param id
   */
  void setId(int id);

  /**
   *
   * @return
   */
  int animation1Hue() const;

  /**
   *
   * @param hue
   */
  void setAnimation1Hue(int hue);

  /**
   *
   * @return
   */
  std::string_view animation1Name() const;

  /**
   *
   * @param animation1Name
   */
  void setAnimation1Name(std::string_view animation1Name);

  /**
   *
   * @return
   */
  int animation2Hue() const;

  /**
   *
   * @param hue
   */
  void setAnimation2Hue(int hue);

  /**
   *
   * @return
   */
  std::string_view animation2Name() const;

  /**
   *
   * @param animation2Name
   */
  void setAnimation2Name(std::string_view animation2Name);

  /**
   *
   * @return
   */
  std::vector<std::vector<FramePart>>& frames();
  /**
   *
   * @return
   */
  const std::vector<std::vector<FramePart>>& frames() const;

  /**
   *
   * @param frames
   */
  void setFrames(const std::vector<std::vector<FramePart>>& frames);

  /**
   *
   * @param index
   * @return
   */
  const std::vector<FramePart>& frame(int index) const;

  /**
   *
   * @param index
   * @param frame
   */
  void setFrame(int index, const std::vector<FramePart>& frame);

  /**
   *
   * @return
   */
  const std::string& name() const;

  /**
   *
   * @param name
   */
  void setName(std::string_view name);

  /**
   *
   * @return
   */
  Position position() const;

  /**
   *
   * @param position
   */
  void setPosition(Position position);

  /**
   *
   * @return
   */
  const std::vector<Timing>& timings() const;

  /**
   *
   * @param timings
   */
  void setTimings(const std::vector<Timing>& timings);

  /**
   *
   * @param index
   * @return
   */
  Timing& timing(int index);
  /**
   *
   * @param index
   * @return
   */
  const Timing& timing(int index) const;

  /**
   *
   * @param index
   * @param timing
   */
  void setTiming(int index, const Timing& timing);

  /**
   *
   *
   */
  bool showTimeLine(int index);
  /**
   *
   * @param index
   * @param show
   */
  void setTimeLine(int index, bool show);

  void setTimeLine(const std::vector<bool>& shows);

  /**
   *
   * @return
   */
  bool isValid() const { return m_isValid; }

  /**
   *
   * @param valid
   */
  void setValid(const bool valid) { m_isValid = valid; }

  // SIGNALS
  rpgmutils::signal<void(Animation*, int)>& idModified();
  rpgmutils::signal<void(Animation*, int)>& animation1HueModified();
  rpgmutils::signal<void(Animation*, std::string_view)>& animation1NameModified();
  rpgmutils::signal<void(Animation*, int)>& animation2HueModified();
  rpgmutils::signal<void(Animation*, std::string_view)>& animation2NameModified();
  rpgmutils::signal<void(Animation*, const std::vector<std::vector<FramePart>>&)>& framesModified();
  rpgmutils::signal<void(Animation*, int, const std::vector<FramePart>&)>& frameModified();
  rpgmutils::signal<void(Animation*, std::string_view)>& nameModified();
  rpgmutils::signal<void(Animation*, Position)>& positionModified();
  rpgmutils::signal<void(Animation*, const std::vector<Timing>&)>& timingsModified();
  rpgmutils::signal<void(Animation*, int, const Timing&)>& timingModified();

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  bool isModified() const override {
    return IModifiable::isModified() |
           std::ranges::any_of(m_frames, [](const std::vector<FramePart>& parts) { return std::ranges::any_of(parts, [](const FramePart& part) { return part.isModified(); }); }) |
           std::ranges::any_of(m_timings, [](const Timing& timing) { return timing.isModified(); });
  }

private:
  int m_id = 0;
  int m_animation1Hue = 0;
  std::string m_animation1Name;
  int m_animation2Hue = 0;
  std::string m_animation2Name;
  std::vector<std::vector<FramePart>> m_frames;
  std::string m_name;
  Position m_position = Position::Head;
  std::vector<Timing> m_timings;
  std::vector<bool> m_shows;

  // !!!DO NOT CHANGE CASING!!!
  std::optional<int> m_oldid;
  std::optional<int> m_oldanimation1Hue;
  std::optional<std::string> m_oldanimation1Name;
  std::optional<int> m_oldanimation2Hue;
  std::optional<std::string> m_oldanimation2Name;
  std::optional<std::vector<std::vector<FramePart>>> m_oldframes;
  std::optional<std::string> m_oldname;
  std::optional<Position> m_oldposition;
  std::optional<std::vector<Timing>> m_oldtimings;

  /* Signals */
  std::optional<rpgmutils::signal<void(Animation*, int)>> m_idModified;
  std::optional<rpgmutils::signal<void(Animation*, int)>> m_animation1HueModified;
  std::optional<rpgmutils::signal<void(Animation*, std::string_view)>> m_animation1NameModified;
  std::optional<rpgmutils::signal<void(Animation*, int)>> m_animation2HueModified;
  std::optional<rpgmutils::signal<void(Animation*, std::string_view)>> m_animation2NameModified;
  std::optional<rpgmutils::signal<void(Animation*, const std::vector<std::vector<FramePart>>&)>> m_framesModified;
  std::optional<rpgmutils::signal<void(Animation*, int, const std::vector<FramePart>&)>> m_frameModified;
  std::optional<rpgmutils::signal<void(Animation*, std::string_view)>> m_nameModified;
  std::optional<rpgmutils::signal<void(Animation*, Position)>> m_positionModified;
  std::optional<rpgmutils::signal<void(Animation*, const std::vector<Timing>&)>> m_timingsModified;
  std::optional<rpgmutils::signal<void(Animation*, int, const Timing&)>> m_timingModified;

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};

void to_json(nlohmann::ordered_json& to, const Animation::Color& color);
void from_json(const nlohmann::ordered_json& from, Animation::Color& color);
void to_json(nlohmann::ordered_json& to, const Animation::FramePart& frame);
void from_json(const nlohmann::ordered_json& from, Animation::FramePart& frame);
void to_json(nlohmann::ordered_json& to, const Animation::Timing& timing);
void from_json(const nlohmann::ordered_json& from, Animation::Timing& timing);
void to_json(nlohmann::ordered_json& to, const Animation& animation);
void from_json(const nlohmann::ordered_json& from, Animation& animation);