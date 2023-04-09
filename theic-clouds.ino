#include <M5Unified.h>
#include "FastLED.h"
#include <OverAnimate.h>
#include <SubStrip.h>

static const int lstrip_count = 80;
CRGB lstrip[lstrip_count];
SubStrip left(lstrip, lstrip_count);
static const int rstrip_count = 80;
CRGB rstrip[rstrip_count];
SubStrip right(rstrip, rstrip_count);

AnimationSystem ansys;

typedef void(*StripFunc)(Animation*, SubStrip *strip, float);
class StripAnimation : public Animation
{
public:
    StripFunc func;
    SubStrip *strip;
    StripAnimation(StripFunc func, SubStrip *strip, TimeInterval duration = 1.0, bool repeats = false) 
      : Animation(duration, repeats), func(func), strip(strip) {}
protected:
    void animate(float absoluteTime) { func(this, strip, absoluteTime); }
};

float frand(void)
{
  return random(1000)/1000.0;
}

float curve(float progress)
{
    return sin(progress*6.28f)/2.0f + 0.5f;
}

void DawnAnim(Animation *self, SubStrip *strip, float t)
{
    for(int i = 0; i < strip->numPixels(); i++)
    {
        (*strip)[i] = CRGB(255, 100, 0) * gammaf(curve(t - i/10.0f))/2.0f + CRGB(240, 255, 0) * gammaf(curve(t + i/4.0f))/2.0f;
    }
}


StripAnimation lclouds(DawnAnim, &left, 8.0, true);
StripAnimation rclouds(DawnAnim, &right, 6.0, true);

void LightningAnim(Animation *self, SubStrip *strip, float t)
{

    strip->fill(CRGB::Black);
    if(t == 1.0) return;
    
    for(int i = 0; i < strip->numPixels(); i++)
    {
        (*strip)[i] = CRGB::Blue * t;
    }
}


StripAnimation lthun(LightningAnim, &left, 1.5);
StripAnimation rthun(LightningAnim, &right, 1.5);
static const int thuns_count = 2;
StripAnimation thuns[] = {lthun, rthun};


void setup() {
    M5.begin();
    FastLED.addLeds<WS2811, G26, GRB>(lstrip, lstrip_count);
    FastLED.addLeds<WS2811, G32, GRB>(rstrip, rstrip_count);
    FastLED.setBrightness(255);
    left.fill(CRGB::Black);
    right.fill(CRGB::Black);
    FastLED.show();

    ansys.addAnimation(&lclouds);
    ansys.addAnimation(&rclouds);
}

unsigned long lastMillis;
void loop() {
    M5.update();

    unsigned long now = millis();
    if(!lastMillis) {
        lastMillis = now;
    }
    unsigned long diff = now - lastMillis;
    lastMillis = now;
    TimeInterval delta = diff/1000.0;
    
    update();

    ansys.playElapsedTime(delta);
    FastLED.show();
}

void update()
{
    for(int i = 0; i < thuns_count; i++)
    {
        StripAnimation *thun = &thuns[i];
        if((thun->beginTime + thun->duration) < ansys.now()) {
            thun->beginTime = ansys.now() + 2 + frand()*10.0;
            ansys.addAnimation(thun);
        }
    }
}
