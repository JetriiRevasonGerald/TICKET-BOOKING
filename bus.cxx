#include <iostream>
#include <vector>
#include <string>
#include <iomanip>  // For formatting output
#include <fstream>  // For file operations

using namespace std;

class Passenger {
public:
    string firstName, middleName, lastName, contact;
    int seatNumber;

    Passenger(string fName, string mName, string lName, string contactInfo, int seatNum)
        : firstName(fName), middleName(mName), lastName(lName), contact(contactInfo), seatNumber(seatNum) {}
};

class Bus {
private:
    string busName;
    int totalSeats;
    float fare;
    vector<bool> seats;
    vector<Passenger> passengers;

public:
    Bus(string name, int seatsCount, float busFare) : busName(name), totalSeats(seatsCount), fare(busFare) {
        seats.resize(totalSeats, false);
        passengers.resize(totalSeats, Passenger("", "", "", "", -1)); // Placeholder passengers
        loadReservationHistory(); // Load previous bookings from file
    }

    void loadReservationHistory() {
        ifstream inFile("reservation_history.txt");
        if (inFile) {
            string firstName, middleName, lastName, contact;
            int seatNumber;
            while (inFile >> firstName >> middleName >> lastName >> contact >> seatNumber) {
                Passenger p(firstName, middleName, lastName, contact, seatNumber);
                if (seatNumber >= 1 && seatNumber <= totalSeats) {
                    seats[seatNumber - 1] = true;
                    passengers[seatNumber - 1] = p;
                }
            }
            inFile.close();
        }
    }

    void saveReservationHistory() {
        ofstream outFile("reservation_history.txt", ios::trunc);
        for (int i = 0; i < totalSeats; i++) {
            if (seats[i]) {
                Passenger p = passengers[i];
                outFile << p.firstName << " " << p.middleName << " " << p.lastName << " "
                        << p.contact << " " << p.seatNumber << endl;
            }
        }
        outFile.close();
    }

    void viewSeats() {
        cout << "\nSeating arrangement for bus: " << busName << "\n";
        for (int i = 0; i < totalSeats; i++) {
            cout << (seats[i] ? "[X] " : "[ ] ");  // Show booked and empty seats
            if ((i + 1) % 5 == 0) cout << "\n";
        }
    }

    bool bookSeat(int seatNumber, Passenger passenger) {
        if (seatNumber < 1 || seatNumber > totalSeats) {
            cout << "Invalid seat number. Please try again.\n";
            return false;
        }
        if (seats[seatNumber - 1]) {
            cout << "Seat " << seatNumber << " is already booked. Please choose another.\n";
            return false;
        }
        seats[seatNumber - 1] = true;
        passengers[seatNumber - 1] = passenger;
        saveReservationHistory();  // Save after booking
        return true;
    }

    void cancelSeat(int seatNumber) {
        if (seatNumber < 1 || seatNumber > totalSeats) {
            cout << "Invalid seat number. Please try again.\n";
            return;
        }
        if (!seats[seatNumber - 1]) {
            cout << "Seat " << seatNumber << " is already vacant.\n";
        } else {
            seats[seatNumber - 1] = false;
            passengers[seatNumber - 1] = Passenger("", "", "", "", -1); // Clear passenger details
            saveReservationHistory();  // Save after cancellation
            cout << "Seat " << seatNumber << " has been successfully canceled.\n";
        }
    }

    void showSummary() {
        cout << "\n--- Booking Summary ---\n";
        for (int i = 0; i < totalSeats; i++) {
            if (seats[i]) {
                Passenger p = passengers[i];
                cout << "Seat " << (i + 1) << " booked by " << p.firstName << " " << p.middleName << " " << p.lastName
                     << " (Contact: " << p.contact << ")\n";
            }
        }
    }

    void searchPassenger(string query) {
        bool found = false;
        cout << "\n--- Search Results ---\n";
        for (int i = 0; i < totalSeats; i++) {
            if (seats[i]) {
                Passenger p = passengers[i];
                string fullName = p.firstName + " " + p.middleName + " " + p.lastName;
                if (fullName.find(query) != string::npos || p.contact.find(query) != string::npos) {
                    cout << "Seat " << (i + 1) << " booked by " << fullName
                         << " (Contact: " << p.contact << ")\n";
                    found = true;
                }
            }
        }
        if (!found) {
            cout << "No passengers found with the given details.\n";
        }
    }

    float getFare() {
        return fare;
    }
};

Passenger collectPassengerDetails(int seatNumber) {
    string firstName, middleName, lastName, contact;
    cout << "\nEnter your first name: ";
    cin >> firstName;
    cout << "Enter your middle name: ";
    cin >> middleName;
    cout << "Enter your last name: ";
    cin >> lastName;
    cout << "Enter your contact information: ";
    cin >> contact;

    return Passenger(firstName, middleName, lastName, contact, seatNumber);
}

int main() {
    Bus bus("Express Bus", 20, 15.00);
    int choice, seatNumber;

    while (true) {
        cout << "\n--- Bus Reservation System ---\n";
        cout << "1. View Available Seats\n";
        cout << "2. Book a Seat\n";
        cout << "3. Cancel a Booking\n";
        cout << "4. View Booking Summary\n";
        cout << "5. Search for a Passenger\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                bus.viewSeats();
                break;

            case 2: {
                cout << "Enter the seat number you want to book: ";
                cin >> seatNumber;
                Passenger passenger = collectPassengerDetails(seatNumber);
                if (bus.bookSeat(seatNumber, passenger)) {
                    cout << "Seat " << seatNumber << " successfully booked!\n";
                }
                break;
            }

            case 3:
                cout << "Enter the seat number you want to cancel: ";
                cin >> seatNumber;
                bus.cancelSeat(seatNumber);
                break;

            case 4:
                bus.showSummary();
                break;

            case 5: {
                string query;
                cout << "Enter the name or contact of the passenger to search: ";
                cin.ignore(); // Clear input buffer
                getline(cin, query);
                bus.searchPassenger(query);
                break;
            }

            case 6:
                cout << "Thank you for using the Bus Reservation System. Goodbye!\n";
                return 0;

            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}