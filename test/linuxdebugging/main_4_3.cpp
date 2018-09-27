#include <stdio.h>

class Point2d{
public:
  virtual void Foo(){}
  virtual void Boo(){}
  virtual void non_overwrite(){}

private:
  void priTest(){}

protected:
  void proTest(){}
  float _x, _y;
};

class Vertex: public virtual  Point2d{
public:
  virtual void Foo(){}
  virtual void BooVer(){}
protected:
  Vertex *next;
};

class Point3d: public virtual Point2d{
public:
  virtual void Boo3d(){}
protected:
  float _z;
};

class Vertex3d: public Vertex, public Point3d{
public:
  void test(){}
protected:
  float mumble;
};

int main(int argc, char* argv[])
{
  Vertex3d obj;
  obj.test();
  int nValue = 0;
  nValue = 05 << 8;
  nValue |= 01;
  printf("nValue: %d\n", nValue);
  return 0;
}
