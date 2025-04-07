#include "Core/MapEditor/MapEvent.hpp"

#include "Core/MainWindow.hpp"
#include "Core/MapEditor.hpp"
#include "Core/ResourceManager.hpp"

#include "Core/Graphics/Texture.hpp"
#include "Utils/JavaMath.hpp"

#include "Database/EventCommands/MovementRoute/BlendMode.hpp"
#include "Database/EventCommands/MovementRoute/ChangeOpacity.hpp"
#include "Database/EventCommands/MovementRoute/Jump.hpp"
#include "Database/EventCommands/MovementRoute/PlaySE.hpp"
#include "Database/EventCommands/MovementRoute/Speed.hpp"
#include "Database/EventCommands/MovementRoute/Wait.hpp"
#include "imgui.h"
#include "imgui_internal.h"

#define _USE_MATH_DEFINES
#include "Core/ImGuiExt/ImGuiUtils.hpp"

#include <math.h>

uint32_t MapEvent::mSpriteId = 0;

MapEvent::MapEvent(Event* event)
: IEventRenderer(event) {
  setupPageSettings();
  m_x = m_realX = m_event->x();
  m_y = m_realY = m_event->y();
  m_event->onModified().connect<&MapEvent::onModified>(this);
}

double oscillate(const double minValue, const double maxValue, const double period, const double currentTime) {
  const double rangeDelta = maxValue - minValue;
  const double phase = (sin(2 * M_PI * currentTime / period) + 1) / 2;
  return minValue + phase * rangeDelta;
}

