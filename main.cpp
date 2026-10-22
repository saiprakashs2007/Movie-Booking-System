
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <iomanip>
#include <limits>
using namespace std;

// ================= SEAT MANAGER =================

class SeatManager {
private:
    bool seats[5][6];

public:
    SeatManager() {
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 6; j++)
                seats[i][j] = true;
    }

    void displaySeats() const {
        cout << "\n========== SCREEN ==========\n\n";

        for (int i = 0; i < 5; i++) {
            char row = 'A' + i;
            cout << row << "   ";

            for (int j = 0; j < 6; j++) {
                if (seats[i][j])
                    cout << "[" << row << j + 1 << "] ";
                else
                    cout << "[XX] ";
            }
            cout << endl;
        }

        cout << "\nSeat labels = Available\n";
        cout << "[XX] = Booked\n";
    }

    bool bookSeats(string& selectedSeats, int& count) {
        displaySeats();

        cout << "\nHow many seats do you want to book? ";

        if (!(cin >> count)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input!\n";
            return false;
        }

        if (count <= 0 || count > 30) {
            cout << "Invalid number of seats!\n";
            return false;
        }

        int selectedRows[30], selectedCols[30];
        string labels[30];

        for (int i = 0; i < count; i++) {
            string seat;

            cout << "Enter seat " << i + 1 << " (e.g. A1): ";
            cin >> seat;

            if (seat.length() != 2) {
                cout << "Invalid seat format!\n";
                return false;
            }

            char row = toupper(
                static_cast<unsigned char>(seat[0]));
            char number = seat[1];

            int r = row - 'A';
            int c = number - '1';

            if (r < 0 || r >= 5 ||
                number < '1' || number > '6') {
                cout << "Seat does not exist!\n";
                return false;
            }

            if (!seats[r][c]) {
                cout << "Seat already booked!\n";
                return false;
            }

            for (int j = 0; j < i; j++) {
                if (selectedRows[j] == r &&
                    selectedCols[j] == c) {
                    cout << "Seat selected twice!\n";
                    return false;
                }
            }

            selectedRows[i] = r;
            selectedCols[i] = c;
            labels[i] = string(1, row) + number;
        }

        // Mark seats as booked only after all are valid.
        selectedSeats = "";

        for (int i = 0; i < count; i++) {
            seats[selectedRows[i]][selectedCols[i]] = false;

            if (i > 0)
                selectedSeats += " ";

            selectedSeats += labels[i];
        }

        cout << "\nSeats reserved: " << selectedSeats << endl;
        return true;
    }

    void releaseSeats(const string& selectedSeats) {
        size_t start = 0;

        while (start < selectedSeats.size()) {
            size_t end = selectedSeats.find(' ', start);

            string seat = selectedSeats.substr(
                start,
                end == string::npos
                    ? string::npos : end - start
            );

            if (seat.length() == 2) {
                char row = toupper(
                    static_cast<unsigned char>(seat[0]));
                char number = seat[1];

                int r = row - 'A';
                int c = number - '1';

                if (r >= 0 && r < 5 &&
                    number >= '1' && number <= '6') {
                    seats[r][c] = true;
                }
            }

            if (end == string::npos)
                break;

            start = end + 1;
        }
    }
};

// ================= MOVIE =================

class Movie {
private:
    int movieId;
    string title;
    string genre;
    double ticketPrice;
    int availableSeats;

public:
    Movie(int id, string t, string g,
          double price, int seats) {
        movieId = id;
        title = t;
        genre = g;
        ticketPrice = price;
        availableSeats = seats;
    }

    void displayMovie() const {
        cout << "Movie ID: " << movieId << endl;
        cout << "Title: " << title << endl;
        cout << "Genre: " << genre << endl;
        cout << fixed << setprecision(2);
        cout << "Ticket Price: Rs. " << ticketPrice << endl;
        cout << "Total Seats: " << availableSeats << endl;
        cout << "-----------------------------\n";
    }

