#include "ProductController.h"

// Add definition of your processing function here
void ProductController::getProducts(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) const
{
    auto clientPtr  = app().getDbClient();
    clientPtr->execSqlAsync(
        "SELECT * FROM product",
        [callback](const drogon::orm::Result &result)
        {
            Json::Value jsonResponse = Json::arrayValue;
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
        [](const drogon::orm::DrogonDbException &e) {
            std::cerr << "error:" << e.base().what() << std::endl;}
        );
}