void MapEvent::draw(const float mapScale, const bool isHovered, const bool selected, const bool halfAlpha, const bool updateOnly) {
  if (!m_mapEditor) {
    return;
  }
  if ((!page()->walkAnime() && page()->moveType() != MoveType::Custom) || m_mapEditor->prisonMode()) {
    m_x = m_realX = m_event->x();
    m_y = m_realY = m_event->y();
    m_direction = page()->image().direction();
  }

  if (!m_mapEditor->prisonMode() && m_event->id() != 0) {
    m_lastFrameTime += ImGui::GetIO().DeltaTime;
    /* Only process event logic once every frame at 60FPS */
    if (m_lastFrameTime >= (1.f / 60.f)) {
      if (isStopping()) {
        updateStop();
      }
      /*
      if (isJumping()) {
        updateJump();
      }
      */
      if (isMoving()) {
        updateMove();
      }
      updateAnimation();
      m_lastFrameTime = 0.f;
    }
  }

  if (updateOnly) {
    return;
  }
  const bool hasCharacterSheet = !page()->image().characterName().empty();
  const bool isTile = page()->image().tileId() > 0;
  const int characterIndex = page()->image().characterIndex();
  const Direction direction = m_direction;
  const ImGuiWindow* win = ImGui::GetCurrentWindow();
  constexpr ImU32 NormalOutlineCol = 0xFF000000;
  constexpr ImU32 NormalBorderCol = 0xFFFFFFFF;

  ImU32 outlineCol = NormalOutlineCol;
  ImU32 borderCol = NormalBorderCol;

  ImU32 bgColor = 0x7f000000;
  ImU32 imageColor = 0xFFFFFFFF;
  if (selected) {
    const ImU8 r = static_cast<ImU8>(oscillate(0.5, 1.0, 2.0, ImGui::GetTime()) * 255.f);
    bgColor &= 0xFFFFFF00;
    bgColor |= r;
    imageColor = (0xFF << 24) | (r << 16) | (r << 8) | (r << 0);
  }

  if (halfAlpha) {
    outlineCol &= 0x00FFFFFF;
    outlineCol |= (127 << 24);
    borderCol &= 0x00FFFFFF;
    borderCol |= (127 << 24);
    bgColor &= 0x00FFFFFF;
    bgColor |= (63 << 24);
    imageColor &= 0x00FFFFFF;
    imageColor |= (127 << 24);
  }

  auto eventX = static_cast<float>(m_realX * m_mapEditor->tileSize()) * mapScale;
  auto eventY = static_cast<float>(m_realY * m_mapEditor->tileSize()) * mapScale;
  eventX += win->ContentRegionRect.Min.x;
  eventY += win->ContentRegionRect.Min.y;
  const auto eventS = m_mapEditor->tileSize() * mapScale;
  auto evMin = ImVec2{eventX, eventY};
  auto evMax = ImVec2{(eventX + eventS), (eventY + eventS)};

  const auto outerThickness = ImGui::GetDPIScaledValue(2.0f);
  const auto innerThickness = ImGui::GetDPIScaledValue(1.5f);
  const auto contraction = ImGui::GetDPIScaledSize(4, 4);

  if (m_mapEditor->prisonMode() || m_event->id() == 0 || (((!hasCharacterSheet || !m_characterSheet) && !isTile) && !m_mapEditor->prisonMode())) {
    win->DrawList->AddRectFilled(evMin + contraction, evMax - contraction, bgColor);
  }

  if (hasCharacterSheet && !isTile && m_characterSheet) {
    if (!m_mapEditor->prisonMode() && m_event->id() != 0) {
      auto [min, max] = m_characterSheet.getRectForCharacter(characterIndex, pattern(), direction);

      evMin.x -= ((static_cast<float>(m_characterSheet.characterWidth()) - m_mapEditor->tileSize()) / 2.f) * mapScale;
      evMax.x += ((static_cast<float>(m_characterSheet.characterWidth()) - m_mapEditor->tileSize()) / 2.f) * mapScale;
      evMin.y -= (static_cast<float>(m_characterSheet.characterHeight()) - m_mapEditor->tileSize()) * mapScale;
      win->DrawList->AddImage(m_characterSheet.texture(), evMin, evMax, min, max, imageColor);
    } else {
      auto [min, max] = m_characterSheet.getRectForCharacter(characterIndex, m_originalPattern, direction);

      if (m_characterSheet.characterWidth() == 72) {
        min.xr() *= m_characterSheet.texture().width();
        max.xr() *= m_characterSheet.texture().width();
        min.xr() += 16;
        max.xr() -= 16;
        min.xr() /= m_characterSheet.texture().width();
        max.xr() /= m_characterSheet.texture().width();
      }

      if (m_characterSheet.characterHeight() == 96) {
        min.yr() *= m_characterSheet.texture().height();
        max.yr() *= m_characterSheet.texture().height();
        min.yr() += 24;
        max.yr() -= 32;
        min.yr() /= m_characterSheet.texture().height();
        max.yr() /= m_characterSheet.texture().height();
      }
      win->DrawList->AddImage(m_characterSheet, evMin + contraction, evMax - contraction, min, max, imageColor);
    }
  } else if (m_mapEditor->map() && isTile) {
    const auto& [min, max] = m_characterSheet.getRectForTile(page()->image().tileId());
    if (m_mapEditor->prisonMode()) {
      win->DrawList->AddImage(m_characterSheet, evMin + contraction, evMax - contraction, min, max, imageColor);
    } else {
      win->DrawList->AddImage(m_characterSheet, evMin, evMax, min, max, imageColor);
    }
  }
  if (m_mapEditor->prisonMode() || m_event->id() == 0 || (((!hasCharacterSheet || !m_characterSheet) && !isTile) && !m_mapEditor->prisonMode())) {
    win->DrawList->AddRect(evMin + contraction, evMax - contraction, outlineCol, 0, 0, outerThickness);
    win->DrawList->AddRect(evMin + contraction, evMax - contraction, borderCol, 0, 0, innerThickness);
  }
}

void MapEvent::setMapEditor(MapEditor* mapEditor) {
  m_mapEditor = mapEditor;
  m_prisonMode = !m_mapEditor || m_mapEditor->prisonMode();
}

void MapEvent::updateAnimation() {
  updateAnimationCount();
  if (m_animationCount >= animationWait()) {
    updatePattern();
    m_animationCount = 0;
  }
}

void MapEvent::updateAnimationCount() {
  if (isMoving() && hasWalkAnime()) {
    m_animationCount += 1.5f;
  } else if (hasStepAnime() || !isOriginalPattern()) {
    m_animationCount += 1.f;
  }
}

void MapEvent::updatePattern() {
  if (!hasStepAnime() && m_stopCount > 0) {
    resetPattern();
  } else {
    m_pattern = (m_pattern + 1) % maxPattern();
  }
}

float MapEvent::animationWait() const { return (9 - realMoveSpeed()) * 3; }

float MapEvent::realMoveSpeed() const { return m_moveSpeed + (isDashing() ? 1 : 0); }

bool MapEvent::isNearThePlayer() const {
  double sx = std::abs(deltaXFrom(m_mapEditor->tileCellX()));
  double sy = std::abs(deltaYFrom(m_mapEditor->tileCellY()));

  return (sx + sy) < 20;
}

