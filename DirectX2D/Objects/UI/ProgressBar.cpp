#include "Framework.h"

ProgressBar::ProgressBar()
    : Quad(L"Textures/Battle/HpGauge.png")
{
    SetPixelShader(L"ProgressBar.hlsl");

    hpBuffer = new FloatValueBuffer();
    hpBuffer->Get()[0] = fullGauge;
}

ProgressBar::~ProgressBar()
{
    delete hpBuffer;
}

void ProgressBar::Render()
{
    hpBuffer->SetPS(1);

    Quad::Render();
}

void ProgressBar::SetAmount(float value)
{
    hpBuffer->Get()[0] = value;
}

