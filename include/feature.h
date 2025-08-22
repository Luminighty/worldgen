#ifndef FEATURE_H
#define FEATURE_H


#include "map.h"
#include "tile.h"


typedef enum {
    FEATURE_NONE,
    FEATURE_VEIN,
    FEATURE_BLOB,
    FEATURE_SURFACE,
    FEATURE_TREE,
} FeatureKind;

typedef struct {
    Tile trunk;
    Tile cap;
    int height_min;
    int height_max;
    int width_min;
    int width_max;
} FeatureTree;


typedef struct {
    Tile tile;
    int depth;
} FeatureSurface;


typedef struct {
    Tile tile;
    double frequency;
    int size_min;
    int size_max;
} FeatureBlob;


typedef struct {
    Tile tile;
    double frequency;
    int size_min;
    int size_max;
} FeatureVein;


typedef struct {
    FeatureKind kind;
    union {
	FeatureVein vein;
	FeatureBlob blob;
	FeatureSurface surface;
	FeatureTree tree;
    } as;
} Feature;


void feature_apply(Map* map, long seed, int chunk_x, int chunk_y, int chunk_size, Feature feature);


#define makeFeatureVein(...) ((Feature){.kind = FEATURE_VEIN, .as={.vein={__VA_ARGS__}}})
#define makeFeatureBlob(...) ((Feature){.kind = FEATURE_BLOB, .as={.blob={__VA_ARGS__}}})
#define makeFeatureSurface(...) ((Feature){.kind = FEATURE_SURFACE, .as={.surface={.depth=1, __VA_ARGS__}}})
#define makeFeatureTree(...) ((Feature){.kind = FEATURE_TREE, .as={.tree={__VA_ARGS__}}})


#endif // FEATURE_H
