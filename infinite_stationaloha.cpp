#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <queue>

using namespace std;

// Constants
const double WAIT_TIME_MILL_SEC = 1.0;
const double EPS = 1e-6; // Small epsilon for time comparisons

// Simulate Pure ALOHA with infinite stations based on flowchart
void simulatePureALOHAInfinite(double arrivalRate, double duration, double &efficiency) {
    double currentTime = 0.0;
    int successCount = 0;
    int totalAttempts = 0;

    queue<double> packets; // Queue to track packets being transmitted

    srand(time(0)); // Seed random number generator

    while (currentTime < duration) {
        // Step 1: Generate packets based on Poisson arrival
        double nextArrival = -log(1.0 - (double)rand() / RAND_MAX) / arrivalRate;
        if (currentTime + nextArrival < duration) {
            packets.push(currentTime + nextArrival);
        }

        // Step 2: Send the next packet
        if (!packets.empty() && packets.front() <= currentTime) {
            totalAttempts++;
            double packetStart = packets.front();
            packets.pop();

            // Step 3: Check if channel is free
            if (packets.empty() || (packets.front() - packetStart >= EPS)) {
                // Successful transmission
                successCount++;
                cout << "Time: " << currentTime << " - Successful transmission.\n";
            } else {
                // Collision: Remove all packets within collision window
                int collisionCount = 1;
                while (!packets.empty() && (packets.front() - packetStart < EPS)) {
                    packets.pop();
                    collisionCount++;
                }
                cout << "Time: " << currentTime << " - Collision detected with " << collisionCount << " packets.\n";
            }
        }

        // Step 4: Increment time
        currentTime += WAIT_TIME_MILL_SEC;
    }

    // Step 5: Calculate efficiency
    efficiency = (double)successCount / totalAttempts * 100;

    // Print summary
    cout << "Total attempts: " << totalAttempts << ", Total collisions: " << (totalAttempts - successCount)
         << ", Successful transmissions: " << successCount << "\n";
}

int main() {
    // Simulation parameters
    double duration = 1000; // Duration in milliseconds
    double arrivalRates[] = {0.05, 0.1, 0.5}; // Light, medium, high traffic

    cout << "--- Pure ALOHA Simulation with Infinite Stations ---\n";
    for (double arrivalRate : arrivalRates) {
        double efficiency;
        simulatePureALOHAInfinite(arrivalRate, duration, efficiency);
        cout << "Arrival Rate = " << arrivalRate << ", Efficiency = " << efficiency << "%\n";
    }

    return 0;
}
