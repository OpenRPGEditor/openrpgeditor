#pragma once
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

class Sound {
public:
  Sound() = default;
  explicit Sound(const sf::SoundBuffer& buffer);
  Sound(const Sound& other);
  Sound& operator=(const Sound& other);

  void play();
  void pause();
  void stop();
  void setBuffers(const sf::SoundBuffer& leftBuffer, const sf::SoundBuffer& rightBuffer);
  void setLoop(bool loop);
  void setPlayingOffset(sf::Time offset);
  [[nodiscard]] const sf::SoundBuffer* getLeftBuffer() const;
  [[nodiscard]] const sf::SoundBuffer* getRightBuffer() const;
  [[nodiscard]] bool getLoop() const;
  [[nodiscard]] sf::Time getPlayingOffset() const;
  [[nodiscard]] sf::Sound::Status getLeftStatus() const;
  [[nodiscard]] sf::Sound::Status getRightStatus() const;
  void setPan(float amount);
  void setVolume(float volume);
  [[nodiscard]] float getVolume() const;
  void setPitch(float pitch);
  [[nodiscard]] float getPitch() const;

  void setLeftPosition(float x, float y, float z);
  void setLeftPosition(const sf::Vector3f& position);
  [[nodiscard]] sf::Vector3f getLeftPosition() const;
  void setRightPosition(float x, float y, float z);
  void setRightPosition(const sf::Vector3f& position);
  [[nodiscard]] sf::Vector3f getRightPosition() const;

  void setRelativeToListener(bool relative);
  bool isRelativeToListener() const;
  void setMinDistance(float distance);
  [[nodiscard]] float getMinDistance() const;

  void setAttenuation(float attenuation);
  [[nodiscard]] float getAttenuation() const;

private:
  sf::SoundBuffer m_leftBuffer;
  sf::SoundBuffer m_rightBuffer;
  sf::Sound m_leftSound;
  sf::Sound m_rightSound;
  float m_volume{100.f};
  float m_pitch{1.f};
  float m_pan{0.f};
  float m_minDistance{1.f};
  float m_attenuation{1.f};
  bool m_relativeToListener{false};
  int m_numChannels = 2;
};
