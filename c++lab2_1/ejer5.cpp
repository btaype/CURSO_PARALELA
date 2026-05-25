#include <iostream>
#include <thread>
#include <future>

using namespace std;

void child(promise<void> p) {

    cout << "child" << endl;

    p.set_value();

}

void custom_join(future<void>& f) {

    f.get();

}

int main() {

    promise<void> p;

    future<void> f = p.get_future();

    thread thread_hijo(child, move(p));

    custom_join(f);

    cout << "parent" << endl;

    thread_hijo.detach();

    return 0;
}