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
        suspend_always final_suspend() noexcept { return { }; }
        void return_void() { }
        void unhandled_exception() { }
    };

    coroutine_handle<> handle;
    CustomTask(coroutine_handle<> handle) : handle(handle) { }
};

CustomTask exampleCoroutine() {
    cout << "Statement A" << endl;
    co_await suspend_always { };
    cout << "Statement B" << endl;
}



struct IntEnumerator {
    struct promise_type {
        int value;
        IntEnumerator get_return_object() { return IntEnumerator(coroutine_handle<promise_type>::from_promise(*this)); }
        suspend_never initial_suspend() { return { }; }
        suspend_always final_suspend() noexcept { return { }; }

        suspend_always yield_value(int value) {
            this->value = value;
            return suspend_always { };
        }
        void return_value(int value) {
            this->value = value;
        }
        void unhandled_exception() { }
    };

    private:
        coroutine_handle<promise_type> handle;
        IntEnumerator(coroutine_handle<promise_type> handle) : handle(handle) { }
    public:
        ~IntEnumerator() { cout << "!?" << endl;}
        int getValue() const { return handle.done() ? -1 : handle.promise().value; }
        bool isDone() const { return handle.done(); }
        void resume() { handle.resume(); }
};

IntEnumerator customRange(int start, int end, int step) {
    for (int i = start; i < end; i += step) {
        cout << "co_yield..." << endl;
        co_yield i;
    }
    //NOTE: This will reach final_suspend() and -1 will never actually be used by the caller:
    //  co_return at the end avoids undefined behavior for going off the end of the coroutine!
    co_return -1;
}



//NOTE: See tutorial series: https://www.youtube.com/watch?v=soHQAVYTmCE&list=PL2EnPlznFzmhKDBfE0lqMAWyr74LZsFVY&index=1
int main() {
    CustomTask task = exampleCoroutine();
    task.handle.resume();

    //NOTE: coroutine_handle's done() method returns true IFF the coroutine is SUSPENDED at its final suspend point!
    //  So, we must make sure the XXX::promise_type::final_suspend() must return suspend_always.
    //  Otherwise, we'll never be done() and may enter undefined behavior!
    //SEE: https://en.cppreference.com/w/cpp/coroutine/coroutine_handle/done
    cout << "The coroutine is now done? " << boolalpha << task.handle.done() << endl;

    IntEnumerator enumerator = customRange(0, 10, 2);
    while (!enumerator.isDone()) {
        cout << "Range element: " << enumerator.getValue() << " is done? " << boolalpha << enumerator.isDone() << endl;
        enumerator.resume();
    }
    return 0;
}
