#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class Ticket {
public:
    string passengerName;
    string seatNumber;
    string flightNumber;
    string date;
    string price;
    int id;
    bool status;

    // Constructor
    Ticket(string n, string p, string d, string f)
            : seatNumber(n), price(p), date(d), flightNumber(f), status(false), id(0){
        passengerName = "";
    }
};

class Airplane {

public:
    string flightNumber;
    string date;
    map<int, Ticket> bookedSeats;
    vector<Ticket> seats;

    Airplane(string flightNumber, string date, vector<Ticket> seats)
            : flightNumber(flightNumber), date(date), seats(seats) {}
};


class BookingSystem {
private:
    void commandList();

    void check();

    void bookTicket();

    void returnTicket();

    void viewId();

    void viewPassengerName();

    void viewFlightNumber();

    void createSeats();

    string userInput;

    string userCommand;

    string userDate;

    string userNumber;

    string passenger;

    string seat;

    int currentTicketId = 1;

    int userId;

    string viewOption;

public:
    vector<Airplane> planes;
    map<int, Ticket> bookedTickets;

    void run();

};

void BookingSystem::createSeats() {
    int numberOfRecords;
    int seatsPerRow;
    string line;

    ifstream file("/Users/ostapturash/Documents/unik/oop/oop_1/info.txt");
    if (!file.is_open()) {
        cerr << "Error: Could not open file." << endl;
        return;
    }

    getline(file, line);
    numberOfRecords = stoi(line);

    map<pair<string, string>, vector<Ticket>> flights;

    for (int i = 0; i < numberOfRecords; ++i) {
        getline(file, line);

        istringstream iss(line);
        string date, flightNumber, seatRange, price;
        iss >> date >> flightNumber >> seatsPerRow;

        while (iss >> seatRange >> price) {
            size_t pos = seatRange.find('-');
            if (pos == string::npos) {
                cerr << "Error: Invalid seat range: " << seatRange << endl;
                continue; // Proceed to next seat range
            }

            int start = stoi(seatRange.substr(0, pos));
            int end = stoi(seatRange.substr(pos + 1));

            char maxColumn = 'A' + seatsPerRow - 1;
            for (int currentRow = start; currentRow <= end; currentRow++) {
                for (char col = 'A'; col <= maxColumn; col++) {
                    string seatName = to_string(currentRow) + col;
                    Ticket ticket(seatName, price, date, flightNumber);
                    flights[{date, flightNumber}].push_back(ticket);
                }
            }
        }
    }

    for (const auto& flight : flights) {
        Airplane airplane(flight.first.second, flight.first.first, flight.second);
        planes.push_back(airplane);
    }

    file.close();
}



void BookingSystem::run() {
    createSeats();

    while (true) {
        commandList();
        getline(cin, userInput);
        stringstream sss(userInput);
        sss >> userCommand;

        int commandNumber = -1;
        if (userCommand == "check") commandNumber = 1;
        else if (userCommand == "book") commandNumber = 2;
        else if (userCommand == "return") commandNumber = 3;
        else if (userCommand == "view") commandNumber = 4;
        else if (userCommand == "exit") commandNumber = 5;

        switch (commandNumber) {
            case 1: // check
                sss >> userDate >> userNumber;
                check();
                break;
            case 2: // book
                sss >> userDate >> userNumber >> seat >> passenger;
                bookTicket();
                break;
            case 3: // return
                sss >> userId;
                returnTicket();
                break;
            case 4: // view
                sss >> viewOption;
                if (viewOption == "username") {
                    sss >> passenger;
                    viewPassengerName();
                } else if (viewOption == "flight") {
                    sss >> userDate >> userNumber;
                    viewFlightNumber();
                } else {
                    userId = stoi(viewOption);
                    viewId();
                }
                break;
            case 5: // exit
                return;
            default: // Invalid command
                cout << "\nThe input is incorrect!\n\n";
                break;
        }
    }
}

void BookingSystem::commandList() {
    cout << """Please choose the command from the list:\n"
            "1) check available places\n"
            "2) book ticket\n"
            "3) return ticket\n"
            "4) view ID\n"
            "5) view username\n"
            "6) view flight number\n""" <<endl;
}

void BookingSystem::check(){
    for (int i = 0; i < planes.size(); i++){
        if (userDate==planes[i].date && userNumber==planes[i].flightNumber){
            for (int j = 0; j < planes[i].seats.size(); j++){
                if (!planes[i].seats[j].status){
                    cout << planes[i].seats[j].seatNumber << " " << planes[i].seats[j].price << endl;
                }
            }
        }
    }
}

void BookingSystem::bookTicket() {
    for (int i = 0; i < planes.size(); i++) {
        if (userDate == planes[i].date && userNumber == planes[i].flightNumber) {
            for (int j = 0; j < planes[i].seats.size(); j++) {
                if (seat == planes[i].seats[j].seatNumber && !planes[i].seats[j].status) {
                    planes[i].seats[j].status = true;
                    planes[i].seats[j].passengerName = passenger;
                    planes[i].seats[j].id = currentTicketId;
                    currentTicketId++;
                    cout << "Ticket booked successfully for " << passenger << " on seat " << seat << ".\n";
                    return;
                }
            }
            cout << "Error: Seat " << seat << " is already booked.\n";
            return;
        }
    }
    cout << "Error: Flight " << userNumber << " on " << userDate << " not found.\n";
}

void BookingSystem::returnTicket(){
    for (int i = 0; i < planes.size(); i++){
        for (int j = 0; j < planes[i].seats.size(); j++){
            if (planes[i].seats[j].id == userId) {
                cout << "Confirmed" << " " << planes[i].seats[j].price << " " << "refund for "
                     << planes[i].seats[j].passengerName << endl;
                planes[i].seats[j].id = 0;
                planes[i].seats[j].passengerName = "";
                planes[i].seats[j].status = false;
                break;
            }
        }
        break;
    }
}

void BookingSystem::viewId() {
    for (int i = 0; i < planes.size(); i++){
        for (int j = 0; j < planes[i].seats.size(); j++){
            if (userId == planes[i].seats[j].id){
                cout << "Flight Number: " << planes[i].seats[j].flightNumber << ", " << planes[i].seats[j].date << ", " <<
                "seat " << planes[i].seats[j].seatNumber << ", price " << planes[i].seats[j].price << ", " << planes[i].seats[j].passengerName << endl;
            }
            break;
        }
    }
}

void BookingSystem::viewPassengerName() {
    int count = 1;
    for (int i = 0; i < planes.size(); i++){
        for (int j = 0; j < planes[i].seats.size(); j++){
            if (passenger == planes[i].seats[j].passengerName){
                cout << count << ". " << "Flight Number: " << planes[i].seats[j].flightNumber << ", " << planes[i].seats[j].date << ", " <<
                     "seat " << planes[i].seats[j].seatNumber << ", price " << planes[i].seats[j].price << endl;
                count++;
            }
        }
    }
}

void BookingSystem::viewFlightNumber() {
    for (int i = 0; i < planes.size(); i++){
        for (int j = 0; j < planes[i].seats.size(); j++){
            if (userNumber == planes[i].seats[j].flightNumber && userDate == planes[i].seats[j].date){
                if (planes[i].seats[j].status){
                    cout <<  planes[i].seats[j].seatNumber << " " << planes[i].seats[j].passengerName << " " <<
                         planes[i].seats[j].price << planes[i].seats[j].price << endl;
                }
            }
        }
    }
}

int main() {

    BookingSystem system;
    system.run();

    return 0;
}