#ifndef GAMEDATA_POSITION_HPP_
#define GAMEDATA_POSITION_HPP_

class Position {
   public:
      Position();
      virtual void setXYZ(const float &x, const float &y, const float &z);
      virtual void setX(const float& x);
      virtual void setY(const float& y);
      virtual void setZ(const float& z);
      virtual float getX() const;
      virtual float getY() const;
      virtual float getZ() const;

   private:
      float x;
      float y;
      float z;
};

#endif /* GAMEDATA_POSITION_HPP_ */
