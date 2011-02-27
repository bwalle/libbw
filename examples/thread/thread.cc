#include <iostream>

#include <thread/thread.h>

class MyThread : public bw::thread::Thread
{
    protected:
        void run() throw ()
        {
            std::cout << "I'm running as thread!" << std::endl;
        }
};

int main(int argc, char *argv[])
{
    try {
        MyThread th;
        th.start();
        std::cout << "I'm running in main!" << std::endl;
        th.join();
    } catch (const bw::Error &err) {
        std::cerr << "Thread error: " << err.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
