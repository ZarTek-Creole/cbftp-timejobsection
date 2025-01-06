#ifndef NUMINPUTARROW_H
#define NUMINPUTARROW_H

#include <string>
#include <map>
#include <vector>
#include <utility>

class NumInputArrow {
private:
  int val;
  bool active;
  bool selected;
  int min;
  int max;
  std::string inputBuffer;
  std::map<int, std::string> substituteTexts;
  std::vector<std::pair<std::string, int>> options;  // New: store predefined options

public:
  NumInputArrow();
  NumInputArrow(int startval, int min, int max);
  bool decrease();
  bool increase();
  bool setValue(int newval);
  int getValue() const;
  void activate();
  void deactivate();
  std::string getVisual() const;
  void setSubstituteText(int value, const std::string & text);
  bool inputChar(int ch);
  
  // New methods for options
  void addOption(const std::string & text, int value);
  void setOption(int value);
  const std::vector<std::pair<std::string, int>> & getOptions() const;
};

#endif
