#include <iostream>
#include <sstream>
#include <string>
#include <pqxx/pqxx>
#include <chrono>
using namespace std;
using namespace pqxx;

typedef struct
{
    int date;
    int month;
    int year;
} date;

// classes for books
class books
{
private:
    int id; // primary key
    string name;
    string author;
    int price;
    int quantity;
    pqxx::connection conn{"dbname=bookshop_management user=postgres password=hyper hostaddr=127.0.0.1 port=5432"};

public:
    void add();
    void update_price();
    void search();
    void update_quantity();
    void delete_book();
};
class suppliers
{
private:
    int id; // primary key
    string name;
    long int phone_number;
    string address_line_1;
    string address_line_2;
    string state;
    string country;
    int pincode;
    pqxx::connection conn{"dbname=bookshop_management user=postgres password=hyper hostaddr=127.0.0.1 port=5432"};

public:
    void add_supplier();
    void remove_supplier();
    void search_supplier_by_id();
};

class purchase
{
private:
    int order_id; // primary key
    int book_id;  // foreign_key ref books;
    int supplier_id;
    int quantity;
    date date_ordered;
    int date_of_arrival;
    char received;
    int invoice;

public:
    void new_order();
    void view_order();
    void cancel_order();
    void received_order();
    pqxx::connection conn{"dbname=bookshop_management user=postgres password=hyper hostaddr=127.0.0.1 port=5432"};
};
class employees
{
private:
    int id; // primary key
    string name;
    string gender;
    string address_line_1;
    string address_line_2;
    string city;
    string state;
    string phone;
    string date_of_joining;
    string salary;
    string manager_status; // default (T or F) def f
    pqxx::connection conn{"dbname=bookshop_management user=postgres password=Kitsaws@2002 hostaddr=127.0.0.1 port=5432"};

public:
    void add_employee();
    void search_employee();
    void assign_manager();
    void update_details();
    void remove_employee();
};
class members
{
private:
    string name, address_1, address_2, city, state, contact_number, email, valid, id, begin_date, end_date;

public:
    void add_member();
    void search_member();
    void refresh();
    pqxx::connection conn{"dbname=bookshop_management user=postgres password=Kitsaws@2002 hostaddr=127.0.0.1 port=5432"};
};
void members::add_member()
{
    cout << "Create ID : ";
    cin >> id;
    cout << "Enter name of member : ";
    cin >> name;
    cout << "Enter e-mail of member : ";
    cin >> email;
    cout << "Enter contact number of member : ";
    cin >> contact_number;
    cout << "Enter address (in 4 lines) : ";
    cin.ignore();
    getline(cin, address_1);
    getline(cin, address_2);
    getline(cin, city);
    getline(cin, state);
    cout << "Membership begin date : ";
    getline(cin , begin_date);
    cout << "Membership end date : ";
    getline(cin , end_date);

    try
    {
        pqxx::work txn(conn);
        txn.exec_params("INSERT INTO members(id,name,email,contact_number,address_1,address_2,city,state,begin_date,end_date)"
                        "VALUES($1,$2,$3,$4,$5,$6,$7,$8,$9,$10)",
                        id,name, email, contact_number, address_1, address_2,city,state,begin_date,end_date);
        txn.commit();
        cout << "Member added successfully\n\n";
        cout << "Enter Registered email to get ID : ";
        string search_email;
        cin >> search_email;
        pqxx::nontransaction N(conn);
        pqxx::result R = N.exec_params("SELECT * FROM members WHERE email = $1", search_email);
        N.commit();
        if(R.empty())
        {
            cout << "No member found with that email\n";
            return;
        }
        else
        {
            for (const auto &row : R)
            {
                cout << "ID: " << row["id"].as<std::string>() << endl;
                cout << "Name: " << row["name"].as<std::string>() << endl;
                cout << "Email: " << row["email"].as<std::string>() << endl;
                cout << "Contact Number: " << row["contact_number"].as<std::string>() << endl;
                cout << "Address: " << row["address_1"].as<std::string>() << endl;
                cout << "Address: " << row["address_2"].as<std::string>() << endl;
                cout << "City: " << row["city"].as<std::string>() << endl;
                cout << "State: " << row["state"].as<std::string>() << endl;
                cout << "Membership Status: " << row["membership_status"].as<std::string>() << endl;
                cout << "Begin Date: " << row["begin_date"].as<std::string>() << endl;
                cout << "End Date: " << row["end_date"].as<std::string>() << endl;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\nUnable to perform the operation.\n";
    }
};
void members :: search_member(){
    cout << "Enter id of member : ";
    cin >> id;
     pqxx::nontransaction N(conn);
        pqxx::result R = N.exec_params("SELECT * FROM members WHERE id = $1", id);
        N.commit();
        if(R.empty())
        {
            cout << "No member found with that ID\n";
            return;
        }
        else
        {
            for (const auto &row : R)
            {
                cout << "Name: " << row["name"].as<std::string>() << endl;
                cout << "Email: " << row["email"].as<std::string>() << endl;
                cout << "Contact Number: " << row["contact_number"].as<std::string>() << endl;
                cout << "Address: " << row["address_1"].as<std::string>() << endl;
                cout << "Address: " << row["address_2"].as<std::string>() << endl;
                cout << "City: " << row["city"].as<std::string>() << endl;
                cout << "State: " << row["state"].as<std::string>() << endl;
                cout << "Membership Status: " << row["membership_status"].as<std::string>() << endl;
                cout << "Begin Date: " << row["begin_date"].as<std::string>() << endl;
                cout << "End Date: " << row["end_date"].as<std::string>() << endl;
            }
        };
};
void members :: refresh(){

    try
    {
        pqxx::nontransaction N(conn);
    std::string refresh_query = "UPDATE members SET membership_status = 'INVALID' WHERE end_date <= CURRENT_DATE;";
    pqxx::result R = N.exec_params(refresh_query);
    N.commit();
    cout << "\nRefreshed Successfully";
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\nUnable to complete request";
    }
    
    

}
void employees::add_employee()
{
    std::cout << "Enter your ID for verification: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    try
    {
        pqxx::nontransaction N(conn);
        pqxx::result R = N.exec_params("SELECT manager_status FROM employees WHERE id = $1", id);

        if (R.empty() || R[0][0].as<std::string>() != "T")
        {
            std::cout << "\nYou don't have manager status and access to employee details.\n";
            return;
        }
        N.commit();

        std::cout << "\n\nEnter the details of the employee\n";
        std::cout << "Employee Name: ";
        std::getline(std::cin, name);
        std::cout << "Enter Employee Gender (M/F): ";
        std::getline(std::cin, gender);
        std::cout << "Enter Employee Address Line 1: ";
        std::getline(std::cin, address_line_1);
        std::cout << "Employee Address Line 2: ";
        std::getline(std::cin, address_line_2);
        std::cout << "Employee City: ";
        std::getline(std::cin, city);
        std::cout << "Employee State: ";
        std::getline(std::cin, state);
        std::cout << "Employee Contact Number: ";
        std::getline(std::cin, phone);
        std::cout << "Employee Date of Joining: ";
        std::getline(std::cin, date_of_joining);
        std::cout << "Employee Salary: ";
        std::getline(std::cin, salary);
        std::cout << "Manager Status: ";

        std::getline(std::cin, manager_status);

        try
        {
            pqxx::work txn(conn);
            txn.exec_params(
                "INSERT INTO employees (employee_name, gender, address_line_1, address_line_2, city, state, phone_number, date_of_joining, salary, manager_status) "
                "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10)",
                name, gender, address_line_1, address_line_2, city, state, phone, date_of_joining, salary, manager_status);
            txn.commit();
            std::cout << "\n\nEmployee has been added successfully.\n";
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << "\nSomething went wrong! Please try again.\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Database connection error: " << e.what() << "\n";
    }
};
void employees::search_employee()
{
    cout << "Enter ID for verfication";
    cin >> id;
    pqxx::nontransaction N(conn);
    pqxx::result R = N.exec_params("SELECT manager_status FROM employees WHERE id = $1", id);
    N.commit();
    if (R.empty() || R[0][0].as<std::string>() != "T")
    {
        cout << "Only manager is permitted to access modify details";
        return;
    };
    cout << "Enter the ID of employee";
    cin >> id;
    // he/she is a manager  std::cout << "NAME : " << search_result[0]["supplier_name"].as<std::string>() << std::endl;
    pqxx::nontransaction txn(conn);
    std::string search_query = "SELECT * FROM employees WHERE id = $1";
    pqxx::result search_result = N.exec_params(search_query, id);
    std::cout << "Name : " << search_result[0]["employee_name"].as<std::string>() << std::endl;
    std::cout << endl;
    std::cout << "Gender : " << search_result[0]["gender"].as<std::string>() << std::endl;
    std::cout << endl;
    std::cout << "Address_1: " << search_result[0]["address_line_1"].as<std::string>() << std::endl;
    std::cout << endl;
    std::cout << "Address_2 : " << search_result[0]["address_line_2"].as<std::string>() << std::endl;
    std::cout << endl;
    std::cout << "City : " << search_result[0]["city"].as<std::string>() << std::endl;
    std::cout << endl;
    std::cout << "State : " << search_result[0]["state"].as<std::string>() << std::endl;
    std::cout << endl;
    std::cout << "Contact : " << search_result[0]["phone_number"].as<std::string>() << std::endl;
    std::cout << endl;
    std::cout << "Date of joining : " << search_result[0]["date_of_joining"].as<std::string>() << std::endl;
    std::cout << endl;
    std::cout << "Salary : " << search_result[0]["salary"].as<std::string>() << std::endl;
};
// assign manager
void employees::assign_manager()
{
    cout << "Enter ID for verification";
    cin >> id;
    pqxx::nontransaction N(conn);
    pqxx::result R = N.exec_params("SELECT manager_status FROM employees WHERE id = $1", id);
    N.commit();
    if (R.empty() || R[0][0].as<std::string>() != "T")
    {
        cout << "Only manager is permitted to access modify details";
        return;
    };
    cout << "Enter ID to assign as manager";
    cin >> id;
    try
    {
        pqxx::work txn(conn);
        std::string update_query = "UPDATE employees SET manager_status = 'T' WHERE id = $1";
        pqxx::result update_result = txn.exec_params(update_query, id);
        txn.commit(); // Commit the update transaction
        cout << "\nManager Status updated successfully.\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error updating manager status: " << e.what() << "\n";
    }
};

// instead of updating each trying to update all at once
void employees::update_details()
{
    cout << "Enter ID for verification: ";
    cin >> id;

    pqxx::nontransaction N(conn);
    pqxx::result R = N.exec_params("SELECT manager_status FROM employees WHERE id = $1", id);
    N.commit(); // Commit the verification transaction

    if (R.empty() || R[0][0].as<std::string>() != "T")
    {
        cout << "Only manager is permitted to access modify details.\n";
        return;
    }
    cout << "Enter ID of Employee to update : ";
    cin >> id;
    pqxx::nontransaction N2(conn);
    pqxx::result search_result = N2.exec_params("SELECT * FROM employees WHERE id = $1", id);
    N2.commit(); // Commit the search transaction

    if (search_result.empty())
    {
        cout << "Invalid ID.\n";
        return;
    }
    auto row = search_result[0];
    cout << "Current Employee Details:\n";
    cout << "ID: " << row["id"].as<int>() << "\n";
    cout << "Name: " << row["employee_name"].as<std::string>() << "\n";
    cout << "Gender: " << row["gender"].as<std::string>() << "\n";
    cout << "Address Line 1: " << row["address_line_1"].as<std::string>() << "\n";
    cout << "Address Line 2: " << row["address_line_2"].as<std::string>() << "\n";
    cout << "City: " << row["city"].as<std::string>() << "\n";
    cout << "State: " << row["state"].as<std::string>() << "\n";
    cout << "Contact Number: " << row["phone_number"].as<std::string>() << "\n";
    cout << "Date of Joining: " << row["date_of_joining"].as<std::string>() << "\n";
    cout << "Salary: " << row["salary"].as<float>() << "\n";
    cout << "Manager Status: " << row["manager_status"].as<std::string>() << "\n";

    try
    {
        string column_name;
        int option;
        cout << "\n\nEnter Number to update";
        cout << "\n1. SALARY ";
        cout << "\n2. EMPLOYEE_NAME ";
        cout << "\n3. ADDRESS LINE 1 ";
        cout << "\n4. ADDRESS LINE 2 ";
        cout << "\n5. CITY ";
        cout << "\n6. STATE";
        cout << "\n7. CONTACT NUMBER\n";
        cin >> option;
        pqxx::work txn(conn);
        if (option == 1)
        {
            cout << "Enter new salary :";
            cin >> salary;
            column_name = "SALARY  ";
            std::string update_query = "UPDATE employees SET salary = $1 WHERE id = $2";
            pqxx::result update_result = txn.exec_params(update_query, salary, id);
        }
        else if (option == 2)
        {
            cout << "Enter new employee_name :";
            cin >> name;
            column_name = "EMPLOYEE NAME  ";
            std::string update_query = "UPDATE employees SET employee_name = $1 WHERE id = $2";
            pqxx::result update_result = txn.exec_params(update_query, name, id);
        }
        else if (option == 3)
        {
            cout << "Enter new address 1 :";
            cin >> address_line_1;
            column_name = "EMPLOYEE ADDRESS 1  ";
            std::string update_query = "UPDATE employees SET address_line_1 = $1 WHERE id = $2";
            pqxx::result update_result = txn.exec_params(update_query, address_line_1, id);
        }
        else if (option == 4)
        {
            cout << "Enter new address 2 :";
            cin >> address_line_2;
            column_name = "EMPLOYEE ADDRESS 2  ";
            std::string update_query = "UPDATE employees SET address_line_2 = $1 WHERE id = $2";
            pqxx::result update_result = txn.exec_params(update_query, address_line_2, id);
        }
        else if (option == 5)
        {
            cout << "Enter new city :";
            cin >> city;
            column_name = "CITY  ";
            std::string update_query = "UPDATE employees SET city = $1 WHERE id = $2";
            pqxx::result update_result = txn.exec_params(update_query, city, id);
        }
        else if (option == 6)
        {
            cout << "Enter new state :";
            cin >> state;
            column_name = "STATE  ";
            std::string update_query = "UPDATE employees SET state = $1 WHERE id = $2";
            pqxx::result update_result = txn.exec_params(update_query, state, id);
        }
        else if (option == 7)
        {
            cout << "Enter new contact number :";
            cin >> phone;
            column_name = "CONTACT NUMBER  ";
            std::string update_query = "UPDATE employees SET phone_number = $1 WHERE id = $2";
            pqxx::result update_result = txn.exec_params(update_query, phone, id);
        }
        else
        {
            return;
        }
        txn.commit();
        cout << column_name << "updated successfully.\n";
    }
    catch (const std::exception &e)
    {
        cerr << "Error updating salary: " << e.what() << "\n";
    }
};
void employees::remove_employee()
{
    int id;
    std::cout << "Enter your ID for verification: ";
    std::cin >> id;

    try
    {
        pqxx::nontransaction N1(conn);
        pqxx::result R1 = N1.exec_params("SELECT manager_status FROM employees WHERE id = $1", id);
        N1.commit();

        if (R1.empty() || R1[0][0].as<std::string>() != "T")
        {
            std::cout << "Only a manager is permitted to modify details.\n";
            return;
        }

        std::cout << "Enter ID of the employee to remove: ";
        std::cin >> id;

        pqxx::nontransaction N2(conn);
        pqxx::result R2 = N2.exec_params("SELECT * FROM employees WHERE id = $1", id);
        N2.commit();

        if (R2.empty())
        {
            std::cout << "Employee with the given ID does not exist.\n";
            return;
        }

        auto row = R2[0];
        std::cout << "Current Employee Details:\n";
        std::cout << "ID: " << row["id"].as<int>() << "\n";
        std::cout << "Name: " << row["employee_name"].as<std::string>() << "\n";
        std::cout << "Gender: " << row["gender"].as<std::string>() << "\n";
        std::cout << "Address Line 1: " << row["address_line_1"].as<std::string>() << "\n";
        std::cout << "Address Line 2: " << row["address_line_2"].as<std::string>() << "\n";
        std::cout << "City: " << row["city"].as<std::string>() << "\n";
        std::cout << "State: " << row["state"].as<std::string>() << "\n";
        std::cout << "Contact Number: " << row["phone_number"].as<std::string>() << "\n";
        std::cout << "Date of Joining: " << row["date_of_joining"].as<std::string>() << "\n";
        std::cout << "Salary: " << row["salary"].as<float>() << "\n";
        std::cout << "Manager Status: " << row["manager_status"].as<std::string>() << "\n";

        char choice;
        std::cout << "Choose Y to remove employee [Y/N]: ";
        std::cin >> choice;

        if (choice == 'Y' || choice == 'y')
        {
            pqxx::work W(conn);
            W.exec_params("DELETE FROM employees WHERE id = $1", id);
            W.commit();
            std::cout << "Deleted Successfully.\n";
        }
        else if (choice == 'N' || choice == 'n')
        {
            std::cout << "No changes made.\n";
        }
        else
        {
            std::cout << "Invalid Character\n";
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\nUnable to complete request\n";
    }
}

void purchase::new_order()
{

    std::cout << "Enter the book id : ";
    std::cin >> book_id;
    std::cout << "Enter supplier id : ";
    std::cin >> supplier_id;
    std::cout << "Enter the quantity :";
    std::cin >> quantity;
    std::cout << "Enter the Date of arrival(in Days) : ";
    std::cin >> date_of_arrival;

    try
    {
        pqxx::work txn(conn);
        std::string eta_sql = "CURRENT_DATE + INTERVAL '1 day' * " + std::to_string(date_of_arrival);
        std::string sql = "INSERT INTO purchase (book_id, supplier_id, quantity, date_ordered, estimated_date_of_arrival) "
                          "VALUES (" +
                          txn.quote(book_id) + ", " + txn.quote(supplier_id) + ", " + txn.quote(quantity) + ", CURRENT_DATE, " + eta_sql + ")";
        txn.exec(sql);
        txn.commit();
        std::cout << "New order added" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\nSomething went wrong while ordering";
    }
};
void purchase::received_order()
{
    try
    {
        std::cout << "Enter order id";
        std::cin >> order_id;
        pqxx::nontransaction N(conn);
        std::string update_query = "UPDATE purchase SET order_status = 'T' WHERE order_id = $1";
        N.exec_params(update_query, order_id);
        std::cout << "\nReceived marked successfully";
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\nsomething went wrong while updating please try again";
    }
};

void purchase::cancel_order()
{
    try
    {
        std::cout << "Enter order id";
        std::cin >> order_id;
        pqxx::nontransaction N(conn);
        std::string update_query = "UPDATE purchase SET order_status = 'C' WHERE order_id = $1";
        N.exec_params(update_query, order_id);
        std::cout << "\nCancelled marked successfully";
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\nsomething went wrong while updating please try again";
    }
};
void purchase ::view_order()
{
    int c;
    cout << "Enter the option";
    cout << "Select an Option" << endl;
    cout << "1. View orders not Recieved" << endl;
    cout << "2. View orders Cancelled" << endl;
    cout << "3. View orders Recieved" << endl;
    cout << "Enter Your choice : ";
    cin >> c;
    if (c == 1)
        received = 'F';
    else if (c == 2)
        received = 'C';
    else if (c == 3)
        received = 'T';
    else
    {
        std::cerr << "Invalid option selected" << std::endl;
    }

    try
    {
        pqxx::nontransaction N(conn);

        // Use a placeholder ($1) in the SQL query
        std::string search_query = "SELECT * FROM purchase WHERE order_status = $1";

        // Bind the parameter and execute the query
        pqxx::result R = N.exec_params(search_query, std::string(1, received));

        // Iterate over the result set and print the rows
        for (const auto &row : R)
        {
            std::cout << std::endl;
            std::cout << "Order Id : " << row["order_id"].c_str() << std::endl;
            std::cout << "Book Id : " << row["book_id"].c_str() << std::endl;
            std::cout << "Supplier Id : " << row["supplier_id"].c_str() << std::endl;
            std::cout << "Quantity : " << row["quantity"].c_str() << std::endl;
            std::cout << "Date Ordered : " << row["date_ordered"].c_str() << std::endl;
            std::cout << "Estimated Delivery Date : " << row["estimated_date_of_arrival"].c_str() << std::endl;
            std::cout << std::endl
                      << std::endl
                      << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\nsomething went wrong while retrieving the orders" << std::endl;
    };
};

void suppliers::add_supplier()
{
    std::cout << "Enter the name of the supplier :";
    getline(std::cin >> std::ws, name);
    std::cout << "Enter the phone number ";
    std::cin >> phone_number;
    std::cout << "Enter address 1 :";
    getline(std::cin >> std::ws, address_line_1);
    std::cout << "Enter address 2 :";
    getline(std::cin >> std::ws, address_line_2);
    std::cout << "Enter the state :";
    getline(std::cin >> std::ws, state);
    std::cout << "Enter country :";
    getline(cin >> std::ws, country);
    std::cout << "Enter pincode";
    std::cin >> pincode;

    try
    {
        pqxx::work txn(conn);
        txn.exec_params(
            "INSERT INTO suppliers (supplier_name,supplier_phonenumber,supplier_address_1,supplier_address_2,supplier_state,supplier_country,supplier_pincode) VALUES ($1,$2,$3,$4,$5,$6,$7)",
            name, phone_number, address_line_1, address_line_2, state, country, pincode);
        txn.commit();
        std::cout << "\n\n\nsupplier recorded successfully\n\n\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\n\nSomething went wrong while uploading suppliers details";
    }
}
void suppliers::remove_supplier()
{
    std::cout << "Enter id of the supplier";
    std::cin >> id;
    try
    {
        pqxx::nontransaction N(conn);
        std::string delete_query = "DELETE FROM suppliers WHERE supplier_id = $1";
        pqxx::result delete_result = N.exec_params(delete_query, id);

        N.commit();

        std::cout << "\n\nSupplier has been deleted successfully";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error" << e.what() << "Deleting failed" << std::endl;
    }
};

void suppliers ::search_supplier_by_id()
{
    std::cout << "Enter id of the supplier";
    std::cin >> id;

    if (id <= 0)
    {
        std::cout << "\nInvalid ID,Enter a valid positive id";
        return;
    }

    try
    {
        pqxx::nontransaction N(conn);
        std::string search_query = "SELECT * FROM suppliers WHERE supplier_id = $1";
        pqxx::result search_result = N.exec_params(search_query, id);

        if (search_result.empty())
        {
            std::cout << "Supplier with id" << id << "does not exists" << endl;
            return;
        }
        else
        {
            std::cout << "NAME : " << search_result[0]["supplier_name"].as<std::string>() << std::endl;
            std::cout << std::endl;
            std::cout << "PHONE NUMBER :  " << search_result[0]["supplier_phonenumber"].as<std::string>() << std::endl;
            std::cout << std::endl;
            std::cout << "ADDRESS 1 : " << search_result[0]["supplier_address_1"].as<std::string>() << std::endl;
            std::cout << std::endl;
            std::cout << "ADDRESS 2 : " << search_result[0]["supplier_address_2"].as<std::string>() << std::endl;
            std::cout << std::endl;
            std::cout << "STATE : " << search_result[0]["supplier_state"].as<std::string>() << std::endl;
            std::cout << std::endl;
            std::cout << "COUNTRY  : " << search_result[0]["supplier_country"].as<std::string>() << std::endl;
            std::cout << std::endl;
            std::cout << "PINCODE : " << search_result[0]["supplier_pincode"].as<std::string>() << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\nunable to fetch suppliers details";
    }
};
void books::add()
{

    std::cout << "Enter the name of the book : ";
    std::getline(std::cin >> std::ws, name);

    std::cout << "Enter the name of the author : ";
    std::getline(std::cin >> std::ws, author);

    std::cout << "Enter the price of the book : ";
    while (!(std::cin >> price))
    {
        std::cin.clear();                                                   // Clear error flags
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
        std::cout << "Invalid input. Please enter a valid price: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter the quantity received of book : ";
    while (!(std::cin >> quantity))
    {
        std::cin.clear();                                                   // Clear error flags
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
        std::cout << "Invalid input. Please enter a valid quantity: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    try
    {
        pqxx ::work txn(conn);

        txn.exec_params(
            "INSERT INTO books (name, author, price, quantity) VALUES ($1, $2, $3, $4)",
            name, author, price, quantity);

        txn.commit();
        std::cout << "\n\nBook record Inserted Successfully\n\n\n";
    }
    catch (const exception &e)
    {
        std::cerr << "\n\nEntry error " << e.what() << "\n\nSomething went wrong while uploading books Please try again later";
    }
};
void books::update_price()
{
    char choice;
    std::string id, price;

    std::cout << "Enter the id of the book you want to update: ";
    std::cin >> id;

    // Create a non-transactional object
    pqxx::nontransaction N(conn);

    // Execute SELECT query to fetch book details
    std::string select_query = "SELECT name, price FROM books WHERE id = '" + id + "'";
    pqxx::result select_result = N.exec(select_query);

    if (!select_result.empty())
    {
        std::cout << "The Name of the book is: " << select_result[0]["name"].as<std::string>() << std::endl;
        std::cout << "The current price of the book is: " << select_result[0]["price"].as<std::string>() << std::endl;

        std::cout << "Do you want to update the price of the book? (y/n): ";
        std::cin >> choice;

        if (choice == 'y' || choice == 'Y')
        {
            std::cout << "Enter the new price of the book: ";
            std::cin >> price;

            // Execute UPDATE query to update the price
            std::string update_query = "UPDATE books SET price = '" + price + "' WHERE id = '" + id + "'";
            pqxx::result update_result = N.exec(update_query);

            std::cout << "Book price has been updated successfully" << std::endl;
        }
        else
        {
            std::cout << "No changes made." << std::endl;
        }
    }
    else
    {
        std::cout << "No book found with ID: " << id << std::endl;
    }
}
void books ::search()
{
    string id;
    std::cout << "Enter id of the book";
    std::cin >> id;
    pqxx::nontransaction N(conn);

    std::string select_query = "SELECT * FROM books WHERE id = '" + id + "'";
    pqxx::result select_result = N.exec(select_query);
    if (!select_result.empty())
    {

        std::cout << "The Name of the book is: " << select_result[0]["name"].as<std::string>() << std::endl;
        std::cout << endl;
        std ::cout << "Author of the book is: " << select_result[0]["author"].as<std::string>() << std::endl;
        std::cout << endl;
        std::cout << "The current price of the book is: " << select_result[0]["price"].as<std::string>() << std::endl;
        std::cout << endl;
        std ::cout << "Quantity availble is: " << select_result[0]["quantity"].as<std::string>() << std::endl;
    }
    else
    {
        cout << "No book found " << endl;
    }
}
void books::update_quantity()
{
    char choice;
    int id, quantity;

    pqxx::nontransaction N(conn);

    // Prompt user for book ID
    std::cout << "Enter the ID of the book: ";
    std::cin >> id;

    // Execute SELECT query to fetch book details
    std::string select_query = "SELECT name, quantity FROM books WHERE id = " + std::to_string(id);
    pqxx::result select_result = N.exec(select_query);

    if (!select_result.empty())
    {
        std::cout << "The Name of the book is: " << select_result[0]["name"].as<std::string>() << std::endl;
        std::cout << "The current quantity of the book is: " << select_result[0]["quantity"].as<int>() << std::endl;

        std::cout << "Do you want to update the quantity of the book? (y/n): ";
        std::cin >> choice;

        if (choice == 'y' || choice == 'Y')
        {
            std::cout << "Enter the new quantity of the book: ";
            std::cin >> quantity;

            // Execute UPDATE query to update the quantity
            std::string update_query = "UPDATE books SET quantity = " + std::to_string(quantity) + " WHERE id = " + std::to_string(id);
            pqxx::result update_result = N.exec(update_query);

            std::cout << "Quantity has been updated successfully" << std::endl;
        }
        else
        {
            std::cout << "No changes made." << std::endl;
        }
    }
    else
    {
        std::cout << "No book found with ID: " << id << std::endl;
    }
}
// delete a book instance from table.
// by providing id we will delete the row from table.
void books::delete_book()
{
    char choice;
    int id;

    pqxx::nontransaction N(conn);
    std::cout << "Enter the id of the book to delete: ";
    std::cin >> id;

    std::string select_query = "SELECT * FROM books WHERE id = $1"; // Using parameterized query
    pqxx::result select_result = N.exec_params(select_query, id);

    if (!select_result.empty())
    {
        std::cout << "The name of the book is: " << select_result[0]["name"].as<std::string>() << std::endl;
        std::cout << "The author of the book is: " << select_result[0]["author"].as<std::string>() << std::endl;
        std::cout << "Quantity: " << select_result[0]["quantity"].as<std::string>() << std::endl;

        std::cout << "Are you sure you want to delete this book from the database? (y/n): ";
        std::cin >> choice;

        if (choice == 'y' || choice == 'Y')
        {
            std::string delete_query = "DELETE FROM books WHERE id = $1"; // Using parameterized query
            pqxx::result delete_result = N.exec_params(delete_query, id);

            std::cout << "Book has been deleted successfully" << std::endl;
        }
        else
        {
            std::cout << "No changes have been made" << std::endl;
        }
    }
    else
    {
        std::cout << "No book with the given ID found" << std::endl;
    }
}

int main()

{
    // +----------------------------------+
    // |		Members  	  |
    // +----------------------------------+
    members members_instance;
    //members_instance.add_member();
    //members_instance.search_member();
    members_instance.refresh();

    // +----------------------------------+
    // |		Employee	  	  |
    // +----------------------------------+

    // employees employee_instance;
    //  employee_instance.add_employee();
    //  employee_instance.search_employee();
    //  employee_instance.assign_manager();
    //  employee_instance.update_details();
    // employee_instance.remove_employee();

    // +----------------------------------+
    // |		purchase	  	  |
    // +----------------------------------+
    // purchase purchase_instance;
    // purchase_instance.new_order();
    // purchase_instance.received_order();
    // purchase_instance.cancel_order();
    // purchase_instance.view_order();

    // +----------------------------------+
    // |		suppliers	  	  |
    // +----------------------------------+

    // suppliers suppliers_instance;
    // suppliers_instance.add_supplier();
    // suppliers_instance.remove_supplier();
    // suppliers_instance.search_supplier_by_id();

    // +----------------------------------+
    // |		books	  	  |
    // +----------------------------------+

    /*int a;
    std::cout << "1) Add book" << std::endl;
    std::cout << endl;
    std::cout << "2) update price" << std::endl;
    std::cout << endl;
    std::cout << "3) search book" << std::endl;
    std::cout << endl;
    std::cout << "4) update quantity" << std::endl;
    std::cout << endl;
    std::cout << "5) delete book" << std::endl;
    std::cout << endl;

    std::cout << "select any number 1-5 to perform action respectively" << std::endl;
    std::cin >> a;
    books books_instance;
    if (a == 1)
    {
        books_instance.add();
    }
    else if (a == 2)
    {
        books_instance.update_price();
    }
    else if (a == 3)
    {
        books_instance.search();
    }
    else if (a == 4)
    {
        books_instance.update_quantity();
    }
    else if (a == 5)
    {
        books_instance.delete_book();
    }
    else
    {
        cout << "unknown selection";
    }*/

    return 0;
}
