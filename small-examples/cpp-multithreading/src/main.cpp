#include <iostream>
#include <string>
#include <future>
#include <thread>

using namespace std;

float testLongMethod() {
    float result = 0;
    for (int i = 0; i < 2000000000; i++) {
        result += (float) i / 2;
    }
    result *= 0.2049;
    return result;
}

int main() {
    cout << "Hello world!" << endl;

    //NOTE: Launch policy async (launch::async) forces this to run on another thread,
    //  Whereas the deferred launch policy (launch::deferred) makes this run on the calling thread (not in parallel)
    //  in a lazily-evaluated fashion (upon request).
    future<float> task = async(
        launch::async,
        []() { return testLongMethod(); }
    );

    while (!task._Is_ready()) {
        cout << ".";
        this_thread::sleep_for(chrono::milliseconds(300));
    }
    cout << "    DONE!" << endl;

    string input;
    getline(cin, input);

    string result = to_string(task.get());
    cout << "The result calculated was " << result << endl;

    cout << "You entered: " << input << endl;
}
