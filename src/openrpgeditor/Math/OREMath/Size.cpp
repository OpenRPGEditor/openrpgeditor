#include "OREMath/Size.hpp"
#include <cinttypes>
#include <algorithm>

void Size::transpose() { std::swap(mWidth, mHeight); }

void Size::scale(const Size& size, const AspectRatioMode mode) {
  if (mode == AspectRatioMode::Ignore || mWidth == 0 || mHeight == 0) {
    mWidth = size.mWidth;
    mHeight = size.height();
    return;
  }

  const uint64_t rw = size.mHeight * mWidth / mHeight;

  if ((mode == AspectRatioMode::Keep && rw <= size.mWidth) ||
      (mode == AspectRatioMode::KeepByExpanding && rw >= size.mWidth)) {
    mWidth = rw;
    mHeight = size.mHeight;
  } else {
    mWidth = size.mWidth;
    mHeight = static_cast<uint64_t>(size.mWidth) * static_cast<uint64_t>(mHeight) / static_cast<uint64_t>(mWidth);
  }
}

void SizeF::transpose() { std::swap(mWidth, mHeight); }

void SizeF::scale(const SizeF& size, const AspectRatioMode mode) {
  if (mode == AspectRatioMode::Ignore || ::isNull(mWidth) || ::isNull(mHeight)) {
    mWidth = size.mWidth;
    mHeight = size.height();
    return;
  }

  const double rw = size.mHeight * mWidth / mHeight;

  if ((mode == AspectRatioMode::Keep && rw <= size.mWidth) ||
      (mode == AspectRatioMode::KeepByExpanding && rw >= size.mWidth)) {
    mWidth = rw;
    mHeight = size.mHeight;
  } else {
    mWidth = size.mWidth;
    mHeight = size.mWidth * mHeight / mWidth;
  }
}
