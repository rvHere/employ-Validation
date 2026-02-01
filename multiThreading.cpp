#include <iostream>
#include <thread>
#include <map>
#include <string>
#include <chrono>
#include <mutex>

// Global mutex for thread-safe printing
std::mutex coutMutex;

// Background thread function
void refreshForecast(std::map<std::string, int>& forecastMap) {
    using namespace std::chrono_literals;

    while (true) {
        // Simulate temperature changes
        for (auto& item : forecastMap) {
            item.second += 1;   // simple increment for demo
        }

        // Thread-safe printing
        {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout << "\nUpdated Forecast:\n";
            for (const auto& item : forecastMap) {
                std::cout << "  " << item.first << ": "
                          << item.second << "°C\n";
            }
            std::cout << "--------------------------\n";
        }

        std::this_thread::sleep_for(2s);
    }
}

int main() {
    // Initial dummy weather data
    std::map<std::string, int> forecastMap = {
        {"New York", 15},
        {"Mumbai",   28},
        {"Berlin",   18}
    };

    // Start background thread (pass map by reference)
    std::thread bgWorker(refreshForecast, std::ref(forecastMap));

    // Main thread doing other work
    for (int i = 0; i < 5; ++i) {
        {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout << "Main thread working... (" << i + 1 << ")\n";
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    // Let background thread run independently
    bgWorker.detach();

    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "Main thread finished.\n";
    }

    return 0;
}
