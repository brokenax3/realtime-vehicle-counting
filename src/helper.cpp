#include "helper.h"

void validate(boost::any& v, const std::vector<std::string>& values, Roi*,
              int) {
    int width, height, x, y;

    namespace bpo = boost::program_options;

    static boost::regex r("(\\d.*),(\\d.*),(\\d.*),(\\d.*)");

    bpo::validators::check_first_occurrence(v);

    const std::string& s = bpo::validators::get_single_string(values);

    boost::smatch match;
    if (boost::regex_match(s, match, r)) {
        width = boost::lexical_cast<int>(match[1]);
        height = boost::lexical_cast<int>(match[2]);
        x = boost::lexical_cast<int>(match[3]);
        y = boost::lexical_cast<int>(match[4]);
        v = Roi{width, height, x, y};
    } else {
        throw bpo::validation_error(
            bpo::validation_error::invalid_option_value);
    }
}

po::variables_map parse_cmd(int argc, char* argv[]) {
    // Generic actions
    po::options_description general("General Options");
    general.add_options()("help", "Print this help");

    // Declare supported actions
    po::options_description config_options("Required Options");
    config_options.add_options()(
        "model_path",
        po::value<std::string>()->default_value("./dnn_files/yolov5n-opt.onnx"),
        "Deep Learning Model");
    config_options.add_options()(
        "class_path",
        po::value<std::string>()->default_value("./dnn_files/coco_c.names"),
        "Class Names");
    config_options.add_options()(
        "model_size", po::value<int>()->default_value(640), "Model Size");
    config_options.add_options()("actual_count",
                                 po::value<int>()->default_value(239),
                                 "Actual Car Count");

    // TODO: Switch to dynamic ROI Creation
    config_options.add_options()(
        "roi_A", po::value<Roi>()->default_value(Roi{640, 100, 0, 0}),
        "Width, Height and Coordinates of Region of Interest A");
    config_options.add_options()(
        "roi_B", po::value<Roi>()->default_value(Roi{640, 100, 0, 350}),
        "Width, Height and Coordinates of Region of Interest B");

    po::options_description cmdline_options;
    po::variables_map vm;

    cmdline_options.add(general).add(config_options);

    po::store(po::parse_command_line(argc, argv, cmdline_options), vm);
    try {
        po::notify(vm);
    } catch (std::exception& e) {
        std::cerr << "Error :" << e.what() << std::endl;
        exit(-1);
    }

    if (vm.count("help")) {
        std::cout << cmdline_options << "\n";
        exit(1);
    }

    return vm;
}
