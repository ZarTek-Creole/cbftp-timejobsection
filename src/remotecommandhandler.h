#pragma once

#include <string>
#include <vector>

#include "core/eventreceiver.h"
#include "engine.h"

enum class RemoteCommandNotify {
  DISABLED,
  ACTION_REQUESTED,
  JOBS_ADDED,
  ALL_COMMANDS
};

enum class ListeningMode {
  LOCALHOST,
  ALL
};

class RemoteCommandHandler : private Core::EventReceiver {
private:
  bool enabled;
  bool encrypted;
  std::string password;
  int port;
  int sockid;
  bool retrying;
  bool connected;
  RemoteCommandNotify notify;
  bool listenall;
  void connect();
  void disconnect();
  void handleMessage(const std::string & message);
  void stopRetry();
  JobStartResult commandRace(const std::vector<std::string> &);
  JobStartResult commandDistribute(const std::vector<std::string> &);
  JobStartResult commandPrepare(const std::vector<std::string> &);
  void commandRaw(const std::vector<std::string> &);
  void commandRawWithPath(const std::vector<std::string> &);
  bool commandFXP(const std::vector<std::string> &);
  bool commandDownload(const std::vector<std::string> &);
  bool commandDownloadToPath(const std::vector<std::string> &);
  bool commandUpload(const std::vector<std::string> &);
  void commandIdle(const std::vector<std::string> &);
  void commandAbort(const std::vector<std::string> &);
  void commandDelete(const std::vector<std::string> &);
  void commandAbortDeleteIncomplete(const std::vector<std::string> &);
  void commandReset(const std::vector<std::string> &, bool);
  JobStartResult parseRace(const std::vector<std::string> &, int);
public:
  RemoteCommandHandler();
  bool isEnabled() const;
  bool isEncrypted() const;
  int getUDPPort() const;
  std::string getPassword() const;
  bool getListenAll() const;
  void setPassword(const std::string &);
  void setPort(int);
  RemoteCommandNotify getNotify() const;
  void setNotify(RemoteCommandNotify notify);
  void setEnabled(bool enable);
  void setEncrypted(bool encrypted);
  void setListenAll(bool listenall);
  void FDData(int, char *, unsigned int);
  void FDFail(int, const std::string &);
  void tick(int);
};
