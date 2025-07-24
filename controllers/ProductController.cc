#include "ProductController.h"

// Add definition of your processing function here
void ProductController::getProducts(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) const
{
    auto clientPtr  = app().getDbClient();
    clientPtr->execSqlAsync(
        "SELECT * FROM product",
        [](const drogon::orm::Result &result)
        {
            std::cout << result.size() << " rows selected!" << std::endl;
            int i = 1;
            for (auto row : result)
            {
                std::cout << i++ << ": product name is " << row["name"].as<std::string>() << std::endl;
            }
        },
        [](const drogon::orm::DrogonDbException &e) {
            std::cerr << "error:" << e.base().what() << std::endl;
        }
        );
}
