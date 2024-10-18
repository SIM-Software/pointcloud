#include <iostream>
#include <opencv2/opencv.hpp>
#include <bitset>
#include <cmath>
#include <chrono>

// Picture MetaData
const int PICTURE_WIDTH = 1920;
const int PICTURE_HEIGHT = 1080;
const float CAMERA_WIDTH = 0.036f;
const float CAMERA_HEIGHT = 0.024f;
const float FOCAL_LENGTH = 0.050f;

// Pre-calculated constants for optimization
const double INV_PICTURE_WIDTH = CAMERA_WIDTH / PICTURE_WIDTH;
const double INV_PICTURE_HEIGHT = CAMERA_HEIGHT / PICTURE_HEIGHT;
const double DEG_TO_RAD = M_PI / 180.0;
const double THETA_OFFSET = 20.0;
const double THETA_SCALING = 52.0 / 63.0;


/**
 * Function which creates a vector of all images used.
 */
std::vector<cv::Mat> generate_images() {
    std::vector<cv::Mat> images;
    std::vector<std::string> imagePaths = {
        "photos/MaskImage0.png", "photos/MaskImage1.png", "photos/MaskImage2.png",
        "photos/MaskImage3.png", "photos/MaskImage4.png", "photos/MaskImage5.png", 
        "photos/MaskImage6.png"
    };

    for (const auto& path : imagePaths) {
        cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);
        if (image.empty()) {
            std::cerr << "Error: Could not open or find the image: " << path << std::endl;
            continue;
        }
        images.push_back(image);
    }

    if (images.empty()) {
        std::cerr << "Error: No images loaded. Exiting." << std::endl;
        exit(EXIT_FAILURE);
    }

    return images;
}


/**
 * Function which creates a vector of points for each white pixel which needs to be analyzed.
 */
std::vector<cv::Point> generate_whitePixelCoordinates(const std::vector<cv::Mat>& images) {
    std::vector<cv::Point> whitePixelCoordinates;

    const cv::Mat& firstImage = images[0];
    for (int y = 0; y < firstImage.rows; ++y) {
        for (int x = 0; x < firstImage.cols; ++x) {
            cv::Vec3b pixel = firstImage.at<cv::Vec3b>(y, x);
            if (pixel == cv::Vec3b(255, 255, 255)) { // Efficient pixel comparison
                whitePixelCoordinates.push_back(cv::Point(x, y));
            }
        }
    }

    return whitePixelCoordinates;
}


/**
 * Function which generates each point to be added to a PointCloud
 */
std::tuple<double, double, double> generate_points(const std::vector<cv::Mat>& images, const cv::Point& point) {
    std::string binaryCode;

    for (size_t j = 1; j < images.size(); ++j) {
        cv::Vec3b pixel = images[j].at<cv::Vec3b>(point.y, point.x);
        binaryCode += (pixel == cv::Vec3b(255, 255, 255)) ? "1" : "0";
    }

    std::bitset<6> bits(binaryCode);
    unsigned long integerValue = bits.to_ulong();

    // Pre-calculate x, y, and z
    double x = (point.x - PICTURE_WIDTH / 2.0) * INV_PICTURE_WIDTH;
    double y = (PICTURE_HEIGHT / 2.0 - point.y) * INV_PICTURE_HEIGHT;
    double z = -FOCAL_LENGTH;

    double theta = THETA_OFFSET + THETA_SCALING * (integerValue - 32);
    theta *= DEG_TO_RAD;

    double a = -cos(theta);
    double c = sin(theta);
    double d = 20 * cos(theta);

    double t = -d / (a * x + c * z);
    x *= t;
    y *= t;
    z *= t;

    return std::make_tuple(x, y, z);
}


int main() {
    std::vector<cv::Mat> images = generate_images();
    std::vector<cv::Point> whitePixelCoordinates = generate_whitePixelCoordinates(images);

    auto start = std::chrono::high_resolution_clock::now();
    int counter = 0;

    while (counter < 1000) { 
        std::vector<std::tuple<double, double, double>> points;
        points.reserve(whitePixelCoordinates.size());

        for (size_t i = 0; i < whitePixelCoordinates.size(); ++i) {
            cv::Point point = whitePixelCoordinates[i];
            points.push_back(generate_points(images, point));
        }

        auto end = std::chrono::high_resolution_clock::now();
        ++counter;

        std::chrono::duration<double> duration = end - start;
        std::cout << "Iterations per second: " << counter / duration.count() << std::endl;
    }

    return 0;
}
