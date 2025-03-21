#include <iostream>
#include <coroutine>

using std::cout;
using std::endl;
using std::boolalpha;
using std::nullptr_t;

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
        ~IntEnumerator() {
            if (handle.address() != nullptr)
                handle.destroy();
        }
        bool isDone() const { return handle.address() == nullptr || handle.done(); }
        void resume() { handle.resume(); }
        
        //ITERATOR PART:
        struct iterator {
            IntEnumerator* enumerator;
            bool operator!=(nullptr_t) const { return !enumerator->isDone(); }
            iterator& operator++() { enumerator->moveNext(); return *this; }
            int operator*() const { return enumerator->current(); }
        };
        iterator begin() {
            return iterator { this };
        }
        nullptr_t end() { return nullptr; }
        int current() const { return handle.promise().value; }
        bool moveNext() {
            if (isDone())
                return false;
            resume();
            return !isDone();
        }
};

IntEnumerator customRange(int start, int end, int step) {
    for (int i = start; i < end; i += step)
        co_yield i;

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
    cout << endl;

    IntEnumerator enumerator = customRange(0, 10, 2);
    while (!enumerator.isDone()) {
        cout << "Range element: " << enumerator.current() << endl;
        enumerator.resume();
    }
    cout << endl;

    cout << "Foreach loop with a coroutine + iterator in one!" << endl;
    for (int value : customRange(5, 17, 3))
        cout << "    " << value << "\n";
    cout << endl;
    return 0;
}
