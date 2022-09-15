//
// Created by lightdress on 12/20/21.
//

#include <complex>
#include <vector>
#include <iostream>

#include "utils.h"

template<typename T>
T &SymmetricMatrix<T>::at(std::size_t i, std::size_t j) {
    if (i < j) return at(j, i);
    return data[i * (i + 1) / 2 + j];
}

template<typename T>
SymmetricMatrix<T>::SymmetricMatrix(std::size_t n): data(new T[n * (n + 1) / 2]){}

auto minusJPiUxDividedByMTimesE(const std::size_t &M) {
    auto ans = SymmetricMatrix<std::complex<double>>(M);
    for (std::size_t x = 0; x < M; ++x) {
        for (std::size_t u = 0; u <= x; ++u) {
            ans.at(x,u)= std::exp<double>(-1j * 2 * acos(-1) * u * x / M);
        }
    }
    return ans;
}

template<typename T>
boost::multi_array<T, 2> shift_copy(const boost::multi_array<T, 2> &img) {
    boost::multi_array<T, 2> img_shift = img;
    auto half_shape_0 = img_shift.shape()[0] / 2;
    auto half_shape_1 = img_shift.shape()[1] / 2;
    for (int i = 0; i < half_shape_0; ++i) {
        for (int j = 0; j < half_shape_1; ++j) {
            std::swap(img_shift[i][j], img_shift[i + half_shape_0][j + half_shape_1]);
            std::swap(img_shift[i][j + half_shape_1], img_shift[i + half_shape_0][j]);
        }
    }
    return img_shift;
}

boost::multi_array<std::complex<double>, 2> dsft(const boost::multi_array<double, 2> &img) {
    std::cerr << "DSFT" << std::endl;
    auto img_dft = boost::multi_array<std::complex<double>, 2>(std::vector<int>{static_cast<int>(img.shape()[0]), static_cast<int>(img.shape()[1])});
    for (int i = 0; i < img.shape()[0]; ++i) {
        for (int j = 0; j < img.shape()[1]; ++j) {
            img_dft[i][j] = 0;
        }
    }

    auto first_exp = minusJPiUxDividedByMTimesE(img.shape()[0]);

    auto second_exp = minusJPiUxDividedByMTimesE(img.shape()[1]);

    for (int u = 0; u < img.shape()[0]; ++u) {
        for (int v = 0; v < img.shape()[1]; ++v) {
            for (int x = 0; x < img.shape()[0]; ++x) {
                for (int y = 0; y < img.shape()[1]; ++y) {
                    img_dft[u][v] += img[x][y] * first_exp.at(x, u) * second_exp.at(y, v);
                }
            }
            std::cerr << "Point (" << u << ", " << v << ") with dft " << img_dft[u][v] << std::endl;
        }
    }
    return shift_copy(img_dft);
}

boost::multi_array<double, 2> idsft(const boost::multi_array<std::complex<double>, 2> &img_dft) {
    std::cerr << "IDSFT" << std::endl;
    auto img_shift = shift_copy(img_dft);
    auto img = boost::multi_array<double, 2>(std::vector<int>{static_cast<int>(img_shift.shape()[0]), static_cast<int>(img_shift.shape()[1])});
    auto img_mn_times_idft = boost::multi_array<std::complex<double>, 2>(std::vector<int>{static_cast<int>(img_shift.shape()[0]), static_cast<int>(img_shift.shape()[1])});
    for (int i = 0; i < img_shift.shape()[0]; ++i) {
        for (int j = 0; j < img_shift.shape()[1]; ++j) {
            img_mn_times_idft[i][j] = 0;
        }
    }

    auto first_exp = minusJPiUxDividedByMTimesE(img_shift.shape()[0]);

    auto second_exp = minusJPiUxDividedByMTimesE(img_shift.shape()[1]);

    for (int x = 0; x < img_shift.shape()[0]; ++x) {
        for (int y = 0; y < img_shift.shape()[1]; ++y) {
            for (int u = 0; u < img_shift.shape()[0]; ++u) {
                for (int v = 0; v < img_shift.shape()[1]; ++v) {
                    img_mn_times_idft[x][y] += img_shift[u][v] / first_exp.at(u, x) / second_exp.at(v, y);
                }
            }
            img[x][y] = std::abs(img_mn_times_idft[x][y]) / static_cast<int>(img_shift.shape()[0] * img_shift.shape()[1]);
            std::cerr << "Point (" << x << ", " << y << ") with value " << img[x][y] << std::endl;
        }
    }
    return img;
}

boost::multi_array<double, 2> show(const boost::multi_array<std::complex<double>, 2> &img) {
    auto img_show = boost::multi_array<double, 2>(std::vector<int>{static_cast<int>(img.shape()[0]), static_cast<int>(img.shape()[1])});
    for (int i = 0; i < img.shape()[0]; ++i) {
        for (int j = 0; j < img.shape()[1]; ++j) {
            img_show[i][j] = 16 * log(1 + std::abs(img[i][j]));
        }
    }
    return img_show;
}



