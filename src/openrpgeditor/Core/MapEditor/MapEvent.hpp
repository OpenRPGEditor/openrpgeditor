#pragma once

#include "Core/Graphics/CharacterSheet.hpp"
#include "Database/Event.hpp"

struct MapEditor;

class MapEvent final : public IEventRenderer {
public:
  MapEvent() = delete;
  void draw(float mapScale, bool isHovered, bool selected, bool halfAlpha, bool updateOnly) override;
  void setupPageSettings() {
    if (page()) {
      m_characterSheet = CharacterSheet(page()->image().characterName(), page()->image().tileId() > 0, page()->image().tileId());
      if (m_originalDirection != page()->image().direction()) {
        m_originalDirection = page()->image().direction();
        m_prelockDirection = Direction::Retain;
        setDirectionFix(false);
        setDirection(page()->image().direction());
      }
      if (m_originalPattern != page()->image().pattern()) {
        m_originalPattern = page()->image().pattern();
        setPattern(page()->image().pattern());
      }
      setMoveFrequency(static_cast<int>(page()->moveFrequency()));
      m_originalMoveFrequency = m_moveFrequency;
      setMoveSpeed(static_cast<int>(page()->moveSpeed()));
      m_originalMoveSpeed = static_cast<int>(page()->moveSpeed());
      setPriorityType(page()->priorityType());
      setThrough(page()->through());
      setWalkAnime(page()->walkAnime());
      setStepAnime(page()->stepAnime());
      setDirectionFix(page()->directionFix());
      setMoveRoute(&page()->moveRoute());
      m_moveType = page()->moveType();
      m_trigger = page()->trigger();
      page()->image().modified().connect<&MapEvent::onModified>(this);
      page()->modified().connect<&MapEvent::onModified>(this);
    }
  }
  void setPage(const int idx) override {
    if (idx == m_page) {
      return;
    }

    if (page()) {
      page()->image().modified().disconnect<&MapEvent::onModified>(this);
      page()->modified().disconnect<&MapEvent::onModified>(this);
    }
    m_page = idx;
    setupPageSettings();
  }

  void setMapEditor(MapEditor* mapEditor);

  bool through() const { return m_through; }
  void setThrough(const bool through) { m_through = through; }
  bool isDashing() const { return m_isDashing; }
  void setDashing(const bool dashing) { m_isDashing = dashing; }
  bool transparent() const { return m_transparent; }
  void setTransparent(const bool transparent) { m_transparent = transparent; }
  bool isMoving() const { return m_x != m_realX || m_y != m_realY; }
  bool directionFix() const { return m_directionFix; }
  void setDirectionFix(const bool fix) { m_directionFix = fix; }
  bool hasWalkAnime() const { return m_walkAnime; }
  void setWalkAnime(const bool walkAnime) { m_walkAnime = walkAnime; }
  bool hasStepAnime() const { return m_stepAnime; }
  void setStepAnime(const bool step) { m_stepAnime = step; }
  bool isOriginalPattern() const { return m_pattern == m_originalPattern; }
  void setPattern(int pattern) { m_pattern = pattern; }
  int pattern() const { return m_pattern < 3 ? m_pattern : 1; }
  int opacity() const { return m_opacity; }
  void setOpacity(const int opacity) { m_opacity = opacity; }
  Blend blendMode() const { return m_blendMode; }
  void setBlendMode(const Blend blend) { m_blendMode = blend; }
  void setPriorityType(EventPriority priority) { m_priorityType = priority; }

  static constexpr int maxPattern() { return 4; }

  EventPage* page() const override { return m_event->page(m_page); }
  int pageIndex() const override { return m_page; }

  double x() const override { return m_realX; }
  double y() const override { return m_realY; }

  int screenZ() const override { return static_cast<int>(page()->priorityType()) * 2 + 1; }
  uint32_t spriteId() const override { return m_spriteId; }

  void increaseSteps() {
    // if (isOnLadder()) {
    //   setDirection(Direction::Up);
    // }

    resetStopCount();
    // refreshBushDepth();
  }

  void memorizeMoveRoute() {
    m_originalMoveRoute = m_moveRoute;
    m_originalMoveRouteIndex = m_moveRouteIndex;
  }

  bool isMoveRouteBruteForcing() const { return m_moveRouteForcing; }

  void setMoveRoute(MovementRoute* moveRoute) {
    m_moveRoute = moveRoute;
    m_moveRouteIndex = 0;
    m_moveRouteForcing = false;
  }

  void forceMoveRoute(MovementRoute* moveRoute) {
    if (!m_originalMoveRoute) {
      memorizeMoveRoute();
    }
    m_moveRoute = moveRoute;
    m_moveRouteIndex = 0;
    m_moveRouteForcing = true;
    m_waitCount = 0;
  }

  void updateJump();
  void updateMoveRoute() {
    if (m_waitCount > 0) {
      m_waitCount--;
    } else {
      setMovementSuccess(true);
      if (const auto cmd = m_moveRouteIndex < m_moveRoute->list().size() ? m_moveRoute->list()[m_moveRouteIndex] : nullptr) {
        processMoveCommand(cmd);
        advanceMoveRouteIndex();
      }
    }
  }

