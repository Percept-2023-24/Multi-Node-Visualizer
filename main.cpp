#include <iostream>
#include <opencv2/opencv.hpp>
#include <QApplication>
#include <QWidget>
#include <QPushButton>

using namespace cv;
using namespace std;
using namespace std::chrono;

class Visualizer {
    // Variables
    int screen_width = 2000;
    int screen_height = 1100;
    int position_x = 1600;  // x-coordinate of the line
    int thickness = 2;  // width of the line
    int rec_width = 200;
    int rec_height = 75;
    Scalar white_color = Scalar(255, 255, 255);  // white color
    int node_height = 100;
    int node_width = 100;
    Scalar black_color = Scalar(0, 0, 0);  // black color
    int font = cv::FONT_HERSHEY_SIMPLEX;
    int numTicks = 11;
    int tickLength = 10;

    int frame;

public:
    // create opening menu
    void openingScreen(){
        
    }

    // read json file with frame data


    // Trianglulation function
    tuple<pair<int, int>, pair<int, int>> triangulation(
            tuple<double, double, double> circle1,
            tuple<double, double, double> circle2) {

        // Unpack circle parameters
        double x1, y1, r1;
        double x2, y2, r2;
        tie(x1, y1, r1) = circle1;
        tie(x2, y2, r2) = circle2;

        // Calculate distance between circle centers
        double d = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

        // Check if circles intersect or touch
        if (abs(d - (r1 + r2)) < 1 || abs(d - std::abs(r1 - r2)) < 1)  {
            // Circles are touching, find point of tangency
            double a = r1 / (r1 + r2);
            double x0 = x1 + a * (x2 - x1);
            double y0 = y1 + a * (y2 - y1);
            pair<int, int> intersection = make_pair(static_cast<int>(x0), static_cast<int>(y0));
            return make_tuple(intersection, make_pair(0, 0));
        } else if (d < r1 + r2) {
            // Circles intersect, find intersection points
            double a = (r1 * r1 - r2 * r2 + d * d) / (2 * d);
            double h = sqrt(r1 * r1 - a * a);
            double x0 = x1 + a * (x2 - x1) / d;
            double y0 = y1 + a * (y2 - y1) / d;
            pair<int, int> intersection1 = make_pair(
                    static_cast<int>(x0 + h * (y2 - y1) / d),
                    static_cast<int>(y0 - h * (x2 - x1) / d)
            );
            pair<int, int> intersection2 = make_pair(
                    static_cast<int>(x0 - h * (y2 - y1) / d),
                    static_cast<int>(y0 + h * (x2 - x1) / d)
            );
            return make_tuple(intersection1, intersection2);
        } else {
            // Circles do not intersect or touch
            return make_tuple(make_pair(0, 0), make_pair(0, 0));
        }
    }

