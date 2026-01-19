#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
using namespace std;

int getInt(const string& msg, int minVal, int maxVal) {
    int x;
    while (true) {
        cout << msg;
        if (cin >> x && x >= minVal && x <= maxVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        cout << "Invalid. Try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string getLine(const string& msg) {
    string s;
    while (true) {
        cout << msg;
        getline(cin, s);
        if (!s.empty()) return s;
        cout << "Cannot be empty.\n";
    }
}

int main() {
    const int MAX = 10;
    string play[MAX]; int att[MAX] = { 0 }, comp[MAX] = { 0 }, count = 0;

    cout << "=== Flag Football Play Tracker ===\n";
    int choice;
    do {
        cout << "\n1) Add play stats\n2) View summary\n3) Suggest best plays (Man/Zone)\n4) Quit\n";
        choice = getInt("Choose (1-4): ", 1, 4);

        if (choice == 1) {
            if (count >= MAX) { cout << "Max plays reached.\n"; continue; }
            play[count] = getLine("Play name: ");
            att[count] = getInt("Attempts (1-100): ", 1, 100);
            comp[count] = getInt("Completions (0-attempts): ", 0, att[count]);
            cout << "Saved.\n";
            count++;
        }
        else if (choice == 2) {
            if (count == 0) { cout << "No plays yet.\n"; continue; }
            cout << left << setw(18) << "Play" << right << setw(8) << "Att" << setw(8) << "Comp" << setw(10) << "Comp%\n";
            cout << "------------------------------------------------\n";
            for (int i = 0; i < count; i++) {
                double pct = (double)comp[i] / att[i] * 100.0;
                cout << left << setw(18) << play[i] << right << setw(8) << att[i] << setw(8) << comp[i]
                    << setw(9) << fixed << setprecision(1) << pct << "%\n";
            }
        }
        else if (choice == 3) {
            if (count == 0) { cout << "No plays yet.\n"; continue; }
            int bestMan = 0, bestZone = 0;
            for (int i = 1; i < count; i++) {
                double pct = (double)comp[i] / att[i] * 100.0;
                if (pct > (double)comp[bestMan] / att[bestMan] * 100.0) bestMan = i;
                if (comp[i] > comp[bestZone]) bestZone = i;
            }
            cout << "Best vs MAN (highest completion %): " << play[bestMan] << "\n";
            cout << "Best vs ZONE (most completions):    " << play[bestZone] << "\n";
            cout << "Tip: Man likes quick separation routes; Zone likes spacing and soft spots.\n";
        }
    } while (choice != 4);

    cout << "Goodbye!\n";
    return 0;
}
