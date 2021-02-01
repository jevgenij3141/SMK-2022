#include "./Position.hpp"

Position::Position() {
   setX(0.0f);
   setY(0.0f);
   setZ(0.0f);
}

inline void Position::setXYZ(const float &x, const float &y, const float &z) {
   Position::setX(x);
   Position::setY(y);
   Position::setZ(z);
}

inline float Position::getX() const {
   return x;
}

inline float Position::getY() const {
   return y;
}

inline float Position::getZ() const {
   return z;
}

inline void Position::setX(const float& x_) {
   x = x_;
}

inline void Position::setX(const float& y_) {
   y = y_;
}

inline void Position::setX(const float& z_) {
   z = z_;
}