    // create UI
    void UIscreen(){
        // Create a blank image
        Mat img = cv::Mat::zeros(screen_height, screen_width, CV_8UC3);

        // Draw rectangle to separate screen
        rectangle(img, Point(0, 0), cv::Point(position_x, screen_height), white_color, -1);

        // Text and data box for "Node 1 Range"
        rectangle(img, Point(1700, 150), Point(1700 + rec_width, 150 + rec_height), white_color, -1);
        putText(img, "Node 1 Range", Point(1700, 140), font, 0.9, white_color, 4, cv::LINE_AA);

        // Text and data box for "Node 1 Angle"
        rectangle(img, Point(1700, 425), Point(1700 + rec_width, 425 + rec_height), white_color, -1);
        putText(img, "Node 1 Angle", cv::Point(1700, 415), font, 0.9, white_color, 4, cv::LINE_AA);

        // Text and data box for "Node 2 Range"
        rectangle(img, Point(1700, 675), Point(1700 + rec_width, 675 + rec_height), white_color, -1);
        putText(img, "Node 2 Range", cv::Point(1700, 665), font, 0.9, white_color, 4, cv::LINE_AA);

        // Text and data box for "Node 2 Angle"
        rectangle(img, Point(1700, 950), Point(1700 + rec_width, 950 + rec_height), white_color, -1);
        putText(img, "Node 2 Angle", Point(1700, 940), font, 0.9, white_color, 4, cv::LINE_AA);

        // Boxes representing the nodes
        // Node 1
        rectangle(img, Point(50, 450), Point(50 + node_height, 450 + node_width), black_color, -1);
        putText(img, "1", Point(90, 510), font, 1, white_color, 4, LINE_AA);

        // Node 2
        rectangle(img, Point(750, 950), Point(750 + node_height, 950 + node_width), black_color, -1);
        putText(img, "2", Point(790, 1010), font, 1, white_color, 4, LINE_AA);

        // rectangle for map
        Point topLeft(150, 50);
        Point bottomRight(1450, 950);
        rectangle(img, topLeft, bottomRight, black_color, thickness);

        // add tick marks around map
        // Calculate the intervals between tick marks
        int tickSpacingX = (bottomRight.x - topLeft.x) / (numTicks + 1);
        int tickSpacingY = (bottomRight.y - topLeft.y) / (numTicks + 1);
        // tick marks: top and bottom
        for (int i = 1; i <= numTicks; ++i) {
            int x = topLeft.x + i * tickSpacingX;
            // Top side
            line(img, Point(x, topLeft.y), Point(x, topLeft.y - tickLength), black_color, thickness);
            // Bottom side
            line(img, Point(x, bottomRight.y), Point(x, bottomRight.y + tickLength), black_color, thickness);
            // Vertical grid lines
            line(img, Point(x, topLeft.y), Point(x, bottomRight.y), Scalar(128,128,128), thickness);
        }

        // Draw tick marks along the left and right sides
        for (int i = 1; i <= numTicks; ++i) {
            int y = topLeft.y + i * tickSpacingY;
            // Left side
            line(img, Point(topLeft.x, y), Point(topLeft.x - tickLength, y), black_color,thickness);
            // Right side
            line(img, Point(bottomRight.x, y), Point(bottomRight.x + tickLength, y), black_color, thickness);
            // Horizontal grid lines
            cv::line(img, Point(topLeft.x, y), Point(bottomRight.x, y), Scalar(128,128,128), thickness);
        }

        // label the scale
        putText(img, "Distance", Point(750, 30), font, 0.9, black_color, 4, LINE_AA);

        // legend
        putText(img, "1m intervals", Point(25, 1050), font, 0.8, black_color, 4, LINE_AA);

        // create range circles for each node
        Scalar red_color = Scalar(0, 0, 255);
        tuple<double, double, double> circle1 = make_tuple(100, 500, 400);  // last number is range (get from json file)
        tuple<double, double, double> circle2 = make_tuple(800, 1000, 460);
        // Find the intersections
        auto intersections = triangulation(circle1, circle2);
        auto intersection1 = get<0>(intersections);
        auto intersection2 = get<1>(intersections);
        // Draw the circles
        circle(img, Point(get<0>(circle1), get<1>(circle1)), get<2>(circle1), red_color, thickness);
        circle(img, Point(get<0>(circle2), get<1>(circle2)), get<2>(circle2), red_color, thickness);

        // Draw the intersection points if they exist
        if (!(intersection1.first == 0 && intersection1.second == 0 && intersection2.first == 0 && intersection2.second == 0)) {
            circle(img,Point(intersection1.first, intersection1.second), 10, Scalar(0, 255, 0), -1);
            if (intersection2.first != 0 || intersection2.second != 0) {
                circle(img, Point(intersection2.first, intersection2.second), 10, Scalar(0, 255, 0), -1);
            }
        }

        // Create window and display the image
        namedWindow("Multi-node UI", cv::WINDOW_NORMAL);
        imshow("Multi-node UI", img);

        // Wait indefinitely until a key is pressed
        waitKey(0);
    }

};
int main() {
    // create window
    Visualizer window;
    window.UIscreen();
    return 0;
}
