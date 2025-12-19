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

void SessionManager::completeSpot(String id) {
  logMsg("Spot is finished, removing current");
  nextSpot = nullptr;
  pos++;
}

void SessionManager::fetchUsers() {
  #if HTTP_ACTIVE
  HTTPResponse usersResponse =
      httpHandler.get(endpointUsers, {{"organization_id", organizationId}});
  #else
  String jsonStr = R"([{"id":"9c1055f3-c3e5-4bdf-9ccb-66dd2fe57767","name":"Lauro Cesar Araujo","email":"lauro.andradearaujo@ost.ch","role_id":"1ca85099-977d-4017-af3d-e233c7193819","organization_id":"39eb05fa-f039-4404-a3dc-0ca5a1a47a6e","created_at":"2025-11-24T17:24:28.12153Z","updated_at":"2025-11-24T17:24:28.121531Z"},{"id":"c3d3f2af-626d-4fa6-b376-08964b21e3ac","name":"Daniel Hannemann","email":"daniel.hannemann@ost.ch","role_id":"1ca85099-977d-4017-af3d-e233c7193819","organization_id":"39eb05fa-f039-4404-a3dc-0ca5a1a47a6e","created_at":"2025-12-15T16:50:00.073574Z","updated_at":"2025-12-15T16:50:00.073574Z"}])";
  HTTPResponse usersResponse(200, jsonStr);
  #endif

  if (usersResponse.isSuccess()) {
    logMsg("Users fetched successfully.");
    logMsg(usersResponse.getContent());
    JsonDocument jsonUsers = usersResponse.getContentAsJson();
    users = parseUsers(jsonUsers);
  } else {
    Serial.printf("Failed to fetch users. Status code: %d\n",
                  usersResponse.getStatusCode());
  }
}

void SessionManager::fetchShovel() {
  #if HTTP_ACTIVE
  HTTPResponse shovelsResponse =
      httpHandler.get(endpointShovels, {{"serial_number", shovelSerialNumber}});
  #else
  String jsonStr = "[{\"id\":\"0d19e629-6f6f-47a3-aff7-2dbc7e018dde\",\"organization_id\":\"39eb05fa-f039-4404-a3dc-0ca5a1a47a6e\",\"serial_number\":\"BS-#13823429-02\",\"created_at\":\"2025-11-24T15:38:27.346901Z\",\"updated_at\":\"2025-11-24T15:38:27.346901Z\"}]";
  HTTPResponse shovelsResponse(200, jsonStr);
  #endif

  if (shovelsResponse.isSuccess()) {
    logMsg("Shovel fetched successfully.");
    logMsg(shovelsResponse.getContent());
    shovelId = extractValues(shovelsResponse.getContentAsJson(), "id")[0];
    organizationId = extractValues(shovelsResponse.getContentAsJson(), "organization_id")[0];
  } else {
    Serial.printf("Failed to fetch shovel. Status code: %d\n",
                  shovelsResponse.getStatusCode());
  }
}

