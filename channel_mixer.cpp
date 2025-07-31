//
// Created by karui on 7/28/2025.
//
#include <opencv2/opencv.hpp>
#include <vector>
#include <cstdint>
using namespace std;

optional<cv::Mat> channel_mixer(const cv::Mat& input, const vector<int32_t>& red_channel, const vector<int32_t>& green_channel, const vector<int32_t>& blue_channel, const vector<int32_t>& constants) {
    if (input.empty()) {
        return nullopt;
    }
    const uint32_t height = input.cols;
    const uint32_t weight = input.rows;


}