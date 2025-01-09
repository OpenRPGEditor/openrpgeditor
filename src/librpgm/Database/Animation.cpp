#include "Animation.hpp"

Animation::Color::Color(const Color& other)
: IModifiable(other)
, m_r(other.m_r)
, m_g(other.m_g)
, m_b(other.m_b)
, m_intensity(other.m_intensity)
, m_oldr(other.m_oldr)
, m_oldg(other.m_oldg)
, m_oldb(other.m_oldb)
, m_oldintensity(other.m_oldintensity) {}

Animation::Color& Animation::Color::operator=(const Color& other) {
  IModifiable::operator=(other);
  m_r = other.m_r;
  m_g = other.m_g;
  m_b = other.m_b;
  m_intensity = other.m_intensity;
  m_oldr = other.m_oldr;
  m_oldg = other.m_oldg;
  m_oldb = other.m_oldb;
  m_oldintensity = other.m_oldintensity;
  return *this;
}

Animation::Color::Color(Color&& other) noexcept
: IModifiable(std::move(other))
, m_r(other.m_r)
, m_g(other.m_g)
, m_b(other.m_b)
, m_intensity(other.m_intensity)
, m_oldr(other.m_oldr)
, m_oldg(other.m_oldg)
, m_oldb(other.m_oldb)
, m_oldintensity(other.m_oldintensity) {}

Animation::Color& Animation::Color::operator=(Color&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_r = other.m_r;
  m_g = other.m_g;
  m_b = other.m_b;
  m_intensity = other.m_intensity;
  m_oldr = other.m_oldr;
  m_oldg = other.m_oldg;
  m_oldb = other.m_oldb;
  m_oldintensity = other.m_oldintensity;
  return *this;
}

int Animation::Color::r() const { return m_r; }

void Animation::Color::setR(const int r) {
  MODIFIABLE_SET_OLD_VALUE(r);
  m_r = r;
  if (!signalsDisabled()) {
    rModified().fire(this, r);
  }
setHasChanges(true);
}

int Animation::Color::g() const { return m_r; }

void Animation::Color::setG(const int g) {
  MODIFIABLE_SET_OLD_VALUE(g);
  m_g = g;
  if (!signalsDisabled()) {
    gModified().fire(this, g);
  }
setHasChanges(true);
}

int Animation::Color::b() const { return m_b; }

void Animation::Color::setB(const int b) {
  MODIFIABLE_SET_OLD_VALUE(b);
  m_b = b;
  if (!signalsDisabled()) {
    bModified().fire(this, b);
  }
setHasChanges(true);
}

int Animation::Color::intensity() const { return m_intensity; }

void Animation::Color::setIntensity(const int intensity) {
  MODIFIABLE_SET_OLD_VALUE(intensity);
  m_intensity = intensity;
  if (!signalsDisabled()) {
    intensityModified().fire(this, intensity);
  }
setHasChanges(true);
}

rpgmutils::signal<void(Animation::Color*, int)>& Animation::Color::rModified() {
  if (!m_rModified) {
    m_rModified.emplace();
  }

  return *m_rModified;
}

rpgmutils::signal<void(Animation::Color*, int)>& Animation::Color::gModified() {
  if (!m_gModified) {
    m_gModified.emplace();
  }
  return *m_gModified;
}

rpgmutils::signal<void(Animation::Color*, int)>& Animation::Color::bModified() {
  if (!m_bModified) {
    m_bModified.emplace();
  }
  return *m_bModified;
}

rpgmutils::signal<void(Animation::Color*, int)>& Animation::Color::intensityModified() {
  if (!m_intensityModified) {
    m_intensityModified.emplace();
  }
  return *m_intensityModified;
}

void Animation::Color::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(r);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(g);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(b);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(intensity);
}

void Animation::Color::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(r);
  MODIFIABLE_ACCEPT_VALUE(g);
  MODIFIABLE_ACCEPT_VALUE(b);
  MODIFIABLE_ACCEPT_VALUE(intensity);
}

nlohmann::ordered_json Animation::Color::serializeOldValues() const {
  nlohmann::ordered_json json;
  MODIFIABLE_SERIALIZE_VALUE_FLAT_ARRAY(json, r);
  MODIFIABLE_SERIALIZE_VALUE_FLAT_ARRAY(json, g);
  MODIFIABLE_SERIALIZE_VALUE_FLAT_ARRAY(json, b);
  MODIFIABLE_SERIALIZE_VALUE_FLAT_ARRAY(json, intensity);
  return json;
}

