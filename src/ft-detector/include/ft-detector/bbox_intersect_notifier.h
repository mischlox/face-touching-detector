//
// Created by mischlox on 11.09.22.
//

#ifndef FT_BBOX_INTERSECT_NOTIFIER_H
#define FT_BBOX_INTERSECT_NOTIFIER_H
#include <ft-detector/bbox_intersect_listener.h>

#include <list>
#include <memory>

class IntersectNotifier {
   private:
    std::list<std::shared_ptr<IBboxIntersectListener>> listeners;

   public:
    void notifyAll() {
        for (const auto& listener : listeners) {
            listener->update();
        }
    }

    void registerListener(const std::shared_ptr<IBboxIntersectListener>& listener) {
        listeners.emplace_back(listener);
    }

    void unregisterListener(const std::shared_ptr<IBboxIntersectListener>& listener) {
        listeners.remove(listener);
    }
};

#endif  // FT_BBOX_INTERSECT_NOTIFIER_H
