#pragma once

#include <drogon/HttpController.h>
#include <json/json.h>

using namespace drogon;

class ProductController : public drogon::HttpController<ProductController>
{
  public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(ProductController::get, "/{2}/{1}", Get); // path is /ProductController/{arg2}/{arg1}
    // METHOD_ADD(ProductController::your_method_name, "/{1}/{2}/list", Get); // path is /ProductController/{arg1}/{arg2}/list
    // ADD_METHOD_TO(ProductController::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
    ADD_METHOD_TO(ProductController::getProducts, "/products", Get);
    ADD_METHOD_TO(ProductController::getProduct, "/products/{1}", Get);
    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
    void getProducts(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback) const;
    void getProduct(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int pId) const;
};
