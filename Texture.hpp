//
// Created by LEI XU on 4/27/19.
//

#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H
#include "global.hpp"
#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>
class Texture{
private:
    cv::Mat image_data;
    float lerp(float s, float e, float t) { return s + (e - s) * t; }
public:
    Texture(const std::string& name)
    {
        image_data = cv::imread(name);
        cv::cvtColor(image_data, image_data, cv::COLOR_RGB2BGR);
        width = image_data.cols;
        height = image_data.rows;
    }

    int width, height;

    Eigen::Vector3f getColor(float u, float v)
    {
        auto u_img = u * width;
        auto v_img = (1 - v) * height;
        auto color = image_data.at<cv::Vec3b>(v_img, u_img);
        return Eigen::Vector3f(color[0], color[1], color[2]);
    }
    Eigen::Vector3f getColorBilinear(float u, float v){
        v=1-v<0?0:1-v;
        auto u_img = u * width;
        auto v_img = v * height;
        int u_int = (int)u_img;
        int v_int = (int)v_img;
        Eigen::Vector2d p00(u_int, v_int),p01(u_int, v_int+1<=height?v_int+1:height),p10(u_int+1<=width?u_int+1:width, v_int),p11(u_int+1<=width?u_int+1:width, v_int+1<=height?v_int+1:height);
        float s=u_img-u_int;
        float t=v_img-v_int;
        auto color00 = image_data.at<cv::Vec3b>(p00.y(), p00.x());
        auto color01 = image_data.at<cv::Vec3b>(p01.y(), p01.x());
        auto color10 = image_data.at<cv::Vec3b>(p10.y(), p10.x());
        auto color11 = image_data.at<cv::Vec3b>(p11.y(), p11.x());
        Eigen::Vector3f color0(lerp(color00[0], color10[0], s), lerp(color00[1], color10[1], s), lerp(color00[2], color10[2], s));
        Eigen::Vector3f color1(lerp(color01[0], color11[0], s), lerp(color01[1], color11[1], s), lerp(color01[2], color11[2], s));
        return Eigen::Vector3f(lerp(color0[0], color1[0], t), lerp(color0[1], color1[1], t), lerp(color0[2], color1[2], t));
    }
};
#endif //RASTERIZER_TEXTURE_H
