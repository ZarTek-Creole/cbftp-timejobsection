#include "editsectionscreen.h"

#include <cassert>

#include "../../globalcontext.h"
#include "../../section.h"
#include "../../sectionmanager.h"

#include "../ui.h"
#include "../menuselectoptionelement.h"
#include "../menuselectoptionnumarrow.h"
#include "../menuselectoptiontextfield.h"
#include "../menuselectoptiontextbutton.h"
#include "../menuselectoptiontextarrow.h"

namespace {

void addSectionTimeOptions(std::shared_ptr<MenuSelectOptionTextArrow>& arrow) {
  arrow->addOption("15s", 15);
  arrow->addOption("30s", 30);
  arrow->addOption("1m", 60);
  arrow->addOption("1m30s", 90);
  arrow->addOption("2m", 120);
  arrow->addOption("3m", 180);
  arrow->addOption("5m", 300);
  arrow->addOption("10m", 600);
  arrow->addOption("15m", 900);
  arrow->addOption("30m", 1800);
  arrow->addOption("1h", 3600);
  arrow->addOption("2h", 7200);
  arrow->addOption("4h", 14400);
  arrow->addOption("8h", 28800);
  arrow->addOption("12h", 43200);
  arrow->addOption("24h", 86400);
  arrow->addOption("Unlimited", 0);
  arrow->addOption("Use global", -1);
}

}

EditSectionScreen::EditSectionScreen(Ui* ui) : UIWindow(ui, "EditSectionScreen"), section(nullptr), mso(*vv) {
  keybinds.addBind(10, KEYACTION_ENTER, "Modify");
  keybinds.addBind(KEY_DOWN, KEYACTION_DOWN, "Next option");
  keybinds.addBind(KEY_UP, KEYACTION_UP, "Previous option");
  keybinds.addBind(KEY_LEFT, KEYACTION_LEFT, "Previous value");
  keybinds.addBind(KEY_RIGHT, KEYACTION_RIGHT, "Next value");
  keybinds.addBind('d', KEYACTION_DONE, "Done");
  keybinds.addBind('c', KEYACTION_BACK_CANCEL, "Cancel");
  keybinds.addBind('S', KEYACTION_SKIPLIST, "Skiplist");
}

EditSectionScreen::~EditSectionScreen() {

}

void EditSectionScreen::initialize(unsigned int row, unsigned int col, const std::string & section) {
  active = false;
  if (this->section != nullptr) {
    delete this->section;
  }
  if (section == "") {
    mode = Mode::ADD;
    this->section = new Section();
    oldname = "";
  }
  else {
    mode = Mode::EDIT;
    this->section = new Section(*global->getSectionManager()->getSection(section));
    oldname = section;
  }
  unsigned int y = 1;
  unsigned int x = 1;
  mso.clear();
  mso.addStringField(y++, x, "name", "Name:", section, false, 64);
  std::shared_ptr<MenuSelectOptionNumArrow> hotkeyarrow = mso.addIntArrow(y++, x, "hotkey", "Hotkey:", this->section->getHotKey(), -1, 9);
  hotkeyarrow->setSelectable(true);
  std::shared_ptr<MenuSelectOptionTextArrow> maxtime = mso.addTextArrow(y++, x, "maxtime", "Max Time (seconds):");
  addSectionTimeOptions(maxtime);
  maxtime->setOption(this->section->getMaxTime());
  init(row, col);
}

void EditSectionScreen::redraw() {
  vv->clear();
  bool highlight;
  for (unsigned int i = 0; i < mso.size(); i++) {
    std::shared_ptr<MenuSelectOptionElement> msoe = mso.getElement(i);
    highlight = false;
    if (mso.isFocused() && mso.getSelectionPointer() == i) {
      highlight = true;
    }
    vv->putStr(msoe->getRow(), msoe->getCol(), msoe->getLabelText(), highlight);
    vv->putStr(msoe->getRow(), msoe->getCol() + msoe->getLabelText().length() + 1, msoe->getContentText());
  }
  std::shared_ptr<MenuSelectOptionElement> msoe = mso.getElement(mso.getLastSelectionPointer());
  vv->putStr(msoe->getRow(), msoe->getCol(), msoe->getLabelText());
  vv->putStr(msoe->getRow(), msoe->getCol() + msoe->getLabelText().length() + 1, msoe->getContentText());
  msoe = mso.getElement(mso.getSelectionPointer());
  vv->putStr(msoe->getRow(), msoe->getCol(), msoe->getLabelText(), true);
  vv->putStr(msoe->getRow(), msoe->getCol() + msoe->getLabelText().length() + 1, msoe->getContentText());
  if (active && msoe->cursorPosition() >= 0) {
    ui->showCursor();
    vv->moveCursor(msoe->getRow(), msoe->getCol() + msoe->getLabelText().length() + 1 + msoe->cursorPosition());
  }
  else {
    ui->hideCursor();
  }
}