Animation::FramePart::FramePart(const FramePart& other)
: IModifiable(other)
, m_pattern(other.m_pattern)
, m_x(other.m_x)
, m_y(other.m_y)
, m_scale(other.m_scale)
, m_rotation(other.m_rotation)
, m_mirror(other.m_mirror)
, m_opacity(other.m_opacity)
, m_blend(other.m_blend)
, m_oldpattern(other.m_oldpattern)
, m_oldx(other.m_oldx)
, m_oldy(other.m_oldy)
, m_oldscale(other.m_oldscale)
, m_oldrotation(other.m_oldrotation)
, m_oldmirror(other.m_oldmirror)
, m_oldopacity(other.m_oldopacity)
, m_oldblend(other.m_oldblend) {}

Animation::FramePart& Animation::FramePart::operator=(const FramePart& other) {
  IModifiable::operator=(other);
  m_pattern = other.m_pattern;
  m_x = other.m_x;
  m_y = other.m_y;
  m_scale = other.m_scale;
  m_rotation = other.m_rotation;
  m_mirror = other.m_mirror;
  m_opacity = other.m_opacity;
  m_blend = other.m_blend;
  m_oldpattern = other.m_oldpattern;
  m_oldx = other.m_oldx;
  m_oldy = other.m_oldy;
  m_oldscale = other.m_oldscale;
  m_oldrotation = other.m_oldrotation;
  m_oldmirror = other.m_oldmirror;
  m_oldopacity = other.m_oldopacity;
  m_oldblend = other.m_oldblend;
  return *this;
}

Animation::FramePart::FramePart(FramePart&& other) noexcept
: IModifiable(std::move(other))
, m_pattern(other.m_pattern)
, m_x(other.m_x)
, m_y(other.m_y)
, m_scale(other.m_scale)
, m_rotation(other.m_rotation)
, m_mirror(other.m_mirror)
, m_opacity(other.m_opacity)
, m_blend(other.m_blend)
, m_oldpattern(other.m_oldpattern)
, m_oldx(other.m_oldx)
, m_oldy(other.m_oldy)
, m_oldscale(other.m_oldscale)
, m_oldrotation(other.m_oldrotation)
, m_oldmirror(other.m_oldmirror)
, m_oldopacity(other.m_oldopacity)
, m_oldblend(other.m_oldblend) {}

Animation::FramePart& Animation::FramePart::operator=(FramePart&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_pattern = other.m_pattern;
  m_x = other.m_x;
  m_y = other.m_y;
  m_scale = other.m_scale;
  m_rotation = other.m_rotation;
  m_mirror = other.m_mirror;
  m_opacity = other.m_opacity;
  m_blend = other.m_blend;
  m_oldpattern = other.m_oldpattern;
  m_oldx = other.m_oldx;
  m_oldy = other.m_oldy;
  m_oldscale = other.m_oldscale;
  m_oldrotation = other.m_oldrotation;
  m_oldmirror = other.m_oldmirror;
  m_oldopacity = other.m_oldopacity;
  m_oldblend = other.m_oldblend;
  return *this;
}

int Animation::FramePart::pattern() const { return m_pattern; }

void Animation::FramePart::setPattern(const int pattern) {
  MODIFIABLE_SET_OLD_VALUE(pattern);
  m_pattern = pattern;
  if (!signalsDisabled()) {
    patternModified().fire(this, pattern);
  }
setHasChanges(true);
}

int Animation::FramePart::x() const { return m_x; }

void Animation::FramePart::setX(const int x) {
  MODIFIABLE_SET_OLD_VALUE(x);
  m_x = x;
  if (!signalsDisabled()) {
    xModified().fire(this, x);
  }
setHasChanges(true);
}

int Animation::FramePart::y() const { return m_y; }

void Animation::FramePart::setY(const int y) {
  MODIFIABLE_SET_OLD_VALUE(y);
  m_y = y;
  if (!signalsDisabled()) {
    yModified().fire(this, y);
  }
setHasChanges(true);
}

int Animation::FramePart::scale() const { return m_scale; }

