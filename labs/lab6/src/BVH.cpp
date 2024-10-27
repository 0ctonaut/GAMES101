#include <algorithm>
#include <cassert>
#include "BVH.hpp"

BVHAccel::BVHAccel(std::vector<Object*> p, int maxPrimsInNode,
                   SplitMethod splitMethod)
    : maxPrimsInNode(std::min(255, maxPrimsInNode)), splitMethod(splitMethod),
      primitives(std::move(p))
{
    time_t start, stop;
    time(&start);
    if (primitives.empty())
        return;

    // root = recursiveBuild(primitives);
    root = recursiveBuildBySAH(primitives);

    time(&stop);
    double diff = difftime(stop, start);
    int hrs = (int)diff / 3600;
    int mins = ((int)diff / 60) - (hrs * 60);
    int secs = (int)diff - (hrs * 3600) - (mins * 60);

    printf(
        "\rBVH Generation complete: \nTime Taken: %i hrs, %i mins, %i secs\n\n",
        hrs, mins, secs);
}

BVHBuildNode* BVHAccel::recursiveBuild(std::vector<Object*> objects)
{
    BVHBuildNode* node = new BVHBuildNode();

    // Compute bounds of all primitives in BVH node
    Bounds3 bounds;
    for (int i = 0; i < objects.size(); ++i)
        bounds = Union(bounds, objects[i]->getBounds());
    if (objects.size() == 1) {
        // Create leaf _BVHBuildNode_
        node->bounds = objects[0]->getBounds();
        node->object = objects[0];
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }
    else if (objects.size() == 2) {
        node->left = recursiveBuild(std::vector{objects[0]});
        node->right = recursiveBuild(std::vector{objects[1]});

        node->bounds = Union(node->left->bounds, node->right->bounds);
        return node;
    }
    else {
        Bounds3 centroidBounds;
        for (int i = 0; i < objects.size(); ++i)
            centroidBounds =
                Union(centroidBounds, objects[i]->getBounds().Centroid());
        int dim = centroidBounds.maxExtent();
        switch (dim) {
        case 0:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->getBounds().Centroid().x <
                       f2->getBounds().Centroid().x;
            });
            break;
        case 1:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->getBounds().Centroid().y <
                       f2->getBounds().Centroid().y;
            });
            break;
        case 2:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->getBounds().Centroid().z <
                       f2->getBounds().Centroid().z;
            });
            break;
        }

        auto beginning = objects.begin();
        auto middling = objects.begin() + (objects.size() / 2);
        auto ending = objects.end();

        auto leftshapes = std::vector<Object*>(beginning, middling);
        auto rightshapes = std::vector<Object*>(middling, ending);

        assert(objects.size() == (leftshapes.size() + rightshapes.size()));

        node->left = recursiveBuild(leftshapes);
        node->right = recursiveBuild(rightshapes);

        node->bounds = Union(node->left->bounds, node->right->bounds);
    }

    return node;
}

Intersection BVHAccel::Intersect(const Ray& ray) const
{
    Intersection isect;
    if (!root)
        return isect;
    isect = BVHAccel::getIntersection(root, ray);
    return isect;
}

Intersection BVHAccel::getIntersection(BVHBuildNode* node, const Ray& ray) const
{
    // TODO Traverse the BVH to find intersectio

    std::array<int, 3> dirIsNeg;
    dirIsNeg[0] = ray.direction.x >= 0;
    dirIsNeg[1] = ray.direction.y >= 0;
    dirIsNeg[2] = ray.direction.z >= 0;

    if (node->bounds.IntersectP(ray, ray.direction_inv, dirIsNeg))
    {
        if (node->left == nullptr && node->right == nullptr)
        {
            return node->object->getIntersection(ray);
        }
        else
        {
            Intersection leftInter = BVHAccel::getIntersection(node->left, ray);
            Intersection rightInter = BVHAccel::getIntersection(node->right, ray);
            return leftInter.distance < rightInter.distance ? leftInter : rightInter;
        }
    }
    else
    {
        return Intersection();
    }
}

// SAH
float BVHAccel::computeSAHCost(const Bounds3& parentBox, const Bounds3& leftBox, const Bounds3& rightBox, int leftCount, int rightCount) const {
    float parentArea = parentBox.SurfaceArea();
    float leftArea = leftBox.SurfaceArea();
    float rightArea = rightBox.SurfaceArea();

    // 表面面积比例
    float Pl = leftArea / parentArea;
    float Pr = rightArea / parentArea;

    // 假设遍历代价和相交测试代价分别为 1，这可以根据实际情况调整
    float traversalCost = 1.0f;
    float intersectionCost = 1.0f;

    return traversalCost + (Pl * leftCount + Pr * rightCount) * intersectionCost;
}

BVHBuildNode* BVHAccel::recursiveBuildBySAH(std::vector<Object*> objects)
{
    BVHBuildNode* node = new BVHBuildNode();

    // Compute bounds of all primitives in BVH node
    Bounds3 bounds;
    for (int i = 0; i < objects.size(); ++i)
    {
        bounds = Union(bounds, objects[i]->getBounds());
    }

    if (objects.size() <= maxPrimsInNode)
    {
        node->bounds = bounds;
        node->object = objects[0];
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }

    Bounds3 centroidBounds;
    for (int i = 0; i < objects.size(); ++i)
    {
        centroidBounds = Union(centroidBounds, objects[i]->getBounds().Centroid());
    }

    int dim = centroidBounds.maxExtent();
    std::sort(objects.begin(), objects.end(), [dim](Object* a, Object* b) {
        return a->getBounds().Centroid()[dim] < b->getBounds().Centroid()[dim];
    });

    float minCost = std::numeric_limits<float>::infinity();
    int bestSplit = 0;
    Bounds3 leftBounds, rightBounds;

    // 尝试每个分割位置
    for (int i = 1; i < objects.size(); ++i) {
        Bounds3 leftBoundsTemp, rightBoundsTemp;
        for (int j = 0; j < i; ++j)
            leftBoundsTemp = Union(leftBoundsTemp, objects[j]->getBounds());
        for (int j = i; j < objects.size(); ++j)
            rightBoundsTemp = Union(rightBoundsTemp, objects[j]->getBounds());

        float cost = computeSAHCost(bounds, leftBoundsTemp, rightBoundsTemp, i, objects.size() - i);

        if (cost < minCost) {
            minCost = cost;
            bestSplit = i;
            leftBounds = leftBoundsTemp;
            rightBounds = rightBoundsTemp;
        }
    }

    auto leftshapes = std::vector<Object*>(objects.begin(), objects.begin() + bestSplit);
    auto rightshapes = std::vector<Object*>(objects.begin() + bestSplit, objects.end());

    node->left = recursiveBuildBySAH(leftshapes);
    node->right = recursiveBuildBySAH(rightshapes);
    node->bounds = Union(leftBounds, rightBounds);

    return node;
}