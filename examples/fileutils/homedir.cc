#include <iostream>

#include <libbw/fileutils.h>
#include <libbw/log/errorlog.h>

int main()
{
    try {
        std::cout << "Your home directory: " << bw::FileUtils::homeDirectory() << std::endl;
    } catch (const bw::Error &err) {
        BW_ERROR_STREAM_ERR("Error in obtaining home dir: " << err.what());
        return -1;
    }

    return 0;
}
