#define STACKCALL __attribute__((regparm(3)))

void STACKCALL func4(int a, int b, int c, int d)
{
  a++;
  b += 2;
  c += 3;
  d += 4;
}

int main()
{
  func4(1111, 2222, 3333, 4444);
  return 0;
}
