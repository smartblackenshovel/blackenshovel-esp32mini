#include "SessionManager.h"
#include "utils/utils.h"
#include "utils/JsonUtils.h"

SessionManager::SessionManager(SerialPortWriter& serialPortWriter,
                               HTTPHandler& httpHandler,
                               GNSSController& gnssController,
                               IMUController& imuController)
    : serialPortWriter(serialPortWriter),
      httpHandler(httpHandler),
      gnssController(gnssController),
      imuController(imuController),
      userLoc(0, 0) {}

void SessionManager::selectUser(String id) {
  if (selectedUser != nullptr) {
    return;
  }
  for (User& user : users) {
    if (user.getId() == id) {
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
    users = parseUsers(jsonUsers);
  } else {
    Serial.printf("Failed to fetch users. Status code: %d\n",
                  usersResponse.getStatusCode());
  }
}

void SessionManager::fetchShovel() {
  HTTPResponse shovelsResponse =
      httpHandler.get(endpointShovels, {{"serial_number", shovelSerialNumber}});

  if (shovelsResponse.isSuccess()) {
    Serial.println("Shovel fetched successfully.");
    Serial.println(shovelsResponse.getContent());
    shovelId = extractValues(shovelsResponse.getContentAsJson(), "id")[0];
    organizationId = extractValues(shovelsResponse.getContentAsJson(), "organization_id")[0];
  } else {
    Serial.printf("Failed to fetch shovel. Status code: %d\n",
                  shovelsResponse.getStatusCode());
  }
}

void SessionManager::fetchOrganization() {
  HTTPResponse organizationsResponse =
      httpHandler.get(endpointOrganizations + "/" + organizationId);

  if (organizationsResponse.isSuccess()) {
    Serial.println("Organization fetched successfully.");
    Serial.println(organizationsResponse.getContent());
  } else {
    Serial.printf("Failed to fetch organization. Status code: %d\n",
                  organizationsResponse.getStatusCode());
  }
}

void SessionManager::updateSession() {
  if (sessionId.isEmpty()) {
    createSession();
    return;
  }

  fetchSpots();
  calcNextSpot();

  userLoc = gnssController.updateLocation();
  userImu = imuController.read();

  const Accelerometer& accel = userImu.getAccelerometer();
  const Gyroscope& gyro = userImu.getGyroscope();

  selectedUser->setLocation(userLoc.getLatitude(), userLoc.getLongitude());

  serialPortWriter.writeImu(userImu);
  serialPortWriter.writeUserLoc(userLoc);
  serialPortWriter.writeSpotLoc(nextSpot->getLocation());

  // TO DO Write Session Log
}

void SessionManager::beginSession() {
  if (organizationId.isEmpty()) {
    fetchShovel();
    fetchOrganization();
  }
  if (users.empty()) {
    fetchUsers();
  }
}

void SessionManager::createSession() {
  if (selectedUser == nullptr) {
    serialPortWriter.writeUsers(users);
    return; 
  }
  JsonDocument sessionPayload;
  sessionPayload["user_id"] = selectedUser->getId();
  sessionPayload["shovel_id"] = shovelId;

  HTTPResponse sessionResponse =
      httpHandler.post(endpointSessions, sessionPayload);

  if (sessionResponse.isSuccess()) {
    Serial.println("Session created successfully.");
    Serial.println(sessionResponse.getContent());
    sessionId = extractValues(sessionResponse.getContentAsJson(), "id")[0];
  } else {
    Serial.printf("Failed to create session. Status code: %d\n",
                  sessionResponse.getStatusCode());
    return;
  }
}

void SessionManager::createSessionLog() {
    JsonDocument sessionLogPayload;
    sessionLogPayload["session_id"] = sessionId;
    sessionLogPayload["latitude"] = userLoc.getLatitude();
    sessionLogPayload["longitude"] = userLoc.getLongitude();
    sessionLogPayload["timestamp"] = currentTime();

    HTTPResponse sessionLogResponse = httpHandler.post(
        endpointSessionLogs,
        sessionLogPayload
    );

    if (sessionLogResponse.isSuccess()) {
        Serial.println("Session log posted successfully.");
        Serial.println(sessionLogResponse.getContent());
    } else {
        Serial.printf("Failed to post session log. Status code: %d\n", sessionLogResponse.getStatusCode());
    }
}

void SessionManager::fetchSpots() {
  if (nextSpot != nullptr) { return; }
  HTTPResponse spotResponse = httpHandler.get(
    endpointSpots, {{"organization_id", organizationId}}
  );

  if (spotResponse.isSuccess()) {
    Serial.println("Spots requested successfully");
    Serial.println(spotResponse.getContent());
    JsonDocument jsonSpots = spotResponse.getContentAsJson();
    spots = parseSpots(jsonSpots);
  } else {
    Serial.printf("Failed to get spots. Status code: %d\n", spotResponse.getStatusCode());
  }
}

void SessionManager::calcNextSpot() {
  // TO DO
  nextSpot = &spots[0];
}