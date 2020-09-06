#include <iostream>

TVerbose& verbose(eSeverity level)
{
  static TVerbose vs;
  static TVerbose quiet {TVerbose::QUIET};
  if (level > gVerboseLevel)
    return quiet;
  
  return vs;
}

int main(int argc, char* argv[])
{
  std::cout << verbose(kInfo) << "Hello World" << std::endl;
  return 0;
}
