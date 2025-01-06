#include "menuselectoptionnumarrow.h"

#include "ncurseswrap.h"

MenuSelectOptionNumArrow::MenuSelectOptionNumArrow(std::string identifier, int row, int col, std::string label, int value, int min, int max) {
  arrow = NumInputArrow(value, min, max);
  init(identifier, row, col, label);
}

FmtString MenuSelectOptionNumArrow::getContentText() const {
  return arrow.getVisual();
}

bool MenuSelectOptionNumArrow::inputChar(int ch) {
  return arrow.inputChar(ch);
}

bool MenuSelectOptionNumArrow::activate() {
  arrow.activate();
  return true;
}

void MenuSelectOptionNumArrow::deactivate() {
  arrow.deactivate();
}

int MenuSelectOptionNumArrow::getData() const {
  return arrow.getValue();
}

void MenuSelectOptionNumArrow::setData(int value) {
  arrow.setValue(value);
}

std::string MenuSelectOptionNumArrow::getLegendText() const {
  return "[Enter] Finish editing - Enter minutes (0 for unlimited, 1-9999) - [Backspace] Delete";
}

void MenuSelectOptionNumArrow::setSubstituteText(int value, const std::string & text) {
  arrow.setSubstituteText(value, text);
}
