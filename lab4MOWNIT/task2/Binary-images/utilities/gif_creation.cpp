#include "gif_creation.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void delete_directory(const std::string& path) {
    std::string command = "rm -r " + path; // -r option for recursive deletion
    int result = std::system(command.c_str());
    if (result == 0) {
        std::cout << "Directory " << path << " deleted successfully." << std::endl;
    } else {
        std::cerr << "Error: Failed to delete directory " << path << std::endl;
    }
}

int create_gif(int num_frames, const std::string& input_dir, const std::string& output_dir) {

    // Combine images into a GIF using ImageMagick
    std::string command = "convert -delay 1 -loop 0";

    // Add input frame paths to the command
    for (int i = 1; i <= num_frames; ++i) {
        command += " " + input_dir + "/frame" + std::to_string(i) + ".png";
    }

    // Add output path to the command
    command += " " + output_dir + "/simulation.gif";

    int result = system(command.c_str());

    //Error check
    if (result != 0) {
        std::cerr << "Error: Failed to execute command." << std::endl;
        return 1;
    }

    std::cout << "GIF created successfully." << std::endl;

    // delete frames directory.
    delete_directory(input_dir);

    return 0;
}

int create_frames(const std::vector<unsigned char*>& frames, const std::string &output_dir){

    std::string general_filename =  output_dir+ "/frame";
    std::string specific_filename;

    // Create the output directory if it doesn't exist
    std::filesystem::create_directory(output_dir);

    // Create PNG of every file.
    int i=1;
    for(unsigned char* frame: frames) {
        specific_filename = general_filename + std::to_string(i) + ".png";
        if (!stbi_write_png(specific_filename.c_str(), IMAGE_SIZE, IMAGE_SIZE, 1, frame, IMAGE_SIZE)) {
            std::cerr << "Error: Failed to write frame "<< i << " file." << std::endl;
            return 1;
        }
        i++;
    }

    std::cout << "frames files saved successfully." << std::endl;
    return 0;
}

int result_png(const unsigned char* start, const unsigned char *result, std::string &output_dir){
    std::string start_filename =  output_dir + "/first_iter.png";
    std::string result_filename = output_dir + "/final_result.png";

    // Create the output directory if it doesn't exist
    std::filesystem::create_directory(output_dir);

    // Create the image of starting position.
    if (!stbi_write_png(start_filename.c_str(), IMAGE_SIZE, IMAGE_SIZE, 1, start, IMAGE_SIZE)) {
        std::cerr << "Error: Failed to write start PNG file." << std::endl;
        return 1;
    }

    // Create the image of result.
    if (!stbi_write_png(result_filename.c_str(), IMAGE_SIZE, IMAGE_SIZE, 1, result, IMAGE_SIZE)) {
        std::cerr << "Error: Failed to write result PNG file." << std::endl;
        return 1;
    }

    std::cout << "start/result files saved successfully." << std::endl;
    return 0;

}
