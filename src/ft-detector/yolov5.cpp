#include <ft-detector/utils.h>
#include <ft-detector/yolov5.h>
#include <spdlog/spdlog.h>

YoloV5::YoloV5(const std::string &modelPath, const std::string &labelsPath, bool enableGPU) {
    if (enableGPU) {
        if (torch::hasCUDA()) {
            spdlog::info("GPU found");
            spdlog::info("Run inference on GPU");
            device_ = torch::kCUDA;
            loadModel(modelPath);
        } else {
            spdlog::warn("No GPU available. Fallback to CPU with model {}", MODEL_CPU_PATH);
            device_ = torch::kCPU;
            loadModel(MODEL_CPU_PATH);
        }
    } else {
        spdlog::info("Run inference on CPU");
        device_ = torch::kCPU;
        loadModel(MODEL_CPU_PATH);
    }
    readLabels(labelsPath);
}

void YoloV5::loadModel(const std::string &modelPath) {
    try {
        model_ = std::make_unique<torch::jit::script::Module>(torch::jit::load(modelPath));
    } catch (const c10::Error &e) {
        spdlog::error("Loading the model failed!");
        std::exit(EXIT_FAILURE);
    }
    model_->to(device_);
}

void YoloV5::preprocess(cv::Mat &img) {
    cv::resize(img, img, cv::Size(INPUT_WIDTH, INPUT_HEIGHT));
    cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
}

torch::Tensor YoloV5::imgToTensor(const cv::Mat &img) {
    torch::Tensor imgTensor =
        torch::from_blob(img.data, {img.rows, img.cols, img.channels()}, torch::kByte);
    imgTensor = imgTensor.permute({2, 0, 1});  // HWC -> CHW
    imgTensor = imgTensor.div(255);            // Normalization
    imgTensor = imgTensor.unsqueeze(0);
    return imgTensor;
}

// Non Maximum Supression based on
// https://github.com/Nebula4869/YOLOv5-LibTorch/blob/master/src/YOLOv5LibTorch.cpp
std::vector<torch::Tensor> YoloV5::nms(torch::Tensor &preds) {
    std::vector<torch::Tensor> output;
    for (size_t i = 0; i < preds.sizes()[0]; ++i) {
        torch::Tensor pred = preds.select(0, i);

        // Filter by scores
        torch::Tensor scores =
            pred.select(1, 4) * std::get<0>(torch::max(pred.slice(1, 5, pred.sizes()[1]), 1));
        pred = torch::index_select(pred, 0, torch::nonzero(scores > SCORE_THRESHOLD).select(1, 0));
        if (pred.sizes()[0] == 0) continue;

        // (center_x, center_y, w, h) to (left, top, right, bottom)
        pred.select(1, 0) = pred.select(1, 0) - pred.select(1, 2) / 2;
        pred.select(1, 1) = pred.select(1, 1) - pred.select(1, 3) / 2;
        pred.select(1, 2) = pred.select(1, 0) + pred.select(1, 2);
        pred.select(1, 3) = pred.select(1, 1) + pred.select(1, 3);

        // Computing scores and classes
        std::tuple<torch::Tensor, torch::Tensor> max_tuple =
            torch::max(pred.slice(1, 5, pred.sizes()[1]), 1);
        pred.select(1, 4) = pred.select(1, 4) * std::get<0>(max_tuple);
        pred.select(1, 5) = std::get<1>(max_tuple);

        torch::Tensor dets = pred.slice(1, 0, 6);

        torch::Tensor keep = torch::empty({dets.sizes()[0]});
        torch::Tensor areas =
            (dets.select(1, 3) - dets.select(1, 1)) * (dets.select(1, 2) - dets.select(1, 0));
        std::tuple<torch::Tensor, torch::Tensor> indexes_tuple =
            torch::sort(dets.select(1, 4), 0, 1);
        torch::Tensor v = std::get<0>(indexes_tuple);
        torch::Tensor indexes = std::get<1>(indexes_tuple);
        int count = 0;
        while (indexes.sizes()[0] > 0) {
            keep[count] = (indexes[0].item().toInt());
            count += 1;

            // Computing overlaps
            torch::Tensor lefts = torch::empty(indexes.sizes()[0] - 1);
            torch::Tensor tops = torch::empty(indexes.sizes()[0] - 1);
            torch::Tensor rights = torch::empty(indexes.sizes()[0] - 1);
            torch::Tensor bottoms = torch::empty(indexes.sizes()[0] - 1);
            torch::Tensor widths = torch::empty(indexes.sizes()[0] - 1);
            torch::Tensor heights = torch::empty(indexes.sizes()[0] - 1);
            for (size_t i = 0; i < indexes.sizes()[0] - 1; ++i) {
                lefts[i] = std::max(dets[indexes[0]][0].item().toFloat(),
                                    dets[indexes[i + 1]][0].item().toFloat());
                tops[i] = std::max(dets[indexes[0]][1].item().toFloat(),
                                   dets[indexes[i + 1]][1].item().toFloat());
                rights[i] = std::min(dets[indexes[0]][2].item().toFloat(),
                                     dets[indexes[i + 1]][2].item().toFloat());
                bottoms[i] = std::min(dets[indexes[0]][3].item().toFloat(),
                                      dets[indexes[i + 1]][3].item().toFloat());
                widths[i] =
                    std::max(float(0), rights[i].item().toFloat() - lefts[i].item().toFloat());
                heights[i] =
                    std::max(float(0), bottoms[i].item().toFloat() - tops[i].item().toFloat());
            }
            torch::Tensor overlaps = widths * heights;

            // Filter by IOUs
            torch::Tensor ious =
                overlaps /
                (areas.select(0, indexes[0].item().toInt()) +
                 torch::index_select(areas, 0, indexes.slice(0, 1, indexes.sizes()[0])) - overlaps);
            indexes = torch::index_select(indexes, 0,
                                          torch::nonzero(ious <= NMS_THRESHOLD).select(1, 0) + 1);
        }
        keep = keep.toType(torch::kInt64);
        output.push_back(torch::index_select(dets, 0, keep.slice(0, 0, count)));
    }
    return output;
}