bool EditSectionScreen::keyPressed(unsigned int ch) {
  int action = keybinds.getKeyAction(ch);
  if (active) {
    if (ch == 10) {
      activeelement->deactivate();
      active = false;
      if (activeelement->getIdentifier() == "name") {
        std::string text = std::static_pointer_cast<MenuSelectOptionTextField>(activeelement)->getData();
        if (text.empty()) {
          ui->goInfo("Invalid name. Section names cannot be empty.");
          return true;
        }
        if (mode == Mode::ADD && global->getSectionManager()->getSection(text) != NULL) {
          ui->goInfo("A section with that name already exists.");
          return true;
        }
      }
      ui->update();
      return true;
    }
    activeelement->inputChar(ch);
    ui->update();
    return true;
  }
  bool activation;
  switch(action) {
    case KEYACTION_ENTER:
      activation = mso.activateSelected();
      if (!activation) {
        ui->update();
      }
      else {
        active = true;
        activeelement = mso.getElement(mso.getSelectionPointer());
        activeelement->activate();
        ui->update();
      }
      return true;
    case KEYACTION_UP:
      mso.goUp();
      ui->update();
      return true;
    case KEYACTION_DOWN:
      mso.goDown();
      ui->update();
      return true;
    case KEYACTION_LEFT:
    case KEYACTION_RIGHT: {
      std::shared_ptr<MenuSelectOptionElement> msoe = mso.getElement(mso.getSelectionPointer());
      if (!msoe) {
        return false;
      }
      if (msoe->getIdentifier() == "maxtime") {
        std::shared_ptr<MenuSelectOptionTextArrow> arrow = std::static_pointer_cast<MenuSelectOptionTextArrow>(msoe);
        if (!arrow->isActive()) {
          arrow->activate();
        }
        arrow->inputChar(ch);
        ui->redraw();
        return true;
      }
      if (action == KEYACTION_LEFT) {
        if (mso.goLeft()) {
          ui->redraw();
          return true;
        }
      }
      else {
        if (mso.goRight()) {
          ui->redraw();
          return true;
        }
      }
      return false;
    }
    case KEYACTION_DONE:
    {
      std::string name = std::static_pointer_cast<MenuSelectOptionTextField>(mso.getElement("name"))->getData();
      if (name.empty()) {
        ui->goInfo("Invalid name. Section names cannot be empty.");
        return true;
      }
      if (mode == Mode::ADD && global->getSectionManager()->getSection(name) != NULL) {
        ui->goInfo("A section with that name already exists.");
        return true;
      }
      section->setName(name);
      section->setHotKey(std::static_pointer_cast<MenuSelectOptionNumArrow>(mso.getElement("hotkey"))->getData());
      section->setMaxTime(std::static_pointer_cast<MenuSelectOptionTextArrow>(mso.getElement("maxtime"))->getData());
      if (mode == Mode::ADD) {
        global->getSectionManager()->addSection(*section);
      }
      else {
        global->getSectionManager()->replaceSection(*section, oldname);
      }
      ui->returnToLast();
      return true;
    }
    case KEYACTION_BACK_CANCEL:
      ui->returnToLast();
      return true;
    case KEYACTION_SKIPLIST:
      ui->goSkiplist(&section->getSkipList());
      return true;
  }
  return false;
}

std::string EditSectionScreen::getLegendText() const {
  if (active) {
    return activeelement->getLegendText();
  }
  return keybinds.getLegendSummary();
}

std::string EditSectionScreen::getInfoLabel() const {
  if (mode == Mode::ADD) {
    return "ADD SECTION";
  }
  return "EDIT SECTION: " + oldname;
}
