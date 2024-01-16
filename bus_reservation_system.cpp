#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

class Bus
{
public:
    std::string id_num;
    std::string src;
    std::string destination;
    std::string date;
    std::string arrival_time;
    std::string departure_time;
    int available_seats;
    int reserved_seats;

    Bus() : available_seats(20), reserved_seats(0) {}
};

class BusReservationSystem
{
private:
    std::vector<Bus> buses;

public:
    void load_buses(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            Bus bus;
            std::getline(ss, bus.id_num, ',');
            std::getline(ss, bus.src, ',');
            std::getline(ss, bus.destination, ',');
            std::getline(ss, bus.date, ',');
            std::getline(ss, bus.arrival_time, ',');
            std::getline(ss, bus.departure_time, ',');
            std::string available_seats_str, reserved_seats_str;
            std::getline(ss, available_seats_str, ',');
            std::getline(ss, reserved_seats_str, ',');

            bus.available_seats = std::stoi(available_seats_str);
            bus.reserved_seats = std::stoi(reserved_seats_str);

            buses.push_back(bus);
        }

        file.close();
    }

    void display_all_buses() const
    {
        std::cout << "All Buses:\n";
        display_table_header();
        for (const auto &bus : buses)
        {
            display_bus_details(bus);
        }
    }

    void src(const std::string &src) const
    {
        std::cout << "Buses from " << src << ":\n";
        display_table_header();
        for (const auto &bus : buses)
        {
            if (bus.src == src)
            {
                display_bus_details(bus);
            }
        }
    }

    void reservation(const std::string &busId, int numSeats)
    {
        for (auto &bus : buses)
        {
            if (bus.id_num == busId)
            {
                if (bus.available_seats >= numSeats)
                {
                    bus.reserved_seats += numSeats;
                    bus.available_seats -= numSeats;
                    std::cout << "Reservation successful!\n";

                    std::ofstream outFile("bus_data.csv", std::ios::out | std::ios::trunc);

                    if (!outFile.is_open())
                    {
                        std::cerr << "Error opening CSV file for writing." << std::endl;
                        return;
                    }

                    for (const auto &bus : buses)
                    {
                        outFile << bus.id_num << "," << bus.src << "," << bus.destination << ","
                                << bus.date << "," << bus.arrival_time << "," << bus.departure_time << ","
                                << bus.available_seats << "," << bus.reserved_seats << "\n";
                    }

                    outFile.close();
                    std::cout << "Reservation details updated in the CSV file." << std::endl;
                }
                else
                {
                    std::cout << "Not enough available seats.\n";
                }
                return;
            }
        }
        std::cout << "Bus not found with ID: " << busId << std::endl;
    }

private:
    void display_table_header() const
    {
        std::cout << "\n";
        std::cout << "ID\tSource\t\tDestination\tDate\t\tArrival\t\tDeparture\tAvailable\tReserved\n";
        std::cout << "\n\n";
    }

    void display_bus_details(const Bus &bus) const
    {
        std::cout << std::left << std::setw(10) << bus.id_num
                  << std::setw(15) << bus.src
                  << std::setw(15) << bus.destination
                  << std::setw(15) << bus.date
                  << std::setw(15) << bus.arrival_time
                  << std::setw(15) << bus.departure_time
                  << std::setw(10) << bus.available_seats
                  << std::setw(10) << bus.reserved_seats
                  << "\n";
    }
};

int main()
{
    BusReservationSystem reservationSystem;

    reservationSystem.load_buses("bus_data.csv");

    int choice;
    do
    {
        std::cout << "\nBus Reservation System Menu:\n";
        std::cout << "1. Display All Buses\n";
        std::cout << "2. Display Buses by src\n";
        std::cout << "3. Reservation\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            reservationSystem.display_all_buses();
            break;
        case 2:
        {
            std::string src;
            std::cout << "Enter src: ";
            std::cin >> src;
            reservationSystem.src(src);
            break;
        }
        case 3:
        {
            std::string busId;
            int numSeats;
            std::cout << "Enter Bus ID: ";
            std::cin >> busId;
            std::cout << "Enter Number of Seats: ";
            std::cin >> numSeats;
            reservationSystem.reservation(busId, numSeats);
            break;
        }
        case 4:
            std::cout << "Exiting program...\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);

    return 0;
}
