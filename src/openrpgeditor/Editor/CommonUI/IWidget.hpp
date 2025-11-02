#pragma once

class IWidget {
  public:
    virtual ~IWidget() = default;
    virtual void draw() = 0;
  };