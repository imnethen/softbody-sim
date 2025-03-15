#include "blob.h"
#include "../vector/vector.h"
#include <SDL3/SDL_pixels.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_rect.h>

// inits (blob->num_points) points uniformally on a circle
void InitPointsCircle(SB_Blob *blob, Vector center, float radius) {
    const float tau = 6.2831853;
    for (int i = 0; i < blob->num_points; i++) {
        float angle = tau * ((float)i / (float)(blob->num_points));
        float x = SDL_cosf(angle) * radius + center.x;
        float y = SDL_sinf(angle) * radius + center.y;
        blob->points[i] = SB_CreatePoint(x, y);
    }
}

SDL_Texture* CreateTexture(SDL_Renderer *renderer) {
    char *bmp_path = NULL;
    SDL_asprintf(&bmp_path, "%s../resources/texture.bmp", SDL_GetBasePath());

    SDL_Surface *surface = SDL_LoadBMP(bmp_path);
    if (!surface) {
        SDL_Log("Couldn't create surface:\n%s\n", SDL_GetError());
        return NULL;
    }
    SDL_free(bmp_path);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Couldn't create texture:\n%s\n", SDL_GetError());
        return NULL;
    }
    SDL_DestroySurface(surface);

    return texture;
}

SB_Blob SB_CreateBlob(int num_points, Vector center, float radius, float desired_volume, float scaling_factor) {
    SB_Blob blob;
    blob.num_points = num_points;
    blob.desired_volume = desired_volume;
    blob.scaling_factor = scaling_factor;

    blob.points = malloc(sizeof(SB_Point) * blob.num_points);
    InitPointsCircle(&blob, center, radius);

    const float tau = 6.2831853;
    blob.constraint_length = radius * SDL_sqrtf(2. * (1. - SDL_cosf(tau / (float)blob.num_points)));

    blob.texture = NULL;

    return blob;
}

float GetBlobArea(SB_Blob *blob) {
    float area = 0.;
    for (int i = 0; i < blob->num_points; i++) {
        Vector cur_pos = blob->points[i].pos;
        Vector next_pos = blob->points[(i + 1) % blob->num_points].pos;
        area += (cur_pos.x - next_pos.x) * (next_pos.y + cur_pos.y) / 2.;
    }
    return area;
}

void SB_UpdateBlob(SB_Blob *blob, float deltatime) {
    for (int i = 0; i < blob->num_points; i++) {
        SB_UpdatePoint(&blob->points[i], deltatime);
    }

    Vector *displacements = malloc(blob->num_points * sizeof(Vector));
    int *displacement_counts = malloc(blob->num_points * sizeof(int));

    memset(displacements, 0, blob->num_points * sizeof(Vector));
    memset(displacement_counts, 0, blob->num_points * sizeof(int));

    const int num_iterations = 80;
    for (int it = 0; it < num_iterations; it++) {
        // distance constraints
        for (int i = 0; i < blob->num_points; i++) {
            int nexti = (i + 1) % blob->num_points;

            Vector cur_pos = blob->points[i].pos;
            Vector next_pos = blob->points[nexti].pos;

            Vector diff = VectorSub(next_pos, cur_pos);
            float double_error = (VectorLen(diff) - blob->constraint_length);
            if (double_error > 0.) { // why
                Vector offset = VectorMul(VectorNormalized(diff), double_error / 2.);
                displacements[i] = VectorAdd(displacements[i], offset);
                displacement_counts[i]++;

                displacements[nexti] = VectorAdd(displacements[nexti], VectorMul(offset, -1));
                displacement_counts[nexti]++;
            }
        }

        // area constraints
        float area = SDL_fabsf(GetBlobArea(blob));
        float area_error = blob->desired_volume - area;
        float offset_length = area_error * blob->scaling_factor;

        for (int i = 0; i < blob->num_points; i++) {
            Vector prev_pos = blob->points[(i - 1 + blob->num_points) % blob->num_points].pos;
            Vector cur_pos = blob->points[i].pos;
            Vector next_pos = blob->points[(i + 1) % blob->num_points].pos;

            Vector secant = VectorSub(next_pos, prev_pos);
            if (VectorLenSq(secant) == 0.) {
                continue;
            }
            Vector normal = VectorNormalized((Vector){ -secant.y, secant.x });
            Vector scaled_normal = VectorMul(normal, -offset_length);

            displacements[i] = VectorAdd(displacements[i], scaled_normal);
            displacement_counts[i]++;
        }

        // apply displacements
        for (int i = 0; i < blob->num_points; i++) {
            if (displacement_counts[i] > 0) {
                blob->points[i].pos = VectorAdd(blob->points[i].pos, VectorDiv(displacements[i], displacement_counts[i]));
            }
            SB_PointHandleBorders(&blob->points[i]);
        }

        // always keep points inbounds
        for (int i = 0; i < blob->num_points; i++) {
            SB_PointHandleBorders(&blob->points[i]);
        }

        // reset displacements
        memset(displacements, 0, blob->num_points * sizeof(Vector));
        memset(displacement_counts, 0, blob->num_points * sizeof(int));
    }

    free(displacements);
    free(displacement_counts);
}

