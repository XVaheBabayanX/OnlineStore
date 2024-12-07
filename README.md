# OnlineStore

---

### **1. Single Responsibility Principle (SRP)**

```cpp
class Product {
public:
    Product(const std::string& name, double price) : name(name), price(price) {}

    std::string getName() const { return name; }
    double getPrice() const { return price; }

private:
    std::string name;
    double price;
};
```

**Что делает:**  
- Класс `Product` отвечает только за хранение информации о продукте (название и цена).  
- Он не занимается ни скидками, ни оплатой — только **одной задачей**, что и соответствует SRP.

---

### **2. Open/Closed Principle (OCP)**

```cpp
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
```

**Что делает:**  
- Класс `Discount` — это **базовый класс**, который определяет метод `apply`.  
- Реализации `NoDiscount` и `PercentageDiscount` показывают, как можно расширять поведение (например, добавлять фиксированные скидки), **не изменяя базовый класс**.  

**Как это полезно:** Если нужно добавить новую логику (например, "Скидка на 3-й товар"), достаточно создать новый класс, не изменяя старый код.

---

### **3. Liskov Substitution Principle (LSP)**

```cpp
class Discount {
public:
    virtual double apply(double total) const = 0;
    virtual ~Discount() = default;
};
```

**Что делает:**  
- Класс `Discount` и его наследники (`NoDiscount`, `PercentageDiscount`) **взаимозаменяемы**.  
- К примеру, `Order` (ниже) не знает, какую именно реализацию `Discount` получает. Он ожидает, что любая переданная реализация будет корректно работать.

**Как это полезно:** Можно безопасно подставить любую реализацию `Discount`, и программа будет работать без изменений.

---

### **4. Interface Segregation Principle (ISP)**

```cpp
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
```

**Что делает:**  
- Интерфейс `PaymentProcessor` описывает **только одно поведение** — обработку платежей.  
- Каждая реализация (например, `CreditCardProcessor` или `PayPalProcessor`) отвечает за свой способ обработки платежей.

**Как это полезно:** Если бы `PaymentProcessor` включал всё подряд (например, скидки, доставку), то каждое изменение затрагивало бы ненужные зависимости. Разделение интерфейсов решает эту проблему.

---

### **5. Dependency Inversion Principle (DIP)**

```cpp
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
```

**Что делает:**  
- Класс `Order` зависит от **абстракций** (`Discount` и `PaymentProcessor`), а не от конкретных реализаций.  
- Мы передаём конкретную стратегию скидок (`PercentageDiscount`) и способ оплаты (`CreditCardProcessor`) в `Order`.  

**Как это полезно:**  
- Если нужно изменить способ оплаты или добавить новый вид скидки, изменения будут минимальными и не затронут код `Order`.

---

### **6. `main()`**

```cpp
int main() {
    // Создание продуктов
    Product laptop("Laptop", 1000);
    Product phone("Phone", 500);

    // Создание заказа
    Order order;
    order.addProduct(laptop);
    order.addProduct(phone);

    // Установка стратегии скидок
    std::shared_ptr<Discount> discount = std::make_shared<PercentageDiscount>(10);
    order.setDiscountStrategy(discount);

    // Выбор способа оплаты
    CreditCardProcessor creditCard;
    order.processOrder(creditCard);

    return 0;
}
```

**Что делает:**  
- Создаются продукты (`laptop`, `phone`) и добавляются в заказ.  
- Устанавливается стратегия скидок (10%).  
- Выбирается способ оплаты (кредитная карта).  

**Результат выполнения:**  
1. Программа рассчитывает общую сумму: \( 1000 + 500 = 1500 \).  
2. Применяет скидку 10%: \( 1500 - 150 = 1350 \).  
3. Обрабатывает оплату через кредитную карту и выводит:  
   ```
   Processing credit card payment of $1350
   ```

---
