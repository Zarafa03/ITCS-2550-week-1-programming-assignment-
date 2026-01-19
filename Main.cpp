#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

// Enum represents a meaningful concept for the hobby
enum DifficultyLevel { EASY = 1, INTERMEDIATE = 2, HARD = 3 };

// Struct models one play session
struct Session {
    string date;              // e.g., "2026-01-18"
    string playName;          // e.g., "Slant Right"
    int attempts;             // number of passes thrown
    int completions;          // number of completions
    double minutes;           // time spent running the play
    DifficultyLevel difficulty;
};

class FlagFootballTracker {
private:
    static const int MAX_SESSIONS = 7;   // no magic number
    Session sessions[MAX_SESSIONS];      // array of structs
    int sessionCount;

public:
    FlagFootballTracker() : sessionCount(0) {}

    // Core UI functions
    void showBanner() const;
    void showMenu() const;

    // Input + validation functions
    string getNonEmptyLine(const string& prompt) const;
    int getIntInRange(const string& prompt, int minVal, int maxVal) const;
    double getDoubleMin(const string& prompt, double minVal) const;

    // Data functions
    void addSession();
    void viewWeeklyReport() const;
    void recommendLevel() const;
    void saveReportToFile(const string& filename) const;

    // Derived calculations
    double completionPercentage(const Session& s) const;
    int totalAttempts() const;
    int totalCompletions() const;
    double overallCompletionPercentage() const;
};

void FlagFootballTracker::showBanner() const {
    cout << "=============================================\n";
    cout << "      FLAG FOOTBALL PLAY TRACKER (C++)\n";
    cout << "   Track sessions and completion percentage\n";
    cout << "=============================================\n\n";
}

void FlagFootballTracker::showMenu() const {
    cout << "\nMenu:\n";
    cout << "1) Add session\n";
    cout << "2) View weekly report\n";
    cout << "3) Recommend difficulty level\n";
    cout << "4) Save report to file\n";
    cout << "5) Quit\n";
}

string FlagFootballTracker::getNonEmptyLine(const string& prompt) const {
    string input;
    while (true) { // while loop used for validation
        cout << prompt;
        getline(cin, input);

        // Trim-like basic check (no advanced code)
        if (input.size() > 0) {
            return input;
        }
        cout << "Input cannot be empty. Please try again.\n";
    }
}

int FlagFootballTracker::getIntInRange(const string& prompt, int minVal, int maxVal) const {
    int value;
    while (true) { // while loop used for numeric validation
        cout << prompt;
        cin >> value;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid number. Please enter an integer.\n";
        }
        else if (value < minVal || value > maxVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a value between " << minVal << " and " << maxVal << ".\n";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // remove leftover newline
            return value;
        }
    }
}