SDL_Vertex *GetVertices(SB_Blob blob) {
    const float tau = 6.2831853;
    SDL_Vertex *vertices = malloc(blob.num_points * sizeof(SDL_Vertex));

    for (int i = 0; i < blob.num_points; i++) {
        vertices[i] = (SDL_Vertex){
            (SDL_FPoint){ blob.points[i].pos.x, blob.points[i].pos.y },
            (SDL_FColor){ 1., 1., 1., 1. },
            (SDL_FPoint){ SDL_cosf(tau * (float)i / blob.num_points) / 2. + 0.5, SDL_sinf(tau * (float)i / blob.num_points) / 2. + 0.5 },
        };
    }

    return vertices;
}

int *GetIndices(SB_Blob blob) {
    int *indices = malloc(3 * (blob.num_points - 2) * sizeof(int));

    for (int i = 1; i + 1 < blob.num_points; i++) {
        indices[3 * (i - 1) + 0] = 0;
        indices[3 * (i - 1) + 1] = i;
        indices[3 * (i - 1) + 2] = i + 1;
    }

    return indices;
}

void SB_RenderBlob(SB_Blob blob, SDL_Renderer *renderer) {
    if (!blob.texture) {
        blob.texture = CreateTexture(renderer);
    }

    /* for (int i = 0; i < blob.num_points; i++) { */
    /*     SB_RenderPoint(&blob.points[i], renderer); */
    /* } */

/*     for (int i = 0; i < blob.num_points; i++) { */
/*         SDL_RenderLine(renderer, blob.points[i].pos.x, blob.points[i].pos.y, blob.points[(i + 1) % blob.num_points].pos.x, blob.points[(i + 1) % blob.num_points].pos.y); */
/*     } */

    /* Vector midpoint = (Vector){ 0., 0. }; */
    /* for (int i = 0; i < blob.num_points; i++) { */
    /*     midpoint = VectorAdd(midpoint, blob.points[i].pos); */
    /* } */
    /* midpoint = VectorDiv(midpoint, (float)blob.num_points); */

    /* SDL_SetRenderDrawColorFloat(renderer, 1., 0.6, 1., 1.); */
    /* SDL_RenderDebugText(renderer, midpoint.x, midpoint.y, ":3"); */
    /* SDL_SetRenderDrawColorFloat(renderer, 1., 1., 1., 1.); */

    SDL_Vertex *vertices = GetVertices(blob);
    int *indices = GetIndices(blob);

    SDL_RenderGeometry(renderer, blob.texture, vertices, blob.num_points, indices, 3 * (blob.num_points - 2));

    free(vertices);
    free(indices);
}