  void advanceMoveRouteIndex() {
    if (m_moveRoute && (isMovementSucceeded() || m_moveRoute->skippable())) {
      const auto numCommands = m_moveRoute->list().size() - 1;
      m_moveRouteIndex++;
      if (m_moveRoute->repeat() && m_moveRouteIndex >= numCommands) {
        m_moveRouteIndex = 0;
      }
    }
  }

  double jumpHeight() { return (m_jumpPeak * m_jumpPeak - std::pow(std::abs(m_jumpCount - m_jumpPeak), 2)) / 2; }

protected:
  friend class IEventRenderer* IEventRenderer::create(Event* ev);
  explicit MapEvent(Event* event);
  void eventPointerInvalidated() override;

  void updateAnimation();
  void updateAnimationCount();
  void updatePattern();
  float animationWait() const;
  float realMoveSpeed() const;
  void resetPattern() { setPattern(1); }

  void updateSelfMove();
  void moveTypeRandom();
  void moveTypeTowardPlayer();
  void moveTypeCustom();
  void resetStopCount() { m_stopCount = 0; }

  void moveRandom();
  void moveForward();
  void moveBackward();
  void moveStraight(Direction d);
  void moveDiagonally(Direction horz, Direction vert);
  void moveTowardPlayer();
  void moveAwayFromPlayer();
  void jump(double x, double y);

  void turnRight90();
  void turnLeft90();
  void turn180();
  void turnLeftOrRight90();
  void turnRandom();
  void moveTowardCharacter(double x, double y);
  void moveAwayFromCharacter(double x, double y);
  void turnTowardCharacter(double x, double y);
  void turnTowardPlayer();
  void turnAwayFromCharacter(double x, double y);
  void turnAwayFromPlayer();

  bool isNearThePlayer() const;

  void setMoveSpeed(double speed) { m_moveSpeed = speed; }
  void setMoveFrequency(double freq) { m_moveFrequency = freq; }

  bool canPass(double x, double y, Direction d);
  bool canPassDiagonally(double x, double y, Direction horz, Direction vert);

  bool isThrough() const override { return m_through; }
  bool isMapPassable(double x, double y, Direction d) const;

  bool isCollidedWithEvents(double x, double y);
  bool isCollidedWithCharacters(double x, double y);
  static Direction reverseDir(Direction d);
  void setMovementSuccess(const bool success) { m_movementSuccess = success; }
  bool isMovementSucceeded() const { return m_movementSuccess; }
  void setDirection(const Direction d) {
    if (!m_directionFix && d != Direction::Retain) {
      m_direction = d;
    }
    resetStopCount();
  }

  bool checkStop(const int threshold) const { return m_stopCount > threshold; }

  double stopCountThreshold() const { return 30 * (5 - moveFrequency()); }

  int moveFrequency() const { return m_moveFrequency; }

  bool isJumping() const { return m_jumpCount > 0; }
  bool isStopping() const { return !isMoving() && !isJumping(); }

  void updateStop() {
    m_stopCount++;

    updateSelfMove();
  }

  void updateMove();

  double distancePerFrame() const { return pow(2, realMoveSpeed()) / 256; }

  void processMoveCommand(const std::shared_ptr<IEventCommand>& command);
  void processRouteEnd();

  double deltaXFrom(double x) const;
  double deltaYFrom(double y) const;
  void restoreMoveRoute();
  void straighten();

  bool isNearTheScreen();

  double scrolledX() const;
  double scrolledY() const;

  void clearPageSettings();

private:
  MapEditor* m_mapEditor = nullptr;
  CharacterSheet m_characterSheet;
  uint32_t m_spriteId = mSpriteId++;
  int m_page{0};
  double m_x{0};
  double m_y{0};
  double m_realX{0};
  double m_realY{0};
  bool m_isDashing{false};
  double m_moveSpeed{4};
  double m_originalMoveSpeed{4};
  double m_moveFrequency{6};
  double m_originalMoveFrequency{6};
  int m_pattern{1};
  int m_originalPattern{1};
  float m_animationCount{0};
  int m_stopCount{0};
  bool m_through{false};
  bool m_movementSuccess{false};
  int m_waitCount{0};
  Direction m_direction{Direction::Retain};
  Direction m_originalDirection{Direction::Retain};
  Direction m_prelockDirection{Direction::Retain};
  static uint32_t mSpriteId;
  bool m_walkAnime{false};
  bool m_stepAnime{false};
  bool m_directionFix{false};
  bool m_transparent{false};
  int m_opacity{0};
  Blend m_blendMode{Blend::Normal};
  MoveType m_moveType;
  EventPriority m_priorityType{EventPriority::Same_as_characters};
  EventTriggerType m_trigger{EventTriggerType::Action_Button};
  MovementRoute* m_moveRoute = nullptr;
  int m_moveRouteIndex = -1;
  bool m_moveRouteForcing{false};

  MovementRoute* m_originalMoveRoute = nullptr;
  int m_originalMoveRouteIndex = -1;

  int m_jumpPeak = 0;
  int m_jumpCount = 0;
  bool m_prisonMode{false};

  float m_lastFrameTime = 0.f;
  void onModified(IModifiable*) { setupPageSettings(); }
};
