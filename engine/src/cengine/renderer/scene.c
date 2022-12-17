#include "scene.h"

#define MAX_QUAD_COUNT 1000
#define MAX_TRIANGLE_COUNT 500

#include <stdlib.h>
#include "../core/logging.h"
#include "../core/event_system.h"

scene* scene_create_empty(const char* name) {
    scene* ret = malloc(sizeof(scene));
    ret->name = name;
    ret->quads = malloc(sizeof(quad) * MAX_QUAD_COUNT);
    ret->triangles = malloc(sizeof(quad) * MAX_TRIANGLE_COUNT);
    ret->quad_count = 0;
    ret->triangle_count = 0;
    ret->_need_sort_triangles = false;
    ret->_need_sort_quads = false;
    return ret;
}

scene* scene_create(const char* name, quad** quads, triangle** triangles, u32 quad_count, u32 triangle_count) {
    scene* ret = malloc(sizeof(scene));
    ret->quads = malloc(sizeof(quad) * MAX_QUAD_COUNT);
    ret->triangles = malloc(sizeof(triangle) * MAX_TRIANGLE_COUNT);
    ret->name = name;
    if (quads != nullptr)
        ret->quads = quads;
    ret->triangle_count = triangle_count;
    if (triangles != nullptr)
        ret->triangles = triangles;
    ret->quad_count = quad_count;
    ret->triangle_count = triangle_count;
    if (quads != nullptr)
        ret->_need_sort_quads = true;
    if (triangles != nullptr)
        ret->_need_sort_triangles = true;
    return ret;
}

void scene_add_quad(scene* scene, quad* quad) {
    scene->quads[scene->quad_count++] = quad;
    scene->_need_sort_quads = true;
}

void scene_add_triangle(scene* scene, triangle* triangle) {
    scene->triangles[scene->triangle_count++] = triangle;
    scene->_need_sort_triangles = true;
}

void scene_remove_quad(scene* scene, quad* quad) {
    ASSERT_MSG(scene_get_quad_index(scene, quad) != -1, "Tried to remove non existent quad from scene->");

    quad_delete(quad);
    free(scene->quads[scene_get_quad_index(scene, quad)]);
    scene->quads[scene_get_quad_index(scene, quad)] = nullptr;
    scene->quad_count--;
}

void scene_remove_triangle(scene* scene, triangle* triangle) {
    ASSERT_MSG(scene_get_triangle_index(scene, triangle) != -1, "Tried to remove non existent triangle from scene->");
    triangle_delete(triangle);
    free(scene->triangles[scene_get_triangle_index(scene, triangle)]);
    scene->triangles[scene_get_triangle_index(scene, triangle)] = nullptr;
    scene->triangle_count--;
}

i32 scene_get_quad_index(scene* scene, quad* quad) {
    for (u32 i = 0; i < scene->quad_count; i++) {
        if (scene->quads[i]->tag == quad->tag) {
            return i;
        }
    }
    return -1;
}

i32 scene_get_triangle_index(scene* scene, triangle* triangle) {
    for (u32 i = 0; i < scene->triangle_count; i++) {
        if (scene->triangles[i]->tag == triangle->tag) {
            return i;
        }
    }
    return -1;
}
quad* scene_get_quad_by_tag(scene* scene, const char* tag) {
    for (u32 i = 0; i < scene->quad_count; i++) {
        if (scene->quads[i]->tag == tag) {
            return scene->quads[i];
        }
    }
    return nullptr;
}

triangle* scene_get_triangle_by_tag(scene* scene, const char* tag) {
    for (u32 i = 0; i < scene->triangle_count; i++) {
        if (scene->triangles[i]->tag == tag) {
            return scene->triangles[i];
        }
    }
    return nullptr;
}

void scene_render(scene* scene) {
    if (scene->_need_sort_quads) {
        scene_sort_quads_by_render_order(scene);
    }
    if (scene->_need_sort_triangles) {
        scene_sort_triangles_by_render_order(scene);
    }
    if (scene->quad_count != 0) {
        for (u32 i = 0; i < scene->quad_count; i++) {
            if (scene->quads[i] != nullptr) {
                quad_render(scene->quads[i]);
            }
        }
    }
    if (scene->triangle_count != 0) {
        for (u32 i = 0; i < scene->triangle_count; i++) {
            if (scene->triangles[i] != nullptr) {
                triangle_render(*scene->triangles[i]);
            }
        }
    }
}

void scene_update(scene* scene) {
    for (u32 i = 0; i < scene->quad_count; i++) {
        vector2 tmp_pos = scene->quads[i]->position;
        vector2 tmp_movement = scene->quads[i]->movement;
        scene->quads[i]->moved = false;
        if (scene->quads[i]->update_callback != default_quad_update_callback)
            scene->quads[i]->update_callback(scene->quads[i]);

        if (vector2_compare(tmp_pos, scene->quads[i]->position)) {
            scene->quads[i]->movement = vector2_create(0.0f, 0.0f);
        }

        if (!vector2_compare(tmp_movement, scene->quads[i]->movement)) {
            dispatch_event(quad_moved_event, scene->quads[i]);
            scene->quads[i]->moved = true;
        }
    }
    if (scene->triangle_count != 0) {
        for (u32 i = 0; i < scene->triangle_count; i++) {
            if (scene->triangles[i]->update_callback != default_triangle_update_callback)
                scene->triangles[i]->update_callback(scene->triangles[i]);
        }
    }
}

void scene_sort_quads_by_render_order(scene* scene) {
    for (u32 i = 0; i < scene->quad_count; i++) {
        for (u32 j = i + 1; j < scene->quad_count; j++) {
            if (scene->quads[i]->render_level > scene->quads[j]->render_level) {
                quad* tmp = scene->quads[i];
                scene->quads[i] = scene->quads[j];
                scene->quads[j] = tmp;
            }
        }
    }
}

void scene_sort_triangles_by_render_order(scene* scene) {
    for (u32 i = 0; i < scene->triangle_count; i++) {
        for (u32 j = i + 1; j < scene->triangle_count; j++) {
            if (scene->triangles[i]->render_level > scene->triangles[j]->render_level) {
                triangle* tmp = scene->triangles[i];
                scene->triangles[i] = scene->triangles[j];
                scene->triangles[j] = tmp;
            }
        }
    }
}

quad* scene_get_quad_in_tilemap(scene* scene, tile_map map, vector2 tile_pos) {
    for (u32 i = 0; i < scene->quad_count; i++) {
        if (scene->quads[i]->associated_tilemap != nullptr) {
            if (tile_map_compare(*scene->quads[i]->associated_tilemap, map) && vector2_compare(scene->quads[i]->tile_map_tile_position, tile_pos)) {
                return scene->quads[i];
            }
        }
    }
    return nullptr;
}