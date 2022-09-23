#ifndef FT_BBOX_INTERSECT_LISTENER_H
#define FT_BBOX_INTERSECT_LISTENER_H
class IBboxIntersectListener {
   public:
    virtual void update() = 0;
};

/**
 * Implementation of IBboxIntersectListener that prints to stdout when notified
 */
class IntersectListenerStdout : public IBboxIntersectListener {
    void update() override;
};
#endif  // FT_BBOX_INTERSECT_LISTENER_H
