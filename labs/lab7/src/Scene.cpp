//
// Created by Göksu Güvendiren on 2019-05-14.
//

#include "Scene.hpp"


void Scene::buildBVH() {
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

Intersection Scene::intersect(const Ray &ray) const
{
    return this->bvh->Intersect(ray);
}

void Scene::sampleLight(Intersection &pos, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum){
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}

bool Scene::trace(
        const Ray &ray,
        const std::vector<Object*> &objects,
        float &tNear, uint32_t &index, Object **hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vector2f uvK;
        if (objects[k]->intersect(ray, tNearK, indexK) && tNearK < tNear) {
            *hitObject = objects[k];
            tNear = tNearK;
            index = indexK;
        }
    }


    return (*hitObject != nullptr);
}

// Implementation of Path Tracing
Vector3f Scene::castRay(const Ray &ray, int depth) const
{
    Vector3f L_indir, L_dir;
    // TO DO Implement Path Tracing Algorithm here
    Intersection inter_shading = intersect(ray);
    if (!inter_shading.happened)
    {
        return backgroundColor;
    }
    else if (inter_shading.m->hasEmission())
    {
        return inter_shading.m->getEmission();
    }

    // for (uint32_t i = 0; i < get_lights().size(); ++i)
    {
        Intersection inter_light;
        float pdf_light;

        sampleLight(inter_light, pdf_light);
        Vector3f shading2light = inter_light.coords - inter_shading.coords;
        Vector3f w_shading2light = shading2light.normalized();
        Intersection inter_shading2light = intersect(Ray(inter_shading.coords, w_shading2light));

        if (inter_shading2light.happened && inter_shading2light.m->hasEmission())
        {
            Vector3f L_i = inter_light.emit;
            Vector3f f_r = inter_shading.m->eval(-w_shading2light, -ray.direction, inter_shading.normal);
            float cos_theta = dotProduct(w_shading2light, inter_shading.normal);
            float cos_theta_ = dotProduct(-w_shading2light, inter_light.normal);
            L_dir += L_i * f_r * cos_theta * cos_theta_ / dotProduct(shading2light, shading2light) / pdf_light;
        }

        float P_RR = get_random_float();
        if (P_RR < RussianRoulette)
        {
            Vector3f next_dir = inter_shading.m->sample(ray.direction, inter_shading.normal).normalized();
            Ray nextRay(inter_shading.coords, next_dir);
            Intersection inter_shading2next = intersect(nextRay);

            if (inter_shading2next.happened && !inter_shading2next.m->hasEmission())
            {
                Vector3f f_r = inter_shading.m->eval(-next_dir, -ray.direction, inter_shading.normal);
                float pdf_hemi = inter_shading.m->pdf(-next_dir, -ray.direction, inter_shading.normal);
                float cos_theta = dotProduct(next_dir, inter_shading.normal);
                L_indir = castRay(nextRay, depth + 1) * f_r * cos_theta / pdf_hemi / RussianRoulette;
            }
        }
    }

    return L_dir + L_indir;
}