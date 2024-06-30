#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Swiggy;

class Restaurant;

class BillingCounter;

class ExceptionHandler{

public:

    int errNo;
    string errMsg;

    ExceptionHandler(int n, string m): errNo(n), errMsg(m){}

    void printError(){
        cout<<"Error "<<errNo<<": "<<errMsg<<endl;
    }
};

class MenuItem {

public:

    string name;

    double price;

    MenuItem(string n, double p): name(n), price(p) {}

    virtual void printInfo() const {

        cout<<"Name: "<<name<<", Price: $"<<price<<endl;

    }
};

class Food: public MenuItem {

public:

    string mainIngredient;

    Food(string n, double p, string mi) : MenuItem(n, p), mainIngredient(mi) {}

    void description() const {

        cout<<"Food: "<<name<< ", Main Ingredient: "<<mainIngredient<<endl;
    }

    void printInfo() const {

        cout<<"Name: "<<name<<", Price: $"<<price<<", Main Ingredient: "<<mainIngredient<<endl;
    }
};

class Beverage : public MenuItem {

public:

    string drinkType;

    Beverage(string n, double p, string dt) : MenuItem(n, p), drinkType(dt) {}

    void description() const {

        cout<<"Beverage: "<<name<<", Drink Type: "<<drinkType<<endl;
    }

    void printInfo() const {

        cout<<"Name: "<<name<<", Price: $"<<price<<", Drink Type: "<<drinkType<<endl;
    }
};

class User {

public:

    int userID;

    string password;

    User(int a, string b) : userID(a), password(b) {}

    virtual void printInfo() const = 0;

    virtual ~User() {}
};

class Customer : public User {

public:

    string name;

    string location;

    Customer(string a, string b, int c, string d) : User(c, d), name(a), location(b) {}

    void changePassword(string newPass) {

        password = newPass;
    }

    void printInfo() const {

        cout<<"UserID: "<<userID<<", Password: "<<password<<", Name: "<<name<<", Location: "<<location<<endl;
    }

    void order(Swiggy* swiggy, Restaurant* restaurant); // defined later

};

class BillingStratergy {

public:

    virtual void billCreation(const vector<MenuItem*>& chosenItems) const = 0;

    virtual bool isEligibleForDiscount(const vector<MenuItem*>& chosenItems) const = 0;

    virtual ~BillingStratergy() {}
};

class RegularBill : public BillingStratergy {

public:

    void billCreation(const vector<MenuItem*>& chosenItems) const {

        double totalAmount = 0;

        for(const auto& item : chosenItems) {

            totalAmount += item->price;
        }

        cout<<"Total amount to pay: Rs."<<totalAmount<<endl;
    }

    bool isEligibleForDiscount(const vector<MenuItem*>& chosenItems) const {

        return false; // this function is called when discount offers are not available
    }
};

class DiscountBill : public BillingStratergy {

public:

    void billCreation(const vector<MenuItem*>& chosenItems) const {

        double totalAmount = 0;

        for(const auto& item : chosenItems) {

            totalAmount += item->price;
        }

        double discountedAmount = totalAmount * 0.85;

        cout<<"Total amount after 15% discount: Rs."<<discountedAmount<<endl;
    }

    bool isEligibleForDiscount(const vector<MenuItem*>& chosenItems) const {

        int foodCount = 0;

        int beverageCount = 0;

        for(const auto& item : chosenItems) {

            if(dynamic_cast<Food*>(item)) {

                foodCount++;

            }
            else if(dynamic_cast<Beverage*>(item)) {

                beverageCount++;
            }
        }

        return (foodCount >= 2 && beverageCount >= 1);
    }
};

class BillingCounter {

public:

    string managerName;

    vector<MenuItem*> latestOrderItems;

    BillingCounter(string manager) : managerName(manager) {}

    void billing(User* user, const vector<MenuItem*>& chosenItems, const BillingStratergy* regularStrategy, const BillingStratergy* discountStrategy) {

        string inputPassword;

        cout<<"Enter password: ";

        cin>>inputPassword;

        try{

        if(inputPassword != user->password) {

            throw ExceptionHandler(1001, "Wrong password");

        }
        }

        catch(ExceptionHandler a){

            a.printError();
            return;
        }

        latestOrderItems = chosenItems;

        if(discountStrategy->isEligibleForDiscount(chosenItems)) {

            discountStrategy->billCreation(chosenItems);

        }
        else {

            regularStrategy->billCreation(chosenItems);

        }
    }

    void printLatestOrder() const {

        cout << "Latest Order Items:" << endl;

        for (const auto& item : latestOrderItems) {

            item->printInfo();
        }
    }
};

class DeliveryPartner : public User{

public:

    string name;

    double salary;

    DeliveryPartner(string a, double b, int c, string d) : User(c, d), name(a), salary(b) {}

    void printInfo() const {

        cout<<"UserID: "<<userID<<", Password: "<<password<<", Name: "<<name<<", Salary: $"<<salary<<endl;
    }

    void getOrder(const BillingCounter& billingCounter) const {

        cout<<"Delivery Partner: "<<name<<" is delivering the following order:"<<endl;

        billingCounter.printLatestOrder();
    }
};

class Restaurant : public User {

public:

    string restName;

    int restID;

    vector<MenuItem*> menu;

    Restaurant(string a, int b, int c, string d) : User(c, d), restName(a), restID(b) {}

