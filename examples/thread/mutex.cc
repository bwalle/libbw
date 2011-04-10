#include <iostream>
#include <cstdlib>

#include <thread/mutex.h>

int main(int argc, char *argv[])
{
    try {
        bw::thread::Mutex m;
        m.lock();
        
        if (!m.tryLock())
            std::cout << "Already locked, ok" << std::endl;
        else
            std::cout << "Not locked????????" << std::endl;

        m.unlock();
    } catch (const bw::Error &err) {
        std::cerr << "Thread error: " << err.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
