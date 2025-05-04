#include "Core/Sound.hpp"

#include "Log.hpp"

sf::Vector3f stereoPanPosition(float panAmount) {
  // make sure that panAmount is in the range [-1, 1] (-1 is left; 1 is right)
  panAmount = std::clamp(panAmount, -1.f, 1.f);
  return {panAmount, 0.f, panAmount < 0.f ? -panAmount - 1.f : panAmount - 1.f};
}

Sound::Sound(const sf::SoundBuffer& buffer) {
  m_numChannels = buffer.getChannelCount();
  if (m_numChannels > 2) {
    APP_DEBUG("Unsupported number of channels {}", m_numChannels);
  }

  if (m_numChannels == 2) {
    auto samples = buffer.getSamples();
    std::vector<sf::Int16> leftSamples;
    std::vector<sf::Int16> rightSamples;

    for (int i = 0; i < buffer.getSampleCount() / 2; ++i) {
      leftSamples.push_back(samples[i * 2]);
      rightSamples.push_back(samples[(i * 2) + 1]);
    }

    m_leftBuffer.loadFromSamples(leftSamples.data(), leftSamples.size(), 1, buffer.getSampleRate());
    m_leftSound.setBuffer(m_leftBuffer);

    m_rightBuffer.loadFromSamples(rightSamples.data(), rightSamples.size(), 1, buffer.getSampleRate());
    m_rightSound.setBuffer(m_rightBuffer);
    /* Retrieve and sync up parameters */
    m_volume = m_leftSound.getVolume();
    m_minDistance = m_leftSound.getMinDistance();
    m_attenuation = m_leftSound.getAttenuation();
    m_pitch = m_leftSound.getPitch();
    m_relativeToListener = false;
    m_leftSound.setRelativeToListener(m_relativeToListener);
    m_rightSound.setVolume(m_volume);
    m_rightSound.setMinDistance(m_minDistance);
    m_rightSound.setPitch(m_pitch);
    m_rightSound.setRelativeToListener(m_relativeToListener);
  } else {
    m_leftBuffer = buffer;
  }

  m_volume = m_leftSound.getVolume();
  m_minDistance = m_leftSound.getMinDistance();
  m_attenuation = m_leftSound.getAttenuation();
  m_pitch = m_leftSound.getPitch();
  m_relativeToListener = false;
  m_leftSound.setRelativeToListener(m_relativeToListener);

  if (m_numChannels == 2) {
    m_rightSound.setVolume(m_volume);
    m_rightSound.setMinDistance(m_minDistance);
    m_rightSound.setPitch(m_pitch);
    m_rightSound.setRelativeToListener(m_relativeToListener);
  }

  m_pan = 0.f;
  setPan(m_pan);
}

Sound::Sound(const Sound& other) {
  m_numChannels = other.m_numChannels;
  m_leftBuffer = other.m_leftBuffer;
  m_rightBuffer = other.m_rightBuffer;
  m_minDistance = other.m_minDistance;
  m_pan = other.m_pan;
  m_volume = other.m_volume;
  m_minDistance = other.m_minDistance;
  m_pitch = other.m_pitch;
  m_attenuation = other.m_attenuation;
  m_leftSound = other.m_leftSound;
  m_rightSound = other.m_rightSound;
  m_leftSound.setBuffer(m_leftBuffer);
  m_leftSound.setVolume(m_volume);
  m_leftSound.setMinDistance(m_minDistance);
  m_leftSound.setPitch(m_pitch);
  m_leftSound.setRelativeToListener(m_relativeToListener);
  m_rightSound.setBuffer(m_rightBuffer);
  m_rightSound.setVolume(m_volume);
  m_rightSound.setMinDistance(m_minDistance);
  m_rightSound.setPitch(m_pitch);
  m_rightSound.setRelativeToListener(m_relativeToListener);
}

Sound& Sound::operator=(const Sound& other) {
  m_numChannels = other.m_numChannels;
  m_leftBuffer = other.m_leftBuffer;
  m_rightBuffer = other.m_rightBuffer;
  m_minDistance = other.m_minDistance;
  m_pan = other.m_pan;
  m_volume = other.m_volume;
  m_minDistance = other.m_minDistance;
  m_pitch = other.m_pitch;
  m_attenuation = other.m_attenuation;
  m_leftSound = other.m_leftSound;
  m_rightSound = other.m_rightSound;
  m_leftSound.setBuffer(m_leftBuffer);
  m_leftSound.setVolume(m_volume);
  m_leftSound.setMinDistance(m_minDistance);
  m_leftSound.setPitch(m_pitch);
  m_leftSound.setRelativeToListener(m_relativeToListener);
  m_rightSound.setBuffer(m_rightBuffer);
  m_rightSound.setVolume(m_volume);
  m_rightSound.setMinDistance(m_minDistance);
  m_rightSound.setPitch(m_pitch);
  m_rightSound.setRelativeToListener(m_relativeToListener);
  return *this;
}