    void changeMenu() {



        int choice;

        cout<<"Enter 1 to add Food, 2 to add Beverage: ";

        cin>>choice;

        string name, mainIngredient, drinkType;

        double price;

        try{

        if (choice == 1) {

            cout<<"Enter food name: ";

            cin>>name;

            cout<<"Enter price: ";

            cin>>price;

            cout<<"Enter main ingredient: ";

            cin>>mainIngredient;

            menu.push_back(new Food(name, price, mainIngredient));

        } else if (choice == 2) {

            cout<<"Enter beverage name: ";

            cin>>name;

            cout<<"Enter price: ";

            cin>>price;

            cout<<"Enter drink type: ";

            cin>>drinkType;

            menu.push_back(new Beverage(name, price, drinkType));

        } else {


            throw ExceptionHandler(01, "No such variety of item");

        }}
        catch(ExceptionHandler a){
            a.printError();
        }
    }

    void printInfo() const override {

        cout << "Restaurant Name: " << restName << ", Restaurant ID: " << restID << ", UserID: " << userID << ", Password: " << password << endl;

        for (const auto& item : menu) {

            item->printInfo();
        }
    }

    ~Restaurant() {

        for (auto item : menu) {

            delete item;
        }
    }
};

class UserFactory {

public:

    static User* createUser(string type, int userID, string password) {

        try{

        if (type == "Customer") {

            string name, location;

            cout<<"Enter name: ";

            cin>>name;

            cout<<"Enter location: ";

            cin>>location;

            return new Customer(name, location, userID, password);

        } else if (type == "DeliveryPartner") {

            string name;

            double salary;

            cout<<"Enter name: ";

            cin>>name;

            cout<<"Enter salary: ";

            cin>>salary;

            return new DeliveryPartner(name, salary, userID, password);

        }
        else if (type == "Restaurant") {

            string restName;

            int restID;

            cout<<"Enter restaurant name: ";

            cin>>restName;

            cout<<"Enter restaurant ID: ";

            cin>>restID;

            return new Restaurant(restName, restID, userID, password);

        }
        else {

            throw ExceptionHandler(10, "No such User type available.");

        }
        }
        catch(ExceptionHandler a){

          a.printError();
          return nullptr;
        }
    }

};

class OrderSection {

public:

    int orderId;

    BillingCounter* biller;

    OrderSection(int id, BillingCounter* b) : orderId(id), biller(b) {}

    void menu(const Restaurant& r) const {

        cout << "Menu for Restaurant: " << r.restName << endl;

        for (const auto& item : r.menu) {

            item->printInfo();
        }
    }

    void defineOrder(User* customer, Restaurant* restaurant) {

        vector<MenuItem*> chosenItems;

        string itemName;

        try{

        while (true) {

            cout << "Enter the name of the item you want to order (type 'stop' to finish): ";

            cin >> itemName;

            if (itemName == "stop") {

                break;
            }

            bool itemFound = false;

            for (const auto& item : restaurant->menu) {

                if (item->name == itemName) {

                    chosenItems.push_back(item);

                    itemFound = true;

                    break;
                }
            }

            if (!itemFound) {

                throw ExceptionHandler(101, "Item not found in the menu");
            }
        }
        }
        catch(ExceptionHandler a){
            a.printError();
        }

        RegularBill regularStrategy;
        DiscountBill discountStrategy;

        biller->billing(customer, chosenItems, &regularStrategy, &discountStrategy);
    }
};

class Swiggy {

private:

    static Swiggy* instance;

    int companyId;

    string ceoName;

    OrderSection* orderNo;

    Swiggy(int a, string b) : companyId(a), ceoName(b), orderNo(nullptr) {}

public:

    static Swiggy* getInstance(int a, string b) {

        if (instance == nullptr) {

            instance = new Swiggy(a, b);
        }

        return instance;
    }

    Restaurant* addRestaurant(string restName, int restID, int userID, string password) {

        return new Restaurant(restName, restID, userID, password);
    }

    void customerOrder(User* customer, Restaurant* restaurant) {

        if (orderNo) {

            orderNo->defineOrder(customer, restaurant);

        } else {

            cout << "OrderSection is not set." << endl;
        }
    }

    void setOrderSection(OrderSection* os) {

        orderNo = os;
    }
};

Swiggy* Swiggy::instance = nullptr;

void Customer::order(Swiggy* swiggy, Restaurant* restaurant) {

    swiggy->customerOrder(this, restaurant);
}

int main() {

    UserFactory factory;

    User* customer = factory.createUser("Customer", 101, "pass123");
    User* user2 = factory.createUser("DeliveryPartner", 102, "pass456");
    User* user3 = factory.createUser("Restaurant", 103, "pass789");

    customer->printInfo();
    user2->printInfo();

    Restaurant* restaurant = dynamic_cast<Restaurant*>(user3);

    if (restaurant) {

        restaurant->changeMenu();

        restaurant->printInfo();
    }

    BillingCounter billingCounter("Manager1");

    OrderSection orderSection(1, &billingCounter);

    Swiggy* swiggy = Swiggy::getInstance(1, "CEO_Name");
    swiggy->setOrderSection(&orderSection);

    Customer* cust = dynamic_cast<Customer*>(customer);

    if (cust && restaurant) {

        cust->order(swiggy, restaurant);
    }

    DeliveryPartner* dp = dynamic_cast<DeliveryPartner*>(user2);

    if (dp) {

        dp->getOrder(billingCounter);
    }

    delete customer;
    delete user2;
    delete user3;

    return 0;
}
