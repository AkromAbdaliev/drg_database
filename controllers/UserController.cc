#include "UserController.h"

// Add definition of your processing function here
void UserController::getUsers(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) const
{
    auto clientrPtr = app().getDbClient();

    auto future = clientrPtr->execSqlAsyncFuture("SELECT * FROM users");

    try{
        auto result = future.get();
        Json::Value jsonResponse(Json::arrayValue);
        for (const auto &row : result)
        {
            Json::Value userJson;
            userJson["id"] = row["id"].as<int>();
            userJson["name"] = row["name"].as<std::string>();
            userJson["phone"] = row["phone"].as<std::string>();
            jsonResponse.append(userJson);
        }

        auto res = HttpResponse::newHttpJsonResponse(jsonResponse);
        callback(res);
    }catch(drogon::orm::DrogonDbException &e)
    {
        Json::Value  error;
        error["error"] = e.base().what();

        auto resp = HttpResponse::newHttpJsonResponse(error);
        resp->setStatusCode(k500InternalServerError);
        callback(resp);
    }
}

void UserController::getUser(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int uId) const
{
    auto clientrPtr = app().getDbClient();

    auto future = clientrPtr->execSqlAsyncFuture("SELECT * FROM users WHERE id = $1", uId);

    try{
        auto result = future.get();
        if(result.empty())
        {
            Json::Value error;
            error["error"] = "User not found.";
            auto res = HttpResponse::newHttpJsonResponse(error);
            callback(res);
            return;
        }

        const auto &row = result.front();
        Json::Value userJson;
        userJson["id"] = row["id"].as<int>();
        userJson["name"] = row["name"].as<std::string>();
        userJson["phone"] = row["phone"].as<std::string>();

        auto  res = HttpResponse::newHttpJsonResponse(userJson);
        callback(res);
    }catch(drogon::orm::DrogonDbException &e)
    {
        Json::Value  error;
        error["error"] = e.base().what();

        auto resp = HttpResponse::newHttpJsonResponse(error);
        resp->setStatusCode(k500InternalServerError);
        callback(resp);
    }

}