    int getMovieId() const {
        return movieId;
    }

    string getTitle() const {
        return title;
    }

    double getTicketPrice() const {
        return ticketPrice;
    }

    int getAvailableSeats() const {
        return availableSeats;
    }
};

// ================= CUSTOMER =================
class Person {
protected:
    string customerName;
    string phoneNumber;

public:
    Person(string name, string phone) {
        customerName = name;
        phoneNumber = phone;
    }
};

class Customer : public Person {
private:
    int customerid;

public:
    Customer(int id, string name, string number)
        : Person(name, number) {
        customerid = id;
    }

    void displayCustomer() const {
        cout << "\n=== Customer Details ===\n";
        cout << "Customer ID: " << customerid << endl;
        cout << "Customer Name: " << customerName << endl;
        cout << "Phone Number: " << phoneNumber << endl;
    }

    int getCustomerId() const {
        return customerid;
    }

    string getCustomerName() const {
        return customerName;
    }
};

// ================= BOOKING =================

class Booking {
private:
    int bookingId;
    Customer customer;
    Movie movie;
    int numberOfTickets;
    double totalPrice;
    string selectedSeats;
    bool bookingActive;

public:
    Booking(int id, Customer customerObj, Movie movieObj,
            int tickets, string seats)
        : customer(customerObj), movie(movieObj) {

        bookingId = id;
        numberOfTickets = tickets;
        selectedSeats = seats;
        totalPrice = 0;
        bookingActive = true;

        calculateTotal();
    }

    friend void printBookingBill(const Booking& b);

    void calculateTotal() {
        double basePrice =
            movie.getTicketPrice() * numberOfTickets;

        double gstRate =
            (movie.getTicketPrice() <= 100) ? 0.12 : 0.18;

        double ticketGST = basePrice * gstRate;
        double platformFee = basePrice * 0.05;
        double platformGST = platformFee * 0.18;

        totalPrice = basePrice + ticketGST
                   + platformFee + platformGST;
    }

    void displayBooking() const {
        cout << "\n========== BOOKING DETAILS ==========\n";
        cout << "Booking ID: " << bookingId << endl;

        if (!bookingActive) {
            cout << "Booking Status: Cancelled\n";
            return;
        }

        double basePrice =
            movie.getTicketPrice() * numberOfTickets;

        double gstRate =
            (movie.getTicketPrice() <= 100) ? 0.12 : 0.18;

        double ticketGST = basePrice * gstRate;
        double platformFee = basePrice * 0.05;
        double platformGST = platformFee * 0.18;

        customer.displayCustomer();

        cout << "\nMovie: " << movie.getTitle() << endl;
        cout << "Number of Tickets: "
             << numberOfTickets << endl;
        cout << "Selected Seats: " << selectedSeats << endl;

        cout << fixed << setprecision(2);
        cout << "\nBase Price: Rs. " << basePrice << endl;
        cout << "Ticket GST: Rs. " << ticketGST << endl;
        cout << "Platform Fee (5%): Rs. "
             << platformFee << endl;
        cout << "GST on Platform Fee (18%): Rs. "
             << platformGST << endl;

        cout << "-------------------------------------\n";
        cout << "TOTAL AMOUNT: Rs. " << totalPrice << endl;
        cout << "Booking Status: Confirmed\n";
    }

    int getBookingId() const {
        return bookingId;
    }

    int getMovieId() const {
        return movie.getMovieId();
    }

    string getSelectedSeats() const {
        return selectedSeats;
    }

    bool cancelBooking() {
        if (!bookingActive) {
            cout << "Booking is already cancelled.\n";
            return false;
        }

        bookingActive = false;

        cout << "Booking " << bookingId
             << " has been cancelled.\n";

        return true;
    }
};

