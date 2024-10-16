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

public:
    Texture(const std::string& name)
    {
        image_data = cv::imread(name);
        cv::cvtColor(image_data, image_data, cv::COLOR_RGB2BGR);
        width = image_data.cols;
        height = image_data.rows;
    }

    int width, height;

    float Lerp(float origin, float vMin, float vMax)
    {
        return vMin + origin * (vMax - vMin);
    }

    Eigen::Vector3f getColor(float u, float v)
    {
        u = std::clamp(u, 0.0f, 1.0f);
        v = std::clamp(v, 0.0f, 1.0f);
        auto u_img = u * (width - 1);
        auto v_img = (1 - v) * (height - 1);

#ifdef BILINEAR
        int u1 = std::floor(u_img);
        int v1 = std::floor(v_img);
        int u2 = std::ceil(u_img);
        int v2 = std::ceil(v_img);
        float du = u_img - u1;
        float dv = v_img - v1;
        auto color1 = image_data.at<cv::Vec3b>(v1, u1);
        auto color2 = image_data.at<cv::Vec3b>(v1, u2);
        auto color3 = image_data.at<cv::Vec3b>(v2, u1);
        auto color4 = image_data.at<cv::Vec3b>(v2, u2);

        Eigen::Vector3f color;
        for (int i = 0; i < 3; i++)
        {
            color[i] = Lerp(dv, Lerp(du, color1[i], color2[i]), Lerp(du, color3[i], color4[i]));
        }
        return color;
#else
        auto color = image_data.at<cv::Vec3b>(v_img, u_img);
        return Eigen::Vector3f(color[0], color[1], color[2]);
#endif
    }
};
#endif //RASTERIZER_TEXTURE_H
