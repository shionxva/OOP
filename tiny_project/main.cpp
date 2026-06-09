#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "Admin.h"
#include "Cart.h"
#include "Customer.h"
#include "Order.h"
#include "Product.h"
#include "RecommendationEngine.h"

using namespace std;

static const string kProductsFile = "products.txt";
static const string kInteractionsFile = "interactions.txt";
static const string kOrdersFile = "orders.txt";

// ─── I/O helpers ─────────────────────────────────────────────────────────────
static void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static int readInt(const string& prompt) {
    int v;
    cout << prompt;
    while (!(cin >> v)) {
        clearInput();
        cout << "  Enter a number: ";
    }
    clearInput();
    return v;
}

static double readDouble(const string& prompt) {
    double v;
    cout << prompt;
    while (!(cin >> v)) {
        clearInput();
        cout << "  Enter a number: ";
    }
    clearInput();
    return v;
}

static string readLine(const string& prompt) {
    string v;
    cout << prompt;
    getline(cin, v);
    return v;
}

// ─── Banner ───────────────────────────────────────────────────────────────────
static void banner() {
    cout << R"(
  ====================================================================
  |      _____ __  __    _    ____ _____   ____  _   _  ___  ____    |
  |     / ____|  \/  |  / \  |  _ \_   _| / ___|| | | |/ _ \|  _ \   |
  |     \___ \| |\/| | / _ \ | |_) || |   \___ \| |_| | | | | |_) |  |
  |      ___) | |  | |/ ___ \|  _ < | |    ___) |  _  | |_| |  __/   |
  |     |____/|_|  |_/_/   \_\_| \_\|_|   |____/|_| |_|\___/|_|      |
  |                                                                  |
  |          AI-Powered E-Commerce Platform  v1.0                    |
  ====================================================================
)" << "\n";
}

// ─── Data helpers ────────────────────────────────────────────────────────────
static vector<string> split(const string& s, char delim) {
    vector<string> parts;
    string item;
    istringstream ss(s);
    while (getline(ss, item, delim)) {
        parts.push_back(item);
    }
    return parts;
}

static string nowDateTime() {
    time_t t = time(nullptr);
    tm* localTime = localtime(&t);
    
    char buf[32];

    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", localTime);
    return string(buf);
}

static vector<Product> g_products;

static Product* findProductById(int id) {
    for (auto& p : g_products) {
        if (p.getId() == id) {
            return &p;
        }
    }
    return nullptr;
}

static void saveProducts() {
    ofstream out(kProductsFile);
    for (const auto& p : g_products) {
        out << p.getId() << "|" << p.getName() << "|" << p.getCategory()
            << "|" << p.getPrice() << "|" << p.getStock() << "|"
            << p.getViewCount() << "|" << p.getPurchaseCount() << "\n";
    }
}

static void loadProducts() {
    g_products.clear();
    ifstream in(kProductsFile);
    if (!in) {
        g_products.push_back(Product(1, "Wireless Mouse", "Electronics", 19.99, 40));
        g_products.push_back(Product(2, "Notebook", "Stationery", 3.49, 200));
        g_products.push_back(Product(3, "Water Bottle", "Outdoor", 12.95, 80));
        saveProducts();
        return;
    }

    string line;
    while (getline(in, line)) {
        if (line.empty()) {
            continue;
        }
        auto parts = split(line, '|');
        if (parts.size() < 7) {
            continue;
        }
        Product p(stoi(parts[0]), parts[1], parts[2], stod(parts[3]), stoi(parts[4]));
        p.setViewCount(stoi(parts[5]));
        p.setPurchaseCount(stoi(parts[6]));
        g_products.push_back(p);
    }
}

struct InteractionCounts {
    int views = 0;
    int purchases = 0;
};

static map<string, map<int, InteractionCounts>> loadAllInteractions() {
    map<string, map<int, InteractionCounts>> data;
    ifstream in(kInteractionsFile);
    if (!in) {
        return data;
    }
    string line;
    while (getline(in, line)) {
        if (line.empty()) {
            continue;
        }
        auto parts = split(line, '|');
        if (parts.size() < 4) {
            continue;
        }
        string username = parts[0];
        int productId = stoi(parts[1]);
        InteractionCounts counts;
        counts.views = stoi(parts[2]);
        counts.purchases = stoi(parts[3]);
        data[username][productId] = counts;
    }
    return data;
}

static void saveAllInteractions(const map<string, map<int, InteractionCounts>>& data) {
    ofstream out(kInteractionsFile);
    for (const auto& userEntry : data) {
        for (const auto& productEntry : userEntry.second) {
            out << userEntry.first << "|" << productEntry.first << "|"
                << productEntry.second.views << "|" << productEntry.second.purchases << "\n";
        }
    }
}