void Animation::FramePart::setScale(const int scale) {
  MODIFIABLE_SET_OLD_VALUE(scale);
  m_scale = scale;
  if (!signalsDisabled()) {
    scaleModified().fire(this, scale);
  }
setHasChanges(true);
}

int Animation::FramePart::rotation() const { return m_rotation; }

void Animation::FramePart::setRotation(const int rotation) {
  MODIFIABLE_SET_OLD_VALUE(rotation);
  m_rotation = rotation;
  if (!signalsDisabled()) {
    rotationModified().fire(this, rotation);
  }
setHasChanges(true);
}

Mirror Animation::FramePart::mirror() const { return m_mirror; }

void Animation::FramePart::setMirror(const Mirror mirror) {
  MODIFIABLE_SET_OLD_VALUE(mirror);
  m_mirror = mirror;
  if (!signalsDisabled()) {
    mirrorModified().fire(this, mirror);
  }
setHasChanges(true);
}

int Animation::FramePart::opacity() const { return m_opacity; }

void Animation::FramePart::setOpacity(const int opacity) {
  MODIFIABLE_SET_OLD_VALUE(opacity);
  m_opacity = opacity;
  if (!signalsDisabled()) {
    opacityModified().fire(this, opacity);
  }
setHasChanges(true);
}

Blend Animation::FramePart::blend() const { return m_blend; }

void Animation::FramePart::setBlend(const Blend blend) {
  MODIFIABLE_SET_OLD_VALUE(blend);
  m_blend = blend;
  if (!signalsDisabled()) {
    blendModified().fire(this, blend);
  }
setHasChanges(true);
}

rpgmutils::signal<void(Animation::FramePart*, int)>& Animation::FramePart::patternModified() {
  if (!m_patternModified) {
    m_patternModified.emplace();
  }
  return *m_patternModified;
}

rpgmutils::signal<void(Animation::FramePart*, int)>& Animation::FramePart::xModified() {
  if (!m_xModified) {
    m_xModified.emplace();
  }
  return *m_xModified;
}

rpgmutils::signal<void(Animation::FramePart*, int)>& Animation::FramePart::yModified() {
  if (!m_yModified) {
    m_yModified.emplace();
  }
  return *m_yModified;
}

rpgmutils::signal<void(Animation::FramePart*, int)>& Animation::FramePart::scaleModified() {
  if (!m_scaleModified) {
    m_scaleModified.emplace();
  }
  return *m_scaleModified;
}

rpgmutils::signal<void(Animation::FramePart*, int)>& Animation::FramePart::rotationModified() {
  if (!m_rotationModified) {
    m_rotationModified.emplace();
  }
  return *m_rotationModified;
}

rpgmutils::signal<void(Animation::FramePart*, Mirror)>& Animation::FramePart::mirrorModified() {
  if (!m_mirrorModified) {
    m_mirrorModified.emplace();
  }
  return *m_mirrorModified;
}

rpgmutils::signal<void(Animation::FramePart*, int)>& Animation::FramePart::opacityModified() {
  if (!m_opacityModified) {
    m_opacityModified.emplace();
  }
  return *m_opacityModified;
}

rpgmutils::signal<void(Animation::FramePart*, Blend)>& Animation::FramePart::blendModified() {
  if (!m_blendModified) {
    m_blendModified.emplace();
  }
  return *m_blendModified;
}

void Animation::FramePart::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(pattern);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(x);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(y);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(scale);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(rotation);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(mirror);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(opacity);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(blend);
}

void Animation::FramePart::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(pattern);
  MODIFIABLE_ACCEPT_VALUE(x);
  MODIFIABLE_ACCEPT_VALUE(y);
  MODIFIABLE_ACCEPT_VALUE(scale);
  MODIFIABLE_ACCEPT_VALUE(rotation);
  MODIFIABLE_ACCEPT_VALUE(mirror);
  MODIFIABLE_ACCEPT_VALUE(opacity);
  MODIFIABLE_ACCEPT_VALUE(blend);
}

