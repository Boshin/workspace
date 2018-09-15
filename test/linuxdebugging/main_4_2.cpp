class Point2d{
public:
  virtual void Foo(){}
  virtual void Boo(){}
  virtual void non_overwrite(){}
protected:
  float _x, _y;
};

class Vertex: public  Point2d{
public:
  virtual void Foo(){}
  virtual void BooVer(){}
protected:
  Vertex *next;
};

class Point3d: public Point2d{
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
  return 0;
}
