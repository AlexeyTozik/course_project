﻿#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <time.h>
#include <windows.h>
#include <sstream>


// Определение класса User
class User {
public:
    User(const std::string& login, const std::string& password, int role)
        : login_(login), password_(password), role_(role) {}

    const std::string& getLogin() const { return login_; }
    const std::string& getPassword() const { return password_; }
    int getRole() const { return role_; }

private:
    std::string login_;
    std::string password_;
    int role_;
};


// Определение класса DataManager
class DataManager {
public:
    DataManager(const std::string& filename) : filename_(filename), file_open_(false) {}

    bool createFile();
    bool openFile();
    bool deleteFile();

    std::fstream& getFile() { return file_; }

private:
    std::string filename_;
    std::fstream file_;
    bool file_open_;
};

bool DataManager::createFile() {
    if (file_open_) {
        std::cerr << "File is already open. Please close it before creating a new file." << std::endl;
        return false;
    }

    file_.open(filename_, std::ios::out);
    if (!file_.is_open()) {
        std::cerr << "Error creating file." << std::endl;
        return false;
    }

    file_.close();
    return true;
}

bool DataManager::openFile() {
    if (file_open_) {
        std::cerr << "File is already open." << std::endl;
        return false;
    }

    file_.open(filename_, std::ios::in | std::ios::out | std::ios::app);
    if (!file_.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return false;
    }

    file_open_ = true;
    return true;
}