double FlagFootballTracker::getDoubleMin(const string& prompt, double minVal) const {
    double value;
    while (true) { // while loop used for numeric validation
        cout << prompt;
        cin >> value;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid number. Please enter a decimal number.\n";
        }
        else if (value < minVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a value that is at least " << minVal << ".\n";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

double FlagFootballTracker::completionPercentage(const Session& s) const {
    if (s.attempts == 0) return 0.0;
    return (static_cast<double>(s.completions) / s.attempts) * 100.0;
}

int FlagFootballTracker::totalAttempts() const {
    int sum = 0;
    for (int i = 0; i < sessionCount; i++) { // for loop fixed range
        sum += sessions[i].attempts;
    }
    return sum;
}

int FlagFootballTracker::totalCompletions() const {
    int sum = 0;
    for (int i = 0; i < sessionCount; i++) { // for loop fixed range
        sum += sessions[i].completions;
    }
    return sum;
}

double FlagFootballTracker::overallCompletionPercentage() const {
    int attempts = totalAttempts();
    int comps = totalCompletions();
    if (attempts == 0) return 0.0;
    return (static_cast<double>(comps) / attempts) * 100.0;
}

void FlagFootballTracker::addSession() {
    if (sessionCount >= MAX_SESSIONS) {
        cout << "You already have " << MAX_SESSIONS << " sessions. View or save the report.\n";
        return;
    }

    Session s;
    cout << "\n--- Add a Session (" << (sessionCount + 1) << "/" << MAX_SESSIONS << ") ---\n";
    s.date = getNonEmptyLine("Enter date (example 2026-01-18): ");
    s.playName = getNonEmptyLine("Enter play name (example Slant Right): ");

    s.attempts = getIntInRange("Enter pass attempts (0 to 100): ", 0, 100);
    s.completions = getIntInRange("Enter completions (0 to attempts): ", 0, s.attempts);

    s.minutes = getDoubleMin("Enter minutes spent running the play (>= 0): ", 0.0);

    int diff = getIntInRange("Difficulty (1=Easy, 2=Intermediate, 3=Hard): ", 1, 3);
    s.difficulty = static_cast<DifficultyLevel>(diff);

    sessions[sessionCount] = s;
    sessionCount++;

    cout << "Session added. Completion % = "
        << fixed << setprecision(1) << completionPercentage(s) << "%\n";
}

void FlagFootballTracker::viewWeeklyReport() const {
    if (sessionCount == 0) {
        cout << "No sessions yet. Add a session first.\n";
        return;
    }

    cout << "\n================== WEEKLY REPORT ==================\n";
    cout << left
        << setw(12) << "Date"
        << setw(18) << "Play"
        << right
        << setw(10) << "Att"
        << setw(10) << "Comp"
        << setw(12) << "Minutes"
        << setw(12) << "Comp%"
        << setw(12) << "Diff"
        << "\n";

    cout << "---------------------------------------------------\n";

    for (int i = 0; i < sessionCount; i++) {
        string diffText;
        switch (sessions[i].difficulty) { // enum used in decision structure
        case EASY: diffText = "Easy"; break;
        case INTERMEDIATE: diffText = "Intermed"; break;
        case HARD: diffText = "Hard"; break;
        default: diffText = "Unknown"; break;
        }

        cout << left
            << setw(12) << sessions[i].date
            << setw(18) << sessions[i].playName
            << right
            << setw(10) << sessions[i].attempts
            << setw(10) << sessions[i].completions
            << setw(12) << fixed << setprecision(1) << sessions[i].minutes
            << setw(12) << fixed << setprecision(1) << completionPercentage(sessions[i])
            << setw(12) << diffText
            << "\n";
    }

    cout << "---------------------------------------------------\n";
    cout << "TOTAL Attempts: " << totalAttempts()
        << " | TOTAL Completions: " << totalCompletions()
        << " | OVERALL Completion %: " << fixed << setprecision(1)
        << overallCompletionPercentage() << "%\n";
}

void FlagFootballTracker::recommendLevel() const {
    if (sessionCount == 0) {
        cout << "No sessions yet. Add a session first.\n";
        return;
    }

    double overallPct = overallCompletionPercentage();
    int attempts = totalAttempts();

    // At least two if/else blocks using compound Boolean conditions
    if (overallPct >= 70.0 && attempts >= 20) {
        cout << "Recommendation: HARD (You are consistent and have enough reps.)\n";
    }
    else if (overallPct >= 50.0 && attempts >= 10) {
        cout << "Recommendation: INTERMEDIATE (Good progress. Keep practicing.)\n";
    }
    else {
        cout << "Recommendation: EASY (Focus on fundamentals and reps.)\n";
    }

    // Second compound-condition if/else example
    if (overallPct < 40.0 && attempts >= 10) {
        cout << "Tip: Slow down and run shorter routes first (slants, quick outs).\n";
    }
    else if (overallPct >= 40.0 && overallPct < 60.0) {
        cout << "Tip: Work timing with the receiver (same steps every rep).\n";
    }
    else {
        cout << "Tip: Add defensive pressure drills to simulate game speed.\n";
    }
}

void FlagFootballTracker::saveReportToFile(const string& filename) const {
    ofstream fout(filename.c_str());

    if (!fout) {
        cout << "Error: Could not open file for writing.\n";
        return;
    }

    fout << "FLAG FOOTBALL PLAY TRACKER REPORT\n";
    fout << "=================================\n\n";

    if (sessionCount == 0) {
        fout << "No sessions recorded.\n";
        fout.close();
        cout << "Saved (empty) report to " << filename << "\n";
        return;
    }

    fout << left
        << setw(12) << "Date"
        << setw(18) << "Play"
        << right
        << setw(10) << "Att"
        << setw(10) << "Comp"
        << setw(12) << "Minutes"
        << setw(12) << "Comp%"
        << setw(12) << "Diff"
        << "\n";

    fout << "---------------------------------------------------\n";

    for (int i = 0; i < sessionCount; i++) {
        string diffText;
        switch (sessions[i].difficulty) {
        case EASY: diffText = "Easy"; break;
        case INTERMEDIATE: diffText = "Intermed"; break;
        case HARD: diffText = "Hard"; break;
        default: diffText = "Unknown"; break;
        }

        fout << left
            << setw(12) << sessions[i].date
            << setw(18) << sessions[i].playName
            << right
            << setw(10) << sessions[i].attempts
            << setw(10) << sessions[i].completions
            << setw(12) << fixed << setprecision(1) << sessions[i].minutes
            << setw(12) << fixed << setprecision(1) << completionPercentage(sessions[i])
            << setw(12) << diffText
            << "\n";
    }

    fout << "---------------------------------------------------\n";
    fout << "TOTAL Attempts: " << totalAttempts()
        << " | TOTAL Completions: " << totalCompletions()
        << " | OVERALL Completion %: " << fixed << setprecision(1)
        << overallCompletionPercentage() << "%\n";

    fout.close();
    cout << "Report saved to " << filename << "\n";
}

int main() {
    FlagFootballTracker app;
    app.showBanner();

    // Beginner-friendly mixed input: name with spaces
    string userName;
    cout << "Enter your name: ";
    getline(cin, userName);

    // do-while loop repeats until user chooses to quit
    int choice;
    do {
        app.showMenu();
        choice = app.getIntInRange("Choose an option (1-5): ", 1, 5);

        // switch menu required
        switch (choice) {
        case 1:
            app.addSession();
            break;
        case 2:
            app.viewWeeklyReport();
            break;
        case 3:
            app.recommendLevel();
            break;
        case 4:
            app.saveReportToFile("report.txt");
            break;
        case 5:
            cout << "Goodbye, " << userName << ". Thanks for using the tracker.\n";
            break;
        }
    } while (choice != 5);

    return 0;
}
