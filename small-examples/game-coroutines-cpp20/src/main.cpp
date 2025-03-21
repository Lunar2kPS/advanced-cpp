#include <iostream>
#include <coroutine>

using std::cout;
using std::endl;
using std::boolalpha;

using std::suspend_always;
using std::suspend_never;
using std::coroutine_handle;

struct CustomTask {
    struct promise_type {
        CustomTask get_return_object() { return CustomTask(coroutine_handle<promise_type>::from_promise(*this)); }
        suspend_never initial_suspend() { return { }; }
        suspend_never final_suspend() noexcept { return { }; }
        void return_void() { }
        void unhandled_exception() { }
    };

    coroutine_handle<> handle;
    CustomTask(coroutine_handle<> handle) : handle(handle) {
        
    }
};

CustomTask exampleCoroutine() {
    cout << "Statement A" << endl;
    co_await suspend_always { };
    cout << "Statement B" << endl;
}

//NOTE: See tutorial series: https://www.youtube.com/watch?v=soHQAVYTmCE&list=PL2EnPlznFzmhKDBfE0lqMAWyr74LZsFVY&index=1
int main() {
    CustomTask task = exampleCoroutine();
    task.handle.resume();
    cout << "The coroutine is now done? " << boolalpha << task.handle.done() << endl;
    return 0;
}
