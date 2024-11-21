#pragma once

class Color;
class ColorTonePreview {
public:
  void update(float r, float g, float b, float gray);

private:
  void* m_texture;
};
