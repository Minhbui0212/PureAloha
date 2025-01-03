#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <unordered_set>

using namespace std;

// Constants
const int FRAME_TIME_STEPS = 1;
const double WAIT_TIME_MILL_SEC = 1.0;

// Station class to represent each station
class Station {
public:
    bool isTransmitting = false;
    bool isCollided = false;

    bool isAvailableToSend(double probability) {
        return ((double)rand() / RAND_MAX) < probability;
    }
};

// Simulate Pure ALOHA with finite stations based on flowchart
void simulatePureALOHA(int numStations, double packetGenProb, double duration, double &efficiency) {
    int totalCollisions = 0;
    int totalAttempts = 0;
    int successfulTransmissions = 0;
    double currentTime = 0.0;

    vector<Station> stations(numStations);
    unordered_set<int> transmittingStationsAtCurrentTime;

    while (currentTime < duration) {
        int collisions = 0;
        transmittingStationsAtCurrentTime.clear();

        // Step 1: Check each station and decide if it transmits
        for (int i = 0; i < numStations; ++i) {
            if (stations[i].isAvailableToSend(packetGenProb)) {
                totalAttempts++;
                stations[i].isTransmitting = true;

                // Add to transmitting set
                transmittingStationsAtCurrentTime.insert(i);
            }
        }

        // Step 2: Handle transmission outcomes
        if (transmittingStationsAtCurrentTime.size() > 1) {
            // Collision occurs
            for (int stationId : transmittingStationsAtCurrentTime) {
                stations[stationId].isCollided = true;
            }
            collisions = transmittingStationsAtCurrentTime.size();
            cout << "Time: " << currentTime << " - Collision detected with " << collisions << " stations.\n";
        } else if (transmittingStationsAtCurrentTime.size() == 1) {
            // Successful transmission
            int stationId = *transmittingStationsAtCurrentTime.begin();
            stations[stationId].isTransmitting = false;
            successfulTransmissions++;
            cout << "Time: " << currentTime << " - Successful transmission by station " << stationId << ".\n";
        }

        // Step 3: Update collision count
        totalCollisions += collisions;

        // Step 4: Move to next time slot
        currentTime += FRAME_TIME_STEPS;

        // Reset stations
        for (int i = 0; i < numStations; ++i) {
            stations[i].isTransmitting = false;
            stations[i].isCollided = false;
        }
    }

    // Step 5: Calculate efficiency
    efficiency = ((double)(totalAttempts - totalCollisions) * FRAME_TIME_STEPS * WAIT_TIME_MILL_SEC) / (currentTime * 100);

    // Print summary
    cout << "Total attempts: " << totalAttempts << ", Total collisions: " << totalCollisions << ", Successful transmissions: " << successfulTransmissions << "\n";
}

int main() {
    srand(time(0)); // Seed random number generator

    // Simulation parameters
    double duration = 1000; // Duration in time slots
    int numStations = 10; // Number of stations
    double packetGenProb[] = {0.04, 0.1, 0.6}; // Light, medium, high traffic

    cout << "--- Pure ALOHA Simulation with Finite Stations ---\n";
    for (double p : packetGenProb) {
        double efficiency;
        simulatePureALOHA(numStations, p, duration, efficiency);
        cout << "Packet Generation Probability = " << p << ", Efficiency = " << efficiency << "\n";
    }

    return 0;
}