bool DataManager::deleteFile() {
    if (file_open_) {
        file_.close();
    }

    if (std::remove(filename_.c_str()) != 0) {
        std::cerr << "Error deleting file." << std::endl;
        return false;
    }

    return true;
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

User* authorize(const std::string& login, const std::string& password) {
    DataManager userManager("users.txt");
    if (!userManager.openFile()) {
        std::cerr << "Error opening users file." << std::endl;
        return nullptr;
    }

    std::string line;
    while (std::getline(userManager.getFile(), line)) {
        std::vector<std::string> tokens = split(line, ';');
        if (tokens.size() != 3) {
            continue;
        }

        if (tokens[0] == login && tokens[1] == password) {
            int role = std::stoi(tokens[2]);
            User* user = new User(login, password, role);
            return user;
        }
    }

    return nullptr;
}

class Vehicle {
public:
    enum class Type { Taxi, Minibus, Limousine };
    Vehicle(Type type, int capacity, double fuel_consumption, double cost, int quantity)
        : type_(type), capacity_(capacity), fuel_consumption_(fuel_consumption),
        cost_(cost), quantity_(quantity) {}

    Type get_type() const { return type_; }
    int get_capacity() const { return capacity_; }
    double get_fuel_consumption() const { return fuel_consumption_; }
    double get_cost() const { return cost_; }
    int get_quantity() const { return quantity_; }

    void print() const {
        std::cout << "Тип: " << static_cast<int>(type_) << ", "
            << "Вместимость: " << capacity_ << ", "
            << "Расход бензина: " << fuel_consumption_ << ", "
            << "Стоимость: " << cost_ << ", "
            << "Количество: " << quantity_ << std::endl;
    }

private:
    Type type_;
    int capacity_;
    double fuel_consumption_;
    double cost_;
    int quantity_;
};



class Menu {
public:
    Menu(User* user) : user_(user) {}
    void display() {
        if (user_->getRole() == 1) {
            displayAdminMenu();
        }
        else {
            displayUserMenu();
        }
    }

    int get_choice() {
        std::cout << "Выберите действие: ";
        int choice;
        std::cin >> choice;
        return choice;
    }

private:
    User* user_;

    void displayAdminMenu() {
        std::cout << "\n";
        std::cout << "======================= Меню администратора =======================\n";
        std::cout << "1. Добавить автомобиль\n";
        std::cout << "2. Подсчитать общую стоимость таксопарка\n";
        std::cout << "3. Подсчитать общую стоимость автомобилей каждого вида\n";
        std::cout << "4. Подобрать автомобили по заданной вместимости\n";
        std::cout << "5. Работа с учетными записями\n";
        std::cout << "6. Показать все данные\n";
        std::cout << "7. Сортировать по типу\n";
        std::cout << "8. Сортировать по вместимости\n";
        std::cout << "9. Сортировать по расходу топлива\n";
        std::cout << "10. Сортировать по стоимости\n";
        std::cout << "11. Сортировать по количеству\n";
        std::cout << "12. Выход\n";
        std::cout << "===================================================================\n";
    }

    void displayUserMenu() {
        std::cout << "\n";
        std::cout << "========================= Меню пользователя ========================\n";
        std::cout << "1. Подсчитать общую стоимость таксопарка\n";
        std::cout << "2. Подсчитать общую стоимость автомобилей каждого вида\n";
        std::cout << "3. Подобрать автомобили по заданной вместимости\n";
        std::cout << "4. Показать все данные\n";
        std::cout << "5. Сортировать по типу\n";
        std::cout << "6. Сортировать по вместимости\n";
        std::cout << "7. Сортировать по расходу топлива\n";
        std::cout << "8. Сортировать по стоимости\n";
        std::cout << "9. Сортировать по количеству\n";
        std::cout << "10. Выход\n";
        std::cout << "===================================================================\n";
    }
};


void calculate_total_cost(const std::vector<Vehicle>& vehicles) {
    double total_cost = 0;
    for (const auto& vehicle : vehicles) {
        total_cost += vehicle.get_cost() * vehicle.get_quantity();
    }
    std::cout << "Общая стоимость таксопарка: " << total_cost << std::endl;
}

void calculate_cost_by_type(const std::vector<Vehicle>& vehicles) {
    std::vector<double> costs(3, 0);
    for (const auto& vehicle : vehicles) {
        int type_index = static_cast<int>(vehicle.get_type());
        costs[type_index] += vehicle.get_cost() * vehicle.get_quantity();
    }
    std::cout << "Стоимость автомобилей каждого вида:\n";
    std::cout << "Такси: " << costs[0] << std::endl;
    std::cout << "Микроавтобусы: " << costs[1] << std::endl;
    std::cout << "Лимузины: " << costs[2] << std::endl;
}

void select_vehicles_by_capacity(const std::vector<Vehicle>& vehicles) {
    int min_capacity, max_capacity;
    std::cout << "Введите минимальную вместимость: ";
    std::cin >> min_capacity;
    std::cout << "Введите максимальную вместимость: ";
    std::cin >> max_capacity;
    std::cout << "Автомобили с заданной вместимостью:\n";
    for (const auto& vehicle : vehicles) {
        if (vehicle.get_capacity() >= min_capacity && vehicle.get_capacity() <= max_capacity) {
            std::string type_str;
            switch (vehicle.get_type()) {
            case Vehicle::Type::Taxi:
                type_str = "Такси";
                break;
            case Vehicle::Type::Minibus:
                type_str = "Микроавтобус";
                break;
            case Vehicle::Type::Limousine:
                type_str = "Лимузин";
                break;
            }
            std::cout << type_str << ": вместимость - " << vehicle.get_capacity() << std::endl;
        }
    }
}

void load_vehicles(std::vector<Vehicle>& vehicles, const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        vehicles.clear();
        int type, capacity, quantity;
        double fuel_consumption, cost;
        while (file >> type >> capacity >> fuel_consumption >> cost >> quantity) {
            vehicles.emplace_back(static_cast<Vehicle::Type>(type), capacity, fuel_consumption, cost, quantity);
        }
        file.close();
    }
    else {
        std::cerr << "Не удалось открыть файл: " << filename << std::endl;
    }
}

void save_vehicles(const std::vector<Vehicle>& vehicles, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& vehicle : vehicles) {
            file << static_cast<int>(vehicle.get_type()) << ' '
                << vehicle.get_capacity() << ' '
                << vehicle.get_fuel_consumption() << ' '
                << vehicle.get_cost() << ' '
                << vehicle.get_quantity() << std::endl;
        }
        file.close();
    }
    else {
        std::cerr << "Не удалось открыть файл: " << filename << std::endl;
    }
}