nlohmann::ordered_json Animation::FramePart::serializeOldValues() const {
  nlohmann::ordered_json json;
  MODIFIABLE_SERIALIZE_VALUE_FLAT_ARRAY(json, pattern);
  MODIFIABLE_SERIALIZE_VALUE_FLAT_ARRAY(json, x);
  MODIFIABLE_SERIALIZE_VALUE_FLAT_ARRAY(json, y);
  MODIFIABLE_SERIALIZE_VALUE_FLAT_ARRAY(json, scale);
  MODIFIABLE_SERIALIZE_VALUE_FLAT_ARRAY(json, rotation);
  MODIFIABLE_SERIALIZE_VALUE_FLAT_ARRAY(json, mirror);
  MODIFIABLE_SERIALIZE_VALUE_FLAT_ARRAY(json, opacity);
  MODIFIABLE_SERIALIZE_VALUE_FLAT_ARRAY(json, blend);
  return json;
}

Animation::Timing::Timing(const Timing& other)
: IModifiable(other)
, m_flashColor(other.m_flashColor)
, m_flashDuration(other.m_flashDuration)
, m_frame(other.m_frame)
, m_se(other.m_se)
, m_oldflashColor(other.m_oldflashColor)
, m_oldflashDuration(other.m_oldflashDuration)
, m_oldframe(other.m_oldframe)
, m_oldse(other.m_oldse) {}

Animation::Timing& Animation::Timing::operator=(const Timing& other) {
  IModifiable::operator=(other);
  m_flashColor = other.m_flashColor;
  m_flashDuration = other.m_flashDuration;
  m_frame = other.m_frame;
  m_se = other.m_se;
  m_oldflashColor = other.m_oldflashColor;
  m_oldflashDuration = other.m_oldflashDuration;
  m_oldframe = other.m_oldframe;
  m_oldse = other.m_oldse;
  return *this;
}

Animation::Timing::Timing(Timing&& other) noexcept
: IModifiable(std::move(other))
, m_flashColor(other.m_flashColor)
, m_flashDuration(other.m_flashDuration)
, m_frame(other.m_frame)
, m_se(std::move(other.m_se))
, m_oldflashColor(other.m_oldflashColor)
, m_oldflashDuration(other.m_oldflashDuration)
, m_oldframe(other.m_oldframe)
, m_oldse(std::move(other.m_oldse)) {}

Animation::Timing& Animation::Timing::operator=(Timing&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_flashColor = other.m_flashColor;
  m_flashDuration = other.m_flashDuration;
  m_frame = other.m_frame;
  m_se = std::move(other.m_se);
  m_oldflashColor = other.m_oldflashColor;
  m_oldflashDuration = other.m_oldflashDuration;
  m_oldframe = other.m_oldframe;
  m_oldse = std::move(other.m_oldse);
  return *this;
}

Animation::Color& Animation::Timing::flashColor() { return m_flashColor; }
const Animation::Color& Animation::Timing::flashColor() const { return m_flashColor; }

void Animation::Timing::setFlashColor(const Color& color) {
  MODIFIABLE_SET_OLD_VALUE(flashColor);
  m_flashColor = color;
  if (!signalsDisabled()) {
    flashColorModified().fire(this, color);
  }
setHasChanges(true);
}

int Animation::Timing::flashDuration() const { return m_flashDuration; }
void Animation::Timing::setFlashDuration(int duration) {
  MODIFIABLE_SET_OLD_VALUE(flashDuration);
  m_flashDuration = duration;
  if (!signalsDisabled()) {
    flashDurationModified().fire(this, duration);
  }
setHasChanges(true);
}

FlashScope Animation::Timing::flashScope() const { return m_flashScope; }
void Animation::Timing::setFlashScope(FlashScope scope) {
  MODIFIABLE_SET_OLD_VALUE(flashScope);
  m_flashScope = scope;
  if (!signalsDisabled()) {
    flashScopeModified().fire(this, scope);
  }
setHasChanges(true);
}

int Animation::Timing::frame() const { return m_frame; }
void Animation::Timing::setFrame(int frame) {
  MODIFIABLE_SET_OLD_VALUE(frame);
  m_frame = frame;
  if (!signalsDisabled()) {
    frameModified().fire(this, frame);
  }
setHasChanges(true);
}

Audio* Animation::Timing::se() {
  if (m_se) {
    return &*m_se;
  }
  return nullptr;
}
const Audio* Animation::Timing::se() const { return const_cast<Timing*>(this)->se(); }

void Animation::Timing::setSe(const Audio& se) {
  MODIFIABLE_SET_OLD_VALUE(se);
  m_se = se;
  if (!signalsDisabled()) {
    seModified().fire(this, se);
  }
setHasChanges(true);
}

