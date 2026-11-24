// OOP Using C++ - Task 1 (Weeks 1-6) : Bank Account Management
// Classes & objects, encapsulation, parameterized constructor, dynamic memory,
// static member, friend function, function overloading, operator overloading (>)
#include <iostream>
#include <cstring>
using namespace std;

class Account {
private:
    int accNo;
    char* holder;                 // dynamically allocated
    double balance;
    static int totalAccounts;
    static double bankTotal;      // sum of all balances
public:
    Account(int no, const char* h, double bal) : accNo(no), balance(bal) {
        holder = new char[strlen(h) + 1];
        strcpy(holder, h);
        totalAccounts++;
        bankTotal += balance;
    }
    Account(const Account& a) : accNo(a.accNo + 1000), balance(a.balance) {
        holder = new char[strlen(a.holder) + 1];
        strcpy(holder, a.holder);
        totalAccounts++;
        bankTotal += balance;
    }
    ~Account() {
        bankTotal -= balance;
        totalAccounts--;
        delete[] holder;
    }

    // Overloaded deposit functions
    void deposit(double amt) {
        balance += amt;
        bankTotal += amt;
    }
    void deposit(double cash, double cheque) {
        deposit(cash + cheque);
    }

    bool withdraw(double amt) {
        if (amt > balance) { cout << "Insufficient balance for " << holder << endl; return false; }
        balance -= amt;
        bankTotal -= amt;
        return true;
    }

    void display() const {
        cout << "Acc No: " << accNo << " | Holder: " << holder
             << " | Balance: Rs." << balance << endl;
    }

    static int getCount() { return totalAccounts; }
    static double getBankTotal() { return bankTotal; }

    bool operator>(const Account& o) const { return balance > o.balance; }

    friend void transfer(Account& from, Account& to, double amt);
};

int Account::totalAccounts = 0;
double Account::bankTotal = 0;

// Friend function: accesses private balance directly
void transfer(Account& from, Account& to, double amt) {
    if (from.balance >= amt) {
        from.balance -= amt;
        to.balance += amt;
        cout << "Transferred Rs." << amt << " from " << from.holder << " to " << to.holder << endl;
    } else {
        cout << "Transfer failed: insufficient funds in " << from.holder << "'s account." << endl;
    }
}

int main() {
    Account a1(1001, "Lekhana", 5000);
    Account a2(1002, "Priya", 8000);

    cout << "--- Initial Accounts ---" << endl;
    a1.display();
    a2.display();
    cout << "Accounts open: " << Account::getCount()
         << " | Bank total: Rs." << Account::getBankTotal() << endl;

    cout << "\n--- Overloaded deposit ---" << endl;
    a1.deposit(1500);          // single amount
    a2.deposit(2000, 500);     // cash + cheque
    a1.display();
    a2.display();

    cout << "\n--- Withdraw ---" << endl;
    a1.withdraw(1000);
    a2.withdraw(20000);

    cout << "\n--- Friend function transfer ---" << endl;
    transfer(a2, a1, 3000);
    transfer(a1, a2, 50000);

    cout << "\n--- Comparison using > ---" << endl;
    if (a1 > a2) cout << "Lekhana has the higher balance." << endl;
    else         cout << "Priya has the higher balance (or equal)." << endl;

    {
        Account a3 = a1;       // copy constructor
        cout << "\nInside block accounts: " << Account::getCount() << endl;
    }
    cout << "After block accounts: " << Account::getCount() << endl;
    cout << "Final bank total: Rs." << Account::getBankTotal() << endl;
    return 0;
}
