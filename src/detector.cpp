#include "detector.h"

Detector::Detector(Config &config)
{
    this->nmsThreshold = config.nmsThreshold;
    this->confThreshold = config.confThreshold;
    ifstream ifs(config.classNamePath);
    string line;
    while (getline(ifs, line))
        this->classNames.push_back(line);
    ifs.close();
    this->model = cv::dnn::readNetFromONNX(config.weightPath);
    this->model.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    this->model.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    this->inSize = config.size;
    this->_auto = config._auto;
}

PadInfo Detector::letterbox(Mat &img, Size new_shape, Scalar color, bool _auto, bool scaleFill, bool scaleup, int stride)
{
    float width = img.cols;
    float height = img.rows;
    float r = min(new_shape.width / width, new_shape.height / height);
    if (!scaleup)
        r = min(r, 1.0f);
    int new_unpadW = int(round(width * r));
    int new_unpadH = int(round(height * r));
    int dw = new_shape.width - new_unpadW;
    int dh = new_shape.height - new_unpadH;
    if (_auto)
    {
        dw %= stride;
        dh %= stride;
    }
    dw /= 2, dh /= 2;
    Mat dst;
    resize(img, img, Size(new_unpadW, new_unpadH), 0, 0, cv::INTER_LINEAR);
    int top = int(round(dh - 0.1));
    int bottom = int(round(dh + 0.1));
    int left = int(round(dw - 0.1));
    int right = int(round(dw + 0.1));
    copyMakeBorder(img, img, top, bottom, left, right, cv::BORDER_CONSTANT, color);
    return {r, top, left, bottom};
}

Detection Detector::detect(Mat &img)
{
    // Inference Time
    cv::TickMeter timeRecorder;

    Mat im;
    img.copyTo(im);
    PadInfo padInfo = letterbox(im, this->inSize, Scalar(114, 114, 114), this->_auto, false, true, 32);
    Mat blob;
    cv::dnn::blobFromImage(im, blob, 1 / 255.0f, Size(im.cols, im.rows), Scalar(0, 0, 0), true, false);
    std::vector<string> outLayerNames = this->model.getUnconnectedOutLayersNames();
    std::vector<Mat> outs;
    this->model.setInput(blob);
    timeRecorder.start();
    this->model.forward(outs, outLayerNames);
    timeRecorder.stop();

    float t = timeRecorder.getTimeMilli();

    return {padInfo, outs, t};
}
std::vector<Rect> Detector::postProcess(Mat &img, Detection &detection, Colors &cl)
{

    letterbox(img, this->inSize, Scalar(114, 114, 114), this->_auto, false, true, 32);
    std::vector<Mat> outs = detection.detection;
    Mat out(outs[0].size[1], outs[0].size[2], CV_32F, outs[0].ptr<float>());
    std::vector<Rect> boxes;
    std::vector<Rect> boxesOut;
    std::vector<float> scores;
    std::vector<int> indices;
    std::vector<int> classIndexList;
    for (int r = 0; r < out.rows; r++)
    {
        float cx = out.at<float>(r, 0);
        float cy = out.at<float>(r, 1);
        float w = out.at<float>(r, 2);
        float h = out.at<float>(r, 3);
        float sc = out.at<float>(r, 4);
        Mat confs = out.row(r).colRange(5, 85);
        confs *= sc;
        double minV, maxV;
        cv::Point minI, maxI;
        // if(Rect(cx - w / 2, cy - h / 2, w, h).area() > boxMaxArea)
        // {
        //     if(Rect(cx - w / 2, cy - h / 2, w, h).area() > boxMaxArea + 400000)
        //     {
        //         std::cout << "Area : " << to_string(Rect(cx - w / 2, cy - h / 2, w, h).area()) << std::endl;
        //     }
        //     continue;
        // }
        boxes.push_back(Rect(cx - w / 2, cy - h / 2, w, h));
        minMaxLoc(confs, &minV, &maxV, &minI, &maxI);
        scores.push_back(maxV);
        indices.push_back(r);
        classIndexList.push_back(maxI.x);
    }

    cv::dnn::NMSBoxes(boxes, scores, this->confThreshold, this->nmsThreshold, indices);
    std::vector<int> clsIndexs;
    for (long unsigned int i = 0; i < indices.size(); i++)
    {
        clsIndexs.push_back(classIndexList[indices[i]]);
        boxesOut.push_back(boxes[indices[i]]);
    }
    drawPrediction(img, boxes, scores, clsIndexs, indices, cl);
    std::string label = cv::format("Inference time : %.2f ms", detection.inference);
    putText(img, label, cv::Point(0, 15), cv::FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);

    return boxesOut;
}

void Detector::drawPrediction(Mat &img, std::vector<Rect> &boxes, std::vector<float> &scores, std::vector<int> &clsIndexs, std::vector<int> &ind, Colors &cl)
{
    for (long unsigned int i = 0; i < ind.size(); i++)
    {
        Rect rect = boxes[ind[i]];
        float score = scores[ind[i]];
        string name = this->classNames[clsIndexs[i]];
        int color_ind = clsIndexs[i]%20;
        Scalar color = cl.palette[color_ind];
        rectangle(img, rect, color);
        char s_text[80];
        sprintf(s_text, "%.2f", round(score * 1e3) / 1e3);
        string label = name + " " + s_text;

        int baseLine = 0;
        Size textSize = getTextSize(label, cv::FONT_HERSHEY_PLAIN, 0.7, 1, &baseLine);
        baseLine += 2;
        rectangle(img, Rect(rect.x, rect.y - textSize.height, textSize.width + 1, textSize.height + 1), color, -1);
        putText(img, label, cv::Point(rect.x, rect.y), cv::FONT_HERSHEY_PLAIN, 0.7, Scalar(255, 255, 255), 1);
    }
}

vector<Rect> Detector::makeBoxes(Mat &img, Detection &detection)
{
    letterbox(img, this->inSize, Scalar(114, 114, 114), this->_auto, false, true, 32);
    vector<Rect> boxesOut;
    std::vector<Mat> outs = detection.detection;
    Mat out(outs[0].size[1], outs[0].size[2], CV_32F, outs[0].ptr<float>());
    std::vector<Rect> boxes;
    std::vector<float> scores;
    std::vector<int> indices;
    std::vector<int> classIndexList;
    for (int r = 0; r < out.rows; r++)
    {
        float cx = out.at<float>(r, 0);
        float cy = out.at<float>(r, 1);
        float w = out.at<float>(r, 2);
        float h = out.at<float>(r, 3);
        float sc = out.at<float>(r, 4);
        Mat confs = out.row(r).colRange(5, 85);
        confs *= sc;
        double minV, maxV;
        cv::Point minI, maxI;
        minMaxLoc(confs, &minV, &maxV, &minI, &maxI);
        scores.push_back(maxV);
        boxes.push_back(Rect(cx - w / 2, cy - h / 2, w, h));
        indices.push_back(r);
    }

    cv::dnn::NMSBoxes(boxes, scores, this->confThreshold, this->nmsThreshold, indices);
    for (long unsigned int i = 0; i < indices.size(); i++)
    {
        boxesOut.push_back(boxes[indices[i]]);
    }
    return boxesOut;
}