static void loadHistoryForUser(const string& username,
                               map<int, int>& views,
                               map<int, int>& purchases) {
    views.clear();
    purchases.clear();
    auto all = loadAllInteractions();
    auto it = all.find(username);
    if (it == all.end()) {
        return;
    }
    for (const auto& entry : it->second) {
        views[entry.first] = entry.second.views;
        purchases[entry.first] = entry.second.purchases;
    }
}

static void saveHistoryForUser(const string& username,
                               const map<int, int>& views,
                               const map<int, int>& purchases) {
    auto all = loadAllInteractions();
    auto& userMap = all[username];
    userMap.clear();
    for (const auto& entry : views) {
        InteractionCounts counts;
        counts.views = entry.second;
        counts.purchases = 0;
        userMap[entry.first] = counts;
    }
    for (const auto& entry : purchases) {
        userMap[entry.first].purchases = entry.second;
    }
    saveAllInteractions(all);
}

static int nextOrderId() {
    ifstream in(kOrdersFile);
    int maxId = 0;
    string line;
    while (getline(in, line)) {
        if (line.empty()) {
            continue;
        }
        auto parts = split(line, '|');
        if (parts.size() < 1) {
            continue;
        }
        maxId = max(maxId, stoi(parts[0]));
    }
    return maxId + 1;
}

static void appendOrder(const Order& order, const string& username) {
    ofstream out(kOrdersFile, ios::app);
    out << order.getId() << "|" << username << "|" << order.getDate() << "|"
        << order.getTotal() << "|";
    const auto& items = order.getItems();
    for (size_t i = 0; i < items.size(); ++i) {
        const auto& item = items[i];
        out << item.product.getId() << ":" << item.product.getName()
            << ":" << item.quantity << ":" << item.product.getPrice();
        if (i + 1 < items.size()) {
            out << ",";
        }
    }
    out << "\n";
}

// ─── Product ──────────────────────────────────────────────────────────────────
Product::Product() : id(0), price(0.0), stock(0), viewCount(0), purchaseCount(0) {}

Product::Product(int id, string name, string category, double price, int stock)
    : id(id), name(name), category(category), price(price), stock(stock), viewCount(0), purchaseCount(0) {}

void Product::display() const {
    cout << "[" << id << "] " << name << " (" << category << ") - $" << price
         << " | stock: " << stock << " | views: " << viewCount
         << " | purchased: " << purchaseCount << "\n";
}

void Product::increaseView() {
    ++viewCount;
}

void Product::increasePurchase(int qty) {
    purchaseCount += qty;
}

int Product::getId() const { return id; }
const string& Product::getName() const { return name; }
const string& Product::getCategory() const { return category; }
double Product::getPrice() const { return price; }
int Product::getStock() const { return stock; }
int Product::getViewCount() const { return viewCount; }
int Product::getPurchaseCount() const { return purchaseCount; }

void Product::setName(const string& value) { name = value; }
void Product::setCategory(const string& value) { category = value; }
void Product::setPrice(double value) { price = value; }
void Product::setStock(int value) { stock = value; }
void Product::setViewCount(int value) { viewCount = value; }
void Product::setPurchaseCount(int value) { purchaseCount = value; }

// ─── Cart ─────────────────────────────────────────────────────────────────────
void Cart::addItem(Product p, int qty) {
    for (auto& item : items) {
        if (item.product.getId() == p.getId()) {
            item.quantity += qty;
            return;
        }
    }
    items.emplace_back(p, qty);
}

void Cart::removeItem(int productId) {
    items.erase(remove_if(items.begin(), items.end(),
                          [productId](const CartItem& item) {
                              return item.product.getId() == productId;
                          }),
                items.end());
}

void Cart::updateQuantity(int productId, int qty) {
    for (auto& item : items) {
        if (item.product.getId() == productId) {
            item.quantity = qty;
            return;
        }
    }
}

double Cart::calculateTotal() {
    double total = 0.0;
    for (const auto& item : items) {
        total += item.product.getPrice() * item.quantity;
    }
    return total;
}

vector<CartItem>& Cart::getItems() { return items; }

// ─── Order ────────────────────────────────────────────────────────────────────
Order::Order() : orderId(0), totalPrice(0.0) {}

Order::Order(int id, const vector<CartItem>& items, double total, const string& date)
    : orderId(id), items(items), totalPrice(total), date(date) {}

void Order::display() {
    cout << "Order #" << orderId << " | " << date << " | Total: $" << totalPrice << "\n";
    for (const auto& item : items) {
        cout << "  - " << item.product.getName() << " x" << item.quantity
             << " ($" << item.product.getPrice() << ")\n";
    }
}

int Order::getId() const { return orderId; }
double Order::getTotal() const { return totalPrice; }
const string& Order::getDate() const { return date; }
const vector<CartItem>& Order::getItems() const { return items; }

