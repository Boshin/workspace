#define USINGSTACK __attribute__((regparm(0)))

class Test
{
public:
  Test():number(3333){}
  void USINGSTACK func2(int a, int b)
  {
    a++;
    b += 2;
  }
private:
  int number;
};

int main(int argc, char* argv[])
{
  Test tInst;
  tInst.func2(1111, 2222);
  return 0;
}
