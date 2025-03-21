#include <iostream>
#include <coroutine>

using std::cout;
using std::endl;

using std::suspend_always;
using std::suspend_never;

struct CustomTask {
    struct promise_type {
        CustomTask get_return_object() { return { }; }
        suspend_never initial_suspend() { return { }; }
        suspend_never final_suspend() noexcept { return { }; }
        void return_void() { }
        void unhandled_exception() { }
    };
};

CustomTask exampleCoroutine() {
    cout << "Statement A" << endl;
    co_await suspend_always { };
    cout << "Statement B" << endl;
}

int main() {
    exampleCoroutine();
    return 0;
}
