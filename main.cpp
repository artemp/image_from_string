// Image readers
#include <iostream>
#include <fstream>
#include <boost/timer/timer.hpp>
#include <mapnik/image_reader.hpp>
#include <mapnik/image_util.hpp>
#include <memory>

#include <reader.hpp>

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
    std::ifstream is(filename.c_str() , std::ios::binary);
    std::string buffer((std::istreambuf_iterator<char>(is)),
                       std::istreambuf_iterator<char>());


    // node-blend
    {
        std::cerr << "========== Node-blend ImageReader" << std::endl;
        std::cout << "NUM_RUNS="<<NUM_RUNS << std::endl;
        boost::timer::auto_cpu_timer t;
        for (unsigned count=0; count < NUM_RUNS; ++count)
        {
            const std::unique_ptr<ImageReader> layer(ImageReader::create((uint8_t*)buffer.c_str(),buffer.size()));
            layer->decode();
        }
    }


    { // mapnik
        std::cerr << "========== Mapnik image_reader" << std::endl;
        try {
            std::cout << "NUM_RUNS="<<NUM_RUNS << std::endl;
            boost::timer::auto_cpu_timer t;
            for (unsigned count=0; count < NUM_RUNS; ++count)
            {
                const std::unique_ptr<mapnik::image_reader> reader(mapnik::get_image_reader((char*)buffer.c_str(),buffer.size()));
                //const std::unique_ptr<mapnik::image_reader> reader(mapnik::get_image_reader(filename));
                if (reader.get())
                {
                    unsigned width = reader->width();
                    unsigned height = reader->height();
                    //std::cout << "Image (" << width << "," << height << ")" << std::endl;
                    mapnik::image_data_32 buffer(width,height);
                    //for (unsigned count=0; count < NUM_RUNS; ++count)
                    //{
                    reader->read(0,0,buffer);
                    //}
                    // write image
                    //mapnik::save_to_file(buffer,"output.png");
                }
                else
                {
                    std::cerr << "Can't create reader ...:)" << std::endl;
                }
            }
        }
        catch (mapnik::image_reader_exception const& ex)
        {
            std::cerr << ex.what() << std::endl;
        }
    }

    return 0;
}
