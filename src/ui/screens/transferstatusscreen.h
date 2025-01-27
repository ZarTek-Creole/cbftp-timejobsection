#pragma once

#include <map>
#include <memory>
#include <string>

#include "../uiwindow.h"
#include "../menuselectoption.h"

class Ui;
class TransferStatus;
class MenuSelectOptionElement;
class TransferStatus;

class TransferStatusScreen : public UIWindow {
public:
  TransferStatusScreen(Ui *);
  ~TransferStatusScreen();
  void initialize(unsigned int, unsigned int, std::shared_ptr<TransferStatus>);
  void redraw() override;
  bool keyPressed(unsigned int) override;
  std::string getLegendText() const override;
  std::string getInfoLabel() const override;
private:
  std::shared_ptr<TransferStatus> ts;
  std::shared_ptr<MenuSelectOptionElement> activeelement;
  std::string defaultlegendtext;
  std::string currentlegendtext;
  std::string abortedlegendtext;
};