void printBookingBill(const Booking& b) {
    cout << "\n========== BOOKING RECEIPT ==========\n";

    cout << "Booking ID: " << b.bookingId << endl;
    cout << "Movie: " << b.movie.getTitle() << endl;
    cout << "Tickets: " << b.numberOfTickets << endl;
    cout << "Seats: " << b.selectedSeats << endl;

    cout << fixed << setprecision(2);
    cout << "Total Amount: Rs. " << b.totalPrice << endl;

    cout << "=====================================\n";
}

// ================= MAIN =================

int main() {
    vector<Movie> movies;

    movies.emplace_back(
        101, "Interstellar", "Sci-Fi", 200.00, 30);
    movies.emplace_back(
        102, "Inception", "Thriller", 150.00, 30);
    movies.emplace_back(
        103, "Toy Story", "Animation", 100.00, 30);

    // Each movie gets its own seat arrangement.
    vector<SeatManager> seatManagers(movies.size());

    vector<Booking> bookings;

    int nextCustomerId = 1;
    int nextBookingId = 1001;
    int choice;

    do {
        cout << "\n=== MOVIE TICKET BOOKING ===\n";
        cout << "1. View Movies\n";
        cout << "2. Book Tickets\n";
        cout << "3. View Booking\n";
        cout << "4. Cancel Booking\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a number from 1 to 5.\n";
            continue;
        }

        switch (choice) {

        case 1: {
            cout << "\n=== AVAILABLE MOVIES ===\n";

            for (const Movie& movie : movies)
                movie.displayMovie();

            break;
        }

        case 2: {
            cout << "\n=== AVAILABLE MOVIES ===\n";

            for (const Movie& movie : movies)
                movie.displayMovie();

            int movieId;

            cout << "Enter Movie ID: ";

            if (!(cin >> movieId)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid Movie ID.\n";
                break;
            }

            int movieIndex = -1;

            for (int i = 0; i < static_cast<int>(movies.size()); i++) {
                if (movies[i].getMovieId() == movieId) {
                    movieIndex = i;
                    break;
                }
            }

            if (movieIndex == -1) {
                cout << "Movie not found.\n";
                break;
            }

            string name, phone;

            cout << "Enter customer name: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, name);

            cout << "Enter phone number: ";
            cin >> phone;

            string selectedSeats;
            int ticketCount = 0;

            // Reserve seats before creating a booking.
            if (!seatManagers[movieIndex].bookSeats(
                    selectedSeats, ticketCount)) {
                cout << "Booking was not created.\n";
                break;
            }

            Customer customer(
                nextCustomerId++, name, phone);

            Booking booking(
                nextBookingId++, customer,
                movies[movieIndex], ticketCount,
                selectedSeats);

            bookings.push_back(booking);

            cout << "\nBooking created successfully!\n";
            bookings.back().displayBooking();

            break;
        }

        case 3: {
            if (bookings.empty()) {
                cout << "No bookings have been made yet.\n";
                break;
            }

            int id;
            cout << "Enter Booking ID: ";

            if (!(cin >> id)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid Booking ID.\n";
                break;
            }

            bool found = false;

            for (const Booking& booking : bookings) {
                if (booking.getBookingId() == id) {
                    booking.displayBooking();
                    found = true;
                    break;
                }
            }

            if (!found)
                cout << "Booking ID not found.\n";

            break;
        }

        case 4: {
            int id;

            cout << "Enter Booking ID to cancel: ";

            if (!(cin >> id)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid Booking ID.\n";
                break;
            }

            bool found = false;

            for (Booking& booking : bookings) {
                if (booking.getBookingId() == id) {
                    found = true;

                    if (booking.cancelBooking()) {
                        for (int i = 0;
                             i < static_cast<int>(movies.size()); i++) {
                            if (movies[i].getMovieId()
                                == booking.getMovieId()) {

                                seatManagers[i].releaseSeats(
                                    booking.getSelectedSeats());

                                break;
                            }
                        }
                    }

                    break;
                }
            }

            if (!found)
                cout << "Booking ID not found.\n";

            break;
        }

        case 5:
            cout << "Exiting Movie Ticket Booking System...\n";
            break;

        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 5);

    return 0;
}