// ─── Recommendation Engine ────────────────────────────────────────────────────
vector<Product> RecommendationEngine::recommendProducts(Customer& customer,
                                                        vector<Product>& products,
                                                        int topN) {
    struct ScoredProduct {
        Product product;
        double score;
    };

    map<string, int> categoryInterest;
    for (const auto& entry : customer.getPurchasedProducts()) {
        Product* p = findProductById(entry.first);
        if (p) {
            categoryInterest[p->getCategory()] += entry.second;
        }
    }

    vector<ScoredProduct> scored;
    for (const auto& p : products) {
        int views = 0;
        int purchases = 0;
        auto viewIt = customer.getViewedProducts().find(p.getId());
        if (viewIt != customer.getViewedProducts().end()) {
            views = viewIt->second;
        }
        auto purchaseIt = customer.getPurchasedProducts().find(p.getId());
        if (purchaseIt != customer.getPurchasedProducts().end()) {
            purchases = purchaseIt->second;
        }
        double categoryBonus = categoryInterest[p.getCategory()] > 0 ? 1.0 : 0.0;
        double score = views * 0.2 + purchases * 0.8 + categoryBonus;
        scored.push_back({p, score});
    }

    sort(scored.begin(), scored.end(), [](const ScoredProduct& a, const ScoredProduct& b) {
        return a.score > b.score;
    });

    vector<Product> result;
    for (int i = 0; i < topN && i < static_cast<int>(scored.size()); ++i) {
        result.push_back(scored[i].product);
    }
    return result;
}

// ─── Admin ────────────────────────────────────────────────────────────────────
void Admin::addProduct() {
    int nextId = 1;
    for (const auto& p : g_products) {
        nextId = max(nextId, p.getId() + 1);
    }
    string name = readLine("Product name: ");
    string category = readLine("Category: ");
    double price = readDouble("Price: ");
    int stock = readInt("Stock: ");
    g_products.push_back(Product(nextId, name, category, price, stock));
    saveProducts();
    cout << "Product added with ID " << nextId << ".\n";
}

void Admin::editProduct() {
    int id = readInt("Enter product ID to edit: ");
    Product* p = findProductById(id);
    if (!p) {
        cout << "Product not found.\n";
        return;
    }
    string name = readLine("New name (leave empty to keep): ");
    string category = readLine("New category (leave empty to keep): ");
    string priceStr = readLine("New price (leave empty to keep): ");
    string stockStr = readLine("New stock (leave empty to keep): ");

    if (!name.empty()) { p->setName(name); }
    if (!category.empty()) { p->setCategory(category); }
    if (!priceStr.empty()) { p->setPrice(stod(priceStr)); }
    if (!stockStr.empty()) { p->setStock(stoi(stockStr)); }
    saveProducts();
    cout << "Product updated.\n";
}

void Admin::deleteProduct() {
    int id = readInt("Enter product ID to delete: ");
    auto it = remove_if(g_products.begin(), g_products.end(),
                        [id](const Product& p) { return p.getId() == id; });
    if (it == g_products.end()) {
        cout << "Product not found.\n";
        return;
    }
    g_products.erase(it, g_products.end());
    saveProducts();
    cout << "Product deleted.\n";
}

void Admin::viewStatistics() {
    if (g_products.empty()) {
        cout << "No products available.\n";
        return;
    }
    const Product* mostViewed = &g_products[0];
    const Product* bestSelling = &g_products[0];
    for (const auto& p : g_products) {
        if (p.getViewCount() > mostViewed->getViewCount()) {
            mostViewed = &p;
        }
        if (p.getPurchaseCount() > bestSelling->getPurchaseCount()) {
            bestSelling = &p;
        }
    }

    auto all = loadAllInteractions();
    int activeUsers = 0;
    for (const auto& entry : all) {
        bool active = false;
        for (const auto& productEntry : entry.second) {
            if (productEntry.second.views > 0 || productEntry.second.purchases > 0) {
                active = true;
                break;
            }
        }
        if (active) {
            ++activeUsers;
        }
    }

    cout << "Most viewed: ";
    mostViewed->display();
    cout << "Best selling: ";
    bestSelling->display();
    cout << "Active users: " << activeUsers << "\n";
}

void Admin::menu() {
    while (true) {
        cout << "\nAdmin Menu\n";
        cout << "1. Add product\n2. Edit product\n3. Delete product\n4. View statistics\n0. Back\n";
        int choice = readInt("Choose: ");
        if (choice == 0) {
            break;
        }
        switch (choice) {
            case 1: addProduct(); break;
            case 2: editProduct(); break;
            case 3: deleteProduct(); break;
            case 4: viewStatistics(); break;
            default: cout << "Invalid choice.\n"; break;
        }
    }
}

