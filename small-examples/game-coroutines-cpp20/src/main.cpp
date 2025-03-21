#include <iostream>
#include <coroutine>
#include <chrono>
#include <thread>

using std::cout;
using std::endl;
using std::boolalpha;
using std::nullptr_t;

using std::suspend_always;
using std::suspend_never;
using std::coroutine_handle;

namespace this_thread = std::this_thread;
namespace chrono = std::chrono;
using chrono::time_point;
using chrono::steady_clock;
using chrono::duration_cast;

//NOTE: Order of API calls for C++ 20 Coroutines:
//1. get_return_object()            This constructs your custom coroutine class/struct initially.
//2. initial_suspend()              This decides whether or not to suspend initially (lazy vs. eager coroutine).
//3. coroutine_handle<>.resume()    This makes the coroutine continue from where it last left off.



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
    //NOTE: It's recommended to store data inside the inner promise_type, since it keeps up-to-date with the yields/returns of the coroutine:
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



//NOTE:
//When I tried to use co_await and get a value back at the same time,
//  I realized they really didn't design it that way. The best way was in
//  my custom awaiter to spawn a thread from await_suspend, but I didn't want to use another thread.
// So I think I might want to use co_yield instead, so I can poll on the same thread,
//  and pass the instructions from the co_yield statement back to the caller, for example,
//  if I want to co_yield WaitForSeconds(2) almost like a Unity C# coroutine.

// struct TimedAwaiter {
//     float duration = 0;
//     time_point<steady_clock> startTime;
//     coroutine_handle<> handle;
//     bool ready = false;

//     TimedAwaiter(float duration) : duration(duration) {
//         startTime = steady_clock::now();
//     }

//     bool await_ready() const noexcept { return ready; }
//     void await_suspend(coroutine_handle<> handle) noexcept {
//         cout << "await_suspend?" << endl;
//         this->handle = handle;
//         startTime = steady_clock::now();
//     }
//     void await_resume() const noexcept { }

//     bool shouldResume() {
//         chrono::seconds elapsed = duration_cast<chrono::seconds>(steady_clock::now() - startTime);
//         long long elapsedL = elapsed.count();
//         cout << "shouldResume? " << elapsedL << endl;
//         return elapsedL >= duration;
//     }

//     bool poll() {
//         cout << "poll..." << boolalpha << (handle.address() != nullptr) << " " << (!(handle.address() != nullptr && handle.done())) << endl;
//         if (handle.address() != nullptr && !handle.done() && shouldResume()) {
//             cout << "RESUMING AFTER 2 SEC!" << endl;
//             handle.resume();
//             handle = nullptr; //NOTE: Prevent double resumption
//             return true;
//         }
//         return false;
//     }
// };

struct WaitForSeconds {
    float duration = 0;
    time_point<steady_clock> startTime;
    bool ready = false;

    WaitForSeconds(float duration) : duration(duration) {
        startTime = steady_clock::now();
    }

    bool shouldResume() {
        chrono::milliseconds elapsedMS = duration_cast<chrono::milliseconds>(steady_clock::now() - startTime);
        long long elapsedMSInt = elapsedMS.count();
        cout << "shouldResume? " << elapsedMSInt << endl;
        return (float) elapsedMSInt / 1000 >= duration;
    }
};

struct ComplexCustomTask {
    struct promise_type {
        WaitForSeconds value = WaitForSeconds(0);

        ComplexCustomTask get_return_object() { return ComplexCustomTask(coroutine_handle<promise_type>::from_promise(*this)); }
        suspend_never initial_suspend() { return { }; }
        suspend_always final_suspend() noexcept { return { }; }

        suspend_always yield_value(WaitForSeconds value) {
            this->value = value;
            return suspend_always { };
        }
        void return_value(nullptr_t value) {
            this->value = WaitForSeconds(0);
        }
        void unhandled_exception() { }
    };

    coroutine_handle<promise_type> handle;
    ComplexCustomTask(coroutine_handle<promise_type> handle) : handle(handle) { }
};

ComplexCustomTask moreComplexCoroutine() {
    cout << "Statement A" << endl;
    for (int i = 0; i < 3; i++) {
        co_yield WaitForSeconds(2);
        cout << "Statement after iteration " << i << "!" << endl;
    }
    co_return nullptr;
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

    cout << "Testing custom awaiter..." << endl;
    ComplexCustomTask complexTask = moreComplexCoroutine();
    while (!complexTask.handle.done()) {
        if (complexTask.handle.promise().value.shouldResume())
            complexTask.handle.resume();
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    return 0;
}
