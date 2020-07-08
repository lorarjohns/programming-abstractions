#include "pbPlots.hpp"
#include "supportLib.hpp"

int main(){

    vector<double> x{20000, 40000, 80000, 160000, 320000, 640000};
    vector<double> y{1.389, 5.287, 22.023, 86.095, 352.235, 1374.22};

    RGBABitmapImageReference *imageReference = CreateRGBABitmapImageReference();

    DrawScatterPlot(imageReference, 600, 400, &x, &y);

    vector<double> *pngData = ConvertToPNG(imageReference->image);
    WriteToFile(pngData, "plot.png");
    DeleteImage(imageReference->image);

    return 0;
 }