// ─── Customer ─────────────────────────────────────────────────────────────────
void Customer::browseProducts() {
    cout << "\nProducts:\n";
    for (const auto& p : g_products) {
        p.display();
    }

    int id = readInt("Enter product ID to view (0 to return): ");
    if (id == 0) {
        return;
    }
    Product* p = findProductById(id);
    if (!p) {
        cout << "Product not found.\n";
        return;
    }
    p->increaseView();
    viewedProducts[id]++;
    saveProducts();
    saveHistoryForUser(username, viewedProducts, purchasedProducts);
    p->display();
}

void Customer::addToCart(Product& p, int qty) {
    if (qty <= 0) {
        cout << "Quantity must be positive.\n";
        return;
    }
    if (p.getStock() < qty) {
        cout << "Not enough stock.\n";
        return;
    }
    cart.addItem(p, qty);
    cout << "Added to cart.\n";
}

void Customer::checkout() {
    auto& items = cart.getItems();
    if (items.empty()) {
        cout << "Cart is empty.\n";
        return;
    }

    for (const auto& item : items) {
        Product* p = findProductById(item.product.getId());
        if (!p || p->getStock() < item.quantity) {
            cout << "Stock issue with product ID " << item.product.getId() << ".\n";
            return;
        }
    }

    for (auto& item : items) {
        Product* p = findProductById(item.product.getId());
        if (!p) {
            continue;
        }
        p->setStock(p->getStock() - item.quantity);
        p->increasePurchase(item.quantity);
        purchasedProducts[p->getId()] += item.quantity;
    }

    double total = cart.calculateTotal();
    Order order(nextOrderId(), items, total, nowDateTime());
    orderHistory.push_back(order);
    appendOrder(order, username);
    saveProducts();
    saveHistoryForUser(username, viewedProducts, purchasedProducts);
    cart.getItems().clear();
    cout << "Checkout complete. Total: $" << total << "\n";
}

void Customer::viewRecommendations() {
    int topN = readInt("Top N recommendations: ");
    auto recs = RecommendationEngine::recommendProducts(*this, g_products, topN);
    cout << "\nRecommendations:\n";
    for (const auto& p : recs) {
        p.display();
    }
}

void Customer::menu() {
    while (true) {
        cout << "\nCustomer Menu\n";
        cout << "1. Browse products\n2. Add to cart\n3. View cart\n4. Remove from cart\n";
        cout << "5. Update cart quantity\n6. Checkout\n7. Recommendations\n0. Back\n";
        int choice = readInt("Choose: ");
        if (choice == 0) {
            break;
        }
        switch (choice) {
            case 1: browseProducts(); break;
            case 2: {
                int id = readInt("Product ID: ");
                int qty = readInt("Quantity: ");
                Product* p = findProductById(id);
                if (!p) {
                    cout << "Product not found.\n";
                } else {
                    addToCart(*p, qty);
                }
                break;
            }
            case 3: {
                const auto& items = cart.getItems();
                if (items.empty()) {
                    cout << "Cart is empty.\n";
                } else {
                    cout << "\nCart Items:\n";
                    for (const auto& item : items) {
                        cout << "- " << item.product.getName() << " x" << item.quantity
                             << " ($" << item.product.getPrice() << ")\n";
                    }
                    cout << "Total: $" << cart.calculateTotal() << "\n";
                }
                break;
            }
            case 4: {
                int id = readInt("Product ID to remove: ");
                cart.removeItem(id);
                break;
            }
            case 5: {
                int id = readInt("Product ID to update: ");
                int qty = readInt("New quantity: ");
                cart.updateQuantity(id, qty);
                break;
            }
            case 6: checkout(); break;
            case 7: viewRecommendations(); break;
            default: cout << "Invalid choice.\n"; break;
        }
    }
}

void Customer::setUsername(const string& value) { username = value; }
const Cart& Customer::getCart() const { return cart; }

void Customer::setHistory(const map<int, int>& views, const map<int, int>& purchases) {
    viewedProducts = views;
    purchasedProducts = purchases;
}

const map<int, int>& Customer::getViewedProducts() const { return viewedProducts; }
const map<int, int>& Customer::getPurchasedProducts() const { return purchasedProducts; }

// ─── Main ─────────────────────────────────────────────────────────────────────
int main() {
    loadProducts();
    banner();

    while (true) {
        cout << "\nMain Menu\n";
        cout << "1. Admin\n2. Customer\n0. Exit\n";
        int choice = readInt("Choose: ");
        if (choice == 0) {
            break;
        }
        if (choice == 1) {
            Admin admin;
            admin.menu();
        } else if (choice == 2) {
            string username = readLine("Enter username: ");
            Customer customer;
            customer.setUsername(username);
            map<int, int> views;
            map<int, int> purchases;
            loadHistoryForUser(username, views, purchases);
            customer.setHistory(views, purchases);
            customer.menu();
        } else {
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}