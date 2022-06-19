int diff(int a, int b) 
{
	if (a > b)
        return a - b;
    else
        return b - a;
}

int digits(int a)
{
    int count = 0;
    
    do {
        a /= 10;
        ++count;
  } while (a != 0);

  return count;
}