#pragma once

#include <cocos2d.h>
#include "structure.h"


#define DESIGN_RESOLUTION_320X568   0
#define DESIGN_RESOLUTION_640X1136  1

// # Приложение будет собрано для этого разрешения.
#define TARGET_DESIGN_RESOLUTION_SIZE  DESIGN_RESOLUTION_640X1136

typedef struct tagResource {
    dimension_t  size;
    std::string  directory;
} Resource;

static const Resource smallResource =
    { dimension_t( 320, 568  ),  "small"  };
static const Resource largeResource =
    { dimension_t( 640, 1136 ),  "large"  };

#if (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_320X568)
static const dimension_t designResolutionSize( 320, 568 );
#elif (TARGET_DESIGN_RESOLUTION_SIZE == DESIGN_RESOLUTION_640X1136)
static const dimension_t designResolutionSize( 640, 1136 );
#else
#error Unknown target design resolution!
#endif

// размер шрифта в зависимости от выбранного разрешения
#define TITLE_FONT_SIZE  (cocos2d::CCEGLView::sharedOpenGLView()->getDesignResolutionSize().width / smallResource.size.width * 24)


#define CCCA(x)   (x->copy()->autorelease())
