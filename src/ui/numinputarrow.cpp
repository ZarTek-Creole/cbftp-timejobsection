#include "numinputarrow.h"
#include "ncurseswrap.h"

NumInputArrow::NumInputArrow() {

}

NumInputArrow::NumInputArrow(int startval, int min, int max) {
  val = startval;
  active = false;
  selected = false;
  this->min = min;
  this->max = max;
  inputBuffer = "";
}

bool NumInputArrow::decrease() {
  if (!options.empty()) {
    // Find current position in options
    for (size_t i = 0; i < options.size(); ++i) {
      if (options[i].second == val) {
        if (i > 0) {
          setValue(options[i-1].second);
          return true;
        }
        break;
      }
    }
  }
  return false;
}

bool NumInputArrow::increase() {
  if (!options.empty()) {
    // Find current position in options
    for (size_t i = 0; i < options.size(); ++i) {
      if (options[i].second == val) {
        if (i < options.size() - 1) {
          setValue(options[i+1].second);
          return true;
        }
        break;
      }
    }
  }
  return false;
}

bool NumInputArrow::setValue(int newval) {
  if (newval == 0) {
    val = -1; // Convert 0 to unlimited (-1)
    return true;
  }
  if ((newval == -1) || (newval >= 1 && newval <= max)) {
    val = newval;
    return true;
  }
  return false;
}

int NumInputArrow::getValue() const {
  return val;
}

void NumInputArrow::activate() {
  active = true;
  if (val == -1) {
    inputBuffer = "0"; // Show 0 for unlimited
  }
  else {
    inputBuffer = std::to_string(val);
  }
  // Sélectionner tout le texte pour qu'il soit remplacé à la prochaine saisie
  selected = true;
}

void NumInputArrow::deactivate() {
  active = false;
  selected = false;
  if (!inputBuffer.empty()) {
    try {
      int newval = std::stoi(inputBuffer);
      setValue(newval);
    }
    catch (...) {
      // Invalid input, keep old value
    }
  }
  inputBuffer = "";
}

std::string NumInputArrow::getVisual() const {
  if (active) {
    if (inputBuffer == "0") {
      return "unlimited";
    }
    return inputBuffer;
  }
  else {
    // Check if we have a predefined option for this value
    for (const auto & opt : options) {
      if (opt.second == val) {
        return opt.first;
      }
    }
    
    // Default display
    if (val == -1) {
      return "unlimited";
    }
    return std::to_string(val) + "m";
  }
}

void NumInputArrow::setSubstituteText(int value, const std::string & text) {
  substituteTexts[value] = text;
}

bool NumInputArrow::inputChar(int ch) {
  if (ch >= '0' && ch <= '9') {
    // Si le texte est sélectionné, on le remplace
    if (selected) {
      inputBuffer = static_cast<char>(ch);
      selected = false;
    }
    else {
      inputBuffer += static_cast<char>(ch);
    }
    try {
      int val = std::stoi(inputBuffer);
      if (val > max) {
        inputBuffer = std::to_string(max); // Limiter à la valeur maximale
        return true;
      }
    }
    catch (...) {
      inputBuffer.pop_back();
      return false;
    }
    return true;
  }
  else if (ch == 127 || ch == 8 || ch == KEY_BACKSPACE) { // Backspace
    if (!inputBuffer.empty()) {
      if (selected) {
        inputBuffer = "0";
        selected = false;
      }
      else {
        inputBuffer.pop_back();
        if (inputBuffer.empty()) {
          inputBuffer = "0"; // Reset to 0 when empty
        }
      }
      return true;
    }
  }
  else if (ch == '0' && inputBuffer.empty()) {
    inputBuffer = "0";
    selected = false;
    return true;
  }
  return false;
}

void NumInputArrow::addOption(const std::string & text, int value) {
  options.push_back(std::make_pair(text, value));
}

void NumInputArrow::setOption(int value) {
  setValue(value);
}

const std::vector<std::pair<std::string, int>> & NumInputArrow::getOptions() const {
  return options;
}
