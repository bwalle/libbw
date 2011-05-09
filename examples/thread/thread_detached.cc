#include <iostream>
#include <cstdlib>

#include <thread/thread.h>

class MyThread : public bw::thread::Thread
{
    public:
        MyThread()
            : bw::thread::Thread(bw::thread::Thread::DETACHED)
        {}

    protected:
        void run() throw ()
        {
            std::cout << "I'm running as thread!" << std::endl;
        }
};

int main(int argc, char *argv[])
{
    try {
        MyThread *th = new MyThread;
        th->start();
        std::cout << "I'm running in main!" << std::endl;
    } catch (const bw::Error &err) {
        std::cerr << "Thread error: " << err.what() << std::endl;
        return EXIT_FAILURE;
    }

    sleep(5);

    return EXIT_SUCCESS;
}
