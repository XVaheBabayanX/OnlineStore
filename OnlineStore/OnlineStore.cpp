#include <iostream>
#include <vector>
#include <memory>
#include <string>

class Product {
public:
    Product(const std::string& name, double price) : name(name), price(price) {}

    std::string getName() const { return name; }
    double getPrice() const { return price; }

private:
    std::string name;
    double price;
};

class Discount {
public:
    virtual double apply(double total) const = 0;
    virtual ~Discount() = default;
};

class NoDiscount : public Discount {
public:
    double apply(double total) const override { return total; }
};

class PercentageDiscount : public Discount {
public:
    PercentageDiscount(double percent) : percent(percent) {}

    double apply(double total) const override {
        return total - (total * percent / 100);
    }

private:
    double percent;
};

class PaymentProcessor {
public:
    virtual void processPayment(double amount) const = 0;
    virtual ~PaymentProcessor() = default;
};

class CreditCardProcessor : public PaymentProcessor {
public:
    void processPayment(double amount) const override {
        std::cout << "Processing credit card payment of $" << amount << std::endl;
    }
};

class PayPalProcessor : public PaymentProcessor {
public:
    void processPayment(double amount) const override {
        std::cout << "Processing PayPal payment of $" << amount << std::endl;
    }
};

class Order {
public:
    void addProduct(const Product& product) {
        products.push_back(product);
    }

    void setDiscountStrategy(std::shared_ptr<Discount> discountStrategy) {
        this->discountStrategy = discountStrategy;
    }

    void processOrder(const PaymentProcessor& paymentProcessor) const {
        double total = calculateTotal();
        if (discountStrategy) {
            total = discountStrategy->apply(total);
        }
        paymentProcessor.processPayment(total);
    }

private:
    double calculateTotal() const {
        double total = 0;
        for (const auto& product : products) {
            total += product.getPrice();
        }
        return total;
    }

    std::vector<Product> products;
    std::shared_ptr<Discount> discountStrategy;
};

int main() {
    Product laptop("Laptop", 1000);
    Product phone("Phone", 500);

    Order order;
    order.addProduct(laptop);
    order.addProduct(phone);

    std::shared_ptr<Discount> discount = std::make_shared<PercentageDiscount>(10);
    order.setDiscountStrategy(discount);

    CreditCardProcessor creditCard;
    order.processOrder(creditCard);

    return 0;
}
