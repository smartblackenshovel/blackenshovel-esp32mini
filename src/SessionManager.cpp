#include "SessionManager.h"

#include "utils/JsonUtils.h"

SessionManager::SessionManager(SerialPortWriter& serialPortWriter,
                               HTTPHandler& httpHandler,
                               GNSSController& gnssController,
                               IMUController& imuController)
    : serialPortWriter(serialPortWriter),
      httpHandler(httpHandler),
      gnssController(gnssController),
      imuController(imuController),
      userLoc(0, 0) {        
      }

void SessionManager::setUsers(std::vector<User> newUsers) {
  if (!users.empty()) {
    return;
  }
  users = newUsers;
}

void SessionManager::selectUser(String name) {
  if (selectedUser != nullptr) {
    return;
  }
  for (User& user : users) {
    if (user.getName() == name) {
      selectedUser = &user;
    }
  }
}

void SessionManager::fetchUsers() {
  HTTPResponse usersResponse =
      httpHandler.get(endpointUsers, {{"organization_id", organizationId}});

  if (usersResponse.isSuccess()) {
    Serial.println("Users fetched successfully.");
    Serial.println(usersResponse.getContent());
    JsonDocument jsonUsers = usersResponse.getContentAsJson();
    setUsers(parseUsers(jsonUsers));
  } else {
    Serial.printf("Failed to fetch users. Status code: %d\n",
                  usersResponse.getStatusCode());
  }
}

void SessionManager::fetchOrganization() {
  HTTPResponse shovelsResponse =
      httpHandler.get(endpointShovels, {{"id", shovelId}});

  if (shovelsResponse.isSuccess()) {
    Serial.println("Shovel fetched successfully.");
    Serial.println(shovelsResponse.getContent());
    String organizationId =
        extractValues(shovelsResponse.getContentAsJson(), "organization_id")[0];

    HTTPResponse organizationsResponse =
        httpHandler.get(endpointOrganizations + "/" + organizationId);

    if (organizationsResponse.isSuccess()) {
      Serial.println("Organization fetched successfully.");
      Serial.println(organizationsResponse.getContent());
    } else {
      Serial.printf("Failed to fetch organization. Status code: %d\n",
                    organizationsResponse.getStatusCode());
    }
  } else {
    Serial.printf("Failed to fetch shovel. Status code: %d\n",
                  shovelsResponse.getStatusCode());
  }
}

void SessionManager::updateSession() {
    if (sessionId.isEmpty()) { createSession(); }
    userLoc = gnssController.updateLocation();
    userImu = imuController.read();

    const Accelerometer& accel = userImu.getAccelerometer();
    const Gyroscope& gyro = userImu.getGyroscope();

    serialPortWriter.writeImu(userImu);
    serialPortWriter.writeUserLoc(userLoc);

    if (spotId.isEmpty()) { fetchSpot(); }

    

    //Write Session Log
}

void SessionManager::createSession() {
    if (selectedUser == nullptr || shovelId.isEmpty()) {
      Serial.println("Cannot create session: User or Shovel ID is missing.");
      return;
    }

    JsonDocument sessionPayload;
    sessionPayload["user_id"] = selectedUser->getId();
    sessionPayload["shovel_id"] = shovelId;

    HTTPResponse sessionResponse = httpHandler.post(
      endpointSessions,
      sessionPayload
    );

    if (sessionResponse.isSuccess()) {
      Serial.println("Session created successfully.");
      Serial.println(sessionResponse.getContent());
      sessionId = extractValues(sessionResponse.getContentAsJson(), "id")[0];
    } else {
      Serial.printf("Failed to create session. Status code: %d\n", sessionResponse.getStatusCode());
      return;
    }
}

void SessionManager::fetchSpot() {
    // TO DO
}