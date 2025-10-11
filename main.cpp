#include <iostream>

using namespace std;

void appclose() {
    cout << endl << "Press enter to stop program...";
    cin.get();
}

int main() {
    cout << "Hello, World!" << endl;
    appclose();
    return 0;
}