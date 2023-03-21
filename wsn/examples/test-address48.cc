#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unordered_set>
#include <iomanip>

#include <sstream>
using namespace std;

string generate_mac_address() {
    srand(time(NULL));
    stringstream mac;
    mac << setfill('0') << setw(2) << hex << (rand() % 256) << ":";
    mac << setfill('0') << setw(2) << hex << (rand() % 256) << ":";
    mac << setfill('0') << setw(2) << hex << (rand() % 256) << ":";
    mac << setfill('0') << setw(2) << hex << (rand() % 256) << ":";
    mac << setfill('0') << setw(2) << hex << (rand() % 256) << ":";
    mac << setfill('0') << setw(2) << hex << (rand() % 256);
    return mac.str();
}

int main() {
    unordered_set<string> mac_set;
    int num_macs = 10;
    for (int i = 0; i < num_macs; i++) {
        string mac = generate_mac_address();
        if (mac_set.find(mac) != mac_set.end()) {
            i--;
            continue;
        }
        mac_set.insert(mac);
        cout << mac << endl;
    }
    return 0;
}
