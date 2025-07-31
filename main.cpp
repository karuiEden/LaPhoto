#include <opencv2/opencv.hpp>
#include <vector>

#include "algorithm.h"
#include "convert.h"
using namespace std;


int main(const int32_t argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <image_path>" << "<output_path>" << std::endl;
        return -1;
    }
    const string image_path = argv[1];
    const string output_path = argv[2];
    cv::Mat image = cv::imread(image_path, cv::IMREAD_UNCHANGED); // Загрузка с альфой, если есть

    if (image.empty()) {
        std::cerr << "Could not open or find the image." << std::endl;
        return -1;
    }

    // Убедимся, что изображение в 4-канальном BGRA формате (CV_8UC4)
    if (image.channels() == 3) { // Если RGB, добавить альфу
        cv::cvtColor(image, image, cv::COLOR_BGR2BGRA);
    } else if (image.channels() == 1) { // Если Grayscale, конвертировать в BGRA
        cv::cvtColor(image, image, cv::COLOR_GRAY2BGRA);
    }
    // Если изображение уже BGRA (или RGBA) с 8 бит на канал, то все хорошо.
    // Если оно в другом формате (например, RGBA), нужно конвертировать.
    if (image.channels() == 4 && image.type() != CV_8UC4) {
        // Конвертация в CV_8UC4, если нужно (например, если было CV_16UC4)
        image.convertTo(image, CV_8UC4);
    }

    const int width = image.cols;
    const int height = image.rows;

    const auto src_pixels_argb_opt = mat_to_argb_vector(image);
    if (!src_pixels_argb_opt) {
        std::cerr << "Could not convert image to ARGB." << std::endl;
        return -1;
    }
    const auto& src_pixels_argb = src_pixels_argb_opt.value();
    std::vector<int32_t> dst_pixels_argb(width * height);

    // Пример параметров (замените на ваши реальные)
    const int32_t thr = 78;
    const int32_t innothr = 50;
    const std::vector rules = {30,110,30}; // Ваши правила
    const std::vector srcChan = {2, 1, 0}; // R, G, B
    const bool msbFromSrc = true;

    // Вызов вашей функции process
    process(src_pixels_argb, dst_pixels_argb, width, height, thr, innothr, rules, srcChan, msbFromSrc);

    // Конвертация обратно в cv::Mat для сохранения
    cv::Mat output_image = argb_vector_to_mat(dst_pixels_argb, width, height);
    cv::imwrite(output_path, output_image); // Сохранение в PNG (поддерживает альфу)
    image.release();
    output_image.release();
    return 0;
}
