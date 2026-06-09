#pragma once
#include "Customer.h"

class RecommendationEngine {
public:
    static vector<Product>
    recommendProducts(
        Customer& customer,
        vector<Product>& products,
        int topN
    );
};