rpgmutils::signal<void(Animation::Timing*, const Animation::Color&)>& Animation::Timing::flashColorModified() {
  if (!m_flashColorModified) {
    m_flashColorModified.emplace();
  }
  return *m_flashColorModified;
}

rpgmutils::signal<void(Animation::Timing*, int)>& Animation::Timing::flashDurationModified() {
  if (!m_flashDurationModified) {
    m_flashDurationModified.emplace();
  }
  return *m_flashDurationModified;
}
rpgmutils::signal<void(Animation::Timing*, FlashScope)>& Animation::Timing::flashScopeModified() {
  if (!m_flashScopeModified) {
    m_flashScopeModified.emplace();
  }
  return *m_flashScopeModified;
}
rpgmutils::signal<void(Animation::Timing*, int)>& Animation::Timing::frameModified() {
  if (!m_frameModified) {
    m_frameModified.emplace();
  }
  return *m_frameModified;
}
rpgmutils::signal<void(Animation::Timing*, const Audio&)>& Animation::Timing::seModified() {
  if (!m_seModified) {
    m_seModified.emplace();
  }
  return *m_seModified;
}

void Animation::Timing::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(flashColor);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(flashDuration);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(flashScope);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(frame);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(se);
}

void Animation::Timing::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(flashColor);
  MODIFIABLE_ACCEPT_VALUE(flashDuration);
  MODIFIABLE_ACCEPT_VALUE(flashScope);
  MODIFIABLE_ACCEPT_VALUE(frame);
  MODIFIABLE_ACCEPT_VALUE(se);
}

nlohmann::ordered_json Animation::Timing::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_CHILD_VALUE(flashColor),
      MODIFIABLE_SERIALIZE_VALUE(flashDuration),
      MODIFIABLE_SERIALIZE_VALUE(flashScope),
      MODIFIABLE_SERIALIZE_VALUE(frame),
      {"se", m_oldse ? m_oldse->serializeOldValues()
             : m_se  ? m_se->serializeOldValues()
                     : nullptr},
  };
}

Animation::Animation(const Animation& other)
: IModifiable(other)
, m_id(other.m_id)
, m_animation1Hue(other.m_animation1Hue)
, m_animation1Name(other.m_animation1Name)
, m_animation2Hue(other.m_animation2Hue)
, m_animation2Name(other.m_animation2Name)
, m_frames(other.m_frames)
, m_name(other.m_name)
, m_position(other.m_position)
, m_timings(other.m_timings)
, m_oldid(other.m_oldid)
, m_oldanimation1Hue(other.m_oldanimation1Hue)
, m_oldanimation1Name(other.m_oldanimation1Name)
, m_oldanimation2Hue(other.m_oldanimation2Hue)
, m_oldanimation2Name(other.m_oldanimation2Name)
, m_oldname(other.m_oldname)
, m_oldposition(other.m_oldposition)
, m_oldtimings(other.m_oldtimings)
, m_isValid(other.m_isValid) {}

Animation& Animation::operator=(const Animation& other) {
  IModifiable::operator=(other);
  m_id = other.m_id;
  m_animation1Hue = other.m_animation1Hue;
  m_animation1Name = other.m_animation1Name;
  m_animation2Hue = other.m_animation2Hue;
  m_animation2Name = other.m_animation2Name;
  m_frames = other.m_frames;
  m_name = other.m_name;
  m_position = other.m_position;
  m_timings = other.m_timings;
  m_oldid = other.m_oldid;
  m_oldanimation1Hue = other.m_oldanimation1Hue;
  m_oldanimation1Name = other.m_oldanimation1Name;
  m_oldanimation2Hue = other.m_oldanimation2Hue;
  m_oldanimation2Name = other.m_oldanimation2Name;
  m_oldposition = other.m_oldposition;
  m_oldtimings = other.m_oldtimings;
  m_isValid = other.m_isValid;
  return *this;
}

