#include "ProductController.h"


// Add definition of your processing function here
void ProductController::getProducts(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) const
{
    auto clientPtr  = app().getDbClient();
    clientPtr->execSqlAsync(
        "SELECT * FROM product",
        [callback](const drogon::orm::Result &result)
        {
            Json::Value jsonResponse(Json::arrayValue);
            for (const auto &row : result)
            {
                Json::Value product;
                product["id"]  = row["id"].as<int>();
                product["name"] = row["name"].as<std::string>();
                product["price"] = row["price"].as<double>();
                jsonResponse.append(product);
            }
            auto res = HttpResponse::newHttpJsonResponse(jsonResponse);
            callback(res);
        },
            [callback](const drogon::orm::DrogonDbException& e) {

            Json::Value error;
            error["error"] = e.base().what();

            auto resp = HttpResponse::newHttpJsonResponse(error);
            resp->setStatusCode(k500InternalServerError);
            callback(resp);}
        );
}

void ProductController::getProduct(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int pId) const
{
    auto clientPtr = app().getDbClient();
    clientPtr->execSqlAsync(
        "SELECT * FROM product WHERE id = $1",
        [callback](const drogon::orm::Result &result)
        {
            if(result.empty())
            {
                Json::Value error;
                error["error"] = "Product not found.";
                auto res = HttpResponse::newHttpJsonResponse(error);
                callback(res);
                return;
            }

            const auto &row = result.front(); // equal to -> result[0]
            Json::Value product;
            product["id"] = row["id"].as<int>();
            product["name"] = row["name"].as<std::string>();
            product["price"] = row["price"].as<double>();

            auto res = HttpResponse::newHttpJsonResponse(product);
            callback(res);
        },
        [callback](const drogon::orm::DrogonDbException& e) {
        Json::Value error;
        error["error"] = e.base().what();

        auto resp = HttpResponse::newHttpJsonResponse(error);
        resp->setStatusCode(k500InternalServerError);
        callback(resp);
    }, pId);
}

