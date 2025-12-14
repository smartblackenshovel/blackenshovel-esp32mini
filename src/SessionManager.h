#pragma once
#include <Arduino.h>

#include <vector>

#include "ISessionUpdater.h"
#include "dataAccess/SerialPortWriter.h"
#include "gnss/GNSSController.h"
#include "http/HTTPHandler.h"
#include "imu/IMUController.h"
#include "imu/IMU.h"
#include "models/User.h"
#include "models/Location.h"
#include "models/Spot.h"

#define shovelSerialNumber "BS-#13823429-02"

class SessionManager : public ISessionUpdater {
 public:
  SessionManager(SerialPortWriter& serialPortWriter, HTTPHandler& httpHandler,
                 GNSSController& gnssController, IMUController& imuController);
  void setUsers(std::vector<User> newUsers) override;
  std::vector<User> getUsers() const { return users; }
  void selectUser(String name) override;
  User* getSessionUser() const { return selectedUser; }

  void beginSession();
  void updateSession();

 private:
  std::vector<User> users;
  User* selectedUser = nullptr;

  SerialPortWriter& serialPortWriter;

  HTTPHandler& httpHandler;
  GNSSController& gnssController;
  IMUController& imuController;

  Location userLoc;
  IMU userImu;


  String shovelId;
  String organizationId;
  String sessionId;
  Spot* spot = nullptr;

  void fetchOrganization();
  void fetchUsers();
  void createSession();
  void fetchSpot();

  String endpointOrganizations = "/organizations";
  String endpointShovels = "/shovels";
  String endpointUsers = "/users";
  String endpointSpots = "/spots";
  String endpointSessions = "/sessions";
  String endpointSpotLogs = "/spot_logs";
  String endpointSessionLogs = "/session_logs";
};