void SessionManager::fetchOrganization() {
  #if HTTP_ACTIVE
  HTTPResponse organizationsResponse =
      httpHandler.get(endpointOrganizations + "/" + organizationId);
  #else
  String jsonStr = "{\"id\":\"39eb05fa-f039-4404-a3dc-0ca5a1a47a6e\",\"name\":\"Ostschweizer Fachhochschule\",\"legal_form\":\"OST CH\",\"registration_number\":\"CHE-987.8374.321\",\"street\":\"Oberseestrasse 10\",\"postal_code\":\"8645\",\"city\":\"Rapperswil-Jona\",\"country\":\"CH\",\"created_at\":\"2025-11-24T15:37:36.88547Z\",\"updated_at\":\"2025-11-24T15:37:36.88547Z\"}";
  HTTPResponse organizationsResponse(200, jsonStr);
  #endif

  if (organizationsResponse.isSuccess()) {
    logMsg("Organization fetched successfully.");
    logMsg(organizationsResponse.getContent());
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
  calcNextSpot(pos);

  #if DEBUG
  Location userLoc(47.2227642, 8.8166369);
  #else
  userLoc = gnssController.updateLocation();
  #endif
  userImu = imuController.read();

  const Accelerometer& accel = userImu.getAccelerometer();
  const Gyroscope& gyro = userImu.getGyroscope();

  selectedUser->setLocation(userLoc.getLatitude(), userLoc.getLongitude());

  serialPortWriter.writeImu(userImu);
  serialPortWriter.writeUserLoc(userLoc);
  serialPortWriter.writeSpot(*nextSpot);

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

  #if HTTP_ACTIVE
  HTTPResponse sessionResponse =
      httpHandler.post(endpointSessions, sessionPayload);
  #else
  String jsonStr = R"({
      "id":"5cbb3386-381c-4368-a90f-4b5d51bb19e7",
      "session_id":"86acd122-075f-4787-8a09-b5a8245feb68",
      "timestamp":"2025-11-26T16:40:53Z",
      "latitude":47.22312,
      "longitude":8.817105,
      "created_at":"2025-12-16T22:02:48.73936+01:00",
      "updated_at":"2025-12-16T22:02:48.73936+01:00"
  })";
  HTTPResponse sessionResponse(200, jsonStr);
  #endif

  if (sessionResponse.isSuccess()) {
    logMsg("Session created successfully.");
    logMsg(sessionResponse.getContent());
    sessionId = sessionResponse.getContentAsJson()["id"].as<String>();
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
        logMsg("Session logMsg posted successfully.");
        logMsg(sessionLogResponse.getContent());
    } else {
        Serial.printf("Failed to post session logMsg. Status code: %d\n", sessionLogResponse.getStatusCode());
    }
}

void SessionManager::fetchSpots() {
  if (nextSpot != nullptr) { return; }
  logMsg("fetching spots");
  #if HTTP_ACTIVE
  HTTPResponse spotResponse = httpHandler.get(
    endpointSpots, {{"organization_id", organizationId}}
  );
  #else
  String jsonStr = R"([
      {
          "id": "6529a8a0-cec7-42db-9316-d31ceb35be5b",
          "organization_id": "39eb05fa-f039-4404-a3dc-0ca5a1a47a6e",
          "latitude": 47.2229528,
          "longitude": 8.8172351,
          "created_at": "2025-12-15T17:31:11.284069Z",
          "updated_at": "2025-12-15T17:31:11.284069Z"
      },
      {
          "id": "6ebc112e-683d-4e39-ab3c-e459185c6d60",
          "organization_id": "39eb05fa-f039-4404-a3dc-0ca5a1a47a6e",
          "latitude": 47.222997,
          "longitude": 8.8169642,
          "created_at": "2025-12-15T17:31:35.197755Z",
          "updated_at": "2025-12-15T17:31:35.197755Z"
      },
      {
          "id": "ee8ccd3d-2ae2-41c6-806d-d44d45388f87",
          "organization_id": "39eb05fa-f039-4404-a3dc-0ca5a1a47a6e",
          "latitude": 47.2228526,
          "longitude": 8.817046,
          "created_at": "2025-12-15T17:31:55.471027Z",
          "updated_at": "2025-12-15T17:31:55.471028Z"
      },
      {
          "id": "18821e29-8753-45a4-8fae-755014cb3bbf",
          "organization_id": "39eb05fa-f039-4404-a3dc-0ca5a1a47a6e",
          "latitude": 47.2227815,
          "longitude": 8.8169843,
          "created_at": "2025-12-15T17:32:15.028645Z",
          "updated_at": "2025-12-15T17:32:15.028645Z"
      }
  ])";
  HTTPResponse spotResponse(200, jsonStr);
  #endif

  if (spotResponse.isSuccess()) {
    logMsg("Spots requested successfully");
    logMsg(spotResponse.getContent());
    JsonDocument jsonSpots = spotResponse.getContentAsJson();
    spots = parseSpots(jsonSpots);
  } else {
    Serial.printf("Failed to get spots. Status code: %d\n", spotResponse.getStatusCode());
  }
}

void SessionManager::calcNextSpot(int pos) {
  // TO DO
  if (nextSpot == nullptr) {
    logMsg("Calculating next spot!");
    nextSpot = &spots[pos];
  }
}