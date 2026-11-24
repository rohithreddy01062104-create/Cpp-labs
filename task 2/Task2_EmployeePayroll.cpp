// OOP Using C++ - Task 2 (Weeks 7-13) : Employee Payroll Management System
// Inheritance, runtime polymorphism, exception handling, file handling,
// templates, STL vector, STL algorithms, lambda expressions
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
#include <stdexcept>
using namespace std;

class InvalidSalaryException : public runtime_error {
public:
    InvalidSalaryException(double s)
        : runtime_error("Invalid salary " + to_string((long)s) + " (must be positive)") {}
};

// Template: generic maximum finder
template <typename T, typename Compare>
const T* findBest(const vector<T>& v, Compare cmp) {
    if (v.empty()) return nullptr;
    return &*max_element(v.begin(), v.end(), cmp);
}

// Base class
class Employee {
protected:
    int id;
    string name;
    double basic;
public:
    Employee(int i, const string& n, double b) : id(i), name(n), basic(b) {
        if (b <= 0) throw InvalidSalaryException(b);
    }
    virtual ~Employee() {}
    int getId() const { return id; }
    string getName() const { return name; }
    virtual double netSalary() const { return basic; }
    virtual string role() const { return "Employee"; }
    virtual void displayDetails() const {
        cout << left << setw(6) << id << setw(12) << name << setw(12) << role();
    }
    virtual void displayPay() const {
        cout << "Net Salary: " << fixed << setprecision(2) << netSalary() << endl;
    }
};

// Derived class 1
class Manager : public Employee {
    double bonus;
public:
    Manager(int i, const string& n, double b, double bon) : Employee(i, n, b), bonus(bon) {}
    double netSalary() const override { return basic + bonus; }
    string role() const override { return "Manager"; }
    void displayPay() const override {
        cout << "Basic: " << basic << " + Bonus: " << bonus
             << " = Net: " << fixed << setprecision(2) << netSalary() << endl;
    }
};

// Derived class 2
class Developer : public Employee {
    int overtimeHours;
public:
    Developer(int i, const string& n, double b, int ot) : Employee(i, n, b), overtimeHours(ot) {}
    double netSalary() const override { return basic + overtimeHours * 250.0; }
    string role() const override { return "Developer"; }
    void displayPay() const override {
        cout << "Basic: " << basic << " + OT(" << overtimeHours << "h x 250) = Net: "
             << fixed << setprecision(2) << netSalary() << endl;
    }
};

struct Record { int id; string name, role; double net; };

int main() {
    vector<Employee*> staff;

    // Add employees; invalid salary throws an exception
    try { staff.push_back(new Manager(1, "Anita", 60000, 15000)); } catch (const exception& e) { cout << "Error: " << e.what() << endl; }
    try { staff.push_back(new Developer(2, "Ravi", 45000, 12)); } catch (const exception& e) { cout << "Error: " << e.what() << endl; }
    try { staff.push_back(new Developer(3, "Meena", -500, 5)); } catch (const exception& e) { cout << "Error: " << e.what() << endl; }
    try { staff.push_back(new Manager(4, "Karthik", 70000, 20000)); } catch (const exception& e) { cout << "Error: " << e.what() << endl; }
    try { staff.push_back(new Developer(5, "Sana", 38000, 20)); } catch (const exception& e) { cout << "Error: " << e.what() << endl; }

    // Runtime polymorphism
    cout << "\n===== PAYROLL REPORT =====" << endl;
    for (const Employee* e : staff) {
        e->displayDetails();
        cout << endl << "      ";
        e->displayPay();
    }

    // Store records in file
    {
        ofstream out("payroll.txt");
        if (!out) { cout << "File error." << endl; return 1; }
        for (const Employee* e : staff)
            out << e->getId() << ' ' << e->getName() << ' ' << e->role() << ' ' << e->netSalary() << '\n';
    }
    cout << "\nPayroll saved to payroll.txt" << endl;

    // Retrieve records into a vector
    vector<Record> records;
    {
        ifstream in("payroll.txt");
        Record r;
        while (in >> r.id >> r.name >> r.role >> r.net) records.push_back(r);
    }
    cout << "Records retrieved: " << records.size() << endl;

    // Lambda + STL algorithms
    double limit = 60000;
    cout << "\n== Employees earning above " << limit << " ==" << endl;
    for_each(records.begin(), records.end(), [limit](const Record& r) {
        if (r.net > limit) cout << r.id << " " << r.name << " (" << r.role << ") - " << r.net << endl;
    });

    long managers = count_if(records.begin(), records.end(),
                             [](const Record& r) { return r.role == "Manager"; });
    cout << "\nManagers: " << managers << ", Developers: " << records.size() - managers << endl;

    sort(records.begin(), records.end(),
         [](const Record& a, const Record& b) { return a.net > b.net; });
    cout << "\n== Ranked by salary ==" << endl;
    int rank = 1;
    for (const Record& r : records) cout << rank++ << ". " << r.name << " - " << r.net << endl;

    const Record* lowest = findBest(records, [](const Record& a, const Record& b) { return a.net > b.net; });
    if (lowest) cout << "\nLowest paid (template findBest): " << lowest->name << " - " << lowest->net << endl;

    for (Employee* e : staff) delete e;
    return 0;
}
