#include <iostream>
#include <vector>
#include "Database.h"
#include "Restaurant.h"
#include "RestaurantDAO.h"
#include "Menu.h"
#include "MenuDAO.h"
#include "Order.h"
#include "OrderDAO.h"
#include "Customer.h"
#include "CustomerDAO.h"

using namespace std;

////////////////////////////////////////////////ّFunctions///////////////////////////////////////////////////////////////

void clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}


int getValidatedInt() {
    int value;
    while (!(cin >> value)) {
        cout << "Invalid input! Please enter a valid number: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    return value;
}

double getValidatedDouble() {
    double value;
    while (!(cin >> value)) {
        cout << "Invalid input! Please enter a valid number: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    return value;
}


bool getValidatedBool() {
    int input;
    while (true) {
        cin >> input;

        if (cin.fail() || (input != 0 && input != 1)) {
            cout << "Invalid input! Please enter 1 for True or 0 for False: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
        else {
            cin.ignore(10000, '\n');
            return (input == 1);
        }
    }
}


void addFoodToMenu(int &ResIdToAddFood , MenuDAO& menuDao){

    int foodID;
    string foodName;
    string foodDescription;
    double foodPrice;
    bool foodAvailability;
    int foodCookingTime;


    cin.ignore(10000, '\n');


    cout << "Enter Name: " << endl;
    getline(cin , foodName);
    cout << "Enter Description : " << endl;
    getline(cin , foodDescription);

    cout << "Enter ID : " << endl;
    foodID = getValidatedInt();
    cout << "Enter Price : " << endl;
    foodPrice = getValidatedDouble();
    cout << "Enter Availability : " << endl;
    foodAvailability = getValidatedBool();
    cout << "Enter Cooking Time : " << endl;
    foodCookingTime = getValidatedInt();


    Food newFood;
    newFood.setItemID(foodID);
    newFood.setItemName(foodName);
    newFood.setItemDescription(foodDescription);
    newFood.setItemPrice(foodPrice);
    newFood.setCookingTime(foodCookingTime);
    newFood.setItemAvailability(foodAvailability);

    menuDao.addFood(ResIdToAddFood, newFood);

}

void addDrinkToMenu(int &ResIdToAddDrink , MenuDAO& menuDao){

    int drinkID;
    string drinkName;
    string drinkDescription;
    double drinkPrice;
    bool drinkAvailability;
    int drinkVolume;




    cin.ignore(10000, '\n');

    cout << "Enter Name: " << endl;
    getline(cin , drinkName);
    cout << "Enter Description : " << endl;
    getline(cin , drinkDescription);

    cout << "Enter ID : " << endl;
    drinkID = getValidatedInt();
    cout << "Enter Price : " << endl;
    drinkPrice = getValidatedDouble();
    cout << "Enter Availability : " << endl;
    drinkAvailability = getValidatedBool();
    cout << "Enter Drink Volume : " << endl;
    drinkVolume = getValidatedInt();

    Drink newDrink;
    newDrink.setItemID(drinkID);
    newDrink.setItemName(drinkName);
    newDrink.setItemDescription(drinkDescription);
    newDrink.setItemPrice(drinkPrice);
    newDrink.setDrinkVolume(drinkVolume);
    newDrink.setItemAvailability(drinkAvailability);

    menuDao.addDrink(ResIdToAddDrink, newDrink);
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {


    /////////////////////////////////////////////////////Variables//////////////////////////////////////////////////////
    int chooseRole = -2;
    vector<Restaurant *> restaurants;

    ////////////////////////////////////////////////////Open and link Database//////////////////////////////////////////
    Database db;
    if (!db.open("Food-Ordering-System.db")) {
        cerr << "Failed to open database! " << endl;
        return 1;
    }

    if (!db.createTables()) {
        cerr << "Failed to create tables!" << endl;
        return 1;
    }

    RestaurantDAO restaurantDao(db);
    MenuDAO menuDao(db);
    OrderDAO orderDao(db);
    CustomerDAO customerDao(db);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    while (true) {
        clearScreen();
        cout << "Choose your role:" << endl;
        cout << "Press 1 for Customer and Enter!" << endl;
        cout << "Press 2 for Restaurant Manager and Enter!" << endl;
        cout << "Press 3 for System Manager and Enter!" << endl;
        cout << "Press -1 for exit!" << endl;
        cout << "Press 0 for back to login page!" << endl;
        cout << "----------------------------------------" << endl;

        chooseRole = getValidatedInt();

        if (chooseRole == -1) {
            cout << "Exiting program!" << endl;
            for (int i = 0; i < restaurants.size(); ++i) {
                delete restaurants[i];
            }
            restaurants.clear();
            return 2;
        }


        /////////////////////////////////////////////////Customer Menu//////////////////////////////////////////////////

        if (chooseRole == 1) {
            clearScreen();
            cout << "Press 1 to Login" << endl;
            cout << "Press 2 to Sign Up" << endl;
            cout << "Press 0 to Go Back" << endl;

            int loginChoice = getValidatedInt();
            Customer *currentCustomer = nullptr;
            if (loginChoice == 0) continue;

            if (loginChoice == 1) {
                int chooseCustomerID = -1;
                cout << "Enter customer ID : " << endl;
                chooseCustomerID = getValidatedInt();

                currentCustomer = customerDao.getCustomerById(chooseCustomerID);

                if (currentCustomer == nullptr) {
                    cout << "Customer ID not found! Press Enter to return." << endl;
                    cin.ignore();
                    cin.get();
                    continue;
                }
            }
            else if (loginChoice == 2) {
                int newCustomerID = -1;
                cout << "Enter new customer ID : " << endl;
                newCustomerID = getValidatedInt();

                Customer *checkExist = customerDao.getCustomerById(newCustomerID);

                if (checkExist != nullptr) {
                    cout << "This ID is already taken! Press Enter to return." << endl;
                    delete checkExist;
                    cin.ignore();
                    cin.get();
                    continue;
                }

                string newCustomerName, newCustomerPhoneNumber;
                cout << "Enter new customer name : " << endl;
                cin.ignore();
                getline(cin, newCustomerName);
                cout << "Enter new customer phone number : " << endl;
                getline(cin, newCustomerPhoneNumber);

                Customer newCustomer;
                newCustomer.setCustomerID(newCustomerID);
                newCustomer.setCustomerName(newCustomerName);
                newCustomer.setCustomerPhoneNumber(newCustomerPhoneNumber);
                newCustomer.setCustomerPoints(0);

                if (customerDao.addCustomer(newCustomer)) {
                    cout << "Sign up successful! Your ID is: " << newCustomerID << endl;
                    currentCustomer = new Customer(newCustomer);
                }
                else {
                    cout << "Registration failed! Press Enter." << endl;
                    cin.get();
                    continue;
                }
            }
            else {
                cout << "Invalid choice!" << endl;
                continue;
            }

            int customerChoice = -1;
            while (true) {
                clearScreen();
                cout << "--- CUSTOMER MENU ---" << endl;
                cout << "Press 0 to Log Out and return to Main Menu." << endl;
                cout << "-------------------------------------------" << endl;


                orderDao.checkAndAllocateMonthlyCoupons(currentCustomer);
                int coupons = orderDao.getActiveCouponsCount(currentCustomer->getCustomerID());

                ///////////////////////////////////////Show Customer Information///////////////////////////////////////

                cout << "Customer ID : " << currentCustomer->getCustomerID() << endl;
                cout << "Customer Name : " << currentCustomer->getCustomerName() << endl;
                cout << "Customer Phone Number : " << currentCustomer->getCustomerPhoneNumber() << endl;
                cout << "Customer Level : " << currentCustomer->getCustomerLevel() << endl;
                cout << "Customer Points : " << currentCustomer->getCustomerPoints() << endl;
                cout << "Customer Coupons: " << coupons << " coupon(s)" << endl;


                cout << ">> " << currentCustomer->getNextLevelRequirement() << " <<" << endl;

                cout << "=========================================" << endl;
                cout << "Press Enter.";
                cin.ignore(10000, '\n');
                cin.get();

                ///////////////////////////////////////Show Active restaurants//////////////////////////////////////////
                vector<Restaurant> allActiveRestaurantsToShow = restaurantDao.getAllRestaurants();
                bool foundAnyActiveRestaurant = false;
                if (allActiveRestaurantsToShow.empty()) {
                    cout << "There is no restaurant!" << endl;
                } else {
                    for (int i = 0; i < allActiveRestaurantsToShow.size(); ++i) {
                        if (allActiveRestaurantsToShow[i].getInActive() == true) {
                            cout << "ID : " << allActiveRestaurantsToShow[i].getID() << endl;
                            cout << "Name : " << allActiveRestaurantsToShow[i].getName() << endl;
                            cout << "Address : " << allActiveRestaurantsToShow[i].getAddress() << endl;
                            cout << "Preparation Time : " << allActiveRestaurantsToShow[i].getOrderPreparationTime() << endl;
                            cout << "-------------------------------" << endl;
                            foundAnyActiveRestaurant = true;
                        }
                    }

                    if (!foundAnyActiveRestaurant) {
                        cout << "There is no active restaurant!" << endl;
                    }
                }

                ////////////////////////////////////////////////////Choose restaurant///////////////////////////////////
                cout << "Please enter restaurant ID : " << endl;
                customerChoice = getValidatedInt();

                if (customerChoice == 0) {
                    break;
                }
                else {
                    vector<menuItems *> loadedMenu = menuDao.getMenuByRestaurantId(customerChoice);
                    Restaurant *chooseResInCustomerMenu = restaurantDao.getRestaurantById(customerChoice);

                    if (chooseResInCustomerMenu == nullptr) {
                        cout << "Wrong Restaurant ID!" << endl;
                        cout << "Press Enter." << endl;
                        for (size_t i = 0; i < loadedMenu.size(); ++i) {
                            delete loadedMenu[i];
                        }
                        loadedMenu.clear();
                        cin.ignore();
                        cin.get();
                        continue;
                    } else {
                        clearScreen();
                        chooseResInCustomerMenu->setMenu(loadedMenu);
                        chooseResInCustomerMenu->printMyMenu();
                        cout << "-----------------------------------------------------------------" << endl;
                    }

                    ////////////////////////////////////////////////////Order///////////////////////////////////////////
                    Order *customerOrder = new Order;
                    int orderChoice = -1, selectOrder = -1, orderNum = -1;
                    while (true) {
                        cout << "Press 0 to exit ordering page." << endl;
                        cout << "Press 100 to finish adding order." << endl;
                        cout << "Press 101 to add to order." << endl;
                        cout << "Press 102 to change number of order." << endl;
                        cout << "Press 103 to remove item from order list." << endl;
                        cout << "Press 104 to show Current order list." << endl;
                        cout << "Press 105 to show all order lists." << endl;
                        cout << "Press 106 to cancel order." << endl;


                        cout << "Total Price: " << customerOrder->getTotalPrice() << endl;
                        cout << "----------------------------------------------------------------" << endl;

                        orderChoice = getValidatedInt();

                        if (orderChoice == 0) {
                            break;
                        }
                        else if (orderChoice == 100) {
                            if (customerOrder->getOrderItems().empty()) {
                                cout << "Your order list is empty! Cannot complete an empty order." << endl;
                                cin.ignore(); cin.get();
                                continue;
                            }
                            cout << "Order completed." << endl;
                            customerOrder->setOrderCondition("Pending");

                            customerOrder->setCustomer(currentCustomer);

                            orderDao.addOrder(*customerOrder);
                            orderDao.printInvoice(*customerOrder);

                            cout << "Press Enter to continue.";
                            cin.ignore(); cin.get();

                            customerOrder->setCustomer(nullptr);
                            break;
                        }
                        else if (orderChoice == 101) {
                            menuItems *selectedItem = nullptr;
                            cout << "Enter item ID to add: ";
                            selectOrder = getValidatedInt();
                            cout << "Enter quantity: ";
                            orderNum = getValidatedInt();

                            for (size_t i = 0; i < loadedMenu.size(); ++i) {
                                if (loadedMenu[i] != nullptr && loadedMenu[i]->getItemID() == selectOrder) {
                                    selectedItem = loadedMenu[i];
                                    break;
                                }
                            }

                            if (selectedItem == nullptr) {
                                cout << "Error: Item ID [" << selectOrder << "] not found in this menu!" << endl;
                                cout << "Press Enter to continue.";
                                cin.ignore(10000, '\n');
                                cin.get();
                            } else if (selectedItem->getItemAvailability() == false || selectedItem->getItemAvailability() == 0) {
                                cout << "The desired product is not available." << endl;
                            } else {
                                if (orderNum <= 0) {
                                    cout << "Invalid quantity!" << endl;
                                } else {
                                    customerOrder->addItem(selectedItem, orderNum);
                                    cout << selectedItem->getItemName() << " (x" << orderNum << ") added." << endl;
                                }
                                cout << "Press Enter to continue.";
                                cin.ignore(10000, '\n');
                                cin.get();
                            }
                        }
                        else if (orderChoice == 102) {
                            if (customerOrder != nullptr && customerOrder->getOrderID() <= 0) {
                                cout << "Enter item ID to update order: " << endl;
                                selectOrder = getValidatedInt();
                                cout << "Enter item quantity to update order : " << endl;
                                orderNum = getValidatedInt();

                                customerOrder->updateOrderNumber(selectOrder, orderNum);
                                cout << "Order updated." << endl;
                            } else {
                                cout << "Error: This order has already been finalized and registered. You cannot modify item quantities!" << endl;
                            }
                        }
                        else if (orderChoice == 103) {
                            if (customerOrder != nullptr && customerOrder->getOrderID() <= 0) {
                                cout << "Enter item ID to remove from order list : " << endl;
                                selectOrder = getValidatedInt();
                                customerOrder->removeItem(selectOrder);
                                cout << "Item removed from your draft order." << endl;
                            } else {
                                cout << "Error: This order has already been finalized and registered. You can only cancel the entire order!" << endl;
                            }
                        }
                        else if (orderChoice == 104) {
                            clearScreen();
                            cout << "--- CURRENT BASKET ---" << endl;

                            customerOrder->setCustomer(currentCustomer);
                            orderDao.printInvoice(*customerOrder);

                            cout << "Press Enter to return." << endl;
                            cin.ignore();
                            cin.get();
                        }


                        else if (orderChoice == 105) {
                            clearScreen();

                            auto myOrders = orderDao.getCustomerOrders(currentCustomer->getCustomerID());

                            if (myOrders.empty()) {
                                cout << "You haven't placed any orders yet!" << endl;
                            }
                            else {
                                for (int i = 0; i < myOrders.size(); ++i) {
                                    cout << "Order ID: " << myOrders[i]->getOrderID()
                                         << " | Status: " << myOrders[i]->getOrderCondition()
                                         << " | Total Price: $" << myOrders[i]->getFinalPaidPrice()
                                         << " | Restaurant`s name : " << myOrders[i]->getRestaurantName() << endl;

                                    cout << "Items purchased:" << endl;

                                    const auto& items = myOrders[i]->getOrderItems();
                                    for (size_t j = 0; j < items.size(); ++j) {
                                        if (items[j] != nullptr && items[j]->getItem() != nullptr) {
                                            cout << "  - " << items[j]->getItem()->getItemName()
                                                 << " (x" << items[j]->getOrderNumber() << ")" << endl;
                                        }
                                    }
                                    cout << "----------------------------------------" << endl;
                                }

                            }

                            for (int i = 0; i < myOrders.size(); ++i) {
                                if (myOrders[i] != nullptr) {
                                    const auto& items = myOrders[i]->getOrderItems();
                                    for (size_t j = 0; j < items.size(); ++j) {
                                        if (items[j] != nullptr) {
                                            if (items[j]->getItem() != nullptr) {
                                                delete items[j]->getItem();
                                            }
                                            delete items[j];
                                        }
                                    }
                                }
                            }
                            myOrders.clear();

                            cout << "Press Enter to return." << endl;
                            cin.ignore(10000, '\n');
                            cin.get();
                        }

                        else if(orderChoice == 106){
                            int orderIdToCancel = -1;
                            clearScreen();

                            auto myOrders = orderDao.getCustomerOrders(currentCustomer->getCustomerID());

                            if (myOrders.empty()) {
                                cout << "You haven't placed any orders yet!" << endl;
                            }
                            else {
                                for (int i = 0; i < myOrders.size(); ++i) {
                                    cout << "Order ID: " << myOrders[i]->getOrderID()
                                         << " | Status: " << myOrders[i]->getOrderCondition()
                                         << " | Total Price: $" << myOrders[i]->getFinalPaidPrice()
                                         << " | Restaurant`s name : " << myOrders[i]->getRestaurantName() << endl;

                                    cout << "Items purchased:" << endl;

                                    const auto& items = myOrders[i]->getOrderItems();
                                    for (size_t j = 0; j < items.size(); ++j) {
                                        if (items[j] != nullptr && items[j]->getItem() != nullptr) {
                                            cout << "  - " << items[j]->getItem()->getItemName()
                                                 << " (x" << items[j]->getOrderNumber() << ")" << endl;
                                        }
                                    }
                                    cout << "----------------------------------------" << endl;
                                }

                            }

                            for (int i = 0; i < myOrders.size(); ++i) {
                                if (myOrders[i] != nullptr) {
                                    const auto& items = myOrders[i]->getOrderItems();
                                    for (size_t j = 0; j < items.size(); ++j) {
                                        if (items[j] != nullptr) {
                                            if (items[j]->getItem() != nullptr) {
                                                delete items[j]->getItem();
                                            }
                                            delete items[j];
                                        }
                                    }
                                }
                            }
                            myOrders.clear();



                            /////////////////////////////////////////Cancel Order//////////////////////////////////////


                            cout << "Enter Order ID : " << endl;
                            orderIdToCancel = getValidatedInt();
                            Order* customerOrderToCancel = orderDao.getOrderById(orderIdToCancel);

                            if(customerOrderToCancel == nullptr){
                                cout << "There is no order with id : " << orderIdToCancel << endl;
                            }

                            else{
                                customerOrderToCancel->setCustomer(currentCustomer);
                                orderDao.cancelOrder(*customerOrderToCancel);
                                cout << "Order " << orderIdToCancel << " has been successfully canceled." << endl;
                            }


                            delete customerOrderToCancel;



                            cout << "Press Enter to return." << endl;
                            cin.ignore(10000, '\n');
                            cin.get();
                        }


                        else {
                            cout << "Invalid number!" << endl;
                        }
                    }


                    if (customerOrder != nullptr) {
                        delete customerOrder;
                        customerOrder = nullptr;
                    }



                    loadedMenu.clear();

                    if (chooseResInCustomerMenu != nullptr) {
                        delete chooseResInCustomerMenu;
                        chooseResInCustomerMenu = nullptr;
                    }
                }
            }

            if (currentCustomer != nullptr) {
                delete currentCustomer;
                currentCustomer = nullptr;
            }
        }





            //////////////////////////////////////////////////Restaurant manager////////////////////////////////////////

        else if (chooseRole == 2) {
            int restaurantManagerChoice = -1;
            while (true) {
                clearScreen();
                cout << "--- RESTAURANT MANAGER MENU ---" << endl;


                vector<Restaurant> allRestaurantsToShow = restaurantDao.getAllRestaurants();
                bool foundAnyRestaurant = false;
                if (allRestaurantsToShow.empty()) {
                    cout << "There is no restaurant!" << endl;
                }

                else{
                    for (int i = 0; i < allRestaurantsToShow.size(); ++i) {
                            cout << "ID : " << allRestaurantsToShow[i].getID() << endl;
                            cout << "Name : " << allRestaurantsToShow[i].getName() << endl;
                            cout << "Address : " << allRestaurantsToShow[i].getAddress() << endl;
                            cout << "Preparation Time : " << allRestaurantsToShow[i].getOrderPreparationTime() << endl;
                            cout << "Activity : " << (allRestaurantsToShow[i].getInActive() == 1 ? "Open" : "Close") << endl;
                            cout << "-------------------------------------" << endl;
                            foundAnyRestaurant = true;
                    }

                    if(!foundAnyRestaurant){
                        cout << "There is no restaurant!" << endl;
                    }
                }


                ////////////////////////////////////////////////////Choose restaurant///////////////////////////////////
                cout << "Please enter restaurant ID : " << endl;


                restaurantManagerChoice = getValidatedInt();

                if (restaurantManagerChoice == 0) {
                    break;
                }

                else {
                    vector<menuItems *> loadedMenu = menuDao.getMenuByRestaurantId(restaurantManagerChoice);
                    Restaurant *chooseResInRestaurantManagerMenu = restaurantDao.getRestaurantById(restaurantManagerChoice);

                    if (chooseResInRestaurantManagerMenu == nullptr) {
                        cout << "Wrong Restaurant ID!" << endl;
                        cout << "Press Enter." << endl;
                        for (int i = 0; i < loadedMenu.size(); ++i) {
                            if (loadedMenu[i] != nullptr) delete loadedMenu[i];
                        }
                        loadedMenu.clear();

                        cin.ignore(10000, '\n'); cin.get();
                        continue;
                    }
                    else {
                        clearScreen();
                        chooseResInRestaurantManagerMenu->setMenu(loadedMenu);

                        chooseResInRestaurantManagerMenu->printMyMenu();
                        cout << "-----------------------------------------------------------------" << endl;
                    }


                    ////////////////////////////////////////////////////////////////////////////////////////////////////
                    int resManagerChoice = -1;
                    while(true){
                        int addMenuItems = -1 ;
                        cout << "Press 0 to exit page." << endl;
                        cout << "Press 201 to add item to menu. " << endl;
                        cout << "Press 202 to delete item from menu. " << endl;
                        cout << "Press 203 to edit menu items." << endl;
                        cout << "Press 204 to show all order lists." << endl;
                        cout << "Press 205 to edit condition." << endl;

                        resManagerChoice = getValidatedInt();

                        if(resManagerChoice == 0){
                            break;
                        }
                        else if(resManagerChoice == 201){
                            while(true){
                                cout << "1. Add Food" << endl;
                                cout << "2. Add Drink" << endl;
                                cout << "0. Finish adding menu items" << endl;

                                addMenuItems = getValidatedInt();

                                if(addMenuItems == 0) break;

                                else if(addMenuItems == 1){
                                    addFoodToMenu(restaurantManagerChoice , menuDao);
                                }


                                else if(addMenuItems == 2){
                                    addDrinkToMenu(restaurantManagerChoice , menuDao);
                                }

                                else {
                                    cout << "Invalid Choice!" << endl;
                                }
                            }
                        }

                        else if(resManagerChoice == 202 ){

                            int deleteRestaurantMenuItemID = -1 ;

                            cout << "Enter item ID for delete it : " << endl;
                            deleteRestaurantMenuItemID = getValidatedInt();

                            if (menuDao.deleteMenuItem(deleteRestaurantMenuItemID)) {
                                cout << "Item deleted from the restaurant's menu!" << endl;

                            } else {
                                cout << "Item not found!" << endl;
                            }
                            cout << "Press Enter to continue.";
                            cin.ignore(10000, '\n'); cin.get();
                        }

                        else if(resManagerChoice == 203){
                            menuItems *selectedItem = nullptr;
                            bool successEditing = false;
                            int itemIdToEdit = -1;

                            cout << "Enter item ID to edit: ";
                            itemIdToEdit = getValidatedInt();

                            for (int i = 0; i < loadedMenu.size(); ++i) {
                                if (loadedMenu[i] != nullptr && loadedMenu[i]->getItemID() == itemIdToEdit) {
                                    selectedItem = loadedMenu[i];
                                    break;
                                }
                            }

                            if(selectedItem == nullptr){
                                cout << "Error: Item ID [" << itemIdToEdit << "] not found in this menu!" << endl;
                                cout << "Press Enter to continue.";
                                cin.ignore(10000, '\n'); cin.get();
                            }

                            else{
                                clearScreen();
                                int editCode = -1;
                                cout << "Press 2031 to edit price." << endl;
                                cout << "Press 2032 to edit description." << endl;
                                cout << "Press 2033 to edit availability." << endl;

                                editCode = getValidatedInt();

                                if(editCode == 2031){
                                    double editPrice = -1;
                                    cout << "Enter new price : " << endl;
                                    editPrice = getValidatedDouble();

                                    selectedItem->setItemPrice(editPrice);
                                    successEditing = true;
                                }

                                else if(editCode == 2032){
                                    string editDescription;
                                    cout << "Enter new description : " <<endl;
                                    cin.ignore(10000, '\n');
                                    getline(cin , editDescription);

                                    selectedItem->setItemDescription(editDescription);
                                    successEditing = true;
                                }

                                else if(editCode == 2033){
                                    bool editAvailability = false;
                                    cout << "Enter new availability (1 for true / 0 for false) : " << endl;
                                    editAvailability = getValidatedBool();

                                    selectedItem->setItemAvailability(editAvailability);
                                    successEditing = true;
                                }

                                else{
                                    cout << "Wrong choice." << endl;
                                }



                                if(successEditing){
                                    Food *foodPtr = dynamic_cast<Food*>(selectedItem);
                                    Drink *drinkPtr = dynamic_cast<Drink*>(selectedItem);

                                    bool successEdit = false;

                                    if(foodPtr != nullptr){
                                        successEdit = menuDao.updateFood(itemIdToEdit , *foodPtr);
                                    }

                                    else if(drinkPtr != nullptr){
                                        successEdit = menuDao.updateDrink(itemIdToEdit , *drinkPtr);
                                    }
                                    if (successEdit) {
                                        cout << "Database updated successfully!" << endl;
                                        chooseResInRestaurantManagerMenu->setMenu(loadedMenu);
                                    }
                                    else {
                                        cout << "Failed to update database!" << endl;
                                    }
                                    cout << "Press Enter to continue.";
                                    cin.ignore(10000, '\n'); cin.get();

                                }
                                else{}


                            }
                        }

                        else if(resManagerChoice == 204){
                            clearScreen();

                            auto allOrders = orderDao.getRestaurantOrders(chooseResInRestaurantManagerMenu->getID());

                            if(allOrders.empty()){
                                cout << "No orders found for your restaurant!" << endl;
                            }
                            else {
                                for(int i = 0; i < allOrders.size(); ++i) {
                                    cout << "Order ID: " << allOrders[i]->getOrderID()
                                         << " | Condition: " << allOrders[i]->getOrderCondition()
                                         << " | Total Price: " << allOrders[i]->getFinalPaidPrice() << endl;

                                    cout << "Items to prepare:" << endl;
                                    const auto& items = allOrders[i]->getOrderItems();
                                    for (size_t j = 0; j < items.size(); ++j) {
                                        if (items[j] != nullptr && items[j]->getItem() != nullptr) {
                                            cout << "  - " << items[j]->getItem()->getItemName()
                                                 << " (x" << items[j]->getOrderNumber() << ")" << endl;
                                        }
                                    }
                                    cout << "----------------------------------------" << endl;
                                }
                            }

                            allOrders.clear();

                            cout << "Press Enter to return." << endl;
                            cin.ignore(10000, '\n');
                            cin.get();
                        }

                        else if(resManagerChoice == 205){
                            clearScreen();

                            auto allOrders = orderDao.getRestaurantOrders(restaurantManagerChoice);

                            if(allOrders.empty()){
                                cout << "No orders found for your restaurant!" << endl;
                            }
                            else {
                                cout << "=== Active Orders for Your Restaurant ===" << endl;
                                for (size_t i = 0; i < allOrders.size(); ++i) {
                                    cout << "Order ID: " << allOrders[i]->getOrderID()
                                         << " | Total: $" << allOrders[i]->getFinalPaidPrice()
                                         << " | Condition: [" << allOrders[i]->getOrderCondition() << "]" << endl;
                                }
                                cout << "=========================================" << endl;

                                int orderIdToChangeCondition = -1, changingCondition = -1;
                                bool updateCondition = false;

                                cout << "Enter order ID to change condition: " << endl;
                                orderIdToChangeCondition = getValidatedInt();

                                bool belongsToThisRestaurant = false;
                                for (size_t i = 0; i < allOrders.size(); ++i) {
                                    if (allOrders[i]->getOrderID() == orderIdToChangeCondition) {
                                        belongsToThisRestaurant = true;
                                        break;
                                    }
                                }

                                if (!belongsToThisRestaurant) {
                                    cout << "Error: This order ID does not belong to your restaurant or doesn't exist!" << endl;
                                }
                                else {
                                    cout << "Press 2051 to change condition to <<In preparation>>." << endl;
                                    cout << "Press 2052 to change condition to <<Ready to send>>." << endl;
                                    cout << "Press 2053 to change condition to <<Delivered>>." << endl;
                                    changingCondition = getValidatedInt();

                                    string newCondition = "";
                                    if (changingCondition == 2051) {
                                        newCondition = "In preparation";
                                    } else if (changingCondition == 2052) {
                                        newCondition = "Ready to send";
                                    } else if (changingCondition == 2053) {
                                        newCondition = "Delivered";
                                    } else {
                                        cout << "Wrong condition choice!" << endl;
                                    }

                                    if (!newCondition.empty()) {
                                        for (size_t i = 0; i < allOrders.size(); ++i) {
                                            if (allOrders[i]->getOrderID() == orderIdToChangeCondition) {
                                                allOrders[i]->setOrderCondition(newCondition);
                                                orderDao.updateOrderCondition(orderIdToChangeCondition, newCondition);
                                                updateCondition = true;
                                                break;
                                            }
                                        }
                                    }
                                }

                                if(updateCondition){
                                    cout << "Condition updated successfully!" << endl;
                                }
                            }

                            for (size_t i = 0; i < allOrders.size(); ++i) {
                                if (allOrders[i] != nullptr) delete allOrders[i];
                            }
                            allOrders.clear();

                            cout << "Press Enter." << endl;
                            cin.ignore(); cin.get();
                        }


                        else {
                            cout << "Invalid choice! Press Enter." << endl;
                            cin.ignore() ; cin.get();
                        }



                        for (int i = 0; i < loadedMenu.size(); ++i) {
                            if (loadedMenu[i] != nullptr) delete loadedMenu[i];
                        }
                        loadedMenu.clear();

                        loadedMenu = menuDao.getMenuByRestaurantId(restaurantManagerChoice);
                        chooseResInRestaurantManagerMenu->setMenu(loadedMenu);
                    }






                    loadedMenu.clear();
                    delete chooseResInRestaurantManagerMenu;
                    //////////////////////////////////////////////
                }
            }
        }






        //////////////////////////////////////////////////////System Manager/////////////////////////////////////////////
        else if (chooseRole == 3) {
            clearScreen();
            int systemManagerChoice = -1;
            while (true) {
                clearScreen();
                cout << "--- SYSTEM MANAGER MENU ---" << endl;
                cout << "Press 0 for exit!" << endl;
                cout << "Press 301 for add restaurant. " << endl;
                cout << "Press 302 for Access to restaurant activity." << endl;
                cout << "Press 303 for show reports." << endl;
                cout << "Press 304 for delete restaurant." << endl;
                cout << "Press 305 for Show restaurant`s menu." << endl;
                cout << "Press 306 for Show all restaurants." << endl;
                cout << "Press 307 for delete restaurant`s menu item." << endl;
                cout << "Press 308 for add restaurant`s menu item." << endl;
                cout << "Press 309 for change customer point and level." << endl;
                cout << "Press 310 for show user level change history." << endl;
                cout << "Press 311 for show number of users at each level." << endl;
                cout << "Press 312 for show all customers with details." << endl;
                systemManagerChoice = getValidatedInt();

                if (systemManagerChoice == 0) {
                    break;
                }
///////////////////////////////////////////////////////Add restaurant///////////////////////////////////////////////////
                else if(systemManagerChoice == 301){
                    clearScreen();
                    int resID , resOrderPreparationTime;
                    string resName , resAddress , resPhoneNumber , resDescription;
                    bool resInActive;


                    cout << "Enter ID : " << endl;
                    resID = getValidatedInt();
                    cout << "Enter Activity : " << endl;
                    resInActive = getValidatedBool();
                    cout << "Enter Order preparation time : " << endl;
                    resOrderPreparationTime = getValidatedInt();

                    cin.ignore(10000, '\n');

                    cout << "Enter Name: " << endl;
                    getline(cin , resName);
                    cout << "Enter Address : " << endl;
                    getline(cin , resAddress);
                    cout << "Enter Phone Number : " << endl;
                    getline(cin , resPhoneNumber);
                    cout << "Enter Description : " << endl;
                    getline(cin , resDescription);


                    Restaurant* res = new Restaurant();

                    res->setID(resID);
                    res->setName(resName);
                    res->setAddress(resAddress);
                    res->setInActive(resInActive);
                    res->setOrderPreparationTime(resOrderPreparationTime);
                    res->setPhoneNumber(resPhoneNumber);
                    res->setDescription(resDescription);

                    restaurants.push_back(res);
                    restaurantDao.addRestaurant(*res);


                    int addMenu = -1;

                    while(true){
                        cout << "1. Add Food" << endl;
                        cout << "2. Add Drink" << endl;
                        cout << "0. Finish adding menu items" << endl;

                        addMenu = getValidatedInt();

                        if(addMenu == 0) break;

                        else if(addMenu == 1){
                            addFoodToMenu(resID , menuDao);
                        }


                        else if(addMenu == 2){
                            addDrinkToMenu(resID , menuDao);
                        }

                        else {
                            cout << "Invalid Choice!" << endl;
                        }
                    }
                    cin.ignore() ; cin.get();

                }
                    ////////////////////////////////////////////////////Access to restaurant activity///////////////////////////////////

                else if(systemManagerChoice == 302){
                    int restaurantID = -1 ;
                    bool changeAvailability = false;
                    clearScreen();
                    cout << "Enter Restaurant ID to access to availability : " << endl;
                    restaurantID = getValidatedInt();
                    cout << "Enter availability : " << endl;
                    changeAvailability = getValidatedBool();

                    Restaurant *res = restaurantDao.getRestaurantById(restaurantID);

                    if (res == nullptr) {
                        cout << "Restaurant with ID " << restaurantID << " not found!" << endl;
                    }
                    else{
                        res->setInActive(changeAvailability);

                        if(restaurantDao.updateRestaurant(restaurantID , *res)){
                            cout << "Restaurant availability updated." << endl;

                            for (int i = 0; i < restaurants.size(); ++i) {
                                if (restaurants[i]->getID() == restaurantID) {
                                    restaurants[i]->setInActive(res->getInActive());
                                    break;
                                }
                            }
                        }
                        else {
                            cout << "Failed to update restaurant status!" << endl;
                        }

                        delete res;

                    }

                    cout << "Press Enter to continue.";
                    cin.ignore(); cin.get();

                }
////////////////////////////////////////////////////////////////Show Reports////////////////////////////////////////////


                else if(systemManagerChoice == 303){
                    clearScreen();
                    long double totalSales = 0;
                    int orderNumber = -1;
                    int totalCustomers = 0;

                    auto allOrders = orderDao.getAllOrders();
                    totalCustomers = orderDao.getCustomerCount();

                    if(allOrders.empty()){
                        cout << "No orders found in database!" << endl;
                        cout << "Total Customers registered: " << totalCustomers << endl;
                    }
                    else {
                        for (int i = 0; i < allOrders.size(); ++i) {
                            totalSales += allOrders[i]->getTotalPrice();
                        }
                        orderNumber = allOrders.size();

                        cout << "================== SYSTEM STATISTICS ==================" << endl;
                        cout << "Total Sales is : $" << totalSales << endl;
                        cout << "Number of orders is : " << orderNumber << endl;
                        cout << "Total Customers registered is : " << totalCustomers << endl;
                        cout << "=======================================================" << endl;
                    }

                    for(int i = 0; i < allOrders.size(); ++i) {
                        if (allOrders[i] != nullptr) {
                            delete allOrders[i];
                        }
                    }
                    allOrders.clear();

                    cout << "Press Enter." <<endl;
                    cin.ignore(); cin.get();
                }

///////////////////////////////////////////////////////////////Delete Restaurant////////////////////////////////////////
                else if(systemManagerChoice == 304 ){
                    clearScreen();
                    cout << "Enter Restaurant ID to delete: " << endl;
                    int deleteRestaurantID = -1;
                    bool findAndDeleteRestaurant = false;
                    deleteRestaurantID = getValidatedInt();

                    for (int i = 0; i < restaurants.size() ; ++i) {
                        if (restaurants[i]->getID() == deleteRestaurantID){
                            delete restaurants[i];
                            restaurants.erase(restaurants.begin() + i);
                            findAndDeleteRestaurant = true;
                            break;
                        }
                    }

                    bool deleteRestaurantFromDatabase = restaurantDao.deleteRestaurant(deleteRestaurantID);

                    if(findAndDeleteRestaurant || deleteRestaurantFromDatabase){
                        cout << "Restaurant with ID " << deleteRestaurantID << " deleted!" << endl;
                    }
                    else {
                        cout << " There is no restaurants with ID : " << deleteRestaurantID << endl;
                    }
                    cin.ignore(); cin.get();
                }
                    ///////////////////////////////////////////////Show restaurant`s menu///////////////////////////////////

                else if(systemManagerChoice == 305){
                    clearScreen();
                    bool findResToShowMenu = false;
                    int showMenuID = -1;
                    cout << "Enter Restaurant`s ID to show its menu: " << endl;
                    showMenuID = getValidatedInt();

                    Restaurant* findRestaurantToShowMenu = restaurantDao.getRestaurantById(showMenuID);

                    if (findRestaurantToShowMenu != nullptr) {
                        vector<menuItems*> loadedMenu = menuDao.getMenuByRestaurantId(showMenuID);

                        if(!loadedMenu.empty()) {
                            findRestaurantToShowMenu->setMenu(loadedMenu);
                            cout << "\n--- MENU ITEMS ---" << endl;
                            findRestaurantToShowMenu->printMyMenu();
                            findResToShowMenu = true;
                        } else {
                            cout << "This restaurant exists but its menu is empty!" << endl;
                            findResToShowMenu = true;
                        }


                        loadedMenu.clear();

                        delete findRestaurantToShowMenu;
                    }

                    if(!findResToShowMenu) {
                        cout << "There is no restaurant with ID: " << showMenuID << endl;
                    }

                    cout << "Press Enter to return to manager menu.";
                    cin.ignore(10000, '\n');
                    cin.get();
                }
////////////////////////////////////////////////////////////Show all restaurants////////////////////////////////////////
                else if (systemManagerChoice == 306) {
                    clearScreen();

                    vector<Restaurant> allRestaurantsToShow = restaurantDao.getAllRestaurants();

                    if (allRestaurantsToShow.empty()) {
                        cout << "No restaurants found in the database!" << endl;
                    }
                    else {
                        for (const auto& res : allRestaurantsToShow) {
                            cout << "ID : " << res.getID()
                                 << " | Name : " << res.getName()
                                 << " | Address : " << res.getAddress()
                                 << " | Availability : " << res.getInActive() << endl;
                            cout << "-------------------------------" << endl;
                        }
                    }
                    cout << "Press Enter to continue." << endl;
                    cin.ignore(); cin.get();
                }

////////////////////////////////////////////////////////////Delete restaurant`s menu item///////////////////////////////
                else if(systemManagerChoice == 307){
                    clearScreen();

                    cout << "Enter Restaurant ID and Menu Item ID to delete it: " << endl;
                    int deleteRestaurantID = -1 , deleteRestaurantMenuItemID = -1;
                    deleteRestaurantID = getValidatedInt();
                    deleteRestaurantMenuItemID = getValidatedInt();

                    Restaurant* currentRes = restaurantDao.getRestaurantById(deleteRestaurantID);

                    if (currentRes == nullptr) {
                        cout << "Restaurant with ID " << deleteRestaurantID << " not found!" << endl;
                    }

                    else {
                        if (menuDao.deleteMenuItem(deleteRestaurantMenuItemID)) {
                            cout << "Item deleted from the restaurant's menu!" << endl;

                        } else {
                            cout << "Item not found!" << endl;
                        }
                    }
                    delete currentRes;

                    cout << "Press Enter to continue." ;
                    cin.ignore(); cin.get();

                }


                //////////////////////////////////////////Add restaurant`s item/////////////////////////////////////////
                else if(systemManagerChoice == 308){
                    clearScreen();
                    int RestaurantID = -1;

                    cout << "Enter Restaurant ID : " << endl;
                    RestaurantID = getValidatedInt();

                    Restaurant* currentRes = restaurantDao.getRestaurantById(RestaurantID);

                    if (currentRes == nullptr) {
                        cout << "Restaurant with ID " << RestaurantID << " not found!" << endl;
                    }
                    else{
                        int addMenuItems = -1;
                        while(true){
                            cout << "1. Add Food" << endl;
                            cout << "2. Add Drink" << endl;
                            cout << "0. Finish adding menu items" << endl;

                            addMenuItems = getValidatedInt();

                            if(addMenuItems == 0) break;

                            else if(addMenuItems == 1){
                                addFoodToMenu(RestaurantID , menuDao);
                            }


                            else if(addMenuItems == 2){
                                addDrinkToMenu(RestaurantID , menuDao);
                            }

                            else {
                                cout << "Invalid Choice!" << endl;
                            }
                        }
                        cin.ignore() ; cin.get();
                    }

                    delete currentRes;
                }


                /////////////////////////////////////Change Customer Level//////////////////////////////////////////////

                else if(systemManagerChoice == 309){
                    int customerTargetId , newCustomerPoint;
                    cout << "Enter Customer ID : " << endl;
                    customerTargetId = getValidatedInt();

                    cout << "Enter customer new point : " << endl;
                    newCustomerPoint = getValidatedInt();

                    Customer* currentCust = customerDao.getCustomerById(customerTargetId);

                    if (currentCust != nullptr) {
                        currentCust->setCustomerPoints(newCustomerPoint);

                        if (orderDao.adminUpdateCustomerPoints(currentCust, newCustomerPoint)) {
                            cout << "Changes successfully applied to the database." << endl;
                            cout << "Customer updated to Level: " << currentCust->getCustomerLevel() << endl;
                        } else {
                            cout << "Error updating database." << endl;
                        }

                        delete currentCust;
                    } else {
                        cout << "Customer not found!" << endl;
                    }

                    cout << "Press Enter." << endl;
                    cin.ignore();
                    cin.get();
                }

                else if(systemManagerChoice == 310){
                    int customerTargetId;
                    cout << "Enter Customer ID : " << endl;

                    customerTargetId = getValidatedInt();

                    orderDao.printLevelHistory(customerTargetId);

                    cout << "---------------------------------------------------\nPlease Enter." << endl;
                    cin.ignore();
                    cin.get();

                }

                else if(systemManagerChoice == 311){
                    orderDao.printCustomerCountPerLevel();
                    cout << "---------------------------------------------------\nPlease Enter." << endl;
                    cin.ignore();
                    cin.get();
                }

                else if(systemManagerChoice == 312){
                    orderDao.printAllCustomersWithDetails();
                    cout << "---------------------------------------------------\nPlease Enter." << endl;
                    cin.ignore();
                    cin.get();
                }

            }
        }
        else {
            cout << "You have chosen the wrong number. Try again! (Press Enter) " << endl;
            cin.ignore();
            cin.get();
        }

    }




    return 0;
}