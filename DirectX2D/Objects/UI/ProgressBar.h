#pragma once

class ProgressBar : public Quad
{
public:
    ProgressBar();
    ~ProgressBar();

    void Render();

    void SetAmount(float value);

private:
    FloatValueBuffer* hpBuffer;

    float fullGauge = 1.0f;
};