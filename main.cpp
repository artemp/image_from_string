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
        std::cout << "Usage:" << argv[0] << " <path-to-image-file> <num_runs>" << std::endl;
        return 1;
    }
    unsigned NUM_RUNS = std::atoi(argv[2]);
    std::string filename(argv[1]);


    // node-blend
    {
        std::cerr << "========== Node-blend ImageReader FROM FILE" << std::endl;
        std::cout << "NUM_RUNS="<<NUM_RUNS << std::endl;
        boost::timer::auto_cpu_timer t;
        for (unsigned count=0; count < NUM_RUNS; ++count)
        {
            std::ifstream is(filename.c_str() , std::ios::binary);
            std::string buffer((std::istreambuf_iterator<char>(is)),
                               std::istreambuf_iterator<char>());
            const std::unique_ptr<ImageReader> layer(ImageReader::create((uint8_t*)buffer.data(),buffer.size()));
            layer->decode();
        }
    }


    { // mapnik
        std::cerr << "========== Mapnik image_reader FROM FILE" << std::endl;

        std::cout << "NUM_RUNS="<<NUM_RUNS << std::endl;
        boost::timer::auto_cpu_timer t;
        for (unsigned count=0; count < NUM_RUNS; ++count)
        {
            try {
                const std::unique_ptr<mapnik::image_reader> reader(mapnik::get_image_reader(filename));
                unsigned width = reader->width();
                unsigned height = reader->height();
                mapnik::image_data_32 buffer(width,height);
                reader->read(0,0,buffer);
            }
            catch (mapnik::image_reader_exception const& ex)
            {
                std::cerr << ex.what() << std::endl;
            }
            catch( std::exception const& ex)
            {
                std::cerr << ex.what() << std::endl;
            }
        }
    }

    std::ifstream is(filename.c_str() , std::ios::binary);
    std::string buffer((std::istreambuf_iterator<char>(is)),
                       std::istreambuf_iterator<char>());

    // node-blend
    {
        std::cerr << "========== Node-blend ImageReader FROM MEM BUFFER" << std::endl;
        std::cout << "NUM_RUNS="<<NUM_RUNS << std::endl;
        boost::timer::auto_cpu_timer t;
        for (unsigned count=0; count < NUM_RUNS; ++count)
        {
            const std::unique_ptr<ImageReader> layer(ImageReader::create((uint8_t*)buffer.data(),buffer.size()));
            layer->decode();
        }
    }


    { // mapnik
        std::cerr << "========== Mapnik image_reader FROM MEM BUFFER" << std::endl;

        std::cout << "NUM_RUNS="<<NUM_RUNS << std::endl;
        boost::timer::auto_cpu_timer t;
        for (unsigned count=0; count < NUM_RUNS; ++count)
        {
            try {
                const std::unique_ptr<mapnik::image_reader> reader(mapnik::get_image_reader(buffer.data(), buffer.size()));
                unsigned width = reader->width();
                unsigned height = reader->height();
                mapnik::image_data_32 buffer(width,height);
                reader->read(0,0,buffer);
            }
            catch (mapnik::image_reader_exception const& ex)
            {
                std::cerr << ex.what() << std::endl;
            }
            catch( std::exception const& ex)
            {
                std::cerr << ex.what() << std::endl;
            }
        }
    }
    return 0;
}
