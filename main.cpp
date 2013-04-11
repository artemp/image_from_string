// Image readers
#include <iostream>
#include <boost/timer/timer.hpp>
#include <mapnik/image_reader.hpp>

int main(int argc, char** argv)
{
    std::cout << "Image from string..." << std::endl;
    if (argc!=3)
    {
        std::cout << "Usage:" << argv[0] << " <png> <num_runs>" << std::endl;
        return 1;
    }
    unsigned NUM_RUNS = std::atoi(argv[2]);
    std::string filename(argv[1]);

    try {

        std::auto_ptr<mapnik::image_reader> reader(mapnik::get_image_reader(filename));
        if (reader.get())
        {
            unsigned width = reader->width();
            unsigned height = reader->height();
            std::cout << "Image (" << width << "," << height << ")" << std::endl;
            std::cout << "NUM_RUNS="<<NUM_RUNS << std::endl;
            boost::timer::auto_cpu_timer t;
            mapnik::image_data_32 buffer(width,height);
            for (unsigned count=0; count < NUM_RUNS; ++count)
            {
                reader->read(0,0,buffer);
            }
        }
    }
    catch (mapnik::image_reader_exception const& ex)
    {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}