double MapEvent::deltaXFrom(double x) const { return m_mapEditor->deltaX(m_x, x); }
double MapEvent::deltaYFrom(double y) const { return m_mapEditor->deltaY(m_y, y); }

void MapEvent::updateSelfMove() {
  if (/*!m_locked &&*/ isNearTheScreen() && checkStop(stopCountThreshold())) {
    switch (m_moveType) {
    case MoveType::Random:
      moveTypeRandom();
      break;
    case MoveType::Approach:
      moveTypeTowardPlayer();
      break;
    case MoveType::Custom:
      moveTypeCustom();
      break;
    default:
      break;
    }
  }
}

void MapEvent::moveTypeRandom() {
  switch (Math::randomInt(6)) {
  case 0:
  case 1:
    moveRandom();
    break;
  case 2:
  case 3:
  case 4:
    moveForward();
    break;
  case 5:
    resetStopCount();
    break;
  default:
    break;
  }
}

void MapEvent::moveTypeTowardPlayer() {
  if (isNearThePlayer()) {
    switch (Math::randomInt(6)) {
    case 0:
    case 1:
    case 2:
    case 3:
      moveTowardPlayer();
      break;
    case 4:
      moveRandom();
      break;
    case 5:
      moveForward();
      break;
    default:
      break;
    }
  } else {
    moveRandom();
  }
}

void MapEvent::moveTypeCustom() { updateMoveRoute(); }

void MapEvent::moveRandom() {
  if (const auto d = static_cast<Direction>(2 + Math::randomInt(4) * 2); canPass(m_x, m_y, d)) {
    moveStraight(d);
  }
}

void MapEvent::moveForward() { moveStraight(m_direction); }

void MapEvent::moveBackward() {
  const auto lastDirectionFix = directionFix();
  setDirectionFix(true);
  moveStraight(reverseDir(m_direction));
  setDirectionFix(lastDirectionFix);
}

void MapEvent::moveStraight(const Direction d) {
  setMovementSuccess(canPass(m_x, m_y, d));
  if (isMovementSucceeded()) {
    setDirection(d);
    m_x = m_mapEditor->roundXWithDirection(m_x, d);
    m_y = m_mapEditor->roundYWithDirection(m_y, d);
    m_realX = m_mapEditor->xWithDirection(m_x, reverseDir(d));
    m_realY = m_mapEditor->yWithDirection(m_y, reverseDir(d));
    increaseSteps();
  } else {
    setDirection(d);
    //  checkEventTriggerTouchFront(d);
  }
}

void MapEvent::moveDiagonally(const Direction horz, const Direction vert) {
  setMovementSuccess(canPassDiagonally(m_x, m_y, horz, vert));
  if (isMovementSucceeded()) {
    m_x = m_mapEditor->roundXWithDirection(m_x, horz);
    m_y = m_mapEditor->roundYWithDirection(m_y, vert);
    m_realX = m_mapEditor->xWithDirection(m_x, reverseDir(horz));
    m_realY = m_mapEditor->yWithDirection(m_y, reverseDir(vert));
    increaseSteps();
  }

  if (m_direction == reverseDir(horz)) {
    setDirection(horz);
  }
  if (m_direction == reverseDir(vert)) {
    setDirection(vert);
  }
}

void MapEvent::moveTowardCharacter(const double x, const double y) {
  const double sx = deltaXFrom(x);
  const double sy = deltaYFrom(y);

  if (std::abs(sx) > std::abs(sy)) {
    moveStraight(sx > 0 ? Direction::Right : Direction::Left);
    if (!isMovementSucceeded() && sy != 0) {
      moveStraight(sy > 0 ? Direction::Down : Direction::Up);
    }
  } else if (sy != 0) {
    moveStraight(sy > 0 ? Direction::Down : Direction::Up);
    if (!isMovementSucceeded() && sx != 0) {
      moveStraight(sx > 0 ? Direction::Right : Direction::Left);
    }
  }
}

void MapEvent::moveAwayFromCharacter(const double x, const double y) {
  const double sx = deltaXFrom(x);
  const double sy = deltaYFrom(y);

  if (std::abs(sx) > std::abs(sy)) {
    moveStraight(sx > 0 ? Direction::Left : Direction::Right);
    if (!isMovementSucceeded() && sy != 0) {
      moveStraight(sy > 0 ? Direction::Up : Direction::Down);
    }
  } else if (sy != 0) {
    moveStraight(sy > 0 ? Direction::Up : Direction::Down);
    if (!isMovementSucceeded() && sx != 0) {
      moveStraight(sx > 0 ? Direction::Left : Direction::Right);
    }
  }
}

