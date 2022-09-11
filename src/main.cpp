#include <ft-detector/detectors.h>
#include <spdlog/spdlog.h>

#include <boost/program_options.hpp>

struct Options {
    std::string inputFile;
    std::string modelPath;
    std::string labelsPath = "labels/classes.txt";
    bool debug = false;
    bool show = false;
    bool gpu = true;
};

namespace po = boost::program_options;
bool parse_options(int argc, char* argv[], Options& options) {
    po::options_description optionsDesc("Allowed options");
    try {
        // clang-format off
        optionsDesc.add_options()
        ("input,i", po::value<std::string>(&options.inputFile)->required(), "video or image input file (folder for mAP measurement)")
        ("model,m", po::value<std::string>(&options.modelPath)->required(), "video or image input file (folder for mAP measurement)")
        ("debug", "Run in debug mode")
        ("cpu", "Run with CPU only (default: Run with GPU)")
        ("help,h", "Show Usage");
        // clang-format on

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, optionsDesc), vm);
        if (vm.count("help")) {
            std::cout << optionsDesc << std::endl;
            return false;
        }
        if (vm.count("debug")) {
            options.debug = true;
        }
        if (vm.count("show")) {
            options.show = true;
        }
        if (vm.count("cpu")) {
            options.gpu = false;
        }

        po::notify(vm);
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cout << optionsDesc << std::endl;
        return false;
    } catch (...) {
        std::cerr << "Unknown error!" << std::endl;
        std::cout << optionsDesc << std::endl;
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    Options options;
    if (!parse_options(argc, argv, options)) {
        return -1;
    }
    if (options.debug) {
        spdlog::set_level(spdlog::level::debug);
        spdlog::debug("App is running in debug mode");
    }

    auto detector = Detectors::yolov5(options.modelPath, options.labelsPath, options.gpu);
    detector->detectVideo(options.inputFile);

    return 0;
}
