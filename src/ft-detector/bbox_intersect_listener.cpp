#include <ft-detector/bbox_intersect_listener.h>
#include <spdlog/spdlog.h>

void IntersectListenerStdout::update() { spdlog::warn("Do not touch your face!"); }