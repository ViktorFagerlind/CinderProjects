#pragma once

class BasicObject
{
public:
  virtual ~BasicObject () {}

  virtual void update () = 0;
  virtual void draw   () = 0;
  virtual bool isDead () = 0;
};


