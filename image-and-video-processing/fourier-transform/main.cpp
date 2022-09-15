#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
#include "utils.h"



auto cv_to_boost(const cv::Mat &mat) {
    auto array = boost::multi_array<double, 2>(boost::extents[mat.size[0]][mat.size[1]]);
    for (int i = 0; i < mat.size[0]; ++i) {
        for (int j = 0; j < mat.size[1]; ++j) {
            array[i][j] = mat.at<uchar>(i, j);
        }
    }
    return array;
}

auto boost_to_cv(const boost::multi_array<double, 2> &array) {
    auto mat = cv::Mat(static_cast<int>(array.shape()[0]), static_cast<int>(array.shape()[1]), CV_8U);
    for (int i = 0; i < array.shape()[0]; ++i) {
        for (int j = 0; j < array.shape()[1]; ++j) {
            auto pixel = static_cast<int>(array[i][j]);
            mat.at<uchar>(i, j) = (pixel > 255 ? 255 : pixel);
        }
    }
    return mat;
}

int main() {
    cv::Mat img = cv::imread("origin.jpg", cv::IMREAD_GRAYSCALE);

    auto img_c = cv_to_boost(img);
    auto img_dft_c = dsft(img_c);

    auto img_dft_show_c = show(img_dft_c);
    auto img_dft_show = boost_to_cv(img_dft_show_c);
    cv::imwrite("dsft.jpg", img_dft_show);

    int option;
    while (true) {
        std::cout << "Choose a filter:\n"
                     "1. ideal low-pass filter\n"
                     "2. Butterworth low-pass filter\n"
                     "3. Gaussian low-pass filter\n"
                     "4. ideal high-pass filter\n"
                     "5. Butterworth high-pass filter\n"
                     "6. Gaussian high-pass filter"
                  << std::endl;
        std::cin >> option;
        if (option >= 1 && option <= 6)
            break;
    }
    double D_0;
    std::cout << "Size of the image is " << img_c.shape()[0] << " x " << img_c.shape()[1] << "\n"
                                                                                             "Input a D_0:"
              << std::endl;
    std::cin >> D_0;

    int n;
    if (option == 2 || option == 5) {
        std::cin >> n;
    }

    std::function<double(const int &, const int &)> D = [](const int &u, const int &v) -> double {
        return sqrt(u * u + v * v);
    };
    std::function<double(const int &, const int &)> H;
    switch (option) {
        case 1:
            H = ilpf(D_0, D);
            break;
        case 2:
            H = blpf(D_0, D, n);
            break;
        case 3:
            H = glpf(D_0, D);
            break;
        case 4:
            H = ihpf(D_0, D);
            break;
        case 5:
            H = bhpf(D_0, D, n);
            break;
        case 6:
            H = ghpf(D_0, D);
            break;
        default:
            break;
    }
    for (int i = 0; i < img_dft_c.shape()[0]; ++i) {
        for (int j = 0; j < img_dft_c.shape()[1]; ++j) {
            auto u = i - static_cast<int>(img_dft_c.shape()[0] / 2);
            auto v = j - static_cast<int>(img_dft_c.shape()[1] / 2);
            img_dft_c[i][j] *= H(u, v);
        }
    }

    auto img_dft_show_c_new = show(img_dft_c);
    auto img_dft_show_new = boost_to_cv(img_dft_show_c_new);
    cv::imwrite("dsft-new.jpg", img_dft_show_new);

    auto img_out_c = idsft(img_dft_c);
    auto img_out = boost_to_cv(img_out_c);
    cv::imwrite("idsft.jpg", img_out);
    return 0;
}
