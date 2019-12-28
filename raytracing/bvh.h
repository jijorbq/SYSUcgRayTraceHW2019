#ifndef BVH_H
#define BVH_H
#include "hit.h"
#include "random.h"
#include "aabb.h"

//定义bvh树的结点，从hittable中继承过来，可以对多个物体实现hit()，和盒子求交的操作。

class bvh_node : public hittable  {
    public:
        bvh_node() {}
        bvh_node(hittable **l, int n, double time0, double time1);
        virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
        virtual bool bounding_box(float t0, float t1, aabb& box) const;
        hittable *left;
        hittable *right;
        aabb box;
};

bool bvh_node::bounding_box(float t0, float t1, aabb& b) const {
    b = box;
    return true;
}

//重写hit函数
//在BVH树上遍历，计算碰撞
//r：探测光束，t_min, t_max:参数区间定义的线段。rec：碰撞记录

bool bvh_node::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    //撞击包围盒是和内部物体相交的必要条件
    if (box.hit(r, t_min, t_max)) {
        hit_record left_rec, right_rec;
        //求出左右的字数的相交情况
        bool hit_left = left->hit(r, t_min, t_max, left_rec);
        bool hit_right = right->hit(r, t_min, t_max, right_rec);
        //该子树的碰撞一定发生在左子树或者右子树
        if (hit_left && hit_right) {
            //如果两边都有就选取相交距离最近的一个
            if (left_rec.t < right_rec.t)
                rec = left_rec;
            else
                rec = right_rec;
            return true;
        }
        else if (hit_left) {
            rec = left_rec;
            return true;
        }
        else if (hit_right) {
            rec = right_rec;
            return true;
        }
        else
            return false;
    }
    else return false;
}


//包围盒三个维度的比较函数

int box_x_compare (const void * a, const void * b) {
        aabb box_left, box_right;
        hittable *ah = *(hittable**)a;
        hittable *bh = *(hittable**)b;
        if(!ah->bounding_box(0,0, box_left) || !bh->bounding_box(0,0, box_right))
                        std::cerr << "no bounding box in bvh_node constructor\n";
        if ( box_left.min().x() - box_right.min().x() < 0.0  )
            return -1;
        else
            return 1;
}

int box_y_compare (const void * a, const void * b)
{
        aabb box_left, box_right;
        hittable *ah = *(hittable**)a;
        hittable *bh = *(hittable**)b;
        if(!ah->bounding_box(0,0, box_left) || !bh->bounding_box(0,0, box_right))
                        std::cerr << "no bounding box in bvh_node constructor\n";
        if ( box_left.min().y() - box_right.min().y() < 0.0  )
            return -1;
        else
            return 1;
}
int box_z_compare (const void * a, const void * b)
{
        aabb box_left, box_right;
        hittable *ah = *(hittable**)a;
        hittable *bh = *(hittable**)b;
        if(!ah->bounding_box(0,0, box_left) || !bh->bounding_box(0,0, box_right))
                        std::cerr << "no bounding box in bvh_node constructor\n";
        if ( box_left.min().z() - box_right.min().z() < 0.0  )
            return -1;
        else
            return 1;
}

//建树的过程，l为待建树的对象，n为数量
bvh_node::bvh_node(hittable **l, int n, double time0, double time1) {
    int axis = int(3*random_double());
    //随机选取一个划分的维度，并排序
    if (axis == 0)
       qsort(l, n, sizeof(hittable *), box_x_compare);
    else if (axis == 1)
       qsort(l, n, sizeof(hittable *), box_y_compare);
    else
       qsort(l, n, sizeof(hittable *), box_z_compare);
    //n==1 or n==2直接建树
    if (n == 1) {
        left = right = l[0];
    }
    else if (n == 2) {
        left = l[0];
        right = l[1];
    }
    else {
        //否则均匀划分n/2，递归进左右子树建树
        left = new bvh_node(l, n/2, time0, time1);
        right = new bvh_node(l + n/2, n - n/2, time0, time1);
    }
    //由左右子树的包围盒建立整个树的包围盒，求并
    aabb box_left, box_right;
    if(!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0,time1, box_right))
        std::cerr << "no bounding box in bvh_node constructor\n";
    box = surrounding_box(box_left, box_right);
}

#endif // BVH_H
