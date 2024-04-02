#include "plot_drawing.h"

// Function to plot array data and save it as a .png image using GNU plot.
void plot_array(const float* arr, int size, const std::string &output_dir) {

    Gnuplot gp;
    std::string filepath = output_dir + "/annealing_plot.png";
    std::vector<std::pair<float, float>> data;
    for (size_t i = 0; i < size; ++i) {
        data.push_back(std::make_pair(static_cast<float>(i*PLOT_DIVISOR), arr[i]));
    }

    gp << "set terminal png\n"; // Set terminal type to PNG
    gp << "set output '" << filepath << "'\n"; // Set output file path
    gp << "set title 'Annealing procces:'\n";
    gp << "set xlabel 'Iteration'\n";
    gp << "set ylabel 'Energy'\n";
    gp << "plot '-' with lines title 'Function'\n";

    // Send data to Gnuplot for plotting
    gp.send(data);

    // Wait for the plot to finish rendering
    gp.flush();

    std::cout << "Plot saved as annealing_plot.png in " << output_dir << std::endl;
}