void MapEvent::moveTowardPlayer() { moveTowardCharacter(m_mapEditor->tileCellX(), m_mapEditor->tileCellY()); }

void MapEvent::moveAwayFromPlayer() { moveAwayFromCharacter(m_mapEditor->tileCellX(), m_mapEditor->tileCellY()); }

void MapEvent::jump(const double x, const double y) {
  if (std::abs(x) > std::abs(y)) {
    if (x != 0) {
      setDirection(x < 0 ? Direction::Left : Direction::Right);
    }
  } else if (y != 0) {
    setDirection(y < 0 ? Direction::Up : Direction::Down);
  }

  m_x += x;
  m_y += y;
  m_jumpPeak = std::round(std::sqrt(x * x + y * y));
  m_jumpCount = m_jumpPeak * 2;

  resetStopCount();
  straighten();
}

void MapEvent::straighten() {
  if (hasWalkAnime() || hasStepAnime()) {
    m_pattern = 1;
  }
  m_animationCount = 0;
}

void MapEvent::turnRight90() {
  switch (m_direction) {
  case Direction::Down:
    setDirection(Direction::Left);
    break;
  case Direction::Left:
    setDirection(Direction::Up);
    break;
  case Direction::Right:
    setDirection(Direction::Down);
    break;
  case Direction::Up:
    setDirection(Direction::Right);
    break;
  default:
    break;
  }
}

void MapEvent::turnLeft90() {
  switch (m_direction) {
  case Direction::Down:
    setDirection(Direction::Right);
    break;
  case Direction::Left:
    setDirection(Direction::Down);
    break;
  case Direction::Right:
    setDirection(Direction::Up);
    break;
  case Direction::Up:
    setDirection(Direction::Left);
    break;
  default:
    break;
  }
}

void MapEvent::turn180() { setDirection(reverseDir(m_direction)); }

void MapEvent::turnLeftOrRight90() {
  switch (Math::randomInt(2)) {
  case 0:
    turnRight90();
    break;
  case 1:
    turnLeft90();
    break;
  default:
    break;
  }
}

void MapEvent::turnRandom() { setDirection(static_cast<Direction>(2 + Math::randomInt(4) * 2)); }

void MapEvent::turnTowardCharacter(const double x, const double y) {
  double sx = deltaXFrom(x);
  double sy = deltaYFrom(y);
  if (std::abs(sx) > std::abs(sy)) {
    setDirection(sx > 0 ? Direction::Right : Direction::Left);
  } else if (sy != 0) {
    setDirection(sy > 0 ? Direction::Down : Direction::Up);
  }
}

void MapEvent::turnTowardPlayer() { turnTowardCharacter(m_mapEditor->tileCellX(), m_mapEditor->tileCellY()); }

void MapEvent::turnAwayFromCharacter(const double x, const double y) {
  const double sx = deltaXFrom(x);
  const double sy = deltaYFrom(y);
  if (std::abs(sx) > std::abs(sy)) {
    setDirection(sx > 0 ? Direction::Left : Direction::Right);
  } else if (sy != 0) {
    setDirection(sy > 0 ? Direction::Up : Direction::Down);
  }
}

void MapEvent::turnAwayFromPlayer() { turnAwayFromCharacter(m_mapEditor->tileCellX(), m_mapEditor->tileCellY()); }

void MapEvent::restoreMoveRoute() {
  m_moveRoute = m_originalMoveRoute;
  m_moveRouteIndex = m_originalMoveRouteIndex;
  m_originalMoveRoute = nullptr;
}

bool MapEvent::canPass(const double x, const double y, const Direction d) {
  const double x2 = m_mapEditor->roundXWithDirection(x, d);
  const double y2 = m_mapEditor->roundYWithDirection(y, d);

  if (!m_mapEditor->isValid(x2, y2)) {
    return false;
  }

  if (isThrough() /* || isDebugThrough() */) {
    return true;
  }

  if (!isMapPassable(x, y, d)) {
    return false;
  }

  return !isCollidedWithCharacters(x2, y2);
}