Animation::Animation(Animation&& other) noexcept
: IModifiable(std::move(other))
, m_id(other.m_id)
, m_animation1Hue(other.m_animation1Hue)
, m_animation1Name(std::move(other.m_animation1Name))
, m_animation2Hue(other.m_animation2Hue)
, m_animation2Name(std::move(other.m_animation2Name))
, m_frames(std::move(other.m_frames))
, m_name(std::move(other.m_name))
, m_position(other.m_position)
, m_timings(std::move(other.m_timings))
, m_oldid(other.m_oldid)
, m_oldanimation1Hue(other.m_oldanimation1Hue)
, m_oldanimation1Name(std::move(other.m_oldanimation1Name))
, m_oldanimation2Hue(other.m_oldanimation2Hue)
, m_oldanimation2Name(std::move(other.m_oldanimation2Name))
, m_oldname(std::move(other.m_oldname))
, m_oldposition(other.m_oldposition)
, m_oldtimings(std::move(other.m_oldtimings))
, m_isValid(other.m_isValid) {}

Animation& Animation::operator=(Animation&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_id = other.m_id;
  m_animation1Hue = other.m_animation1Hue;
  m_animation1Name = std::move(other.m_animation1Name);
  m_animation2Hue = other.m_animation2Hue;
  m_animation2Name = std::move(other.m_animation2Name);
  m_frames = std::move(other.m_frames);
  m_name = std::move(other.m_name);
  m_position = other.m_position;
  m_timings = std::move(other.m_timings);
  m_oldid = other.m_oldid;
  m_oldanimation1Hue = other.m_oldanimation1Hue;
  m_oldanimation1Name = std::move(other.m_oldanimation1Name);
  m_oldanimation2Hue = other.m_oldanimation2Hue;
  m_oldanimation2Name = std::move(other.m_oldanimation2Name);
  m_oldposition = other.m_oldposition;
  m_oldtimings = std::move(other.m_oldtimings);
  m_isValid = other.m_isValid;
  return *this;
}

int Animation::id() const { return m_id; }
void Animation::setId(const int id) {
  MODIFIABLE_SET_OLD_VALUE(id);
  m_id = id;
  if (!signalsDisabled()) {
    idModified().fire(this, id);
  }
setHasChanges();
}

int Animation::animation1Hue() const { return m_animation1Hue; }
void Animation::setAnimation1Hue(const int animation1Hue) {
  MODIFIABLE_SET_OLD_VALUE(animation1Hue);
  m_animation1Hue = animation1Hue;
  if (!signalsDisabled()) {
    animation1HueModified().fire(this, animation1Hue);
  }
setHasChanges();
}

std::string_view Animation::animation1Name() const { return m_animation1Name; }
void Animation::setAnimation1Name(const std::string_view animation1Name) {
  MODIFIABLE_SET_OLD_VALUE(animation1Name);
  m_animation1Name = animation1Name;
  if (!signalsDisabled()) {
    animation1NameModified().fire(this, m_animation1Name);
  }
setHasChanges();
}

int Animation::animation2Hue() const { return m_animation2Hue; }
void Animation::setAnimation2Hue(const int animation2Hue) {
  MODIFIABLE_SET_OLD_VALUE(animation2Hue);
  m_animation2Hue = animation2Hue;
  if (!signalsDisabled()) {
    animation2HueModified().fire(this, animation2Hue);
  }
setHasChanges();
}

std::string_view Animation::animation2Name() const { return m_animation2Name; }
void Animation::setAnimation2Name(const std::string_view animation2Name) {
  MODIFIABLE_SET_OLD_VALUE(animation2Name);
  m_animation2Name = animation2Name;
  if (!signalsDisabled()) {
    animation2NameModified().fire(this, m_animation2Name);
  }
setHasChanges();
}

const std::vector<std::vector<Animation::FramePart>>& Animation::frames() const { return m_frames; }
void Animation::setFrames(const std::vector<std::vector<FramePart>>& frames) {
  MODIFIABLE_SET_OLD_VALUE(frames);
  m_frames = frames;
  if (!signalsDisabled()) {
    framesModified().fire(this, m_frames);
  }
setHasChanges();
}

const std::vector<Animation::FramePart>& Animation::frame(const int index) const {
  assert(index >= 0 && index < m_frames.size());
  return m_frames[index];
}

void Animation::setFrame(const int index, const std::vector<FramePart>& frame) {
  assert(index >= 0 && index < m_frames.size());
  MODIFIABLE_SET_OLD_VALUE(frames);
  m_frames[index] = frame;
  if (!signalsDisabled()) {
    frameModified().fire(this, index, frame);
  }
setHasChanges();
}

