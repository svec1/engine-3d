#ifndef PHYSICOBJECT__HPP_
#define PHYSICOBJECT__HPP_

class physicObject {
protected:
  physicObject(float mass = 1.f);
  ~physicObject() = default;

protected:
  float getMass();

private:
  float mass = 0.f;
};

#endif