void add_vehicle(std::vector<Vehicle>& vehicles) {
    int type, capacity, quantity;
    double fuel_consumption, cost;
    std::cout << "Введите вид автомобиля (0 - Такси, 1 - Микроавтобус, 2 - Лимузин): ";
    std::cin >> type;
    std::cout << "Введите вместимость: ";
    std::cin >> capacity;
    std::cout << "Введите расход топлива: ";
    std::cin >> fuel_consumption;
    std::cout << "Введите стоимость автомобиля: ";
    std::cin >> cost;
    std::cout << "Введите количество автомобилей данного вида: ";
    std::cin >> quantity;
    vehicles.emplace_back(static_cast<Vehicle::Type>(type), capacity, fuel_consumption, cost, quantity);
}

// Добавьте новую функцию `add_account`
void add_account(const std::string& users_filename) {
    std::string login, password;
    int role;
    std::cout << "Введите логин: ";
    std::cin >> login;
    std::cout << "Введите пароль: ";
    std::cin >> password;
    std::cout << "Введите роль (0 - Пользователь, 1 - Администратор): ";
    std::cin >> role;

    DataManager userManager(users_filename);
    if (!userManager.openFile()) {
        std::cerr << "Error opening users file." << std::endl;
        return;
    }

    userManager.getFile() << login << ";" << password << ";" << role << std::endl;
    userManager.getFile().close();
    std::cout << "Учетная запись успешно добавлена." << std::endl;
}


// Добавьте новую функцию `remove_account`
void remove_account(const std::string& users_filename) {
    std::string login_to_remove;
    std::cout << "Введите логин удаляемой учетной записи: ";
    std::cin >> login_to_remove;

    DataManager userManager(users_filename);
    if (!userManager.openFile()) {
        std::cerr << "Error opening users file." << std::endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(userManager.getFile(), line)) {
        std::vector<std::string> tokens = split(line, ';');
        if (tokens.size() != 3) {
            continue;
        }
        if (tokens[0] != login_to_remove) {
            lines.push_back(line);
        }
    }

    userManager.getFile().close();
    userManager.deleteFile();
    userManager.createFile();
    userManager.openFile();

    for (const auto& line_to_write : lines) {
        userManager.getFile() << line_to_write << std::endl;
    }

    userManager.getFile().close();
    std::cout << "Учетная запись успешно удалена." << std::endl;
}

// Добавьте новую функцию `modify_account`
void modify_account(const std::string& users_filename) {
    std::string login_to_modify;
    std::cout << "Введите логин изменяемой учетной записи: ";
    std::cin >> login_to_modify;

    std::string new_login, new_password;
    int new_role;
    std::cout << "Введите новый логин: ";
    std::cin >> new_login;
    std::cout << "Введите новый пароль: ";
    std::cin >> new_password;
    std::cout << "Введите новую роль (0 - Пользователь, 1 - Администратор): ";
    std::cin >> new_role;
    
    DataManager userManager(users_filename);
    if (!userManager.openFile()) {
        std::cerr << "Error opening users file." << std::endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    bool account_found = false;
    while (std::getline(userManager.getFile(), line)) {
        std::vector<std::string> tokens = split(line, ';');
        if (tokens.size() != 3) {
            continue;
        }
        if (tokens[0] == login_to_modify) {
            lines.push_back(new_login + ";" + new_password + ";" + std::to_string(new_role));
            account_found = true;
        }
        else {
            lines.push_back(line);
        }
    }

    userManager.getFile().close();
    userManager.deleteFile();
    userManager.createFile();
    userManager.openFile();

    for (const auto& line_to_write : lines) {
        userManager.getFile() << line_to_write << std::endl;
    }

    userManager.getFile().close();

    if (account_found) {
        std::cout << "Учетная запись успешно изменена." << std::endl;
    }
    else {
        std::cout << "Учетная запись не найдена." << std::endl;
    }
}


// функцию manage_accounts
void manage_accounts(const std::string& users_filename) {
    while (true) {
        std::cout << "===================================================================\n";
        std::cout << "1. Добавить учетную запись\n"
            << "2. Удалить учетную запись\n"
            << "3. Изменить учетную запись\n"
            << "4. Вернуться в главное меню\n";
        std::cout << "===================================================================\n";
        std::cout << "Введите номер действия: ";

            int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            add_account(users_filename);
            break;
        case 2:
            remove_account(users_filename);
            break;
        case 3:
            modify_account(users_filename);
            break;
        case 4:
            return;
        default:
            std::cout << "Некорректный выбор. Попробуйте снова.\n";
        }
    }
}

