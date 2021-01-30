#include "app.h"
#include "dots.h"
#include "render.h"
#include "util.h"

static view_list contextStack;

static float scrollVelocity = 0;

static int drawBaseX = 0;
static int drawBaseY = 0;
static int drawX = 0;
static int drawY = 0;
static int drawColorPair = 0;
static bool drawBold = false;
static bool drawReverse = false;
static bool drawItalic = false;
static bool drawUnderline = false;

static int dragX = 0;
static int dragY = 0;

static int keyMods = 0;

static std::map<int, int> colorMap;

static bool dirtyScreen = true;

struct BgFg {
    color_info_t bg;
    color_info_t fg;
};

struct RGB {
    unsigned short b;
    unsigned short g;
    unsigned short r;
};

static RGB drawColor;
static RGB drawBg;

#define screenWidth 80 
#define screenHeight 50

#define SCREEN_DATA_SIZE (screenWidth * screenHeight)
char _screenData[SCREEN_DATA_SIZE];
RGB _screenFg[SCREEN_DATA_SIZE];
RGB _screenBg[SCREEN_DATA_SIZE];

static std::map<int, BgFg> colorPairs;

void pushKey(char c, std::string keySequence);

int pairForColor(int colorIdx, bool selected)
{
    if (selected && colorIdx == color_pair_e::NORMAL) {
        return color_pair_e::SELECTED;
    }
    return colorMap[colorIdx + (selected ? SELECTED_OFFSET : 0)];
}

static view_t* context()
{
    if (!contextStack.size())
        return NULL;
    return contextStack.back();
}

void _clrtoeol(int w)
{
    if (drawY >= screenHeight) return;
    if (w > screenWidth) w = screenWidth;

    char *target = _screenData;
    target += (drawY * screenWidth);
    memset(target, 0, w * sizeof(char));

    dirtyScreen = true;
}

void _move(int y, int x)
{
    drawX = x;
    drawY = y;
}

void _addch(char c)
{
    int xx = (drawX + drawBaseX);
    int yy = (drawY + drawBaseY);

    // app_t::log("%d %d %c", xx, yy, c);

    if (xx >= screenWidth || yy >= screenHeight) return;
    _screenData[yy * screenWidth + xx] = c;

    _screenBg[yy * screenWidth + xx] = drawBg;
    _screenFg[yy * screenWidth + xx] = drawColor;

    drawX ++;

    dirtyScreen = true;
}

bool _drawdots(int dots, int* colors)
{
    return true;
}

void _addstr(const char* str)
{
    int l = strlen(str);
    for (int i = 0; i < l; i++) {
        _addch(str[i]);
    }
}

void _addwstr(const wchar_t* str)
{
}

void _attron(int attr)
{
}

void _attroff(int attr)
{
}

int _color_pair(int pair)
{
    drawColorPair = pair;
    BgFg bgFg = colorPairs[drawColorPair];

    RGB color = {
        .b = (uint8_t)bgFg.fg.blue,
        .g = (uint8_t)bgFg.fg.green,
        .r = (uint8_t)bgFg.fg.red
    };
    drawColor = color;

    RGB bg = {
        .b = (uint8_t)bgFg.bg.blue,
        .g = (uint8_t)bgFg.bg.green,
        .r = (uint8_t)bgFg.bg.red
    };
    drawBg = bg;

    return drawColorPair;
}

void _underline(bool b)
{
    drawUnderline = b;
}

void _bold(bool b)
{
    drawBold = b;
}

void _reverse(bool b)
{
    drawReverse = b;
}

void _italic(bool b)
{
    drawItalic = b;
}

void _begin(view_t* view)
{
    contextStack.push_back(view);
    // drawBaseX = view->x + view->padding;
    // drawBaseY = view->y + view->padding;
    // app_t::log("?%s %d %d %d %d", view->name.c_str(), drawBaseX, drawBaseY, (int)view->width, (int)view->height);
}

void _end()
{
    contextStack.pop_back();
    view_t *view = context();
    if (view) {
        // drawBaseX = view->x + view->padding;
        // drawBaseY = view->y + view->padding;   
    } else {
        drawBaseX = 0;
        drawBaseY = 0;
    }
}

static struct render_t* renderInstance = 0;

struct render_t* render_t::instance()
{
    return renderInstance;
}

render_t::render_t()
{
    renderInstance = this;
}

render_t::~render_t()
{
}

void render_t::initialize()
{
    drawX = 0;
    drawY = 0;
    width = screenWidth;
    height = screenHeight;
    fw = 1;
    fh = 1;
    memset(_screenData, 0, sizeof(int) * SCREEN_DATA_SIZE);
    memset(_screenFg, 0, sizeof(RGB) * SCREEN_DATA_SIZE);
    memset(_screenBg, 0, sizeof(RGB) * SCREEN_DATA_SIZE);
}

void render_t::shutdown()
{
}

static int poll_event()
{
    return 0;
}

void render_t::update(int delta)
{
}

void render_t::input()
{
}

static void renderView(view_t* view)
{
}

void render_t::render()
{
}

static void addColorPair(int idx, int fg, int bg)
{
    app_t* app = app_t::instance();

    BgFg pair = {
        .bg = color_info_t::true_color(bg),
        .fg = color_info_t::true_color(fg)
    };

    colorPairs[idx] = pair;
}

void render_t::updateColors()
{
    app_t* app = app_t::instance();
    theme_ptr theme = app->theme;

    //---------------
    // build the color pairs
    //---------------
    addColorPair(color_pair_e::NORMAL, app->fg, app->bgApp);
    addColorPair(color_pair_e::SELECTED, app->selFg, app->selBg);

    colorMap[color_pair_e::NORMAL] = color_pair_e::NORMAL;
    colorMap[color_pair_e::SELECTED] = color_pair_e::SELECTED;

    int idx = 32;

    auto it = theme->colorIndices.begin();
    while (it != theme->colorIndices.end()) {
        colorMap[it->first] = idx;
        addColorPair(idx++, it->first, app->bgApp);
        it++;
    }

    it = theme->colorIndices.begin();
    while (it != theme->colorIndices.end()) {
        colorMap[it->first + SELECTED_OFFSET] = idx;
        addColorPair(idx++, it->first, app->selBg);
        if (it->first == app->selBg) {
            colorMap[it->first + SELECTED_OFFSET] = idx + 1;
        }
        it++;
    }
}

void render_t::delay(int ms)
{
}

bool render_t::isTerminal()
{
    return true;
}

int _keyMods()
{
    return keyMods;
}

// bridge

bool isScreenDirty() {
    if (dirtyScreen) {
        dirtyScreen = false;
        return true;
    }
    return false;
}

void* getScreenColor()
{
    return (void*)&_screenFg;
}

char* getScreenData()
{
    return _screenData;
}