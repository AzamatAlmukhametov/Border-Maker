#include "pch.h"

std::vector<coord>
borderCoords(1, coord()),
borderInCoords(1, coord()),
borderOutCoords(1, coord());

int main()
{
    bitmap_image image("colorIm.bmp"), borderMarkedImage, inflationFiltering;

    if (!image)
    {
        std::cout << "Failed to open file" << std::endl;
        return 0;
    }

    intoTwoColorIm(image);
    image.save_image("twoColorImage.bmp");

    std::cout << "inflationFiltering" << std::endl;
    inflationFiltering = smoothingEdges(image);
    inflationFiltering = fillGapsBetweenWalls(inflationFiltering);
    inflationFiltering.save_image("inflationFiltering1.bmp");

    std::cout << "borderMarking" << std::endl;
    borderMarkedImage = markBorders(inflationFiltering, black, white, red);
    borderMarkedImage.save_image("borderMarkedImage2.bmp");


    //test ccC
    std::cout << "contourConting" << std::endl;
    std::vector< std::vector<coord> > ccC = countContours(borderMarkedImage, borderCoords);

    centerOfTheContour(ccC);

    bitmap_image contoursMap, initMap("colorIm.bmp");

    contoursMap.setwidth_height(image.width(), image.height(), true);
    contoursMap.set_all_channels(255, 255, 255);

    for (int i = 0; i < ccC.size(); i++)
        for (int j = 0; j < ccC[i].size(); j++)
        {
            contoursMap.set_pixel(ccC[i][j].x, ccC[i][j].y, char((i + 1) * 255 / ccC.size()), 160, 160);
            initMap.set_pixel(ccC[i][j].x, ccC[i][j].y, char((i + 1) * 255 / ccC.size()), 160, 160);
        }

    contoursMap.save_image("contoursMap4.bmp");
    initMap.save_image("initMap_Plus_contoursMap5.bmp");
    std::cout << ccC.size() << std::endl;
    //end of the test ccC

    return 1;
}