bool MapEvent::canPassDiagonally(const double x, const double y, const Direction horz, const Direction vert) {
  const double x2 = m_mapEditor->roundXWithDirection(x, horz);
  const double y2 = m_mapEditor->roundYWithDirection(y, vert);

  if (canPass(x, y, vert) && canPass(x, y2, horz)) {
    return true;
  }

  if (canPass(x, y, horz) && canPass(x2, y, vert)) {
    return true;
  }

  return false;
}

void MapEvent::updateMove() {
  if (m_event->id() == 21) {
    // printf("BEFORE %s %i %.03f %.03f %.03f %.03f %s\n", m_event->name().c_str(), m_event->id(), m_x, m_y, m_realX, m_realY, magic_enum::enum_name(m_direction).data());
  }
  if (m_x < m_realX) {
    m_realX = std::max(m_realX - distancePerFrame(), m_x);
  }

  if (m_x > m_realX) {
    m_realX = std::min(m_realX + distancePerFrame(), m_x);
  }

  if (m_y < m_realY) {
    m_realY = std::max(m_realY - distancePerFrame(), m_y);
  }

  if (m_y > m_realY) {
    m_realY = std::min(m_realY + distancePerFrame(), m_y);
  }

  if (m_event->id() == 21) {
    // printf("AFTER  %s %i %.03f %.03f %.03f %.03f %s\n", m_event->name().c_str(), m_event->id(), m_x, m_y, m_realX, m_realY, magic_enum::enum_name(m_direction).data());
  }
  if (isMoving()) {
    // refreshBushDepth();
  }
}

void MapEvent::processMoveCommand(const std::shared_ptr<IEventCommand>& command) {
  switch (command->code()) {
  case EventCode::Event_Dummy:
    processRouteEnd();
    break;
  case EventCode::Move_Down:
    moveStraight(Direction::Down);
    break;
  case EventCode::Move_Left:
    moveStraight(Direction::Left);
    break;
  case EventCode::Move_Right:
    moveStraight(Direction::Right);
    break;
  case EventCode::Move_Up:
    moveStraight(Direction::Up);
    break;
  case EventCode::Move_Lower_Left:
    moveDiagonally(Direction::Left, Direction::Down);
    break;
  case EventCode::Move_Lower_Right:
    moveDiagonally(Direction::Right, Direction::Down);
    break;
  case EventCode::Move_Upper_Left:
    moveDiagonally(Direction::Left, Direction::Up);
    break;
  case EventCode::Move_Upper_Right:
    moveDiagonally(Direction::Right, Direction::Up);
    break;
  case EventCode::Move_at_Random:
    moveRandom();
    break;
  case EventCode::Move_toward_Player:
    moveTowardPlayer();
    break;
  case EventCode::Move_away_from_Player:
    moveAwayFromPlayer();
    break;
  case EventCode::_1_Step_Forward:
    moveForward();
    break;
  case EventCode::_1_Step_Backward:
    moveBackward();
    break;
  case EventCode::Jump: {
    const auto jumpCmd = std::dynamic_pointer_cast<MovementJumpCommand>(command);
    jump(jumpCmd->x, jumpCmd->y);
  } break;
  case EventCode::Wait_del_Movement: {
    const auto waitCmd = std::dynamic_pointer_cast<MovementWaitCommand>(command);
    m_waitCount = waitCmd->duration - 1;
  } break;
  case EventCode::Turn_Down:
    setDirection(Direction::Down);
    break;
  case EventCode::Turn_Left:
    setDirection(Direction::Left);
    break;
  case EventCode::Turn_Right:
    setDirection(Direction::Right);
    break;
  case EventCode::Turn_Up:
    setDirection(Direction::Up);
    break;
  case EventCode::Turn_90_deg_Right:
    turnRight90();
    break;
  case EventCode::Turn_90_deg_Left:
    turnLeft90();
    break;
  case EventCode::Turn_180_deg:
    turn180();
    break;
  case EventCode::Turn_90_deg_Left_or_Right:
    turnLeftOrRight90();
    break;
  case EventCode::Turn_at_Random:
    turnRandom();
    break;
  case EventCode::Turn_toward_Player:
    turnTowardPlayer();
    break;
  case EventCode::Turn_away_from_Player:
    turnAwayFromPlayer();
    break;
  case EventCode::Switch_ON:
    // TODO: Event/Variable state?
    break;
  case EventCode::Switch_OFF:
    break;
  case EventCode::Speed: {
    const auto speedCmd = std::dynamic_pointer_cast<MovementSpeedCommand>(command);
    setMoveSpeed(speedCmd->speed);
  } break;
  case EventCode::Frequency: {
    const auto freqCmd = std::dynamic_pointer_cast<MovementFrequencyCommand>(command);
    setMoveFrequency(freqCmd->frequency);
  } break;
  case EventCode::Walking_Animation_ON:
    setWalkAnime(true);
    break;
  case EventCode::Walking_Animation_OFF:
    setWalkAnime(false);
    break;
  case EventCode::Stepping_Animation_ON:
    setStepAnime(true);
    break;
  case EventCode::Stepping_Animation_OFF:
    setStepAnime(false);
    break;
  case EventCode::Direction_Fix_ON:
    setDirectionFix(true);
    break;
  case EventCode::Direction_Fix_OFF:
    setDirectionFix(false);
    break;
  case EventCode::Through_ON:
    setThrough(true);
    break;
  case EventCode::Through_OFF:
    setThrough(false);
    break;
  case EventCode::Transparent_ON:
    setTransparent(true);
    break;
  case EventCode::Transparent_OFF:
    setTransparent(false);
    break;
  case EventCode::Change_Opacity: {
    const auto opacityCmd = std::dynamic_pointer_cast<MovementChangeOpacityCommand>(command);
    setOpacity(opacityCmd->opacity);
  } break;
  case EventCode::Change_Blend_Mode: {
    const auto blendModeCmd = std::dynamic_pointer_cast<MovementChangeBlendModeCommand>(command);
    setBlendMode(blendModeCmd->mode);
  } break;
  case EventCode::Play_SE_del_Movement:
    // const auto seCmd = std::dynamic_pointer_cast<MovementPlaySECommand>(command);
    //  TODO: SE audio
    break;
  case EventCode::Script_del_Movement:
    // TODO: How to handle being unable to run Javascript?
    break;
  default:
    break;
  }
}

