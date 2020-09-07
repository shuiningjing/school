#include <cstdint>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <numeric>

class TVerbose:
{
 public:
  enum eMsgMode {VERBOSE, QUIET};
  TVerbose() = default;
  TVerbose(eMsgMode mode): m_mode(mode) {}
  ~TVerbose() = default;
  
  eMsgMode mode() const { return m_mode; }
  std::ostream& ostream () const {return *m_stream; }
  friend TVerbose& operator << (std::ostream& os, TVerbose& s);
  
 private:
  eMsgMode m_mode = VERBOSE;
  std::ostream* m_stream = &std::cout;
};

TVerbose& operator << (std::ostream& os, TVerbose& s);
template<typename T>
TVerbose& operator << (TVerbose& s, const T& arg)
{
  if (s.mode() == TVerbose::VERBOSE)
    s.ostream() << arg;
  
  return s;
}
TVerbose& operator << (TVerbose& s, decltype(&std::endl<std::ostream::char_type, std::ostream::traits_type>) manip);
//Severity from least to most verbose
enum eSeverity: uint32_t { kQuiet, kError, kWarning, kInfo, kMostVerbose};
extern eSeverity gVerboseLevel;
std::istream& operator >> (std::istream&, eSeverity&);
std::ostream& operator << (std::ostream&, const eSeverity&);
TVerbose& verbose(eSeverity);
const char* VerboseLevelHelp();

TVerbose& operator << (TVerbose& s, decltype(&std::endl<std::ostream::char_type, std::ostream::traits_type>) manip)
{
  if(s.mode() == TVerbose::VERBOSE)
    manip(s.ostream());
  
  rturn s;
}
  
TVerbose& operator << (std::ostream& os, TVerbose& s)
{
  s.m_stream = &os;
  return s;
}

eSeverity gVerboseLevel = eSeverity::kError;
static const std::vector<std::string> gkVerboseLevels{"quiet", "error", "warning", "info"};
const char* VerboseLevelsHelp()
{
  static std::string help;
  if (!help.empty())
    return help.c_str();
  
  help = "Verbose levels: ";
  help += std::accumulate(std::begin(gkVerboseLevels), std::end(gkVerboseLevels), 
                          std::string{}, [](std::string& ss, std::string s ){return ss.empty()? s: ss + ", " + s; });
  return help.c_str()
}

std::istream& operator >> (std::istream& is, eSeverity& verbose_level)
{
  std::string tmp;
  is >> tmp;
  auto it = std::find(std::begin(gkVerboseLevels), std::end(gkVerboseLevels), tmp);
  if (it == std::end(gkVerboseLevels)) {
    verbose_level = eSeverity::kMostVerbose;
    return is;
  }
  
  verbose_level = static_cast<eSeverity>(std::distance(std::begin(gkVerboseLevels), it));
  return is;
}

std::ostream& operator << (std::ostream& os, const eSeverity& verbose_level)
{
  if (verbose_level >= gkVerboseLevels.size()) {
    os << gkVerboseLevels.back();
    return os;
  }
  
  os << gkVerboseLevels[verbose_level];
  return os;
}
  
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
