//
// Created by lightdress on 12/20/21.
//

#include <memory>
#include <boost/multi_array.hpp>

#ifndef FOURIER_TRANSFORM_UTILS_H
#define FOURIER_TRANSFORM_UTILS_H

#endif //FOURIER_TRANSFORM_UTILS_H

template<typename T>
std::function<T(const int &, const int &)> ilpf(const T &D_0, std::function<T(const int &, const int &)> D) {
    return [D_0, D](const int &u, const int &v) -> T {
        return (D(u, v) > D_0) ? 0 : 1;
    };
}

template<typename T>
std::function<T(const int &, const int &)>
blpf(const T &D_0, std::function<T(const int &, const int &)> D, const int &n) {
    return [D_0, D, n](const int &u, const int &v) -> T {
        return 1 / (1 + std::pow(D(u, v) / D_0, 2 * n));
    };
}

template<typename T>
std::function<T(const int &, const int &)> glpf(const T &D_0, std::function<T(const int &, const int &)> D) {
    return [D_0, D](const int &u, const int &v) -> T {
        return std::exp(-D(u, v) * D(u, v) / 2 / D_0 / D_0);
    };
}

template<typename T>
std::function<T(const int &, const int &)> ihpf(const T &D_0, std::function<T(const int &, const int &)> D) {
    return [D_0, D](const int &u, const int &v) -> T {
        return (D(u, v) > D_0) ? 1 : 0;
    };
}

template<typename T>
std::function<T(const int &, const int &)>
bhpf(const T &D_0, std::function<T(const int &, const int &)> D, const int &n) {
    return [D_0, D, n](const int &u, const int &v) -> T {
        return 1 / (1 + std::pow(D_0 / D(u, v), 2 * n));
    };
}

template<typename T>
std::function<T(const int &, const int &)> ghpf(const T &D_0, std::function<T(const int &, const int &)> D) {
    return [D_0, D](const int &u, const int &v) -> T {
        return 1 - std::exp(-D(u, v) * D(u, v) / 2 / D_0 / D_0);
    };
}

template<typename T>
class SymmetricMatrix {
public:
    explicit SymmetricMatrix(std::size_t n);

    T &at(std::size_t i, std::size_t j);

private:
    std::unique_ptr<T[]> data{};
};

boost::multi_array<std::complex<double>, 2> dsft(const boost::multi_array<double, 2> &img);

boost::multi_array<double, 2> show(const boost::multi_array<std::complex<double>, 2> &img);

boost::multi_array<double, 2> idsft(const boost::multi_array<std::complex<double>, 2> &img_shift);


