#include "detector.h"

Detector::Detector(Config &config) {
    this->nmsThreshold = config.nmsThreshold;
    this->confThreshold = config.confThreshold;
    ifstream ifs(config.classNamePath);
    string line;
    while (getline(ifs, line)) this->classNames.push_back(line);
    ifs.close();
    this->model = cv::dnn::readNetFromONNX(config.weightPath);
    this->model.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
    this->model.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
    this->inSize = config.size;
    this->_auto = config._auto;
    this->night = config.night;
}

PadInfo Detector::letterbox(Mat &img, Size new_shape, Scalar color, bool _auto,
                            bool scaleFill, bool scaleup, int stride) {
    float width = img.cols;
    float height = img.rows;
    float r = min(new_shape.width / width, new_shape.height / height);
    if (!scaleup) r = min(r, 1.0f);
    int new_unpadW = int(round(width * r));
    int new_unpadH = int(round(height * r));
    int dw = new_shape.width - new_unpadW;
    int dh = new_shape.height - new_unpadH;
    if (_auto) {
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
    copyMakeBorder(img, img, top, bottom, left, right, cv::BORDER_CONSTANT,
                   color);
    return {r, top, left, bottom};
}

Detection Detector::detect(Mat &img) {
    Mat im;
    img.copyTo(im);
    Detection detection;
    PadInfo padInfo = letterbox(im, this->inSize, Scalar(114, 114, 114),
                                this->_auto, false, true, 32);

    if (this->night == false) {
        Mat blob;
        cv::dnn::blobFromImage(im, blob, 1 / 255.0f, Size(im.cols, im.rows),
                               Scalar(0, 0, 0), true, false);
        std::vector<string> outLayerNames =
            this->model.getUnconnectedOutLayersNames();
        std::vector<Mat> outs;
        this->model.setInput(blob);
        this->model.forward(outs, outLayerNames);

        detection.info = padInfo;
        detection.detection = outs;

        return detection;
    } else {
        // Inference Time
        cv::TickMeter timeRecorder;
        // Call night mode detection
        // vector<Mat> outs;
        vector<Rect> rectangles;
        timeRecorder.start();
        preprocessNight(im);

        rectangles = detectNight(im);

        // imshow("Test 2", im);

        detection.info = padInfo;
        // detection.detection = outs;
        detection.boxes = rectangles;
        timeRecorder.stop();
        float t = timeRecorder.getTimeMilli();
        detection.inference = t;

        return detection;
    }
}

std::vector<Rect> Detector::postProcess(Mat &img, Detection &detection,
                                        Colors &cl) {
    letterbox(img, this->inSize, Scalar(114, 114, 114), this->_auto, false,
              true, 32);
    std::vector<Rect> boxesOut;
    if (this->night == false) {
        std::vector<Mat> outs = detection.detection;
        Mat out(outs[0].size[1], outs[0].size[2], CV_32F, outs[0].ptr<float>());
        std::vector<Rect> boxes;
        std::vector<float> scores;
        std::vector<int> indices;
        std::vector<int> classIndexList;
        for (int r = 0; r < out.rows; r++) {
            float cx = out.at<float>(r, 0);
            float cy = out.at<float>(r, 1);
            float w = out.at<float>(r, 2);
            float h = out.at<float>(r, 3);
            float sc = out.at<float>(r, 4);
            Mat confs = out.row(r).colRange(5, 85);
            confs *= sc;
            double minV, maxV;
            cv::Point minI, maxI;

            boxes.push_back(Rect(cx - w / 2, cy - h / 2, w, h));
            minMaxLoc(confs, &minV, &maxV, &minI, &maxI);
            scores.push_back(maxV);
            indices.push_back(r);
            classIndexList.push_back(maxI.x);
        }

        cv::dnn::NMSBoxes(boxes, scores, this->confThreshold,
                          this->nmsThreshold, indices);
        std::vector<int> clsIndexs;
        for (long unsigned int i = 0; i < indices.size(); i++) {
            clsIndexs.push_back(classIndexList[indices[i]]);
            boxesOut.push_back(boxes[indices[i]]);
        }
        // drawPrediction(img, boxes, scores, clsIndexs, indices, cl);
        std::vector<double> layersTimes;
        double freq = cv::getTickFrequency() / 1000;
        double t = this->model.getPerfProfile(layersTimes) / freq;

        detection.inference = t;
        // std::string label = cv::format("Inference time : %.2f ms",
        // detection.inference); putText(img, label, cv::Point(0, 15),
        // cv::FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);
    } else {
        boxesOut = detection.boxes;
        drawPredictionNight(img, boxesOut);
    }
    std::string label =
        cv::format("Inference time : %.2f ms", detection.inference);
    putText(img, label, cv::Point(0, 15), cv::FONT_HERSHEY_SIMPLEX, 0.5,
            Scalar(0, 255, 0), 1);

    return boxesOut;
}

void Detector::drawPrediction(Mat &img, std::vector<Rect> &boxes,
                              std::vector<float> &scores,
                              std::vector<int> &clsIndexs,
                              std::vector<int> &ind, Colors &cl) {
    for (long unsigned int i = 0; i < ind.size(); i++) {
        Rect rect = boxes[ind[i]];
        float score = scores[ind[i]];
        string name = this->classNames[clsIndexs[i]];
        int color_ind = clsIndexs[i] % 20;
        Scalar color = cl.palette[color_ind];
        rectangle(img, rect, color);
        char s_text[80];
        sprintf(s_text, "%.2f", round(score * 1e3) / 1e3);
        string label = name + " " + s_text;

        int baseLine = 0;
        Size textSize =
            getTextSize(label, cv::FONT_HERSHEY_PLAIN, 0.7, 1, &baseLine);
        baseLine += 2;
        rectangle(img,
                  Rect(rect.x, rect.y - textSize.height, textSize.width + 1,
                       textSize.height + 1),
                  color, -1);
        putText(img, label, cv::Point(rect.x, rect.y), cv::FONT_HERSHEY_PLAIN,
                0.7, Scalar(255, 255, 255), 1);
    }
}

void Detector::drawPredictionNight(Mat &img, std::vector<Rect> &boxes) {
    cv::putText(img, "Night Mode ON", cv::Point(img.rows - 200, 30),
                cv::FONT_HERSHEY_PLAIN, 1.4, Scalar(0, 255, 0), 2);
    for (size_t i = 0; i < boxes.size(); i++) {
        rectangle(img, boxes[i], Scalar(0, 255, 0), 1);
    }
}

vector<Rect> Detector::makeBoxes(Mat &img, Detection &detection) {
    letterbox(img, this->inSize, Scalar(114, 114, 114), this->_auto, false,
              true, 32);
    vector<Rect> boxesOut;
    std::vector<Mat> outs = detection.detection;
    Mat out(outs[0].size[1], outs[0].size[2], CV_32F, outs[0].ptr<float>());
    std::vector<Rect> boxes;
    std::vector<float> scores;
    std::vector<int> indices;
    std::vector<int> classIndexList;
    for (int r = 0; r < out.rows; r++) {
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

    cv::dnn::NMSBoxes(boxes, scores, this->confThreshold, this->nmsThreshold,
                      indices);
    for (long unsigned int i = 0; i < indices.size(); i++) {
        boxesOut.push_back(boxes[indices[i]]);
    }
    return boxesOut;
}

void Detector::setNight(bool night) { this->night = night; }

void Detector::preprocessNight(Mat &img) {
    GaussianBlur(img, img, Size(3, 3), 0);
    cvtColor(img, img, cv::COLOR_BGR2GRAY);
    Mat kernel = getStructuringElement(cv::MORPH_RECT, Size(5, 5));
    threshold(img, img, 200, 255, cv::THRESH_BINARY);
    erode(img, img, kernel);
    dilate(img, img, kernel);
}

vector<vector<cv::Point>> getHulls(Mat imgInput) {
    vector<vector<cv::Point>> contours;

    findContours(imgInput, contours, cv::RETR_EXTERNAL,
                 cv::CHAIN_APPROX_SIMPLE);

    vector<Rect> boundRect(contours.size());
    vector<vector<cv::Point>> hull(contours.size());

    for (size_t i = 0; i < contours.size(); i++) {
        convexHull(contours[i], hull[i]);
        // std::cout << i << std::endl;
        // for(size_t j = 0; j < hull[i].size(); j++){
        //     std::cout << hull[i][j] << std::endl;
        // }
    }

    return hull;
}

vector<cv::Rect> pairHeadlights(vector<vector<cv::Point>> hulls) {
    vector<cv::Rect> rectangles;
    vector<cv::Moments> mu;
    vector<vector<cv::Moments>> v_mu;

    for (size_t i = 0; i < hulls.size(); i++) {
        cv::Moments tmp_m = moments(hulls[i]);

        // Remove very large areas
        if (tmp_m.m00 > 300 || tmp_m.m00 < 100) {
            continue;
        }

        mu.push_back(tmp_m);
        // std::cout << "Index : " << i << std::endl;
        // std::cout << "Area : " << tmp_m.m00;
        // std::cout << "Centroid X : " << tmp_m.m10 / tmp_m.m00;
        // std::cout << "Centroid Y : " << tmp_m.m01 / tmp_m.m00 << std::endl;
    }

    // Make a copy of mu
    vector<cv::Moments> cpy_mu = mu;
    // Assign contours based on centroid and size
    while (cpy_mu.empty() == 0) {
        if (cpy_mu.size() == mu.size()) {
            vector<cv::Moments> tmp_mu;
            cv::Moments tmp_m_item = cpy_mu.back();
            tmp_mu.push_back(tmp_m_item);

            v_mu.push_back(tmp_mu);
            cpy_mu.pop_back();
        } else {
            cv::Moments tmp_m_item = cpy_mu.back();

            float cx = tmp_m_item.m10 / tmp_m_item.m00;
            float cy = tmp_m_item.m01 / tmp_m_item.m00;

            float area = tmp_m_item.m00;

            bool matched = 0;

            for (size_t i = 0; i < v_mu.size(); i++) {
                // if area difference less than 10
                // std::cout << "Area diff " << abs(area - v_mu[i][0].m00) <<
                // std::endl;
                if (abs(area - v_mu[i][0].m00) < 100) {
                    float stored_y = v_mu[i][0].m01 / v_mu[i][0].m00;
                    // if height difference less than 10
                    if (abs(cy - stored_y) < 10) {
                        // check distance
                        float stored_x = v_mu[i][0].m10 / v_mu[i][0].m00;

                        // if(abs(cx - stored_x) < 40)
                        // std::cout << "x dist between " << cx << " and " <<
                        // stored_x << " is " << abs(cx - stored_x) <<
                        // std::endl;
                        if ((abs(cx - stored_x) < 80) &&
                            (abs(cx - stored_x) > 20)) {
                            v_mu[i].push_back(tmp_m_item);

                            // set matched flag
                            matched = 1;
                        }
                    }
                }
            }

            if (matched == 0) {
                vector<cv::Moments> tmp_mu;
                cv::Moments tmp_m_item = cpy_mu.back();
                tmp_mu.push_back(tmp_m_item);

                v_mu.push_back(tmp_mu);
            }
            cpy_mu.pop_back();
        }

        // std::cout << "cpy_mu count : " << cpy_mu.size() << std::endl;
    }

    // vector<vector<cv::Point>> new_hulls;

    for (size_t i = 0; i < v_mu.size(); i++) {
        // std::cout << "Index i : " << i << std::endl;
        if (v_mu[i].size() < 2) {
            continue;
        }
        // for(size_t j = 0; j < v_mu[i].size(); j++)
        // {
        //     std::cout << "Index j : " << j;
        //     std::cout << "Area : " << v_mu[i][j].m00;
        //     std::cout << "Centroid X : " << v_mu[i][j].m10 / v_mu[i][j].m00;
        //     std::cout << "Centroid Y : " << v_mu[i][j].m01 / v_mu[i][j].m00
        //     << std::endl;
        // }
        // int width = 100;
        // int height = 50;
        int y1 = v_mu[i][0].m01 / v_mu[i][0].m00;
        int y2 = v_mu[i][1].m01 / v_mu[i][1].m00;
        int x1 = v_mu[i][0].m10 / v_mu[i][0].m00;
        int x2 = v_mu[i][1].m10 / v_mu[i][1].m00;

        int width = abs(x1 - x2) + 20;
        int height = 50;
        int y = (y1 + y2) / 2;
        int x = x1 < x2 ? x1 : x2;

        x = x - 10;
        y = y - height / 2;

        cv::Rect rectangle = cv::Rect(x, y, width, height);
        rectangles.push_back(rectangle);
    }

    return rectangles;
}

vector<Rect> Detector::detectNight(Mat &img) {
    vector<cv::Rect> rectangles;
    vector<vector<cv::Point>> hulls;
    hulls = getHulls(img);

    // Mat drawing = Mat::zeros(img.size(), CV_8UC3);
    rectangles = pairHeadlights(hulls);

    // for(cv::Rect rectangle : rectangles)
    // {
    //     cv::rectangle(drawing, rectangle, cv::Scalar(0, 255, 0), 1);
    // }
    // for(size_t i = 0; i < hulls.size(); i++)
    // {
    //     cv::drawContours(drawing, hulls, i, Scalar(0, 0, 255), 1);
    // }

    // imshow("Test", drawing);
    return rectangles;
}