const std::string& Animation::name() const { return m_name; }
void Animation::setName(const std::string_view name) {
  MODIFIABLE_SET_OLD_VALUE(name);
  m_name = name;
  if (!signalsDisabled()) {
    nameModified().fire(this, m_name);
  }
setHasChanges();
}

Position Animation::position() const { return m_position; }
void Animation::setPosition(const Position position) {
  MODIFIABLE_SET_OLD_VALUE(position);
  m_position = position;
  if (!signalsDisabled()) {
    positionModified().fire(this, m_position);
  }
setHasChanges();
}

const std::vector<Animation::Timing>& Animation::timings() const { return m_timings; }
void Animation::setTimings(const std::vector<Timing>& timings) {
  MODIFIABLE_SET_OLD_VALUE(timings);
  m_timings = timings;
  if (!signalsDisabled()) {
    timingsModified().fire(this, m_timings);
  }
setHasChanges();
}

const Animation::Timing& Animation::timing(const int index) const {
  assert(index >= 0 && index < m_timings.size());
  return m_timings[index];
}

void Animation::setTiming(const int index, const Timing& timing) {
  assert(index >= 0 && index < m_timings.size());
  MODIFIABLE_SET_OLD_VALUE(timings);
  m_timings[index] = timing;
  if (!signalsDisabled()) {
    timingModified().fire(this, index, timing);
  }
setHasChanges();
}
// SIGNALS
rpgmutils::signal<void(Animation*, int)>& Animation::idModified() {
  if (!m_idModified) {
    m_idModified.emplace();
  }
  return *m_idModified;
}

rpgmutils::signal<void(Animation*, int)>& Animation::animation1HueModified() {
  if (!m_animation1HueModified) {
    m_animation1HueModified.emplace();
  }
  return *m_animation1HueModified;
}
rpgmutils::signal<void(Animation*, std::string_view)>& Animation::animation1NameModified() {
  if (!m_animation1NameModified) {
    m_animation1NameModified.emplace();
  }
  return *m_animation1NameModified;
}

rpgmutils::signal<void(Animation*, int)>& Animation::animation2HueModified() {
  if (!m_animation2HueModified) {
    m_animation2HueModified.emplace();
  }
  return *m_animation2HueModified;
}

rpgmutils::signal<void(Animation*, std::string_view)>& Animation::animation2NameModified() {
  if (!m_animation2NameModified) {
    m_animation2NameModified.emplace();
  }
  return *m_animation2NameModified;
}

rpgmutils::signal<void(Animation*, const std::vector<std::vector<Animation::FramePart>>&)>& Animation::framesModified() {
  if (!m_framesModified) {
    m_framesModified.emplace();
  }
  return *m_framesModified;
}

rpgmutils::signal<void(Animation*, int, const std::vector<Animation::FramePart>&)>& Animation::frameModified() {
  if (!m_frameModified) {
    m_frameModified.emplace();
  }
  return *m_frameModified;
}

rpgmutils::signal<void(Animation*, std::string_view)>& Animation::nameModified() {
  if (!m_nameModified) {
    m_nameModified.emplace();
  }
  return *m_nameModified;
}

rpgmutils::signal<void(Animation*, Position)>& Animation::positionModified() {
  if (!m_positionModified) {
    m_positionModified.emplace();
  }
  return *m_positionModified;
}

rpgmutils::signal<void(Animation*, const std::vector<Animation::Timing>&)>& Animation::timingsModified() {
  if (!m_timingsModified) {
    m_timingsModified.emplace();
  }
  return *m_timingsModified;
}

rpgmutils::signal<void(Animation*, int, const Animation::Timing&)>& Animation::timingModified() {
  if (!m_timingModified) {
    m_timingModified.emplace();
  }
  return *m_timingModified;
}

void Animation::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(id);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(animation1Hue);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(animation1Name);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(animation2Hue);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(animation2Name);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(frames);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(name);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(position);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(timings);
}

