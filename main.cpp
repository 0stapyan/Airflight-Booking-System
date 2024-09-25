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
    bool status;

    // Constructor
    Ticket(string n, string p, string d, string f)
            : seatNumber(n), price(p), date(d), flightNumber(f), status(false) {
        passengerName = "";
    }
};

class Airplane {
private:
    string flightNumber;
    string date;
    vector<Ticket> bookedSeats;
    map<string, bool> seats;

public:
    Airplane(string flightNumber, string date, map<string, bool> seats, map<string, string> seatPrices)
            : flightNumber(flightNumber), date(date), seats(seats) {}
};

class FileReader {
public:
    int numberOfRecords;
    int seatsPerRow;
    string line;
    vector<Ticket> tickets;
    string seatRange;
    string price;

    void createSeats() {
        ifstream file("/Users/ostapturash/Documents/unik/oop/oop_1/info.txt");
        if (!file.is_open()) {
            cerr << "Error: Could not open file." << endl;
            return;
        }

        getline(file, line);
        numberOfRecords = stoi(line); // Assuming this will always be valid

        for (int i = 0; i < numberOfRecords; ++i) {
            getline(file, line);

            istringstream iss(line);
            string date, flightNumber;
            iss >> date >> flightNumber >> seatsPerRow;

            while (iss >> seatRange >> price) {
                size_t pos = seatRange.find('-');
                if (pos == string::npos) {
                    cerr << "Error: Invalid seat range: " << seatRange << endl;
                    continue; // Proceed to next seat range
                }

                int start = stoi(seatRange.substr(0, pos)); // Assuming valid input
                int end = stoi(seatRange.substr(pos + 1)); // Assuming valid input

                char maxColumn = 'A' + seatsPerRow - 1;
                for (int currentRow = start; currentRow <= end; currentRow++) {
                    for (char col = 'A'; col <= maxColumn; col++) {
                        string seatName = to_string(currentRow) + col;
                        Ticket ticket(seatName, price, date, flightNumber);
                        tickets.push_back(ticket);
                    }
                }
            }
        }

        file.close();
    }
};

int main() {

    FileReader fileReader;
    fileReader.createSeats();

    if (!fileReader.tickets.empty()) {
        Ticket firstTicket = fileReader.tickets[0];
        cout << "Seat Number: " << firstTicket.seatNumber << endl;
        cout << "Price: " << firstTicket.price << endl;
        cout << "Date: " << firstTicket.date << endl;
        cout << "Flight Number: " << firstTicket.flightNumber << endl;
        cout << "Status: " << (firstTicket.status ? "Confirmed" : "Pending") << endl;
    }

    return 0;
}