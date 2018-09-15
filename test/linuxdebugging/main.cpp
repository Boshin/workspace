int func1(int a)
{
  int b = a + 1;
  return b;
}

int func0(int a)
{
  int b = func1(a);
  return b;
}

int main()
{
  int a = 1234;
  func0(a);
  return 0;
}
