This course took place in the first semester of Grade 1.
The textbook is _Handbook of image & video processing Second Edition_ by Alan C. Bovik.

One of the assignments is implementing Fourier transformation.
I wrote it in C++. I found that the C++ ecosystem is too chaotic.
For instance, both STD and OpenCV libraries contain complex number functions.
I think that I should use Rust rather than C++.
Above all, [opencv-rust crate](https://crates.io/crates/opencv/0.67.0) chooses to depend on [num-traits crate](https://crates.io/crates/num-traits/0.2.15) rather than implement complex functions again.

The original image came from [the Chinese wiki of _A Certain Magical Index_](https://toaru.huijiwiki.com/wiki/%E4%BA%94%E5%92%8C).

The final project is reading and representing a CV project about a specified topic.
Our group chose to read [this project](https://github.com/EmptySamurai/pytorch-reconet.git).
I made the presentation file in LaTeX, which I lost now.