void Sound::play() {
  m_leftSound.play();
  if (m_numChannels == 2) {
    m_rightSound.play();
  }
}

void Sound::pause() {
  m_leftSound.pause();
  if (m_numChannels == 2) {
    m_rightSound.pause();
  }
}

void Sound::stop() {
  m_leftSound.stop();
  if (m_numChannels) {
    m_rightSound.stop();
  }
}

void Sound::setBuffers(const sf::SoundBuffer& leftBuffer, const sf::SoundBuffer& rightBuffer) {
  m_leftSound.setBuffer(leftBuffer);
  if (m_numChannels == 2) {
    m_rightSound.setBuffer(rightBuffer);
  }
}

void Sound::setLoop(const bool loop) {
  m_leftSound.setLoop(loop);
  if (m_numChannels == 2) {
    m_rightSound.setLoop(loop);
  }
}
void Sound::setPlayingOffset(const sf::Time offset) {
  m_leftSound.setPlayingOffset(offset);
  if (m_numChannels == 2) {
    m_rightSound.setPlayingOffset(offset);
  }
}

[[nodiscard]] const sf::SoundBuffer* Sound::getLeftBuffer() const { return m_leftSound.getBuffer(); }
[[nodiscard]] const sf::SoundBuffer* Sound::getRightBuffer() const { return m_rightSound.getBuffer(); }
[[nodiscard]] bool Sound::getLoop() const { return m_leftSound.getLoop() | m_rightSound.getLoop(); }
[[nodiscard]] sf::Time Sound::getPlayingOffset() const { return m_leftSound.getPlayingOffset(); }
[[nodiscard]] sf::Sound::Status Sound::getLeftStatus() const { return m_leftSound.getStatus(); }
[[nodiscard]] sf::Sound::Status Sound::getRightStatus() const { return m_rightSound.getStatus(); }

void Sound::setPan(const float amount) {
  m_leftSound.setPosition(stereoPanPosition(amount));
  if (m_numChannels) {
    m_rightSound.setPosition(stereoPanPosition(amount));
  }
  m_pan = amount;
}

void Sound::setVolume(const float volume) {
  m_leftSound.setVolume(volume);
  if (m_numChannels == 2) {
    m_rightSound.setVolume(volume);
  }
  m_volume = volume;
}

float Sound::getVolume() const { return m_volume; }

void Sound::setPitch(const float pitch) {
  m_leftSound.setPitch(pitch);
  if (m_numChannels == 2) {
    m_rightSound.setPitch(pitch);
  }
  m_pitch = pitch;
}

float Sound::getPitch() const { return m_pitch; }

void Sound::setLeftPosition(const float x, const float y, const float z) { m_leftSound.setPosition(x, y, z); }
void Sound::setLeftPosition(const sf::Vector3f& position) { m_leftSound.setPosition(position); }

sf::Vector3f Sound::getLeftPosition() const { return m_leftSound.getPosition(); }

void Sound::setRightPosition(float x, float y, float z) { m_rightSound.setPosition(x, y, z); }
void Sound::setRightPosition(const sf::Vector3f& position) { m_rightSound.setPosition(position); }

sf::Vector3f Sound::getRightPosition() const { return m_rightSound.getPosition(); }

void Sound::setRelativeToListener(const bool relative) {
  m_leftSound.setRelativeToListener(relative);
  if (m_numChannels == 2) {
    m_rightSound.setRelativeToListener(relative);
  }
  m_relativeToListener = relative;
}

bool Sound::isRelativeToListener() const { return m_relativeToListener; }

void Sound::setMinDistance(float distance) {
  m_leftSound.setMinDistance(distance);
  if (m_numChannels == 2) {
    m_rightSound.setMinDistance(distance);
  }
  m_minDistance = distance;
}

float Sound::getMinDistance() const { return m_minDistance; }

void Sound::setAttenuation(const float attenuation) {
  m_leftSound.setAttenuation(attenuation);
  if (m_numChannels == 2) {
    m_rightSound.setAttenuation(attenuation);
  }
  m_attenuation = attenuation;
}

float Sound::getAttenuation() const { return m_attenuation; }