void display_all_vehicles(const std::vector<Vehicle>& vehicles) {
    std::cout << "Все автомобили:\n";
    for (const auto& vehicle : vehicles) {
        vehicle.print();
    }
}

void sort_by_type(std::vector<Vehicle>& vehicles) {
    std::sort(vehicles.begin(), vehicles.end(), [](const Vehicle& a, const Vehicle& b) {
        return a.get_type() < b.get_type();
        });
}

void sort_by_capacity(std::vector<Vehicle>& vehicles) {
    std::sort(vehicles.begin(), vehicles.end(), [](const Vehicle& a, const Vehicle& b) {
        return a.get_capacity() < b.get_capacity();
        });
}

void sort_by_fuel_consumption(std::vector<Vehicle>& vehicles) {
    std::sort(vehicles.begin(), vehicles.end(), [](const Vehicle& a, const Vehicle& b) {
        return a.get_fuel_consumption() < b.get_fuel_consumption();
        });
}

void sort_by_cost(std::vector<Vehicle>& vehicles) {
    std::sort(vehicles.begin(), vehicles.end(), [](const Vehicle& a, const Vehicle& b) {
        return a.get_cost() < b.get_cost();
        });
}

void sort_by_vehicle_count(std::vector<Vehicle>& vehicles) {
    std::sort(vehicles.begin(), vehicles.end(), [](const Vehicle& a, const Vehicle& b) {
        return a.get_quantity() < b.get_quantity();
        });
}


int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    // Получение логина и пароля от пользователя
    std::string login, password;
    std::cout << "Введите логин: ";
    std::cin >> login;
    std::cout << "Введите пароль: ";
    std::cin >> password;

    // Авторизация пользователя
    const std::string users_filename = "users.txt";
    User* user = authorize(login, password);
    // Проверка успешной авторизации
    if (!user) {
        std::cout << "Неверный логин или пароль. Выход из программы." << std::endl;
        return 1;
    }
    // Создаем объект класса Menu, передавая авторизованного пользователя
    Menu menu(user);
    std::vector<Vehicle> vehicles;
    const std::string filename = "cars.txt";

    load_vehicles(vehicles, filename);

    while (true) {
        // Отображаем соответствующее меню, в зависимости от роли пользователя
        menu.display();
        int choice = menu.get_choice();
        // Если пользователь является администратором (роль 1)
        if (user->getRole() == 1) {
            // Обработка действий администратора
            switch (choice) {
            case 1:
                add_vehicle(vehicles);
                save_vehicles(vehicles, filename);
                break;
            case 2:
                calculate_total_cost(vehicles);
                break;
            case 3:
                calculate_cost_by_type(vehicles);
                break;
            case 4:
                select_vehicles_by_capacity(vehicles);
                break;
            case 5:
                manage_accounts(users_filename);
                break;
            case 6:
                display_all_vehicles(vehicles);
                break;
            case 7:
                sort_by_type(vehicles);
                display_all_vehicles(vehicles);
                break;
            case 8:
                sort_by_capacity(vehicles);
                display_all_vehicles(vehicles);
                break;
            case 9:
                sort_by_fuel_consumption(vehicles);
                display_all_vehicles(vehicles);
                break;
            case 10:
                sort_by_cost(vehicles);
                display_all_vehicles(vehicles);
                break;
            case 11:
                sort_by_vehicle_count(vehicles);
                display_all_vehicles(vehicles);
                break;
            case 12:
                std::cout << "Выход...\n";
                return 0;
            default:
                std::cout << "Некорректный выбор. Попробуйте снова.\n";
            }
        }
        else {
            // Обработка действий пользователя (не администратора)
            switch (choice) {
            case 1:
                calculate_total_cost(vehicles);
                break;
            case 2:
                calculate_cost_by_type(vehicles);
                break;
            case 3:
                select_vehicles_by_capacity(vehicles);
                break;
            case 4:
                std::cout << "Выход...\n";
                return 0;
            default:
                std::cout << "Некорректный выбор. Попробуйте снова.\n";
            }
        }
    }

    return 0;
}