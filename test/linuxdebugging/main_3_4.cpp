class Test
{
public:
  Test():number(5555){}
  void func9(int a, int b, int c, int d,char*str, long e, long f, float h, double i)
  {
    a++;
    b += 2;
    c += 3;
    d += 4;
  }
private:
  int number;
};

int main(int argc, char* argv[])
{
  Test tInst;
  tInst.func9(1111, 2222, 3333, 4444, "hello, world!", 6666,7777, 8.888, 9.999);
  return 0;
}