void YoloV5::postprocess(torch::Tensor &modelOutput, std::vector<Detection> &detections,
                         int imgWidth, int imgHeight) {
    auto preds = nms(modelOutput);

    if (!preds.empty()) {
        for (int i = 0; i < preds[0].sizes()[0]; ++i) {
            int left = preds[0][i][0].item().toFloat() * imgWidth / INPUT_WIDTH;
            int top = preds[0][i][1].item().toFloat() * imgHeight / INPUT_HEIGHT;
            int right = preds[0][i][2].item().toFloat() * imgWidth / INPUT_WIDTH;
            int bottom = preds[0][i][3].item().toFloat() * imgHeight / INPUT_HEIGHT;
            float score = preds[0][i][4].item().toFloat();
            int classID = preds[0][i][5].item().toInt();

            detections.emplace_back(
                Detection{.classID = classID,
                          .confidence = score,
                          .box = cv::Rect(left, top, (right - left), (bottom - top))});
        }
    }
}

void YoloV5::detect(const cv::Mat &inputImage, std::vector<Detection> &detections) {
    Timer clock;

    cv::Mat img;
    inputImage.copyTo(img);

    // Preprocessing
    clock.tick();
    preprocess(img);
    auto imgTensor = imgToTensor(img).to(device_);
    clock.tock();
    auto preprocTime = clock.elapsedTime();

    // Inference
    clock.tick();
    auto modelOutput = model_->forward({imgTensor}).toTuple()->elements()[0].toTensor().cpu();
    clock.tock();
    auto inferTime = clock.elapsedTime();

    // Postprocessing
    clock.tick();
    postprocess(modelOutput, detections, inputImage.cols, inputImage.rows);
    clock.tock();
    auto postprocTime = clock.elapsedTime();

    spdlog::debug("Timing:\tPreproc: {0}ms\tInference: {1}ms\tPostproc: {1}ms", preprocTime,
                  inferTime, postprocTime);
}