void Animation::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(id);
  MODIFIABLE_ACCEPT_VALUE(animation1Hue);
  MODIFIABLE_ACCEPT_VALUE(animation1Name);
  MODIFIABLE_ACCEPT_VALUE(animation2Hue);
  MODIFIABLE_ACCEPT_VALUE(animation2Name);
  MODIFIABLE_ACCEPT_VALUE(frames);
  MODIFIABLE_ACCEPT_VALUE(name);
  MODIFIABLE_ACCEPT_VALUE(position);
  MODIFIABLE_ACCEPT_VALUE(timings);
}

nlohmann::ordered_json Animation::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(id),
      MODIFIABLE_SERIALIZE_VALUE(animation1Hue),
      MODIFIABLE_SERIALIZE_VALUE(animation1Name),
      MODIFIABLE_SERIALIZE_VALUE(animation2Hue),
      MODIFIABLE_SERIALIZE_VALUE(animation2Name),
      MODIFIABLE_SERIALIZE_VALUE(frames),
      MODIFIABLE_SERIALIZE_VALUE(name),
      MODIFIABLE_SERIALIZE_VALUE(position),
      MODIFIABLE_SERIALIZE_VALUE(timings),
  };
}

/* JSON Serialization */
void to_json(nlohmann::ordered_json& to, const Animation::Color& color) {
  to.push_back(color.m_r);
  to.push_back(color.m_g);
  to.push_back(color.m_b);
  to.push_back(color.m_intensity);
}

void from_json(const nlohmann::ordered_json& from, Animation::Color& color) {
  from.at(0).get_to(color.m_r);
  from.at(1).get_to(color.m_g);
  from.at(2).get_to(color.m_b);
  from.at(3).get_to(color.m_intensity);
}

void to_json(nlohmann::ordered_json& to, const Animation::FramePart& frame) {
  to.push_back(frame.m_pattern);
  to.push_back(frame.m_x);
  to.push_back(frame.m_y);
  to.push_back(frame.m_scale);
  to.push_back(frame.m_rotation);
  to.push_back(frame.m_mirror);
  to.push_back(frame.m_opacity);
  to.push_back(frame.m_blend);
}

void from_json(const nlohmann::ordered_json& from, Animation::FramePart& frame) {
  from.at(0).get_to(frame.m_pattern);
  from.at(1).get_to(frame.m_x);
  from.at(2).get_to(frame.m_y);
  from.at(3).get_to(frame.m_scale);
  from.at(4).get_to(frame.m_rotation);
  from.at(5).get_to(frame.m_mirror);
  from.at(6).get_to(frame.m_opacity);
  from.at(7).get_to(frame.m_blend);
}

void to_json(nlohmann::ordered_json& to, const Animation::Timing& timing) {
  to = {
      {"flashColor", timing.m_flashColor}, {"flashDuration", timing.m_flashDuration}, {"flashScope", timing.m_flashScope}, {"frame", timing.m_frame}, {"se", timing.m_se},
  };
}

void from_json(const nlohmann::ordered_json& from, Animation::Timing& timing) {
  from.at("flashColor").get_to(timing.m_flashColor);
  from.at("flashDuration").get_to(timing.m_flashDuration);
  from.at("flashScope").get_to(timing.m_flashScope);
  from.at("frame").get_to(timing.m_frame);
  from.at("se").get_to(timing.m_se);
}

void to_json(nlohmann::ordered_json& to, const Animation& animation) {
  to = {
      {"id", animation.m_id},
      {"animation1Hue", animation.m_animation1Hue},
      {"animation1Name", animation.m_animation1Name},
      {"animation2Hue", animation.m_animation2Hue},
      {"animation2Name", animation.m_animation2Name},
      {"frames", animation.m_frames},
      {"name", animation.m_name},
      {"position", animation.m_position},
      {"timings", animation.m_timings},
  };
}

void from_json(const nlohmann::ordered_json& from, Animation& animation) {
  animation.m_id = from.value("id", animation.m_id);
  animation.m_animation1Hue = from.value("animation1Hue", animation.m_animation1Hue);
  animation.m_animation1Name = from.value("animation1Name", animation.m_animation1Name);
  animation.m_animation2Hue = from.value("animation2Hue", animation.m_animation2Hue);
  animation.m_animation2Name = from.value("animation2Name", animation.m_animation2Name);
  animation.m_frames = from.value("frames", animation.m_frames);
  animation.m_name = from.value("name", animation.m_name);
  animation.m_position = from.value("position", animation.m_position);
  animation.m_timings = from.value("timings", animation.m_timings);
}