void MapEvent::processRouteEnd() {
  if (m_moveRoute->repeat()) {
    m_moveRouteIndex = 0;
  } else if (m_moveRouteForcing) {
    m_moveRouteForcing = false;
    restoreMoveRoute();
  }
}

bool MapEvent::isCollidedWithEvents(double x, double y) {
  auto events = m_mapEditor->eventsAtNoThrough(x, y);
  return std::ranges::any_of(events, [&](auto& e) { return e != m_event && e->renderer()->page()->priorityType() == EventPriority::Same_as_characters; });
}

bool MapEvent::isCollidedWithCharacters(const double x, const double y) { return isCollidedWithEvents(x, y) /*|| isCollidedWithVehicles(x, y)*/; }

Direction MapEvent::reverseDir(const Direction d) {
  switch (d) {
  case Direction::Up:
    return Direction::Down;
  case Direction::Down:
    return Direction::Up;
  case Direction::Left:
    return Direction::Right;
  case Direction::Right:
    return Direction::Left;
  default:
    return d;
  }
}

bool MapEvent::isMapPassable(const double x, const double y, const Direction d) const {
  const double x2 = m_mapEditor->roundXWithDirection(x, d);
  const double y2 = m_mapEditor->roundYWithDirection(y, d);
  const Direction d2 = reverseDir(d);
  return m_mapEditor->isPassable(x, y, d) && m_mapEditor->isPassable(x2, y2, d2);
}

void MapEvent::clearPageSettings() {
  m_moveType = MoveType::Fixed;
  m_trigger = EventTriggerType::Action_Button;
  setThrough(true);
}

void MapEvent::eventPointerInvalidated() {
  clearPageSettings();
  const auto page = m_page;
  setPage(-1);
  setPage(page);
}

IEventRenderer* IEventRenderer::create(Event* ev) { return new MapEvent(ev); }

double MapEvent::scrolledX() const { return m_mapEditor->adjustX(m_realX); }
double MapEvent::scrolledY() const { return m_mapEditor->adjustY(m_realY); }

bool MapEvent::isNearTheScreen() {
  double gw = ImGui::GetCurrentWindow()->ClipRect.Max.x;
  double gh = ImGui::GetCurrentWindow()->ClipRect.Max.y;
  double tw = m_mapEditor->tileSize();
  double th = m_mapEditor->tileSize();

  double px = scrolledX() * tw + tw / 2 - gw / 2;
  double py = scrolledY() * th + th / 2 - gh / 2;

  return px >= -gw && px <= gw && py >= -gh && py <= gh;
}
