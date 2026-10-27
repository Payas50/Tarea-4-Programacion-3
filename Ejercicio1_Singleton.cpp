#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
using namespace std;
class Account {
private:
    string id;
    double balance;
    mutable mutex mtx;
public:
    Account(const string& accountId,double initialBalance):id(accountId),balance(initialBalance) {}
    bool deposit(double amount) {
        if (amount<=0)
            return false;
        lock_guard<mutex> lock(mtx);
        balance+=amount;
        return true;
    }
    bool withdraw(double amount) {
        if (amount<=0)
            return false;
        lock_guard<mutex> lock(mtx);
        if (balance<amount)
            return false;
        balance-=amount;
        return true;
    }
    double getBalance() const {
        lock_guard<mutex> lock(mtx);
        return balance;
    }
    const string& getId() const {
        return id;
    }
};
class Transaction {
private:
    string type;
    string accountId;
    double amount;
    bool successful;
public:
    Transaction(const string& transactionType,const string& id,double value,bool result):type(transactionType),accountId(id),amount(value),successful(result) {}
    void print() const {
        cout<<type<<" | cuenta "<<accountId<<" | monto "<<amount<<" | "<<(successful?"exitosa":"rechazada")<<"\n";
    }
};
class TransactionManager {
private:
    TransactionManager() = default;
    TransactionManager(const TransactionManager&) = delete;
    TransactionManager& operator=(const TransactionManager&) = delete;
    mutex transactionsMutex;
    vector<Transaction> transactions;
public:
    static TransactionManager& getInstance() {
        static TransactionManager instance;
        return instance;
    }
    bool deposit(Account* account,double amount) {
        if (account==nullptr)
            return false;
        bool ok=account->deposit(amount);
        saveTransaction(Transaction("Deposito",account->getId(),amount,ok));
        return ok;
    }
    bool withdraw(Account* account,double amount) {
        if (account==nullptr)
            return false;
        bool ok=account->withdraw(amount);
        saveTransaction(Transaction("Retiro",account->getId(),amount,ok));
        return ok;
    }
    void printTransactions() {
        lock_guard<mutex> lock(transactionsMutex);
        for (const auto& transaction:transactions)
            transaction.print();
    }
private:
    void saveTransaction(const Transaction& transaction) {
        lock_guard<mutex> lock(transactionsMutex);
        transactions.push_back(transaction);
    }
};
int main() {
    auto& manager=TransactionManager::getInstance();
    Account account("A1",1000);
    vector<thread> threads;
    for (int i=0;i<10;++i) {
        threads.emplace_back([&manager,&account]() {
            manager.deposit(&account,100);
            manager.withdraw(&account,50);
        });
    }
    for (auto& t:threads)
        t.join();
    cout<<"Balance final: "<<account.getBalance()<<"\n";
    cout<<"Direccion manager: "<<&TransactionManager::getInstance()<<"\n";
    // Descomenta para ver el registro de operaciones:
    // manager.printTransactions();
    return 0;
}